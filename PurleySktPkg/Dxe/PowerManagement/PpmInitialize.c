//
// This file contains an 'Intel Peripheral Driver' and is
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may
// be modified by the user, subject to additional terms of the
// license agreement
//
/** @file

  Code File for CPU Power Management

  Copyright (c) 2006 - 2019, Intel Corporation. All rights reserved.<BR>
  This software and associated documentation (if any) is furnished
  under a license and may only be used or copied in accordance
  with the terms of the license. Except as permitted by such
  license, no part of this software or documentation may be
  reproduced, stored in a retrieval system, or transmitted in any
  form or by any means without the express written consent of
  Intel Corporation.

  Module Name:  PpmInitialize.c

**/

#include "PpmInitialize.h"
#include <Library/PlatformHooksLib.h> //SKX_TODO for IsSimicsPlatform() will remove when Simic fixIsSimicsPlatform
#include <Protocol/DxeSmmReadyToLock.h>
#include <Protocol/CpuPpmProtocol.h>
#include <Library/CpuConfigLib.h>
#include <Library/S3BootScriptLib.h>
#include <Guid/MemoryMapData.h>
#define   B_LOCK_DESIRED_CORES_CSR  BIT31

EFI_CPU_PPM_PROTOCOL          mCpuPpmProtocol;

SPIN_LOCK            CpuPmSpinLock;
EFI_CPU_PM_STRUCT    mPpmLib ={ 0, 0, 0, 0 };

IIO_UDS              *UdsHobPtr;

VOID
EFIAPI
CpuPmProgramMsrs (
  EFI_CPU_PM_STRUCT *ppm
  );

VOID InitCpuPPMLib (
  VOID
  );

VOID   CpuPmProgramCsrs (
  VOID
  );

EFI_STATUS
CpuPpmInitDone (
  VOID
  );

VOID
EnableOsrProgramming (
  IN SYSTEM_MEMORY_MAP_HOB  *SystemMemoryMap
  );

EFI_STATUS
EFIAPI
EnableOSR (
  VOID
  );

VOID
EFIAPI
PpmOnReadyToBoot (
  IN      EFI_EVENT                 Event,
  IN      VOID                      *Context
  );

/**

  The function puts MSR programing and P state transition together to improve boot performance.

  Arguments: None

  @retval EFI_SUCCESS

**/
EFI_STATUS
EFIAPI
CpuPmMpInitialize (
  VOID
  )
{
  CpuPmProgramMsrs((EFI_CPU_PM_STRUCT *)&mPpmLib);

  //
  // Switch to max non-turbo ratio if user choose
  //
  PStateTransition((EFI_CPU_PM_STRUCT *)&mPpmLib);

  return EFI_SUCCESS;
}

/**

  The function puts EnableAutonomousCStateControl() and PpmSetMsrCstConfigCtrlLockBit() together to improve boot performance.

  Arguments: None

  @retval EFI_SUCCESS

**/
EFI_STATUS
EFIAPI
CpuPmMpInitDone (
  VOID
  )
{
  UINTN                       ProcessorNumber;
  EFI_STATUS                  Status;
  UINT32                      ThreadNumber;

  Status = mMpService->WhoAmI (
                mMpService,
                &ProcessorNumber
                );
  ASSERT_EFI_ERROR (Status);

  GetProcessorLocation (
    ProcessorNumber,
    NULL,
    NULL,
    &ThreadNumber
    );

  //
  // Limit core scoped MSR manipulation to thread 0 for each core
  //
  if (ThreadNumber == 0) {
    //
    // Enables Autonomous Cstate feature by setting MSR_PMG_CST_CONFIG_CONTROL Bit[16]
    //
    EnableAutonomousCStateControl((EFI_CPU_PM_STRUCT *)&mPpmLib);

    //
    // Set LOCK bit in MSR_PMG_CST_CONFIG_CONTROL Bit[15]
    //
    PpmSetMsrCstConfigCtrlLockBit((EFI_CPU_PM_STRUCT *)&mPpmLib);
  }

  return EFI_SUCCESS;
}

/**

  Set BIOS init done to enable PPM features.

  @param PPMPolicy Pointer to PPM Policy protocol instance

  @retval EFI_SUCCESS
**/
EFI_STATUS
CpuPpmInitDone (
    VOID
    )
{
  UINT8                       SocketNumber;
  UINT8                       Index;
  UINTN                       ProcessorId;
  EFI_PROCESSOR_INFORMATION   ProcInfo;
  UINTN                       NumberOfProcessors;
  UINTN                       NumberEnabledProcessors;
  UINT16                      PackageDoneBitmap;

  mPpmLib.CpuCsrAccess->BreakAtCheckpoint (STS_PPM_ON_READY_TO_BOOT, 0, 0xFF);
  //Fix HSD 4986378: PROMOTE from haswell_server: BIOS 18D15 fails to achieve package C6 under Red Hat
  if ((mPpmInfo->CpuType == CPU_FAMILY_HSX) || (mPpmInfo->CpuType == CPU_FAMILY_SKX)) {
    //Because IioInit driver may be after PPM driver
    //Set PCIE PkgCstEntryCriteria before hand off to OS
    for (SocketNumber = 0; SocketNumber < MAX_SOCKET; SocketNumber++) {
      if (mPpmInfo->SocketPresentBitMap & (1 << SocketNumber)){
        mPpmLib.Info->SocketNumber = SocketNumber;
        SetupPCIEPkgCstEntryCriteria((EFI_CPU_PM_STRUCT   *)&mPpmLib);
      }
    }
  }

  SocketNumber = 0;
  PackageDoneBitmap = 0;
  mMpService->GetNumberOfProcessors(
                  mMpService,
                  &NumberOfProcessors,
                  &NumberEnabledProcessors
                  );

  //
  // Count number of sockets present
  //
  for(Index = 0; Index < MAX_SOCKET; Index++){
    if(mPpmInfo->SocketPresentBitMap & (1 << Index)) {
      SocketNumber++;
    }
  }

  //
  // Set Package MSR lock bits for each socket
  //
  for(ProcessorId = 0; ProcessorId < NumberOfProcessors; ProcessorId++){
    mMpService->GetProcessorInfo(
                    mMpService,
                    ProcessorId,
                    &ProcInfo
                    );
    //
    // Only call PpmSetMsrLockBit on each socket one time
    //
    if(!(PackageDoneBitmap & (1 << ProcInfo.Location.Package)) && (ProcInfo.StatusFlag & PROCESSOR_ENABLED_BIT)){
      PackageDoneBitmap |= (1 << ProcInfo.Location.Package);
      SocketNumber--;
      if(ProcInfo.StatusFlag & PROCESSOR_AS_BSP_BIT) {
        PpmSetMsrLockBit((EFI_CPU_PM_STRUCT *)&mPpmLib);
      } else {
        mMpService->StartupThisAP(mMpService,
                                  PpmSetMsrLockBit,
                                  ProcessorId,
                                  NULL,
                                  0,
                                  (VOID *)&mPpmLib,
                                  NULL
                                  );
      }
      //
      // If we have reached the last socket, break loop
      //
      if(SocketNumber == 0){
        break;
      }
    }
  }

  //
  // Set CSR lock bits on all processors.
  //
  //DEBUG ( (EFI_D_ERROR, ":: Set LOCK bit in CSR 1:30:1:0xb0 & 1:30:2:0xc8...\n") );
  for (SocketNumber = 0; SocketNumber < MAX_SOCKET; SocketNumber++) {
    if (mPpmInfo->SocketPresentBitMap & (1 << SocketNumber)){
      mPpmLib.Info->SocketNumber = SocketNumber;
      PpmSetCsrLockBit((EFI_CPU_PM_STRUCT *)&mPpmLib);
    }
  }

  //
  // Set BIOS Init Done on all processors.
  //
  for (SocketNumber = 0; SocketNumber < MAX_SOCKET; SocketNumber++) {
    if (mPpmInfo->SocketPresentBitMap & (1 << SocketNumber)){
      mPpmLib.Info->SocketNumber = SocketNumber;
      PpmSetBiosInitDone((EFI_CPU_PM_STRUCT *)&mPpmLib);
    }
  }

  mMpService->StartupAllAPs(mMpService,
                            (EFI_AP_PROCEDURE) CpuPmMpInitDone,
                            FALSE,
                            NULL,
                            0,
                            NULL,
                            NULL
                            );

  CpuPmMpInitDone ();

  return EFI_SUCCESS;

}


/**

  Enables Opportunistic Self-Refresh per channel.

  @param[in] *SystemMemoryMap - Pointer to system memory map struct

  @retval None
**/
VOID
EnableOsrProgramming (
    IN SYSTEM_MEMORY_MAP_HOB  *SystemMemoryMap
    )
{
  UINT64                       RegAddr = 0;
  UINT8                        Size = 4;
  UINT8                        SocketNumber;
  UINT8                        Channel;
  SREF_LL0_MCDDC_CTL_STRUCT    sref_ll0;
  SREF_LL1_MCDDC_CTL_STRUCT    sref_ll1;
  SREF_LL2_MCDDC_CTL_STRUCT    sref_ll2;
  SREF_LL3_MCDDC_CTL_STRUCT    sref_ll3;
  EFI_CPU_CSR_ACCESS_PROTOCOL  *CpuCsrAccess;

  CpuCsrAccess = mPpmLib.CpuCsrAccess;

  for (SocketNumber = 0; SocketNumber < MAX_SOCKET; SocketNumber++) {
    if ((mPpmInfo->SocketPresentBitMap & (1 << SocketNumber)) == 0){
        continue;
    }

    for (Channel = 0; Channel < MAX_CH; Channel++) {
        if (SystemMemoryMap->Socket[SocketNumber].ChannelInfo[Channel].Enabled == 0) {
            continue;
        }

        // SREF_LL0_MCDDC_CTL_REG - Self Refresh controls at Load Line point 0
        sref_ll0.Data = CpuCsrAccess->ReadCpuCsr(SocketNumber, Channel, SREF_LL0_MCDDC_CTL_REG);
        sref_ll0.Bits.opp_sref_en = 1;
        CpuCsrAccess->WriteCpuCsr(SocketNumber, Channel, SREF_LL0_MCDDC_CTL_REG, sref_ll0.Data);
        RegAddr = CpuCsrAccess->GetCpuCsrAddress(SocketNumber, Channel, SREF_LL0_MCDDC_CTL_REG, &Size);
        S3BootScriptSaveMemWrite (S3BootScriptWidthUint32, RegAddr, 1, &sref_ll0.Data );

         // SREF_LL1_MCDDC_CTL_REG - Self Refresh controls at Load Line point 1
        sref_ll1.Data = CpuCsrAccess->ReadCpuCsr(SocketNumber, Channel, SREF_LL1_MCDDC_CTL_REG);
        sref_ll1.Bits.opp_sref_en = 1;
        CpuCsrAccess->WriteCpuCsr(SocketNumber, Channel, SREF_LL1_MCDDC_CTL_REG, sref_ll1.Data);
        RegAddr = CpuCsrAccess->GetCpuCsrAddress(SocketNumber, Channel, SREF_LL1_MCDDC_CTL_REG, &Size);
        S3BootScriptSaveMemWrite (S3BootScriptWidthUint32, RegAddr, 1, &sref_ll1.Data );

        // SREF_LL2_MCDDC_CTL_REG - Self Refresh controls at Load Line point 2
        sref_ll2.Data = CpuCsrAccess->ReadCpuCsr(SocketNumber, Channel, SREF_LL2_MCDDC_CTL_REG);
        sref_ll2.Bits.opp_sref_en = 1;
        CpuCsrAccess->WriteCpuCsr(SocketNumber, Channel, SREF_LL2_MCDDC_CTL_REG, sref_ll2.Data);
        RegAddr = CpuCsrAccess->GetCpuCsrAddress(SocketNumber, Channel, SREF_LL2_MCDDC_CTL_REG, &Size);
        S3BootScriptSaveMemWrite (S3BootScriptWidthUint32, RegAddr, 1, &sref_ll2.Data );

        // SREF_LL3_MCDDC_CTL_REG - Self Refresh controls at Load Line point 3
        sref_ll3.Data = CpuCsrAccess->ReadCpuCsr(SocketNumber, Channel, SREF_LL3_MCDDC_CTL_REG);
        sref_ll3.Bits.opp_sref_en = 1;
        CpuCsrAccess->WriteCpuCsr(SocketNumber, Channel, SREF_LL3_MCDDC_CTL_REG, sref_ll3.Data);
        RegAddr = CpuCsrAccess->GetCpuCsrAddress(SocketNumber, Channel, SREF_LL3_MCDDC_CTL_REG, &Size);
        S3BootScriptSaveMemWrite (S3BootScriptWidthUint32, RegAddr, 1, &sref_ll3.Data );

    } // Channel loop
  } // Socket loop

} // EnableOsrProgramming


/**

  Opportunistic Self-Refresh (OSR) cannot be enabled before CPL3/4 because it requres
  FIRV Ramp to be disabled which happens after CPL3/4. This function enables OSR after CPL3/4

  @param None

  @retval EFI_SUCCESS
  @retval EFI_DEVICE_ERROR: failed to obtain gEfiMemoryMapGuid

**/
EFI_STATUS
EFIAPI
EnableOSR (
  VOID
  )
{
  EFI_STATUS             Status;
  EFI_HOB_GUID_TYPE      *GuidHob = NULL;
  SYSTEM_MEMORY_MAP_HOB  *pSystemMemoryMap = NULL;

  Status = EFI_SUCCESS;

  //
  // Get Memory Map Hob to use in identifying enabled channels.
  //
  GuidHob = GetFirstGuidHob (&gEfiMemoryMapGuid);
  if (GuidHob != NULL) {
    pSystemMemoryMap = (SYSTEM_MEMORY_MAP_HOB*)GET_GUID_HOB_DATA (GuidHob);
  } else {
    ASSERT_EFI_ERROR (EFI_DEVICE_ERROR);
    return EFI_DEVICE_ERROR;
  }

  //
  // Program OSR registers
  //
  EnableOsrProgramming(pSystemMemoryMap);


  return Status;

} // EnableOSR

/**

  Call back routine used to do the Power Management related Register Programming

  @param Event - PpmProcessorSupportProtocol Install

  @retval Status
  @retval EFI_SUCCESS
**/

EFI_STATUS
EFIAPI
PpmInitializeEntry (
  IN EFI_HANDLE             ImageHandle,
  IN EFI_SYSTEM_TABLE       *SystemTable
  )

{
  EFI_STATUS                Status;
  EFI_EVENT                 EndOfDxeEvent;
  EFI_HANDLE                Handle = NULL;


  //DEBUG ( (EFI_D_ERROR, "\n\n ::PpmRegisterProgrammingCallback \n\n") );

  Status = EFI_SUCCESS;

  InitializeSpinLock(&CpuPmSpinLock);

  InitializeCpuInfoStruct();
  mPpmLib.CpuCsrAccess->BreakAtCheckpoint (STS_PPM_STRUCT_INIT, 0, 0xFF);
  InitCpuPPMLib();

  mPpmLib.CpuCsrAccess->BreakAtCheckpoint (STS_PPM_CSR_PROGRAMMING, 0, 0xFF);
  CpuPmProgramCsrs();

  mMpService->StartupAllAPs(mMpService,
                              (EFI_AP_PROCEDURE) CpuPmMpInitialize,
                              FALSE,
                              NULL,
                              0,
                              NULL,
                              NULL
                              );

  mPpmLib.CpuCsrAccess->BreakAtCheckpoint (STS_PPM_MSR_PROGRAMMING, 0, 0xFF);
  mPpmLib.CpuCsrAccess->BreakAtCheckpoint (STS_PPM_PSTATE_TRANSITION, 0, 0xFF);
  CpuPmMpInitialize ();

  Status = gBS->CreateEventEx (
                EVT_NOTIFY_SIGNAL,
                TPL_CALLBACK,
                PpmOnReadyToBoot,
                NULL,
                &gEfiEndOfDxeEventGroupGuid,
                &EndOfDxeEvent
                );
  ASSERT_EFI_ERROR (Status);

  mPpmLib.CpuCsrAccess->BreakAtCheckpoint (STS_PPM_EXIT, 0, 0xFF);
  mCpuPpmProtocol.CpuPpmStruct = &mPpmLib;
  Status = gBS->InstallProtocolInterface (&Handle, &gEfiCpuPpmProtocolGuid, EFI_NATIVE_INTERFACE, &mCpuPpmProtocol);
  ASSERT_EFI_ERROR (Status);

  return Status;
}


/**

  GC_TODO: add routine description

  @param Event   - GC_TODO: add arg description
  @param Context - GC_TODO: add arg description

  @retval None

**/
VOID
EFIAPI
PpmOnReadyToBoot (
  IN   EFI_EVENT    Event,
  IN   VOID         *Context
  )
{
  EFI_STATUS Status;
  Status = CpuPpmInitDone();

  if(mPpmLib.Setup->IsOppSrefEn == 1) {
      Status = EnableOSR();
  }

  return;
}
//End of OnReadyToBoot

/**

  GC_TODO: add routine description

  @param ppm  - GC_TODO: add arg description

  @retval None

**/
VOID
EFIAPI
CpuPmProgramMsrs (
  EFI_CPU_PM_STRUCT *ppm
  )
{
  UINTN    ProcessorNumber = 0;

  mMpService->WhoAmI(mMpService, &ProcessorNumber);

  ProgramMsrPowerCtl (ppm, ProcessorNumber);
  ProgMsrPriPlaneCurtCfgCtrL (ppm, ProcessorNumber);
  ProgramMsrTurboPowerLimit (ppm, ProcessorNumber);
  ProgramMsrTurboRatioLimit(ppm, ProcessorNumber);
  ProgramEnergyPerfBiasConfigMsr(ppm, ProcessorNumber);

  if ((ppm->Info->CpuType == CPU_FAMILY_HSX) || (ppm->Info->CpuType == CPU_FAMILY_SKX)) {
    ProgramMsrTemperatureTarget(ppm, ProcessorNumber);
    ProgramMsrMiscPwrMgmt(ppm, ProcessorNumber);
    ProgramMsrCLatency(ppm, ProcessorNumber);
  }

  if(ppm->Info->CpuType == CPU_FAMILY_SKX) {

    //
    //Enable HWPM modes only if CPU supports
    //
    if(ppm->Info->HwpmSupport & HWP_ENABLE) {
      EnableHwpFeatures(ppm, ProcessorNumber);
    }
  }

  ProgramMsrHwpRequest(ppm, ProcessorNumber);

  return;

}

/**

    Initializes the platform power management library for each socket.
    This must be called prior to any of the library functions being used.

    Arguments: None

    Returns: None

**/
VOID
InitCpuPPMLib (
  VOID
  )
{
  UINT8  SocketNumber;

  for (SocketNumber = 0; SocketNumber < MAX_SOCKET; SocketNumber++) {
    if (mPpmInfo->SocketPresentBitMap & (1 << SocketNumber)){
      mPpmLib.Info->SocketNumber = SocketNumber;
      InitializeCpuPPMLib((EFI_CPU_PM_STRUCT *)&mPpmLib);
    }
  }
}

/**

    GC_TODO: add routine description

    @param None

    @retval None

**/
VOID
CpuPmProgramCsrs (
  VOID
  )
{

  UINT8  SocketNumber;

  //DEBUG ( (EFI_D_ERROR, " \n\n ::Power Management CSR Programming \n\n") );

  for (SocketNumber = 0; SocketNumber < MAX_SOCKET; SocketNumber++) {
    if (mPpmInfo->SocketPresentBitMap & (1 << SocketNumber)){
      mPpmLib.Info->SocketNumber = SocketNumber;

      ProgramCSRTurboPowerLimit((EFI_CPU_PM_STRUCT *)&mPpmLib);
      ProgramCsrPkgCstEntryCriteriaMask((EFI_CPU_PM_STRUCT *)&mPpmLib);
      //ProgramCsrPcuBiosSpare2((EFI_CPU_PM_STRUCT *)&mPpmLib);  //take care in MPM (MemThrot.c)
      ProgramCsrPerfPlimitControl((EFI_CPU_PM_STRUCT *)&mPpmLib);
      ProgramCsrDynamicPerfPowerCtl((EFI_CPU_PM_STRUCT *)&mPpmLib);
      ProgramCsrSapmCtl((EFI_CPU_PM_STRUCT *)&mPpmLib);
      ProgramCsrSwLtrOvrd((EFI_CPU_PM_STRUCT *)&mPpmLib);
      ProgramCsrPmaxConfig((EFI_CPU_PM_STRUCT *)&mPpmLib);
      ProgramCsrResponseRatioCfg((EFI_CPU_PM_STRUCT *)&mPpmLib);
      ProgramB2PFastRaplDutyCycle((EFI_CPU_PM_STRUCT *)&mPpmLib);
      ProgramB2PPcuMiscConfig((EFI_CPU_PM_STRUCT *)&mPpmLib);
      ProgramB2PHWPMMiscConfig((EFI_CPU_PM_STRUCT *)&mPpmLib);
      ProgramB2PDynamicL1((EFI_CPU_PM_STRUCT *)&mPpmLib);
      if ((mPpmInfo->CpuType == CPU_FAMILY_SKX) && ((mPpmInfo->CapId4 & CAPID4_PHYSICAL_CHOP) == SKX_REV_AxBxH0)) {  // execute on XCC (A0/A1/A2/B0/B1/H0)
        ProgramB2PForceUncoreAndMeshRatio((EFI_CPU_PM_STRUCT *)&mPpmLib);
      }

      ProgramB2PMiscWorkaroundEnable((EFI_CPU_PM_STRUCT *)&mPpmLib);
      ProgramB2PAVXICCPPreGrantLevel((EFI_CPU_PM_STRUCT *)&mPpmLib);
      ProgramB2PSetDcstLuts((EFI_CPU_PM_STRUCT *)&mPpmLib);
    }
  }
}


