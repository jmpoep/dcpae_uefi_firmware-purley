/**++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// This file contains an 'Intel Peripheral Driver' and is
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may
// be modified by the user, subject to additional terms of the
// license agreement
//

Copyright (c) 2009 - 2019 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


 @file DdrtErrorHandler.c

 This file will contain all definitions related to DCPMM Error Handler.
 Refer Intel Optane DC Persistent Memory Module (DCPMM) Software
 RAS and Error Recovery Specification revision 1.0

----------------------------------------------------------------**/

#include <PiSmm.h>
#include <IndustryStandard/Acpi.h>
#include <IndustryStandard/WheaDefs.h>
#include <Library/BaseMemoryLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/mpsyncdatalib.h>
#include <Protocol/ErrorHandlingProtocol.h>
#include <Protocol/PlatformErrorHandlingProtocol.h>
#include <Protocol/PlatformRasPolicyProtocol.h>
#include <Protocol/CpuCsrAccess.h>
#include <Protocol/CrystalRidge.h>
#include <Protocol/MemRasProtocol.h>
#include <Library/TimerLib.h>
#include <RcRegs.h>


#include <Protocol/SmmCpuService.h>
#include <Library/MemoryAllocationLib.h>
#include "DdrtErrorHandler.h"
#include <MemRas.h>

extern  EFI_GUID                       gErrRecordNotifyCmcGuid;
extern  EFI_GUID                       gErrRecordPlatformMemErrGuid;

// Module data
EFI_ERROR_HANDLING_PROTOCOL           *mErrorHandlingProtocol;
EFI_PLATFORM_ERROR_HANDLING_PROTOCOL  *mPlatformErrorHandlingProtocol;
EFI_PLATFORM_RAS_POLICY_PROTOCOL      *mPlatformRasPolicyProtocol;
EFI_CPU_CSR_ACCESS_PROTOCOL           *mCpuCsrAccess;
EFI_CRYSTAL_RIDGE_PROTOCOL            *mCrystalRidge;
EFI_RAS_SYSTEM_TOPOLOGY               *mRasTopology;
SYSTEM_RAS_SETUP                      *mRasSetup;
EFI_MEM_RAS_PROTOCOL                  *mMemRasProtocol;
REPORTER_PROTOCOL                      mDdrtReporter;
EFI_MP_SERVICES_PROTOCOL              *mMpService = NULL;
EFI_SMM_CPU_SERVICE_PROTOCOL          *mSmmCpuService;
UINTN                                 BspProcessorNumber;
UINTN                                 BspSocketNumber;


static DIMM_ERROR_LOG_DATA DimmErrorLogData[MAX_SOCKET][MAX_CH][MAX_DIMM][LOG_LEVEL_NUM];
AP_DIMM_STATE              ApState[MAX_SOCKET];
INT16                      CpuNum[MAX_SOCKET];
UINT8                      SocketUsed[MAX_SOCKET];

BOOLEAN   mDdrtErrorsEnabled = FALSE;     // Are DDRT errors enabled to signal SMI?
UINT8     mSmiPriority = 0;               // Which Bucket low or high is set for SMI signaling?

static BOOLEAN                         mClearSmiSrc = FALSE;
static CONST UINT16 mErrorTypeBitmap[5] = {BIT7, BIT5, BIT3, BIT8, BIT10};

EFI_STATUS
GetSpaFromDpa (
  UINT8            Socket,
  UINT8            Ch,
  UINT8            Dimm,
  UINT64           Dpa,
  UINT64   * CONST Spa
  )
{
  EFI_STATUS            Status;
  TRANSLATED_ADDRESS    TranslatedAddr;

  ZeroMem (&TranslatedAddr, sizeof(TRANSLATED_ADDRESS));
  TranslatedAddr.SocketId           = Socket;
  TranslatedAddr.MemoryControllerId = Ch / MAX_MC_CH;
  TranslatedAddr.ChannelId          = Ch % MAX_MC_CH;
  TranslatedAddr.DimmSlot           = Dimm;
  TranslatedAddr.PhysicalRankId     = 0;
  TranslatedAddr.DPA                = Dpa;

  Status = mMemRasProtocol->DimmAddressToSystemAddress(&TranslatedAddr);

  RASDEBUG ((DEBUG_INFO, "====================================================\n"));
  RASDEBUG ((DEBUG_INFO, RAS_ERROR_STR ("DDRT") "Dpa to Spa translation: %r\n", Status));
  RASDEBUG ((DEBUG_INFO, RAS_ERROR_STR ("DDRT") "Socket = %d\n", TranslatedAddr.SocketId));
  RASDEBUG ((DEBUG_INFO, RAS_ERROR_STR ("DDRT") "MC = %d\n", TranslatedAddr.MemoryControllerId));
  RASDEBUG ((DEBUG_INFO, RAS_ERROR_STR ("DDRT") "Channel = %d\n", TranslatedAddr.ChannelId));
  RASDEBUG ((DEBUG_INFO, RAS_ERROR_STR ("DDRT") "DimmSlot = %d\n", TranslatedAddr.DimmSlot));
  RASDEBUG ((DEBUG_INFO, RAS_ERROR_STR ("DDRT") "RankAddress: 0x%Lx\n", TranslatedAddr.RankAddress));
  RASDEBUG ((DEBUG_INFO, RAS_ERROR_STR ("DDRT") "SystemAddress: 0x%Lx\n", TranslatedAddr.SystemAddress));
  RASDEBUG ((DEBUG_INFO, "====================================================\n"));

  if (EFI_ERROR(Status)) {
    return Status;
  }

  *Spa = TranslatedAddr.SystemAddress;
  return EFI_SUCCESS;
}

BOOLEAN
IsAlertEnabled (
  UINT16        Policy,
  UINT8         Error
  )
{
  if ((Policy & (BIT1 | BIT0)) == 0) {
    return FALSE;
  }

  return (Policy & (mErrorTypeBitmap[Error])) != 0;
}

/**
  Enables error logging for DDRT errors

  @retval EFI_SUCCESS      - Error logging for DDRT errors is enabled
  @retval EFI_UNSUPPOERTED - Error logging for DDRT is disabled, or not DDRT DIMMs present
**/
EFI_STATUS
DdrtEnableError (
  VOID
  )
{
  UINT8       Socket;
  UINT8       Ch;           // at socket level
  UINT8       Dimm;         // with in channel
  EFI_STATUS  Status;

  //
  // Skip if DDRT error logging is not enabled in BIOS setup
  //
  if (!mRasSetup->Memory.FnvErrorEn ||
      mRasSetup->Memory.FnvErrorLowPrioritySignal == NO_LOG && mRasSetup->Memory.FnvErrorHighPrioritySignal == NO_LOG) {

    RASDEBUG ((DEBUG_WARN, RAS_INFO_STR ("DDRT") "DDRT error logging is disabled in BIOS setup\n"));
    return EFI_UNSUPPORTED;
  }

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {

    if (!mRasTopology->SystemInfo.SocketInfo[Socket].Valid) {
      continue;
    }
    for (Ch = 0; Ch < MAX_CH; Ch++) {

      if (!mRasTopology->SystemInfo.SocketInfo[Socket].ChannelInfo[Ch].Valid) {
        continue;
      }
      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {

        if (mCrystalRidge->IsNvmDimm(Socket, Ch, Dimm)) {

          Status = mCrystalRidge->NvmCtlrSetHostAlert (Socket, Ch, Dimm, &mRasSetup->Memory.HostAlerts[0]);
          if (EFI_ERROR(Status)) {

            RASDEBUG ((DEBUG_ERROR, RAS_ERROR_STR ("DDRT") "S%d.C%d.D%d: Enabling DDRT alerts failed (%r)\n",
                       Socket, Ch, Dimm, Status));
            continue;
          }
          Status = mCrystalRidge->NvmCtlrErrorLogProgramSignal(Socket, Ch, Dimm, LOW_PRIORITY_EVENT,
                                                               mRasSetup->Memory.FnvErrorLowPrioritySignal); // can we abstract the HIGH/LOW terminology?
          ASSERT_EFI_ERROR (Status);
          Status = mCrystalRidge->NvmCtlrErrorLogProgramSignal(Socket, Ch, Dimm, HIGH_PRIORITY_EVENT,
                                                               mRasSetup->Memory.FnvErrorHighPrioritySignal);
          ASSERT_EFI_ERROR (Status);

          if ((mRasSetup->Memory.FnvErrorLowPrioritySignal & SMI) ||
              (mRasSetup->Memory.FnvErrorHighPrioritySignal & SMI)) {

            mDdrtErrorsEnabled = TRUE; // Flag to psyshost runtime FNV Error Handler whether to run or not
          }
        }
      } // for (Dimm...)
    } // for (Ch...)
  } // for (Socket...)

  if (mDdrtErrorsEnabled) {

    RASDEBUG ((DEBUG_INFO, RAS_INFO_STR ("DDRT") "Error reporting has been enabled\n"));
    return EFI_SUCCESS;
  }
  RASDEBUG ((DEBUG_INFO, RAS_INFO_STR ("DDRT") "Error reporting not enabled, no DDRT DIMMs\n"));
  return EFI_UNSUPPORTED;
}


EFI_STATUS
DdrtDisableError (
  UINT32 DeviceType,
  UINT32 EventType,
  UINT32 *ExtData
  )
{
  // Not implemented for Purley.  OEM can implement at their discretion.
  return EFI_UNSUPPORTED;
}


BOOLEAN
EFIAPI
DdrtCheckStatus (
  VOID
  )
{
  EFI_STATUS Status;
  UINT32 Log;
  UINT8 Socket;
  UINT8 Ch;
  UINT8 Dimm;
  UINT8 Priority;

  if (mDdrtErrorsEnabled == FALSE) {
    return FALSE;
  }
  // check to see if AEP DIMM has any error that are not covered by MCAbanks
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    for (Ch = 0; Ch < MAX_CH; Ch++) {
      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        for (Priority = LOW_PRIORITY_EVENT; Priority <= HIGH_PRIORITY_EVENT; Priority++) {
          Status = mCrystalRidge->NvmCtlrErrorLogDetectError(Socket, Ch, Dimm, Priority, DO_NOT_CLEAR_ON_READ, &Log);
          if (Status == EFI_SUCCESS) {
            return TRUE;
          }
        }
      }
    }
  }

  return mClearSmiSrc;
}


BOOLEAN
EFIAPI
DdrtIsSupported (
  VOID
  )
{
  return mDdrtErrorsEnabled;
}


/**
 * @brief Classify NVDIMM access error.
 *
 * @return 'True' if error is fatal, 'false' otherwise.
**/
BOOLEAN
DdrtNvdimmFisErrorIsFatal (
  IN EFI_STATUS Status
  )
{
  switch (Status) {
    case EFI_DEVICE_ERROR:
    case EFI_NO_RESPONSE:
    case EFI_NO_MEDIA:
      return TRUE;
    default:
      return FALSE;
  }
}


/**
  Creates the memory error record based on the inputs.

  This function will only handle correctable errors.

  @param[in]  Type      - The error type
  @param[in]  SubType   - The error sub type
  @param[in]  Node      - The node the error was detected on
  @param[in]  Channel   - The channel the error was detected on
  @param[in]  Rank      - The rank the error was detected on
  @param[in]  Dimm      - The dimm the error was detected on
  @param[in]  McaCode   - The MCA code for the error

  @retval None
**/
VOID
CreateMemErrRecord (
  IN  UINT8                          Socket,
  IN  UINT8                          Ch,
  IN  UINT8                          Dimm,
  IN  UINT8                          ErrorType,
  IN  BOOLEAN                        Fatal,
  IN  UINT64         CONST  * CONST  Spa,
  IN  BOOLEAN                        WholeSocket,
  OUT UEFI_MEM_ERROR_RECORD * CONST  MemRecord
  )
{
  RASDEBUG ((DEBUG_ERROR, RAS_ERROR_STR ("DDRT") "LogDcpMemError : Socket = %d, Channel = %d, Dimm = %d\n", Socket, Ch, Dimm));

  // All clear memory error record structure
  ZeroMem (MemRecord, sizeof (UEFI_MEM_ERROR_RECORD));

  // Build Error Record Header
  MemRecord->Header.SignatureStart = SIGNATURE_32('C','P','E','R');
  MemRecord->Header.Revision = 0x0204;  // UEFI Spec version 2.4
  MemRecord->Header.SignatureEnd = 0xffffffff;
  MemRecord->Header.SectionCount = 1;  // Only supporting one section per error record

  MemRecord->Header.Severity = Fatal ? EFI_ACPI_6_1_ERROR_SEVERITY_FATAL : EFI_ACPI_6_1_ERROR_SEVERITY_CORRECTABLE;
  MemRecord->Header.ValidBits = 0;
  MemRecord->Header.RecordLength = sizeof (UEFI_MEM_ERROR_RECORD);
  CopyMem (&MemRecord->Header.NotificationType, &gErrRecordNotifyCmcGuid, sizeof (EFI_GUID));
  MemRecord->Header.Flags = HW_ERROR_FLAGS_RECOVERED;

  // Fill error descriptor
  MemRecord->Descriptor.Offset = sizeof (UEFI_ERROR_RECORD_HEADER) + sizeof (UEFI_ERROR_SECT_DESC);
  MemRecord->Descriptor.Length = sizeof (MEMORY_ERROR_SECTION);
  MemRecord->Descriptor.Revision = 0x0204;  // UEFI Spec version 2.4
  MemRecord->Descriptor.ValidBits = 0;
  MemRecord->Descriptor.Flags = 1;   // Primary
  CopyMem (&MemRecord->Descriptor.SectionType, &gErrRecordPlatformMemErrGuid, sizeof (EFI_GUID));
  MemRecord->Descriptor.Severity = Fatal ? EFI_ACPI_6_1_ERROR_SEVERITY_FATAL : EFI_ACPI_6_1_ERROR_SEVERITY_CORRECTABLE;;

  // Fill location details
  MemRecord->Section.ValidBits = 0;
  MemRecord->Section.Node = Socket;
  MemRecord->Section.ValidBits = PLATFORM_MEM_NODE_VALID;

  if (WholeSocket == FALSE) {
    MemRecord->Section.Card = Ch;
    MemRecord->Section.Module = Dimm;
    //
    //MemRecord->Section.CardHandle = mRasTopology->SystemInfo.SmbiosType16Handle; // Type 16 handle - ?????
    //
    RASDEBUG ((DEBUG_ERROR, RAS_ERROR_STR ("DDRT") "LogDcpMemError : MemRecord->Section.Card = %d, MemRecord->Section.Node = %d MemRecord->Setion.Module=%d\n", MemRecord->Section.Card, MemRecord->Section.Node,MemRecord->Section.Module));
    MemRecord->Section.ModuleHandle = mRasTopology->SystemInfo.SocketInfo[Socket].ChannelInfo[Ch].DimmInfo[Dimm].SmbiosType17Handle; // Type 17 handle
    MemRecord->Section.ValidBits = PLATFORM_MEM_NODE_VALID | PLATFORM_MEM_MODULE_VALID | PLATFORM_Module_HANDLE_VALID; // | PLATFORM_CARD_HANDLE_VALID
  }

  MemRecord->Section.ErrorType = ErrorType;

  if (Spa != NULL) {
    MemRecord->Section.PhysAddr = *Spa;
    MemRecord->Section.PhysAddrMask = 0xFFFFFFFFFFFFFF00; //AEP poison granularity is 256B.
    MemRecord->Section.ValidBits |= (UINT64)(PLATFORM_MEM_PHY_ADDRESS_VALID | PLATFORM_MEM_PHY_ADDRESS_MASK_VALID);
  }
  return;
}

/**
  Get Ddrt error logs from socket.

  This function get all type of handled alerts from Ddrt per specific socket.

  @param[in]  SocketPtr      - Pointer to socket number

**/
VOID
GetErrorLogFromSocket (
  IN UINT8 *SocketPtr
  )
{
  ERR_LOG_DATA            ErrLog;
  EFI_STATUS              Status;
  NVDIMM_INFO             NvdimmInfo;
  BOOLEAN                 Overflow;
  BOOLEAN                 ReadMediaLog = FALSE;
  BOOLEAN                 ReadThermalLog = FALSE;
  BOOLEAN                 CheckLongOpState = FALSE;
  BOOLEAN                 FwHang = FALSE;
  BOOLEAN                 FwFatalErr = FALSE;
  UINT32                  Data;
  UINT32                  ErrorMap;
  UINT16                  *SequenceNumber;
  UINT8                   Priority = 0;
  UINT8                   Socket = 0;
  UINT8                   Ch = 0;
  UINT8                   Dimm = 0;
  UINT8                   Index = 0;


  Socket = *SocketPtr;

  for (Priority = 0; Priority <= HIGH_LEVEL_LOG; Priority++) {
    for (Ch = 0; Ch < MAX_CH; Ch++) {
      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        if (mCrystalRidge->IsNvmDimm(Socket, Ch, Dimm)) {
          //
          // Retrieve the error log data
          //
          Status = mCrystalRidge->NvmCtlrErrorLogDetectError (Socket, Ch, Dimm, Priority, CLEAR_ON_READ, &Data);
          DimmErrorLogData[Socket][Ch][Dimm][Priority].DetectErrorStatus = Status;
          DimmErrorLogData[Socket][Ch][Dimm][Priority].DetectErrorData = Data;

          if (EFI_ERROR (Status)) {
            continue;
          }

          //
          // We have a valid DDRT alert. Force the clearance of SMI
          //
          mClearSmiSrc = TRUE;

          //
          // Get the error bitmap
          //
          ErrorMap = (Data & FNVERR_MASK);
          Overflow = !!(Data & FNVERR_OVERFLOW);

          //
          // Check error bitmap if there is anything to do
          //
          if (ErrorMap == 0) {
            continue;
          }

          //
          // Retrive the NVDIMM info data
          //
          Status = mCrystalRidge->NvmCtlrGetDimmInfo (Socket, Ch, Dimm, &NvdimmInfo);
          if (EFI_ERROR (Status)) {
            continue;
          }

          //
          // Firmware may set multiple bits in DDRT interrupt packet
          // so ensure we check all bits we handle in every iteration
          //
          if (Priority == LOW_LEVEL_LOG) {

            if (!Overflow) {
              if (ErrorMap & FNVERR_LONGOP_COMPLETE) {
                CheckLongOpState = TRUE;
                DimmErrorLogData[Socket][Ch][Dimm][Priority].ProcessLongOp = TRUE;
              }
              if (NvdimmInfo.FisVersion >= FIS_1_13) {
                if (ErrorMap & FNVERR_MEDIA_ERROR_LOG) {
                  ReadMediaLog = TRUE;
                }

                if (ErrorMap & FNVERR_THERMAL_ERROR_LOG) {
                  ReadThermalLog = TRUE;
                }
              } else {
                if (ErrorMap & FNVERR_NGNVM_ERROR_LOG) {
                  ReadMediaLog = TRUE;
                  ReadThermalLog = TRUE;
                }
              }
            } else {
                CheckLongOpState = TRUE;
                ReadMediaLog = TRUE;
                ReadThermalLog = TRUE;
            }
          } else {
            if (ErrorMap & FNVERR_FW_HANG) {
              FwHang = TRUE;
              DimmErrorLogData[Socket][Ch][Dimm][Priority].ProcessFwHang = TRUE;
            }

            if (ErrorMap & FNVERR_FATAL) {
              FwFatalErr = TRUE;
              DimmErrorLogData[Socket][Ch][Dimm][Priority].ProcessFwFatalErr = TRUE;
            }

            if (!Overflow) {
              if (NvdimmInfo.FisVersion >= FIS_1_13) {
                if (ErrorMap & FNVERR_MEDIA_ERROR_LOG) {
                  ReadMediaLog = TRUE;
                }

                if (ErrorMap & FNVERR_THERMAL_ERROR_LOG) {
                  ReadThermalLog = TRUE;
                }
              } else {
                if (ErrorMap & FNVERR_NGNVM_ERROR_LOG) {
                  ReadMediaLog = TRUE;
                }
              }
            } else {
              ReadMediaLog = TRUE;
              ReadThermalLog = TRUE;
            }
          }

          if (CheckLongOpState) {
            Status = mCrystalRidge->NvmCtlrLongOpCompleteGet (Socket, Ch, Dimm, &DimmErrorLogData[Socket][Ch][Dimm][Priority].LongOpRsp[0]);
            DimmErrorLogData[Socket][Ch][Dimm][Priority].LongOpStatus = Status;
          }

          if (ReadMediaLog) {
            DimmErrorLogData[Socket][Ch][Dimm][Priority].ProcessMediaErrLogData = TRUE;
            SequenceNumber = &mRasTopology->SystemInfo.SocketInfo[Socket].ChannelInfo[Ch].DimmInfo[Dimm].FnvErrorHandlerLastSeqNum[MEDIA_LOG_REQ][Priority];
            DimmErrorLogData[Socket][Ch][Dimm][Priority].MediaErrSeqFirst = *SequenceNumber;
            for (Index = 0; Index < MAX_MEDIA_PAYLOADS; Index++) {

              Status = mCrystalRidge->NvmCtlrGetErrorLog (Socket, Ch, Dimm, MEDIA_LOG_REQ, Priority, mRasSetup->Memory.FnvErrorMailbox, TRUE, SequenceNumber, &ErrLog);

              DimmErrorLogData[Socket][Ch][Dimm][Priority].MediaErrLogDataRsp[Index].ErrorMediaLogStatus = Status;
              DimmErrorLogData[Socket][Ch][Dimm][Priority].MediaErrLogDataRsp[Index].MediaErrLog = ErrLog.MediaLogData;
              DimmErrorLogData[Socket][Ch][Dimm][Priority].MediaErrLogDataRsp[Index].NewSeqNumber = *SequenceNumber;

              if (Status == EFI_NOT_FOUND || DimmErrorLogData[Socket][Ch][Dimm][Priority].MediaErrLogDataRsp[Index].MediaErrLog.ReturnCount == 0) {
                //
                // This is the case when numbers of new media log is times of MAX_MEDIA_LOGS(3)
                //
                break;
              }

              if (EFI_ERROR (Status)) {
                break;
              }

              if (DimmErrorLogData[Socket][Ch][Dimm][Priority].MediaErrLogDataRsp[Index].MediaErrLog.ReturnCount !=
                  ARRAY_SIZE (DimmErrorLogData[Socket][Ch][Dimm][Priority].MediaErrLogDataRsp[Index].MediaErrLog.MediaLogs)) {
                //
                // The only condition we want to continue reading is ReturnCount == MediaLogData array size
                //
                break;
              }
            }
          }

          if (ReadThermalLog) {
            DimmErrorLogData[Socket][Ch][Dimm][Priority].ProcessThermalErrLogData = TRUE;
            SequenceNumber = &mRasTopology->SystemInfo.SocketInfo[Socket].ChannelInfo[Ch].DimmInfo[Dimm].FnvErrorHandlerLastSeqNum[THERMAL_LOG_REQ][Priority];
            DimmErrorLogData[Socket][Ch][Dimm][Priority].ThermalErrSeqFirst = *SequenceNumber;

            for (Index = 0; Index < MAX_THERMAL_PAYLOADS; Index++) {

              Status = mCrystalRidge->NvmCtlrGetErrorLog (Socket, Ch, Dimm, THERMAL_LOG_REQ, Priority, mRasSetup->Memory.FnvErrorMailbox, TRUE, SequenceNumber, &ErrLog);

              DimmErrorLogData[Socket][Ch][Dimm][Priority].ThermalErrLogDataRsp[Index].ErrorThermalLogStatus = Status;
              DimmErrorLogData[Socket][Ch][Dimm][Priority].ThermalErrLogDataRsp[Index].ThermalErrLog = ErrLog.ThermalLogData;
              DimmErrorLogData[Socket][Ch][Dimm][Priority].ThermalErrLogDataRsp[Index].NewSeqNumber = *SequenceNumber;

              if (Status == EFI_NOT_FOUND || ErrLog.ThermalLogData.ReturnCount == 0) {
                break;
              }

              if (EFI_ERROR (Status)) { //We should break the while loop whenever there is error status and already retried.
                break;
              }

              if (ErrLog.ThermalLogData.ReturnCount != ARRAY_SIZE (ErrLog.ThermalLogData.ThermalLogs)) {
                break;
              }
            }
          }
        }

        if(!ReadMediaLog && !ReadThermalLog) {
          continue;
        }
      }
    }
  }

  MemoryFence ();
  ApState[Socket] = AP_FINISHED;
}

/**
  Retrieve Event Data From All Dimms.

  This function checking if on specific socket occurs any Ddrt alerts.

  @retval None

**/
EFI_STATUS
RetrieveEventDataFromAllDimms (
  IN UINT8 *SocketPtr
  )
{
  UINT8                      Socket;
  EFI_PROCESSOR_INFORMATION  ProcessorInfoBuffer;
  BOOLEAN                    ErrorLogIsNotReady = TRUE;
  UINTN                      BspProcessorNumberTemp;
  UINTN                      BspSocketNumberTemp;
  UINT64                     TimeoutCounter = ONE_SECOND_TIME_IN_NANOSEC;
  INT64                      WaitTime;
  UINT64                     TotalTimeInNanoSeconds = 0;
  UINT64                     StartTimeBsp;
  UINT64                     EndTimeBsp;
  UINT64                     CounterStart = 0;
  UINT64                     CounterEnd = 0 ;
  INT64                      CounterCycle;
  EFI_STATUS                 Status;
  BOOLEAN                    ApError = FALSE;

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    SocketUsed[Socket] = Socket;
  }

  Status = mSmmCpuService->WhoAmI(mSmmCpuService, &BspProcessorNumberTemp);

  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = mSmmCpuService->GetProcessorInfo (
                  mSmmCpuService,
                  BspProcessorNumberTemp,
                  &ProcessorInfoBuffer
                  );

  if (EFI_ERROR (Status)) {
    return Status;
  }

  BspSocketNumberTemp = ProcessorInfoBuffer.Location.Package;

  if (BspSocketNumberTemp != BspSocketNumber) {
    BspSocketNumber = BspSocketNumberTemp;
  }

  for (Socket = 0 ; Socket < MAX_SOCKET; Socket++) {
    if (CpuNum[Socket] != -1) {
      if (Socket != BspSocketNumber) {
        ApState[Socket] = AP_IN_PROGRESS;

        Status = gSmst->SmmStartupThisAp ((EFI_AP_PROCEDURE)GetErrorLogFromSocket, CpuNum[Socket], &SocketUsed[Socket]);

        if (EFI_ERROR (Status)) {
          ApState[Socket] = AP_ERROR;
          continue;
        }
      }
    }
  }

  StartTimeBsp = GetPerformanceCounter ();

  GetErrorLogFromSocket ((UINT8*)&BspSocketNumber);

  EndTimeBsp = GetPerformanceCounter ();

  GetPerformanceCounterProperties (&CounterStart, &CounterEnd);

  CounterCycle = CounterEnd - CounterStart;
  if (CounterCycle < 0) {
    CounterCycle = -CounterCycle;
  }
  CounterCycle++;

  WaitTime = EndTimeBsp - StartTimeBsp;
  if (CounterStart > CounterEnd) {
    WaitTime = -WaitTime;
  }

  if (WaitTime < 0) {
    WaitTime += CounterCycle;
  }

  TotalTimeInNanoSeconds = GetTimeInNanoSecond (WaitTime);

  if (TotalTimeInNanoSeconds > TimeoutCounter) {
    DEBUG ((DEBUG_ERROR, RAS_INFO_STR ("DDRT") "RetrieveEventDataFromAllDimms: BSP execution exceeded 1s\n"));
    *SocketPtr = (UINT8)BspSocketNumber;
    return EFI_ABORTED;
  }

  TimeoutCounter = (TimeoutCounter - TotalTimeInNanoSeconds) / 10000;

  while (ErrorLogIsNotReady) {
    if (TimeoutCounter == 0 || TimeoutCounter > ONE_SECOND_TIME_IN_NANOSEC) {
      break;
    }

    ErrorLogIsNotReady = FALSE;
    for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
      if (CpuNum[Socket] == -1 || Socket == BspSocketNumber) {
        continue;
      }
      if (ApState[Socket] == AP_IN_PROGRESS) {
        ErrorLogIsNotReady = TRUE;
        break;
      }
    }

    MicroSecondDelay (10);
    TimeoutCounter--;
  }

  if (ErrorLogIsNotReady == TRUE) {
    DEBUG ((DEBUG_ERROR, RAS_INFO_STR ("DDRT") "RetrieveEventDataFromAllDimms: BSP+AP execution exceeded 1s\n"));
    return EFI_ABORTED;
  }

  *SocketPtr = 0;
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (CpuNum[Socket] == -1 || Socket == BspSocketNumber) {
      continue;
    }
    if (ApState[Socket] != AP_FINISHED) {
      DEBUG ((DEBUG_ERROR, RAS_INFO_STR ("DDRT") "AP[%d] from socket %d has incorrect state:%d\n", CpuNum[Socket], Socket, ApState[Socket]));
      *SocketPtr = Socket;
      ApError = TRUE;
    }
  }

  return ApError ? EFI_ABORTED : EFI_SUCCESS;
}


/**
  Retrieve Event Data From All Dimms.

  This function initializing Error Log Data structure.

  @retval EFI_SUCCESS    - Initialize Error Log Data performed successfully

**/
EFI_STATUS
InitializeErrorLogData (
  VOID
  )
{
UINT16                     CpuNumber;
EFI_PROCESSOR_INFORMATION  ProcessorInfoBuffer;
UINT8                      Index;
EFI_STATUS                 Status;

  for (Index = 0; Index <ARRAY_SIZE(CpuNum); Index++) {
    CpuNum[Index] = -1;
  }

  BspSocketNumber = 0;
  BspProcessorNumber = 0;

  Status = gSmst->SmmLocateProtocol (&gEfiSmmCpuServiceProtocolGuid, NULL, &mSmmCpuService);

  if (EFI_ERROR(Status)) {
    RASDEBUG ((DEBUG_INFO, RAS_INFO_STR ("DDRT") "DdrtDetectAndHandle : MSmmCpuService protocol return wrong status: %r\n", Status));
    return EFI_NOT_FOUND;
  }

  Status = mSmmCpuService->WhoAmI (mSmmCpuService, &BspProcessorNumber);

  if (EFI_ERROR (Status)) {
    return Status;
  }

  for (CpuNumber = 0; CpuNumber < gSmst->NumberOfCpus; CpuNumber++) {
    Status = mSmmCpuService->GetProcessorInfo (
                    mSmmCpuService,
                    (UINTN)CpuNumber,
                    &ProcessorInfoBuffer
                    );

    if (EFI_ERROR(Status)) {
      if (Status == EFI_NOT_FOUND) {
        continue;
      }
      return Status;
    }

    if ((ProcessorInfoBuffer.StatusFlag & BIT1) && ((UINTN)CpuNumber != BspProcessorNumber) && (CpuNum[ProcessorInfoBuffer.Location.Package] == -1)) {
      CpuNum[ProcessorInfoBuffer.Location.Package] = CpuNumber;
    }
  };

  Status = mSmmCpuService->GetProcessorInfo (
  mSmmCpuService,
  BspProcessorNumber,
  &ProcessorInfoBuffer
  );

  if (EFI_ERROR (Status)) {
    return Status;
  }

  BspSocketNumber = ProcessorInfoBuffer.Location.Package;

  return EFI_SUCCESS;
}


/**
  @brief Handles DDRT High Priority errors

  Refer Figure 5-2. DDRT Alert Hierarchy in
  Intel Optane DC Persistent Memory Module (DCPMM) Software
  RAS and Error Recovery Specification revision 1.0

  @param[in]  Socket    Socket index
  @param[in]  Ch        Channel index (at socket level)
  @param[in]  Dimm      Dimm index (within the channel)
 **/
VOID
NvdimmHighPriorityError (
  IN UINT8 Socket,
  IN UINT8 Ch,
  IN UINT8 Dimm
  )
{
  STATIC UINT32           VerboseLevel = DEBUG_INFO;
  UEFI_MEM_ERROR_RECORD   MemRecord;
  MEDIA_ERR_LOG           *MediaLogPtr;
  THERMAL_ERR_LOG         *ThermalLogPtr;
  EFI_STATUS              Status;
  UINT64                  Spa;
  UINT32                  Data;
  UINT16                  *SequenceNumber;
  UINT8                   i = 0;
  UINT8                   ErrorType;
  UINT8                   LogsIndex = 0;
  BOOLEAN                 FwHang;
  BOOLEAN                 FwFatalErr;

  if (ApState[Socket] != AP_FINISHED) {
    RASDEBUG ((DEBUG_ERROR, RAS_ERROR_STR ("DDRT") "S%d.C%d.D%d: ApState status is not finished!\n", Socket, Ch, Dimm));
    return;
  }

  Status = DimmErrorLogData[Socket][Ch][Dimm][HIGH_LEVEL_LOG].DetectErrorStatus;

  if (EFI_ERROR (Status)) {
    if (Status != EFI_NOT_FOUND) {
      RASDEBUG ((DEBUG_ERROR, RAS_ERROR_STR ("DDRT") "NvdimmHighPriorityError: S%d.C%d.D%d: failed to detect errors: %r\n", Socket, Ch, Dimm, Status));
    } else {
      RASDEBUG ((DEBUG_INFO, RAS_INFO_STR ("DDRT") "NvdimmHighPriorityError: S%d.C%d.D%d: no errors for high priority\n", Socket, Ch, Dimm));
    }
    return;
  }

  Data = DimmErrorLogData[Socket][Ch][Dimm][HIGH_LEVEL_LOG].DetectErrorData;
  RASDEBUG ((DEBUG_INFO, RAS_INFO_STR ("DDRT") "NvdimmHighPriorityError: S%d.C%d.D%d, DDRT Event Reg: 0x%08x\n", Socket, Ch, Dimm, Data));

  FwHang = DimmErrorLogData[Socket][Ch][Dimm][HIGH_LEVEL_LOG].ProcessFwHang;
  FwFatalErr = DimmErrorLogData[Socket][Ch][Dimm][HIGH_LEVEL_LOG].ProcessFwFatalErr;

  if (FwHang || FwFatalErr) {
    CreateMemErrRecord (Socket, Ch, Dimm, PLATFORM_MEM_ERROR_UNKNOWN, TRUE, NULL, FALSE, &MemRecord);

    Status = mPlatformErrorHandlingProtocol->LogReportedError (&MemRecord.Header);
    if (EFI_ERROR (Status)) {
      RASDEBUG ((DEBUG_ERROR, RAS_ERROR_STR ("DDRT") "Failed to report the memory error record (%r)\n", Status));
    }

    if (FwFatalErr) {
      mCrystalRidge->AcpiAddLeafNotification (Socket, Ch, Dimm);
    }
  }

  if (DimmErrorLogData[Socket][Ch][Dimm][HIGH_LEVEL_LOG].ProcessMediaErrLogData) {
    SequenceNumber = &DimmErrorLogData[Socket][Ch][Dimm][HIGH_LEVEL_LOG].MediaErrSeqFirst;
    RASDEBUG ((DEBUG_INFO, RAS_INFO_STR ("DDRT") "NvdimmHighPriorityError: S%d.C%d.D%d: ", Socket, Ch, Dimm));
    RASDEBUG ((DEBUG_INFO, "checking media log starting at sequence number %d\n", *SequenceNumber));
    for (LogsIndex = 0; LogsIndex < MAX_MEDIA_PAYLOADS; LogsIndex++) {
      if (LogsIndex > MAX_MEDIA_PAYLOADS / 2) {
        //
        // We should never have loop more times the max required loop. If so, turn on more debug messages
        //
        VerboseLevel = DEBUG_ERROR;
      }
      Status = DimmErrorLogData[Socket][Ch][Dimm][HIGH_LEVEL_LOG].MediaErrLogDataRsp[LogsIndex].ErrorMediaLogStatus;
      if (Status == EFI_NOT_FOUND || DimmErrorLogData[Socket][Ch][Dimm][HIGH_LEVEL_LOG].MediaErrLogDataRsp[LogsIndex].MediaErrLog.ReturnCount == 0) {
        //
        // This is the case when numbers of new media log is times of MAX_MEDIA_LOGS(3)
        //
        RASDEBUG ((VerboseLevel, RAS_INFO_STR ("DDRT") "NvdimmHighPriorityError: S%d.C%d.D%d: ", Socket, Ch, Dimm));
        RASDEBUG ((VerboseLevel, "no media logs found, new sequence number: %d\n",
                   DimmErrorLogData[Socket][Ch][Dimm][HIGH_LEVEL_LOG].MediaErrLogDataRsp[LogsIndex].NewSeqNumber));
        break;
      }

      if (EFI_ERROR (Status)) {
        //
        // We should break the while loop whenever there is error status and already retried
        //
        RASDEBUG ((DEBUG_ERROR, RAS_ERROR_STR ("DDRT") "NvdimmHighPriorityError: S%d.C%d.D%d: ", Socket, Ch, Dimm));
        RASDEBUG ((DEBUG_ERROR, "NvmCtlrGetErrorLog (..., MEDIA_LOG_REQ, ..., %d, ...) failed: %r\n", *SequenceNumber, Status));
        CreateMemErrRecord (Socket, Ch, Dimm, PLATFORM_MEM_ERROR_UNKNOWN, DdrtNvdimmFisErrorIsFatal (Status), NULL, FALSE, &MemRecord);
        Status = mPlatformErrorHandlingProtocol->LogReportedError (&MemRecord.Header);
        if (EFI_ERROR (Status)) {
          RASDEBUG ((DEBUG_ERROR, RAS_ERROR_STR ("DDRT") "Failed to report the memory error record (%r)\n", Status));
        }
        break;
      }

      RASDEBUG ((VerboseLevel, RAS_INFO_STR ("DDRT") "NvdimmHighPriorityError: S%d.C%d.D%d: ", Socket, Ch, Dimm));
      RASDEBUG ((VerboseLevel, "found %d media logs, new sequence number: %d\n",
                 DimmErrorLogData[Socket][Ch][Dimm][HIGH_LEVEL_LOG].MediaErrLogDataRsp[LogsIndex].MediaErrLog.ReturnCount,
                 DimmErrorLogData[Socket][Ch][Dimm][HIGH_LEVEL_LOG].MediaErrLogDataRsp[LogsIndex].NewSeqNumber));

      for (i = 0; i < DimmErrorLogData[Socket][Ch][Dimm][HIGH_LEVEL_LOG].MediaErrLogDataRsp[LogsIndex].MediaErrLog.ReturnCount && i < MAX_MEDIA_LOGS; i++) {
        MediaLogPtr = &DimmErrorLogData[Socket][Ch][Dimm][HIGH_LEVEL_LOG].MediaErrLogDataRsp[LogsIndex].MediaErrLog.MediaLogs[i];
        RASDEBUG ((VerboseLevel, RAS_INFO_STR ("DDRT") "NvdimmHighPriorityError: S%d.C%d.D%d: ", Socket, Ch, Dimm));
        RASDEBUG ((VerboseLevel, "media log: seq: %d, timestamp: %lld, error type %d\n",
                   MediaLogPtr->SequenceNumber,
                   MediaLogPtr->SysTimeStamp,
                   MediaLogPtr->ErrorType));
        RASDEBUG ((VerboseLevel, "                                                        "));
        RASDEBUG ((VerboseLevel, "error flags: 0x%2x, transaction type: %d\n",
                   MediaLogPtr->ErrorFlags,
                   MediaLogPtr->TransactionType));
        RASDEBUG ((VerboseLevel, "                                                        "));
        RASDEBUG ((VerboseLevel, "dpa: 0x%llx, pda: 0x%llx, range: %d\n",
                   MediaLogPtr->Dpa,
                   MediaLogPtr->Pda,
                   MediaLogPtr->Range));

        if (MediaLogPtr->ErrorType == FNV_ET_USER_SPARE_BLOCK_ALARM_TRIP ||
            MediaLogPtr->ErrorType == FNV_ET_SMART_HEALTH_STATUS_CHANGED) {
          //
          // SMART critical/fatal health status comes at high priority, notify NVDIMM device.
          //
          mCrystalRidge->AcpiAddLeafNotification (Socket, Ch, Dimm);
          continue;
        }
        if (!IsAlertEnabled(mRasSetup->Memory.HostAlerts[MediaLogPtr->TransactionType], MediaLogPtr->ErrorType)) {

          RASDEBUG ((DEBUG_ERROR, RAS_ERROR_STR ("DDRT") "Alert for transaction type 0x%X with error type 0x%X is not enabled, skip report\n", MediaLogPtr->TransactionType, MediaLogPtr->ErrorType));
          continue;
        }

        if (MediaLogPtr->ErrorType == FNV_ET_DATA_PATH_ERROR) {
          if (MediaLogPtr->TransactionType != FNV_TT_ADDRESS_RANGE_SCRUB) {
            mCrystalRidge->AcpiAddRootNotification (Socket, Ch, Dimm, MediaLogPtr->Dpa, MediaLogPtr->Range);
          }
        }

        switch (MediaLogPtr->TransactionType) {
          case FNV_TT_2LM_READ:
          case FNV_TT_2LM_WRITE:
            ErrorType = PLATFORM_MEM_ERROR_MLTIBIT_ECC;
            break;
          case FNV_TT_PM_READ:
          case FNV_TT_PM_WRITE:
            if (MediaLogPtr->ErrorType == FNV_ET_UNCORRECTABLE) {
              mCrystalRidge->AcpiAddToErrorList(Socket, Ch, Dimm, MediaLogPtr->Dpa, MediaLogPtr->Range);
            }
            ErrorType = PLATFORM_MEM_ERROR_MLTIBIT_ECC;
            break;
          case FNV_TT_PATROL_SCRUB:
            if (MediaLogPtr->ErrorFlags & BIT1) {
              mCrystalRidge->AcpiAddRootNotification (Socket, Ch, Dimm, MediaLogPtr->Dpa, MediaLogPtr->Range);
            }
            ErrorType = PLATFORM_MEM_ERROR_SCRUB_UNCOR;
            break;
          default:
            ErrorType = PLATFORM_MEM_ERROR_UNKNOWN;
            break;
        }
        RASDEBUG ((DEBUG_ERROR, RAS_INFO_STR ("DDRT") "Create correctable memory error log (type 0x%X)\n", ErrorType));

        //
        // Do UNC Poison explicitly for Read Transactions
        //
        if ((mRasSetup->Memory.DcpmmUncPoison) && (MediaLogPtr->ErrorType == FNV_ET_UNCORRECTABLE) && 
            ((MediaLogPtr->TransactionType == FNV_TT_2LM_READ) ||(MediaLogPtr->TransactionType == FNV_TT_PM_READ))) {

          RASDEBUG ((DEBUG_ERROR, RAS_INFO_STR ("DDRT") "MediaLogPtr->ErrorType = type 0x%X\n", MediaLogPtr->ErrorType));
          RASDEBUG ((DEBUG_ERROR, RAS_INFO_STR ("DDRT") "MediaLogPtr->TransactionType = type 0x%X\n", MediaLogPtr->TransactionType));

          Status = GetSpaFromDpa (Socket, Ch, Dimm, MediaLogPtr->Dpa, &Spa);
          if (EFI_ERROR (Status)) {
            RASDEBUG ((DEBUG_ERROR, RAS_ERROR_STR ("DDRT") "Failed to translate SPA from DPA (%r)\n", Status));
          }

          RASDEBUG ((DEBUG_ERROR, RAS_INFO_STR ("DDRT") "Calling AsmWriteFlushCacheLine on SPA 0x%lx\n", Spa));

          AsmWriteFlushCacheLine ((VOID*)(UINTN)Spa);
        }

        //
        // Only try to report SPA when 1. DPA valid bit is set AND 2. ReportSPA is enabled in setup and 3. DDRT alert is enabled for the transaction cross error type
        //
        if ((MediaLogPtr->ErrorFlags & BIT1) && (mRasSetup->Memory.ReportAlertSPA)) {
          Status = GetSpaFromDpa (Socket, Ch, Dimm, MediaLogPtr->Dpa, &Spa);
          if (EFI_ERROR (Status)) {
            RASDEBUG ((DEBUG_ERROR, RAS_ERROR_STR ("DDRT") "Failed to translate SPA from DPA (%r)\n", Status));
          }
          CreateMemErrRecord (Socket, Ch, Dimm, ErrorType, FALSE, !EFI_ERROR(Status) ? &Spa : NULL, FALSE, &MemRecord);
        } else {
          CreateMemErrRecord (Socket, Ch, Dimm, ErrorType, FALSE, NULL, FALSE, &MemRecord);
        }

        Status = mPlatformErrorHandlingProtocol->LogReportedError (&MemRecord.Header);
        if (EFI_ERROR (Status)) {
          RASDEBUG ((DEBUG_ERROR, RAS_ERROR_STR ("DDRT") "Failed to report the memory error record (%r)\n", Status));
        }
      }

      if (DimmErrorLogData[Socket][Ch][Dimm][HIGH_LEVEL_LOG].MediaErrLogDataRsp[LogsIndex].MediaErrLog.ReturnCount !=
          ARRAY_SIZE (DimmErrorLogData[Socket][Ch][Dimm][HIGH_LEVEL_LOG].MediaErrLogDataRsp[LogsIndex].MediaErrLog.MediaLogs)) {
        //
        // The only condition we want to continue reading is ReturnCount == MediaLogData array size
        //
        break;
      }
    }

    if (LogsIndex == MAX_MEDIA_PAYLOADS) {
      RASDEBUG ((DEBUG_ERROR, RAS_ERROR_STR ("DDRT") "NvdimmHighPriorityError: S%d.C%d.D%d: ", Socket, Ch, Dimm));
      RASDEBUG ((DEBUG_ERROR, "media log loop reached the limit.\n"));
    }
  }

  if (DimmErrorLogData[Socket][Ch][Dimm][HIGH_LEVEL_LOG].ProcessThermalErrLogData) {
    SequenceNumber = &DimmErrorLogData[Socket][Ch][Dimm][HIGH_LEVEL_LOG].ThermalErrSeqFirst;
    RASDEBUG ((DEBUG_INFO, RAS_INFO_STR ("DDRT") "NvdimmHighPriorityError: S%d.C%d.D%d: ", Socket, Ch, Dimm));
    RASDEBUG ((DEBUG_INFO, "checking thermal log starting at sequence number %d\n", *SequenceNumber));
    VerboseLevel = DEBUG_INFO;

    for (LogsIndex = 0; LogsIndex < MAX_THERMAL_PAYLOADS; LogsIndex++) {
      if (LogsIndex > MAX_THERMAL_PAYLOADS / 2) {
        //
        // We should never have loop more times the max required loop. If so, turn on more debug messages
        //
        VerboseLevel = DEBUG_ERROR;
      }

    Status = DimmErrorLogData[Socket][Ch][Dimm][HIGH_LEVEL_LOG].ThermalErrLogDataRsp[LogsIndex].ErrorThermalLogStatus;
      if (Status == EFI_NOT_FOUND || DimmErrorLogData[Socket][Ch][Dimm][HIGH_LEVEL_LOG].ThermalErrLogDataRsp[LogsIndex].ThermalErrLog.ReturnCount == 0) {
        RASDEBUG ((VerboseLevel, RAS_INFO_STR ("DDRT") "NvdimmHighPriorityError: S%d.C%d.D%d: ", Socket, Ch, Dimm));
        RASDEBUG ((VerboseLevel, "no thermal logs found, new sequence number: %d\n",
                   DimmErrorLogData[Socket][Ch][Dimm][HIGH_LEVEL_LOG].ThermalErrLogDataRsp[LogsIndex].NewSeqNumber));
        break;
      }

      if (EFI_ERROR (Status)) { //We should break the while loop whenever there is error status and already retried.
        RASDEBUG ((DEBUG_ERROR, RAS_INFO_STR ("DDRT") "NvdimmHighPriorityError: S%d.C%d.D%d: ", Socket, Ch, Dimm));
        RASDEBUG ((DEBUG_ERROR, "NvmCtlrGetErrorLog (..., THERMAL_LOG_REQ, ..., %d, ...) failed: %r\n", *SequenceNumber, Status));
        CreateMemErrRecord (Socket, Ch, Dimm, PLATFORM_MEM_ERROR_UNKNOWN, DdrtNvdimmFisErrorIsFatal (Status), NULL, FALSE, &MemRecord);
        mPlatformErrorHandlingProtocol->LogReportedError (&MemRecord.Header);
        break;
      }

      RASDEBUG ((VerboseLevel, RAS_INFO_STR ("DDRT") "NvdimmHighPriorityError: S%d.C%d.D%d: ", Socket, Ch, Dimm));
      RASDEBUG ((VerboseLevel, "found %d thermal log(s), new sequence number: %d\n",
                 DimmErrorLogData[Socket][Ch][Dimm][HIGH_LEVEL_LOG].ThermalErrLogDataRsp[LogsIndex].ThermalErrLog.ReturnCount,
                 DimmErrorLogData[Socket][Ch][Dimm][HIGH_LEVEL_LOG].ThermalErrLogDataRsp[LogsIndex].NewSeqNumber));

      for (i = 0; i < DimmErrorLogData[Socket][Ch][Dimm][HIGH_LEVEL_LOG].ThermalErrLogDataRsp[LogsIndex].ThermalErrLog.ReturnCount && i < MAX_THERMAL_LOGS; i++) {
        ThermalLogPtr = &DimmErrorLogData[Socket][Ch][Dimm][HIGH_LEVEL_LOG].ThermalErrLogDataRsp[LogsIndex].ThermalErrLog.ThermalLogs[i];
        RASDEBUG ((VerboseLevel, RAS_INFO_STR ("DDRT") "NvdimmHighPriorityError: S%d.C%d.D%d: ", Socket, Ch, Dimm));
        RASDEBUG ((VerboseLevel, "thermal log: seq: %d, timestamp: %lld, temp type %d\n",
                   ThermalLogPtr->SequenceNumber,
                   ThermalLogPtr->SysTimestamp,
                   ThermalLogPtr->TempType));
        RASDEBUG ((VerboseLevel, "                                                     "
                                 "temp: 0x%x, sign: %d, reported: %d\n",
                   ThermalLogPtr->Temperature,
                   ThermalLogPtr->Sign,
                   ThermalLogPtr->Reported));
        if (ThermalLogPtr->Reported == FNV_HRTD_REPORTED_USER_ALARM_TRIP) {
          mCrystalRidge->AcpiAddLeafNotification (Socket, Ch, Dimm);
          continue;
        }
      }

      if (DimmErrorLogData[Socket][Ch][Dimm][HIGH_LEVEL_LOG].ThermalErrLogDataRsp[LogsIndex].ThermalErrLog.ReturnCount !=
          ARRAY_SIZE (DimmErrorLogData[Socket][Ch][Dimm][HIGH_LEVEL_LOG].ThermalErrLogDataRsp[LogsIndex].ThermalErrLog.ThermalLogs)) {
        break;
      }
    }

    if (LogsIndex == MAX_THERMAL_PAYLOADS) {
      RASDEBUG ((DEBUG_ERROR, RAS_ERROR_STR ("DDRT") "NvdimmHighPriorityError: S%d.C%d.D%d: ", Socket, Ch, Dimm));
      RASDEBUG ((DEBUG_ERROR, "thermal log loop reached the limit.\n"));
    }
  }
  ZeroMem (&DimmErrorLogData[Socket][Ch][Dimm][HIGH_LEVEL_LOG], sizeof(DimmErrorLogData[Socket][Ch][Dimm][HIGH_LEVEL_LOG]));
}


/**
  @brief Handles DDRT Low Priority errors

  Refer Figure 5-2. DDRT Alert Hierarchy in
  Intel Optane DC Persistent Memory Module (DCPMM) Software
  RAS and Error Recovery Specification revision 1.0

  @param[in]  Socket    Socket index
  @param[in]  Ch        Channel index (at socket level)
  @param[in]  Dimm      DIMM index (within the channel)
 **/
VOID
NvdimmLowPriorityError (
  IN UINT8 Socket,
  IN UINT8 Ch,
  IN UINT8 Dimm
  )
{
  STATIC UINT32           VerboseLevel = DEBUG_INFO;
  UEFI_MEM_ERROR_RECORD   MemRecord;
  MEDIA_ERR_LOG           *MediaLogPtr;
  THERMAL_ERR_LOG         *ThermalLogPtr;
  EFI_STATUS              Status;
  UINT64                  Spa;
  UINT32                  Data;
  UINT16                  *SequenceNumber;
  UINT8                   i = 0;
  UINT8                   ErrorType;
  UINT8                   LogsIndex = 0;

  if (ApState[Socket] != AP_FINISHED) {
    RASDEBUG ((DEBUG_ERROR, RAS_ERROR_STR ("DDRT") "S%d.C%d.D%d: ApState status is not finished!\n", Socket, Ch, Dimm));
    return;
  }

  Status = DimmErrorLogData[Socket][Ch][Dimm][LOW_LEVEL_LOG].DetectErrorStatus;

  if (EFI_ERROR (Status)) {
    if (Status != EFI_NOT_FOUND) {
      RASDEBUG ((DEBUG_ERROR, RAS_ERROR_STR ("DDRT") "NvdimmLowPriorityError: S%d.C%d.D%d: failed to detect errors: %r\n", Socket, Ch, Dimm, Status));
    } else {
      RASDEBUG ((DEBUG_INFO, RAS_INFO_STR ("DDRT") "NvdimmLowPriorityError: S%d.C%d.D%d: no errors for low priority\n", Socket, Ch, Dimm));
    }
    return;
  }

  Data = DimmErrorLogData[Socket][Ch][Dimm][LOW_LEVEL_LOG].DetectErrorData;
  RASDEBUG ((DEBUG_INFO, RAS_INFO_STR ("DDRT") "NvdimmLowPriorityError: S%d.C%d.D%d, DDRT Event Reg: 0x%08x\n", Socket, Ch, Dimm, Data));

  if (DimmErrorLogData[Socket][Ch][Dimm][LOW_LEVEL_LOG].ProcessLongOp) {
    mCrystalRidge->NvmCtlrLongOpCompleteProcess (Socket, Ch, Dimm, &DimmErrorLogData[Socket][Ch][Dimm][LOW_LEVEL_LOG].LongOpRsp[0]);
    Status = DimmErrorLogData[Socket][Ch][Dimm][LOW_LEVEL_LOG].LongOpStatus;
    if (EFI_ERROR (Status)) {
      RASDEBUG ((DEBUG_ERROR, RAS_ERROR_STR ("DDRT") "Failed to handle long operation completion (%r)\n", Status));
      CreateMemErrRecord (Socket, Ch, Dimm, PLATFORM_MEM_ERROR_UNKNOWN, DdrtNvdimmFisErrorIsFatal (Status), NULL, FALSE, &MemRecord);

      Status = mPlatformErrorHandlingProtocol->LogReportedError (&MemRecord.Header);
      if (EFI_ERROR (Status)) {
        RASDEBUG ((DEBUG_ERROR, RAS_ERROR_STR ("DDRT") "Failed to report the memory error record (%r)\n", Status));
      }
    }
  }

  if (DimmErrorLogData[Socket][Ch][Dimm][LOW_LEVEL_LOG].ProcessMediaErrLogData) {
    SequenceNumber = &DimmErrorLogData[Socket][Ch][Dimm][LOW_LEVEL_LOG].MediaErrSeqFirst;
    RASDEBUG ((DEBUG_INFO, RAS_INFO_STR ("DDRT") "NvdimmLowPriorityError: S%d.C%d.D%d: ", Socket, Ch, Dimm));
    RASDEBUG ((DEBUG_INFO, "checking media log starting at sequence number %d\n", *SequenceNumber));
    for (LogsIndex = 0; LogsIndex < MAX_MEDIA_PAYLOADS; LogsIndex++) {
      if (LogsIndex > MAX_MEDIA_PAYLOADS / 2) {
        //
        // We should never have loop more times the max required loop. If so, turn on more debug messages
        //
        VerboseLevel = DEBUG_ERROR;
      }

      Status = DimmErrorLogData[Socket][Ch][Dimm][LOW_LEVEL_LOG].MediaErrLogDataRsp[LogsIndex].ErrorMediaLogStatus;
      if (Status == EFI_NOT_FOUND || DimmErrorLogData[Socket][Ch][Dimm][LOW_LEVEL_LOG].MediaErrLogDataRsp[LogsIndex].MediaErrLog.ReturnCount == 0) {
        //
        // This is the case when numbers of new media log is times of MAX_MEDIA_LOGS(3)
        //
        RASDEBUG ((VerboseLevel, RAS_INFO_STR ("DDRT") "NvdimmLowPriorityError: S%d.C%d.D%d: ", Socket, Ch, Dimm));
        RASDEBUG ((VerboseLevel, "no media logs found, new sequence number: %d\n",
                   DimmErrorLogData[Socket][Ch][Dimm][LOW_LEVEL_LOG].MediaErrLogDataRsp[LogsIndex].NewSeqNumber));
        break;
      }

      if (EFI_ERROR (Status)) {
        //
        // We should break the while loop whenever there is error status and already retried
        //
        RASDEBUG ((DEBUG_ERROR, RAS_ERROR_STR ("DDRT") "NvdimmLowPriorityError: S%d.C%d.D%d: ", Socket, Ch, Dimm));
        RASDEBUG ((DEBUG_ERROR, "NvmCtlrGetErrorLog (..., MEDIA_LOG_REQ, ..., %d, ...) failed: %r\n", *SequenceNumber, Status));
        CreateMemErrRecord (Socket, Ch, Dimm, PLATFORM_MEM_ERROR_UNKNOWN, DdrtNvdimmFisErrorIsFatal (Status), NULL, FALSE, &MemRecord);
        Status = mPlatformErrorHandlingProtocol->LogReportedError (&MemRecord.Header);
        if (EFI_ERROR (Status)) {
          RASDEBUG ((DEBUG_ERROR, RAS_ERROR_STR ("DDRT") "Failed to report the memory error record (%r)\n", Status));
        }
        break;
      }

     RASDEBUG ((VerboseLevel, RAS_INFO_STR ("DDRT") "NvdimmLowPriorityError: S%d.C%d.D%d: ", Socket, Ch, Dimm));
     RASDEBUG ((VerboseLevel, "found %d media logs, new sequence number: %d\n",
                DimmErrorLogData[Socket][Ch][Dimm][LOW_LEVEL_LOG].MediaErrLogDataRsp[LogsIndex].MediaErrLog.ReturnCount,
                DimmErrorLogData[Socket][Ch][Dimm][LOW_LEVEL_LOG].MediaErrLogDataRsp[LogsIndex].NewSeqNumber));

      for (i = 0; i < DimmErrorLogData[Socket][Ch][Dimm][LOW_LEVEL_LOG].MediaErrLogDataRsp[LogsIndex].MediaErrLog.ReturnCount && i < MAX_MEDIA_LOGS; i++) {
        MediaLogPtr = &DimmErrorLogData[Socket][Ch][Dimm][LOW_LEVEL_LOG].MediaErrLogDataRsp[LogsIndex].MediaErrLog.MediaLogs[i];
        RASDEBUG ((VerboseLevel, RAS_INFO_STR ("DDRT") "NvdimmLowPriorityError: S%d.C%d.D%d: ", Socket, Ch, Dimm));
        RASDEBUG ((VerboseLevel, "media log: seq: %d, timestamp: %lld, error type %d\n",
                   MediaLogPtr->SequenceNumber,
                   MediaLogPtr->SysTimeStamp,
                   MediaLogPtr->ErrorType));
        RASDEBUG ((VerboseLevel, "                                                        "));
        RASDEBUG ((VerboseLevel, "error flags: 0x%2x, transaction type: %d\n",
                   MediaLogPtr->ErrorFlags,
                   MediaLogPtr->TransactionType));
        RASDEBUG ((VerboseLevel, "                                                        "));
        RASDEBUG ((VerboseLevel, "dpa: 0x%llx, pda: 0x%llx, range: %d\n",
                   MediaLogPtr->Dpa,
                   MediaLogPtr->Pda,
                   MediaLogPtr->Range));

        if (MediaLogPtr->ErrorType == FNV_ET_USER_SPARE_BLOCK_ALARM_TRIP ||
            MediaLogPtr->ErrorType == FNV_ET_SMART_HEALTH_STATUS_CHANGED) {
          //
          // Non-critical SMART health status comes at low priority, notify NVDIMM device.
          //
          mCrystalRidge->AcpiAddLeafNotification (Socket, Ch, Dimm);
          continue;
        }
        if (!IsAlertEnabled (mRasSetup->Memory.HostAlerts[MediaLogPtr->TransactionType], MediaLogPtr->ErrorType)) {

          RASDEBUG ((DEBUG_ERROR, RAS_ERROR_STR ("DDRT") "Alert for transaction type 0x%X with error type 0x%X is not enabled, skip report\n", MediaLogPtr->TransactionType, MediaLogPtr->ErrorType));
          continue;
        }

        if (MediaLogPtr->ErrorType == FNV_ET_DATA_PATH_ERROR) {
          if (MediaLogPtr->TransactionType != FNV_TT_ADDRESS_RANGE_SCRUB) {
            mCrystalRidge->AcpiAddRootNotification (Socket, Ch, Dimm, MediaLogPtr->Dpa, MediaLogPtr->Range);
          }
        }

        switch (MediaLogPtr->TransactionType) {
          case FNV_TT_2LM_READ:
          case FNV_TT_2LM_WRITE:
            ErrorType = PLATFORM_MEM_ERROR_MLTIBIT_ECC;
            break;
          case FNV_TT_PM_READ:
          case FNV_TT_PM_WRITE:
            if (MediaLogPtr->ErrorType == FNV_ET_UNCORRECTABLE) {
              mCrystalRidge->AcpiAddToErrorList(Socket, Ch, Dimm, MediaLogPtr->Dpa, MediaLogPtr->Range);
            }
            ErrorType = PLATFORM_MEM_ERROR_MLTIBIT_ECC;
            break;
          case FNV_TT_PATROL_SCRUB:
            if (MediaLogPtr->ErrorFlags & BIT1) {
              mCrystalRidge->AcpiAddRootNotification (Socket, Ch, Dimm, MediaLogPtr->Dpa, MediaLogPtr->Range);
            }
            ErrorType = PLATFORM_MEM_ERROR_SCRUB_UNCOR;
            break;
          default:
            ErrorType = PLATFORM_MEM_ERROR_UNKNOWN;
            break;
        }
        RASDEBUG ((DEBUG_ERROR, RAS_INFO_STR ("DDRT") "Create correctable memory error log (type 0x%X)\n", ErrorType));

        //
        // Do UNC Poison explicitly for Read Transactions
        //
        if ((mRasSetup->Memory.DcpmmUncPoison) && (MediaLogPtr->ErrorType == FNV_ET_UNCORRECTABLE) && 
           ((MediaLogPtr->TransactionType == FNV_TT_2LM_READ) ||(MediaLogPtr->TransactionType == FNV_TT_PM_READ))) {

          RASDEBUG ((DEBUG_ERROR, RAS_INFO_STR ("DDRT") "MediaLogPtr->ErrorType = type 0x%X\n", MediaLogPtr->ErrorType));
          RASDEBUG ((DEBUG_ERROR, RAS_INFO_STR ("DDRT") "MediaLogPtr->TransactionType = type 0x%X\n", MediaLogPtr->TransactionType));

          Status = GetSpaFromDpa (Socket, Ch, Dimm, MediaLogPtr->Dpa, &Spa);
          if (EFI_ERROR (Status)) {
            RASDEBUG ((DEBUG_ERROR, RAS_ERROR_STR ("DDRT") "Failed to translate SPA from DPA (%r)\n", Status));
          }

          RASDEBUG ((DEBUG_ERROR, RAS_INFO_STR ("DDRT") "Calling AsmWriteFlushCacheLine on SPA 0x%lx\n", Spa));

          AsmWriteFlushCacheLine ((VOID*)(UINTN)Spa);
        }

        //
        // Only try to report SPA when 1. DPA valid bit is set AND 2. ReportSPA is enabled in setup and 3. DDRT alert is enabled for the transaction cross error type
        //
        if ((MediaLogPtr->ErrorFlags & BIT1) && (mRasSetup->Memory.ReportAlertSPA)) {
          Status = GetSpaFromDpa (Socket, Ch, Dimm, MediaLogPtr->Dpa, &Spa);
          if (EFI_ERROR (Status)) {
            RASDEBUG ((DEBUG_ERROR, RAS_ERROR_STR ("DDRT") "Failed to translate SPA from DPA (%r)\n", Status));
          }
          CreateMemErrRecord (Socket, Ch, Dimm, ErrorType, FALSE, !EFI_ERROR (Status) ? &Spa : NULL, FALSE, &MemRecord);
        } else {
          CreateMemErrRecord (Socket, Ch, Dimm, ErrorType, FALSE, NULL, FALSE, &MemRecord);
        }

        Status = mPlatformErrorHandlingProtocol->LogReportedError (&MemRecord.Header);
        if (EFI_ERROR (Status)) {
          RASDEBUG ((DEBUG_ERROR, RAS_ERROR_STR ("DDRT") "Failed to report the memory error record (%r)\n", Status));
        }
      }

      if (DimmErrorLogData[Socket][Ch][Dimm][LOW_LEVEL_LOG].MediaErrLogDataRsp[LogsIndex].MediaErrLog.ReturnCount !=
          ARRAY_SIZE (DimmErrorLogData[Socket][Ch][Dimm][LOW_LEVEL_LOG].MediaErrLogDataRsp[LogsIndex].MediaErrLog.MediaLogs)) {
        //
        // The only condition we want to continue reading is ReturnCount == MediaLogData array size
        //
        break;
      }
    }
    if (LogsIndex == MAX_MEDIA_PAYLOADS) {
       RASDEBUG ((DEBUG_ERROR, RAS_INFO_STR ("DDRT") "NvdimmLowPriorityError: S%d.C%d.D%d: ", Socket, Ch, Dimm));
       RASDEBUG ((DEBUG_ERROR, "media log loop reached the limit.\n"));
    }
  }

  if (DimmErrorLogData[Socket][Ch][Dimm][LOW_LEVEL_LOG].ProcessThermalErrLogData) {
    SequenceNumber = &DimmErrorLogData[Socket][Ch][Dimm][LOW_LEVEL_LOG].ThermalErrSeqFirst;
    RASDEBUG ((DEBUG_INFO, RAS_INFO_STR ("DDRT") "NvdimmLowPriorityError: S%d.C%d.D%d: ", Socket, Ch, Dimm));
    RASDEBUG ((DEBUG_INFO, "checking thermal log starting at sequence number %d\n", *SequenceNumber));
    VerboseLevel = DEBUG_INFO;

    for (LogsIndex = 0; LogsIndex < MAX_THERMAL_PAYLOADS; LogsIndex++) {
      if (LogsIndex > MAX_THERMAL_PAYLOADS / 2) {
         //
         // We should never have loop more times the max required loop. If so, turn on more debug messages
         //
         VerboseLevel = DEBUG_ERROR;
      }
      Status = DimmErrorLogData[Socket][Ch][Dimm][LOW_LEVEL_LOG].ThermalErrLogDataRsp[LogsIndex].ErrorThermalLogStatus;

      if (Status == EFI_NOT_FOUND || DimmErrorLogData[Socket][Ch][Dimm][LOW_LEVEL_LOG].ThermalErrLogDataRsp[LogsIndex].ThermalErrLog.ReturnCount == 0) {
        RASDEBUG ((VerboseLevel, RAS_INFO_STR ("DDRT") "NvdimmLowPriorityError: S%d.C%d.D%d: ", Socket, Ch, Dimm));
        RASDEBUG ((VerboseLevel, "no thermal logs found, new sequence number: %d\n",
                   DimmErrorLogData[Socket][Ch][Dimm][LOW_LEVEL_LOG].ThermalErrLogDataRsp[LogsIndex].NewSeqNumber));
        break;
      }

      if (EFI_ERROR (Status)) { //We should break the while loop whenever there is error status and already retried.
        RASDEBUG ((DEBUG_ERROR, RAS_INFO_STR ("DDRT") "NvdimmLowPriorityError: S%d.C%d.D%d: ", Socket, Ch, Dimm));
        RASDEBUG ((DEBUG_ERROR, "NvmCtlrGetErrorLog (..., THERMAL_LOG_REQ, ..., %d, ...) failed: %r\n", *SequenceNumber, Status));
        CreateMemErrRecord (Socket, Ch, Dimm, PLATFORM_MEM_ERROR_UNKNOWN, DdrtNvdimmFisErrorIsFatal (Status), NULL, FALSE, &MemRecord);
        mPlatformErrorHandlingProtocol->LogReportedError (&MemRecord.Header);
        break;
      }

      RASDEBUG ((VerboseLevel, RAS_INFO_STR ("DDRT") "NvdimmLowPriorityError: S%d.C%d.D%d: ", Socket, Ch, Dimm));
      RASDEBUG ((VerboseLevel, "found %d thermal log(s), new sequence number: %d\n",
                 DimmErrorLogData[Socket][Ch][Dimm][LOW_LEVEL_LOG].ThermalErrLogDataRsp[LogsIndex].ThermalErrLog.ReturnCount,
                 DimmErrorLogData[Socket][Ch][Dimm][LOW_LEVEL_LOG].ThermalErrLogDataRsp[LogsIndex].NewSeqNumber));

      for (i = 0; i < DimmErrorLogData[Socket][Ch][Dimm][LOW_LEVEL_LOG].ThermalErrLogDataRsp[LogsIndex].ThermalErrLog.ReturnCount && i < MAX_THERMAL_LOGS; i++) {
        ThermalLogPtr = &DimmErrorLogData[Socket][Ch][Dimm][LOW_LEVEL_LOG].ThermalErrLogDataRsp[LogsIndex].ThermalErrLog.ThermalLogs[i];
        RASDEBUG ((VerboseLevel, RAS_INFO_STR ("DDRT") "NvdimmLowPriorityError: S%d.C%d.D%d: ", Socket, Ch, Dimm));
        RASDEBUG ((VerboseLevel, "thermal log: seq: %d, timestamp: %lld, temp type %d\n",
                   ThermalLogPtr->SequenceNumber,
                   ThermalLogPtr->SysTimestamp,
                   ThermalLogPtr->TempType));
        RASDEBUG ((VerboseLevel, "                                                     "
                                 "temp: 0x%x, sign: %d, reported: %d\n",
                   ThermalLogPtr->Temperature,
                   ThermalLogPtr->Sign,
                   ThermalLogPtr->Reported));
        if (ThermalLogPtr->Reported == FNV_HRTD_REPORTED_USER_ALARM_TRIP) {
          mCrystalRidge->AcpiAddLeafNotification (Socket, Ch, Dimm);
          continue;
        }
      }

      if (DimmErrorLogData[Socket][Ch][Dimm][LOW_LEVEL_LOG].ThermalErrLogDataRsp[LogsIndex].ThermalErrLog.ReturnCount !=
          ARRAY_SIZE (DimmErrorLogData[Socket][Ch][Dimm][LOW_LEVEL_LOG].ThermalErrLogDataRsp[LogsIndex].ThermalErrLog.ThermalLogs)) {
        break;
      }
    } // for (LogsIndex...)

    if (LogsIndex == MAX_THERMAL_PAYLOADS) {
      RASDEBUG ((DEBUG_ERROR, RAS_INFO_STR ("DDRT") "NvdimmLowPriorityError: S%d.C%d.D%d: ", Socket, Ch, Dimm));
      RASDEBUG ((DEBUG_ERROR, "thermal log loop reached the limit.\n"));
    }
  }
  ZeroMem (&DimmErrorLogData[Socket][Ch][Dimm][LOW_LEVEL_LOG], sizeof(DimmErrorLogData[Socket][Ch][Dimm][LOW_LEVEL_LOG]));
}


/**
  DDDRT Error Handler

  @retval EFI_SUCCESS - Error Handler performed successfully
**/
EFI_STATUS
EFIAPI
DdrtDetectAndHandle (
  UINT32      *Severity,
  ClearOption  Clear
  )
{
  UINT8                      Socket;
  UINT8                      Ch;         // at socket level
  UINT8                      Dimm;       // with in channel
  EFI_STATUS                 Status;
  UEFI_MEM_ERROR_RECORD      MemRecord;
  UINT8                      Skt = 0;

  if (!mCrystalRidge->CheckAllApCheckedIn()) {
      CreateMemErrRecord (0, 0, 0, PLATFORM_MEM_ERROR_UNKNOWN, FALSE, NULL, TRUE, &MemRecord);
      mPlatformErrorHandlingProtocol->LogReportedError (&MemRecord.Header);
      return EFI_SUCCESS;
  }

  if (!mDdrtErrorsEnabled) {
    RASDEBUG ((DEBUG_INFO, RAS_INFO_STR ("DDRT") "DdrtDetectAndHandle : NVMCTLR errors Not enabled\n"));
    return EFI_SUCCESS;
  }

  mCrystalRidge->AcpiClearLeafNotifications ();
  mCrystalRidge->AcpiClearRootNotification ();

  Status = RetrieveEventDataFromAllDimms(&Skt);

  if (EFI_ERROR (Status)) {
    CreateMemErrRecord (Skt, 0, 0, PLATFORM_MEM_ERROR_UNKNOWN, TRUE, NULL, TRUE, &MemRecord);
    mPlatformErrorHandlingProtocol->LogReportedError (&MemRecord.Header);
  }

  for (Socket = 0 ; Socket < MAX_SOCKET; Socket++) {
    for (Ch = 0; Ch < MAX_CH; Ch++) {
      for (Dimm = 0 ; Dimm < MAX_DIMM; Dimm++) {
        if (mCrystalRidge->IsNvmDimm(Socket, Ch, Dimm)) {
          NvdimmHighPriorityError (Socket, Ch, Dimm);
          NvdimmLowPriorityError (Socket, Ch, Dimm);
        }
      }
    }
    ApState[Socket] = AP_STOP;
  }
  mCrystalRidge->AcpiLeafNotify ();
  mCrystalRidge->AcpiRootNotify ();

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
DdrtClearStatus (
  VOID
  )
{
  UINT8                         Socket;
  UBOXERRSTS_UBOX_CFG_STRUCT    UboxErrStsReg;
  UBOXERRMISC_UBOX_CFG_STRUCT   UboxErrMisc;
  UBOXERRMISC2_UBOX_CFG_STRUCT  UboxErrMisc2;

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (mRasTopology->SystemInfo.SocketInfo[Socket].Valid == 0) {
      continue;
    }

    UboxErrStsReg.Data = mCpuCsrAccess->ReadCpuCsr (Socket, 0, UBOXERRSTS_UBOX_CFG_REG);
    UboxErrStsReg.Bits.poisonrsvd = 0;

    UboxErrStsReg.Bits.smisrcimc = 0;
    UboxErrStsReg.Bits.unsupportedopcode = 0;
    UboxErrStsReg.Bits.smi_delivery_valid  = 0;
    mCpuCsrAccess->WriteCpuCsr (Socket, 0, UBOXERRSTS_UBOX_CFG_REG, UboxErrStsReg.Data);

    UboxErrMisc.Data = 0;
    mCpuCsrAccess->WriteCpuCsr (Socket, 0, UBOXERRMISC_UBOX_CFG_REG, UboxErrMisc.Data);

    UboxErrMisc2.Data = 0;
    mCpuCsrAccess->WriteCpuCsr (Socket, 0, UBOXERRMISC2_UBOX_CFG_REG, UboxErrMisc2.Data);
  }

  RASDEBUG ((DEBUG_INFO, RAS_INFO_STR ("DDRT") "UBox SMI source cleared\n"));
  mClearSmiSrc = FALSE;
  return EFI_SUCCESS;
}

/**
  Entry point for the Processor Handler initialization.

  This function initializes the error handling and enables error
  sources for the CPU Core.


  @param[in] ImageHandle          Image handle of this driver.
  @param[in] SystemTable          Global system service table.

  @retval Status.

**/
EFI_STATUS
EFIAPI
DdrtErrorHandlerEntryPoint (
  IN EFI_HANDLE             ImageHandle,
  IN EFI_SYSTEM_TABLE       *SystemTable
  )
{
  EFI_STATUS                Status;

  RASDEBUG ((DEBUG_INFO, RAS_INFO_STR ("DDRT") "Initialize DDRT error handler\n"));

  Status = gSmst->SmmLocateProtocol (&gEfiErrorHandlingProtocolGuid, NULL, &mErrorHandlingProtocol);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  Status = gSmst->SmmLocateProtocol (&gEfiPlatformErrorHandlingProtocolGuid, NULL, &mPlatformErrorHandlingProtocol);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  Status = gSmst->SmmLocateProtocol (&gEfiPlatformRasPolicyProtocolGuid, NULL, &mPlatformRasPolicyProtocol);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  Status = gSmst->SmmLocateProtocol (&gEfiMemRasProtocolGuid, NULL, &mMemRasProtocol);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  mRasTopology = mPlatformRasPolicyProtocol->EfiRasSystemTopology;
  mRasSetup = mPlatformRasPolicyProtocol->SystemRasSetup;

  Status = gSmst->SmmLocateProtocol (&gEfiCpuCsrAccessGuid, NULL, &mCpuCsrAccess);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  Status = gSmst->SmmLocateProtocol (&gEfiCrystalRidgeSmmGuid, NULL, &mCrystalRidge);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  GetPerformanceCounterProperties (NULL, NULL);

  Status = InitializeErrorLogData();

  if (EFI_ERROR(Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  Status = DdrtEnableError();
  if (!EFI_ERROR (Status)) {

    mDdrtReporter.IsSupported = DdrtIsSupported;
    mDdrtReporter.EnableReporting = DdrtEnableError;
    mDdrtReporter.DisableReporting = DdrtDisableError;
    mDdrtReporter.CheckStatus = DdrtCheckStatus;
    mDdrtReporter.ClearStatus = DdrtClearStatus;
    mDdrtReporter.DetectAndHandle = DdrtDetectAndHandle;
    //
    // Register the MCA Error Handler
    //
    Status = mErrorHandlingProtocol->RegisterHandler (&mDdrtReporter, HighPriority);
    ASSERT_EFI_ERROR (Status);
  }

  return EFI_SUCCESS;
}

