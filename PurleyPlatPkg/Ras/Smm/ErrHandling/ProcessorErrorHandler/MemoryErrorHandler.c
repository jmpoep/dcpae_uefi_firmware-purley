/** @file
  Implementation of the Memory Error Handler.

  Copyright (c) 2009-2019 Intel Corporation.
  All rights reserved. This software and associated documentation
  (if any) is furnished under a license and may only be used or
  copied in accordance with the terms of the license.  Except as
  permitted by such license, no part of this software or
  documentation may be reproduced, stored in a retrieval system, or
  transmitted in any form or by any means without the express
  written consent of Intel Corporation.

**/

#include <Library/BaseMemoryLib.h>
#include <Library/IoLib.h>
#include <Library/TimerLib.h>
#include <Library/BaseLib.h>
#include <DimmIsolationFlow.h>
#include <Setup/IioUniversalData.h>
#include "CommonErrorHandlerDefs.h"
#include "MemoryErrorHandler.h"
#include "ProcessorErrorHandler.h"
#include <Include/MeUma.h>
//#include "ivxJck.h"

UINT8       mHaCmciOnCorrecetdThr;
UINT32      mEccErrorSmiCount[MC_MAX_NODE];
TIME_STAMP  mCorrectedErrorRankTimeStamp[MAX_SOCKET][MAX_CH][MAX_RANK_CH];

UINT8       mCSMIEnNode[MAX_SOCKET];

extern SAVED_PPR_TA   *mSavedPprTas;
extern SPIN_LOCK      mSavedPprTasLock;

DIMM_ISOLATION_SKIP_DESC mSkipDimmIsolation[] = {
  // MCACOD , MCACODMASK, MSCOD, MSCODMASK
  { IMC_SPARING_ERR_MCACOD, IMC_SPARING_ERR_MCACOD_MSK, IMC_UNC_SPARING_ERR_MSCOD, IMC_SPARING_ERR_MSCOD_MSK},
  { IMC_SPARING_ERR_MCACOD, IMC_SPARING_ERR_MCACOD_MSK, IMC_COR_SPARING_ERR_MSCOD, IMC_SPARING_ERR_MSCOD_MSK},
  { IMC_DDR4_CAP_ERR_MCACOD, IMC_DDR4_CAP_ERR_MCACOD_MSK, IMC_DDR4_CAP_ERR_MSCOD, IMC_DDR4_CAP_ERR_MSCOD_MSK}
};

UINTN mSkipDimmIsolationEntries = sizeof(mSkipDimmIsolation)/sizeof(DIMM_ISOLATION_SKIP_DESC);
////////////////////////////////////// Common Routines //////////////////////////////////////


////////////////////////////////// Error Handling Routines //////////////////////////////////


/**
  Handle Spare Node migration.

  @param[in]      NodeID          - Memory controller ID
  @param[in]      EventType       - In progress / new events
  @param[in, out] *IsEventHandled - ptr to buffer (BOOLEAN) to get Event handled status

  @retval status - EFI_SUCCESS - no error
       non-zero code - if error
  IsEventHandled - Event handled status - TRUE/FALSE

**/
static
EFI_STATUS
HandleSpareNodeMigration (
  IN        UINT8       NodeId,
  IN        EVENT_TYPE  EventType,
  IN  OUT   BOOLEAN     *IsEventHandled
  )
{
  *IsEventHandled = FALSE;

  RASDEBUG ((DEBUG_ERROR, "CheckAndHandleSpareNodeMigration : routine not implemented\n"));
  return EFI_SUCCESS;
}

/**
  This function gets the Memory Fru information.

  @param[in]  McBank      - MC Bank the error was detected on

  @retval EFI_STATUS
**/
EFI_STATUS
GetMemoryFruInfo (
  IN        UEFI_MEM_ERROR_RECORD   *MemRecord,
      OUT   CHAR8                   *FruString
)
{
  EFI_STATUS            Status;
  CHAR8                 SlotStr[] = {'C', 'a', 'r', 'd', '?', '?', ',', ' ', 'C', 'h', 'n', '?', ',', ' ', 'D', 'I', 'M', 'M', '?', 0};
  UINT8                 RiserCard;

  Status = EFI_SUCCESS;

  if ((MemRecord->Section.ValidBits & PLATFORM_MEM_NODE_VALID) &&
        (MemRecord->Section.ValidBits & PLATFORM_MEM_CARD_VALID) &&
        (MemRecord->Section.ValidBits & PLATFORM_MEM_MODULE_VALID)) {
    //
    // Data for Thunder Ridge memory.
    // FruString buffer is 20 bytes.
    //

    //
    // MemInfo->Node + 1 indicates the memory riser card number.
    //
    if ((MemRecord->Section.ValidBits & PLATFORM_MEM_NODE_VALID) && (MemRecord->Section.Node <= 98)) {
      RiserCard = (UINT8) (MemRecord->Section.Node + 1);
      RiserCard = DecimalToBcd8 (RiserCard);
      SlotStr[4] = (CHAR8) (((RiserCard & 0x00F0) >> 4) + '0');
      SlotStr[5] = (CHAR8) ((RiserCard & 0x000F) + '0');
    }

    //
    // Channel A, B, C, D on memory riser card.
    //
    if ((MemRecord->Section.ValidBits & PLATFORM_MEM_CARD_VALID) && (MemRecord->Section.Card <= 9)) {
      SlotStr[11] = (CHAR8) (MemRecord->Section.Card + 'A');
    }

    //
    // DIMM slot number on memory riser card.
    //
    if ((MemRecord->Section.ValidBits & PLATFORM_MEM_MODULE_VALID) && (MemRecord->Section.Module <= 9)) {
      SlotStr[18] = (CHAR8) (MemRecord->Section.Module + '0');
    }

    Status = AsciiStrCpyS (FruString, sizeof(MemRecord->Descriptor.FruText), SlotStr);
    ASSERT_EFI_ERROR (Status);
    if (EFI_ERROR(Status)) return Status;
  } else {
    Status = EFI_UNSUPPORTED;
  }

  return Status;
}

/**
  Fills error record information for detected error.

  @param[in]  McBank      - MC Bank the error was detected on
  @param[in]  skt         - The socket the MCE occured on
  @param[in]  Sev         - The error severity
  @param[in]  McErrSig    - MCE error information
  @param[out] MemRecord   - The error record to fill

  @retval EFI_STATUS
    MemRecord - Error record for the detected error
**/
EFI_STATUS
FillPlatformMemorySection (
  IN        UINT8                         McBank,
  IN        UINT8                         skt,
  IN        UINT32                        Sev,
  IN        EMCA_MC_SIGNATURE_ERR_RECORD  *McErrSig,
  OUT       UEFI_MEM_ERROR_RECORD         *MemRecord
)
{
  EFI_STATUS                  Status;
  UINT8                       SktId;
  UINT16                      SktNum;
  UINT16                      SktCh;
  UINT8                       core;
  IA32_MCI_STATUS_MSR_STRUCT  MsrIa32MCiStatus;
  BOOLEAN                     DimmIsolation;
  UINTN                       i;
  UINT64                      MeUmaBase;
  UINT64                      MeUmaLimit;
  TRANSLATED_ADDRESS          TA;

  Status = EFI_SUCCESS;
  SktId = 0;
  core = 0;
  DimmIsolation = FALSE;

  if (MemRecord == NULL || McErrSig == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  ZeroMem (MemRecord, sizeof(UEFI_MEM_ERROR_RECORD));
  ZeroMem (&TA, sizeof(TRANSLATED_ADDRESS));

  // Build Error Record Header
  MemRecord->Header.SignatureStart = SIGNATURE_32('C','P','E','R');
  MemRecord->Header.SignatureEnd = 0xffffffff;
  MemRecord->Header.Revision = 0x0204;  // UEFI Spec version 2.4
  MemRecord->Header.SectionCount = 1;  // Only supporting one section per error record
  MemRecord->Header.Severity = Sev;
  MemRecord->Header.ValidBits = 0;
  MemRecord->Header.RecordLength = sizeof (UEFI_MEM_ERROR_RECORD);
  if (Sev == EFI_ACPI_5_0_ERROR_SEVERITY_CORRECTED) {
    MemRecord->Header.Flags = HW_ERROR_FLAGS_RECOVERED;
    CopyMem (&MemRecord->Header.NotificationType, &gErrRecordNotifyCmcGuid, sizeof (EFI_GUID));
  } else {
    CopyMem (&MemRecord->Header.NotificationType, &gErrRecordNotifyMceGuid, sizeof (EFI_GUID));
  }

  // Fill error descriptor
  MemRecord->Descriptor.Offset = sizeof (UEFI_ERROR_RECORD_HEADER) + sizeof (UEFI_ERROR_SECT_DESC);
  MemRecord->Descriptor.Length = sizeof (MEMORY_ERROR_SECTION);
  MemRecord->Descriptor.Revision = 0x0204; // UEFI Spec version 2.4
  MemRecord->Descriptor.ValidBits = 0;
  MemRecord->Descriptor.Flags = 1; // Primary
  CopyMem (&MemRecord->Descriptor.SectionType, &gErrRecordPlatformMemErrGuid, sizeof (EFI_GUID));
  MemRecord->Descriptor.Severity = Sev;

  // Fill error section body
  ExtractingNodeCoreFromApicId (&SktId, &core);
  MemRecord->Section.Node = SktId;
  MemRecord->Section.ValidBits = PLATFORM_MEM_NODE_VALID;

  MemRecord->Section.ErrorType = PLATFORM_MEM_ERROR_UNKNOWN;

  MsrIa32MCiStatus.Data = McErrSig->Signature.McSts;
  if (MsrIa32MCiStatus.Bits.addrv == 1 && MsrIa32MCiStatus.Bits.miscv == 1) {
    MemRecord->Section.PhysAddr = MCAddress (McErrSig->Signature.McAddr, McErrSig->Signature.McMisc);
    MemRecord->Section.ValidBits |= PLATFORM_MEM_PHY_ADDRESS_VALID;
    DimmIsolation = TRUE;

    //Error types for which address traslation is not appropiate
    for (i = 0; i < mSkipDimmIsolationEntries; i++) {
      if (((MsrIa32MCiStatus.Bits.mca_code & mSkipDimmIsolation[i].McaCodMsk) == mSkipDimmIsolation[i].McaCod) &&
          ((MsrIa32MCiStatus.Bits.ms_code & mSkipDimmIsolation[i].MsCodMsk) == mSkipDimmIsolation[i].MsCod)) {
        DimmIsolation   = FALSE;
      }
    }

    Status = IsolateFaultyDimmForErrSig(McBank, skt, McErrSig, &(MemRecord->Section));
    if (EFI_ERROR(Status)) {
      return Status;
    }

    // Try create PPR log for UC case. The actual switch of creating PPR var or not is control in PPR library based on BIOS knobs.
    if ( MsrIa32MCiStatus.Bits.uc ) {
      Status = mMemRas->SystemAddressToDimmAddress (McErrSig->Signature.McAddr, &TA);
      RASDEBUG ((DEBUG_ERROR, "Going to log PPR for : Channel = %x, DIMM = %x, Rank = %x, Col = %x, Row = %x\n",
        TA.ChannelId, TA.DimmSlot, TA.ChipSelect, TA.Col, TA.Row));
        AcquireSpinLock (&mSavedPprTasLock);
        if (mSavedPprTas->NumOfRecord < MAX_PPR_ADDR_ENTRIES) {
          CopyMem (&mSavedPprTas->mSavedPprTas[mSavedPprTas->NumOfRecord], &TA, sizeof(TRANSLATED_ADDRESS));
          mSavedPprTas->NumOfRecord++;
        }
        ReleaseSpinLock (&mSavedPprTasLock);
    }

    Status = GetMemoryFruInfo (MemRecord, MemRecord->Descriptor.FruText);
    if (Status == EFI_SUCCESS) {
      MemRecord->Descriptor.ValidBits |= UEFI_DESC_FRU_STRING_VALID;
    }
  }

  Status = MCBankCsrInfoHook (McBank, &skt, McErrSig, &MemRecord->Section);

  SktCh = NODECH_TO_SKTCH(MemRecord->Section.Node, MemRecord->Section.Card);
  SktNum = NODE_TO_SKT(MemRecord->Section.Node);

  RASDEBUG ((DEBUG_ERROR, "FillPlatformMemorySection : MemRecord->Section.Node = %d, skt = %d, MemRecord->Section.Card = %d\n",
      MemRecord->Section.Node, skt, MemRecord->Section.Card));

  if ((MemRecord->Section.ValidBits & PLATFORM_MEM_NODE_VALID) != 0 &&
    (MemRecord->Section.ValidBits & PLATFORM_MEM_CARD_VALID) != 0 &&
    (MemRecord->Section.ValidBits & PLATFORM_MEM_MODULE_VALID) != 0) {

    MemRecord->Section.CardHandle = mRasTopology->SystemInfo.SmbiosType16Handle; // Type 16 handle
    MemRecord->Section.ModuleHandle =
      mRasTopology->SystemInfo.SocketInfo[SktNum].ChannelInfo[SktCh].DimmInfo[MemRecord->Section.Module].SmbiosType17Handle; // Type 17 handle

    MemRecord->Section.ValidBits |= PLATFORM_CARD_HANDLE_VALID | PLATFORM_Module_HANDLE_VALID;
  }

  MemRecord->Section.McaBankInfo.ApicId = GetApicID ();
  MemRecord->Section.McaBankInfo.BankNum = McBank;
  MemRecord->Section.McaBankInfo.BankType = GetMcBankUnitType (McBank);
  MemRecord->Section.McaBankInfo.BankScope = GetMcBankScope (McBank);
  MemRecord->Section.McaBankInfo.McaStatus = McErrSig->Signature.McSts;
  MemRecord->Section.McaBankInfo.McaAddress = McErrSig->Signature.McAddr;
  MemRecord->Section.McaBankInfo.McaMisc = McErrSig->Signature.McMisc;
  MemRecord->Section.McaBankInfo.Valid = TRUE;


  // Report MEM UCE inside MESEG region
  if (Sev == EFI_ACPI_5_0_ERROR_SEVERITY_FATAL) {
    MeUmaBase  = MmioRead32 (MmPciBase (0, 5, 0) + R_MESEG_BASE) |
                 (((UINT64) MmioRead32 (MmPciBase (0, 5, 0) + R_MESEG_BASE + 0x04)) << 32);
    MeUmaLimit = MmioRead32 (MmPciBase (0, 5, 0) + R_MESEG_BASE + 0x08) |
                 (((UINT64) MmioRead32 (MmPciBase (0, 5, 0) + R_MESEG_BASE + 0x0C)) << 32);

    if (MeUmaBase < MeUmaLimit &&
        McErrSig->Signature.McAddr >= MeUmaBase &&
        McErrSig->Signature.McAddr <= MeUmaLimit) {
      MemRecord->Section.RequesterId = BIT63;
      MemRecord->Section.ValidBits |= PLATFORM_MEM_REQUESTOR_ID_VALID;
    }
  }

	return EFI_SUCCESS;
}

/**
  Calculates and returns physical address from Addr and Misc.

  @param[in]  Addr      - MCA_ADDR
  @param[in]  Misc      - MCA_MISC

  @retval None
**/
UINT64
MCAddress (
  IN        UINT64  Addr,
  IN        UINT64  Misc
  )
{
  UINT64  Mask;
  UINT64  McAddress;
  UINT8   Lsb;

  Mask = MCA_ADDR_BIT_MASK;
  McAddress = Addr & Mask;

  Lsb = Misc & MCA_MISC_LSB_MASK;
  Mask = ~((1 << Lsb) - 1);

  McAddress &= Mask;

  return McAddress;
}

/**
  Check each channel for correctable error threshold overflow.

  @param[in]  Node      - The node to handle

  @retval BOOLEAN       - True if there is an error, otherwise false
          DdrCh         - The channel with the error
          Dimm          - The dimm with the error
          Rank          - The dimm rank that had the error
**/


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
LogCorrectedMemError (
  IN        UINT8               Type,
  IN        UINT8               SubType,
  IN        UINT8               Node,
  IN        UINT8               Ch,
  IN        UINT8               Rank,
  IN        UINT8               Dimm,
  IN        UINT8               McaCode
  )
{
  UINT8   Socket;
  UINT8   Mc;
  BOOLEAN GetPhyAddr;
  EFI_STATUS Status;
  TRANSLATED_ADDRESS  TA;

  UEFI_MEM_ERROR_RECORD                   MemRecord;
  MCI_STATUS_SHADOW_N1_M2MEM_MAIN_STRUCT  Mc5ShadowStatus;
  MCI_ADDR_SHADOW0_M2MEM_MAIN_STRUCT      Mc5Shadow0Addr;
  MCI_ADDR_SHADOW1_M2MEM_MAIN_STRUCT      Mc5Shadow1Addr;

  ZeroMem (&TA, sizeof(TRANSLATED_ADDRESS));

  Socket = NODE_TO_SKT(Node); // use Socket & Mc temporarily in XXXXMcCpuCsr() calls
  Mc = NODE_TO_MC(Node);

  //
  // Clear the MemInfo structure so that unused elements
  // have a predictable value in the error record.
  //
  ZeroMem (&MemRecord, sizeof (UEFI_MEM_ERROR_RECORD));

  RASDEBUG ((DEBUG_ERROR, "LogCorrectedMemError : SubType = %d, Node = %d, channel = %d, Rank = %d Dimm = %d, McaCode = %d\n",
          SubType, Node, Ch, Rank, Dimm, McaCode));

  //
  // Skip if Correctable and Uncorrectable error logging is disabled
  //
  if (mRasSetup->SystemErrorEn == 0) {
    RASDEBUG ((DEBUG_ERROR, "LogCorrectedMemError : routine disabled\n"));
    return;
  }

  // Build Error Record Header
  MemRecord.Header.SignatureStart = SIGNATURE_32('C','P','E','R');
  MemRecord.Header.Revision = 0x0204;  // UEFI Spec version 2.4
  MemRecord.Header.SignatureEnd = 0xffffffff;
  MemRecord.Header.SectionCount = 1;  // Only supporting one section per error record
  MemRecord.Header.Severity = EFI_ACPI_5_0_ERROR_SEVERITY_CORRECTED;
  MemRecord.Header.ValidBits = 0;
  MemRecord.Header.RecordLength = sizeof (UEFI_MEM_ERROR_RECORD);
  CopyMem (&MemRecord.Header.NotificationType, &gErrRecordNotifyCmcGuid, sizeof (EFI_GUID));
  MemRecord.Header.Flags = HW_ERROR_FLAGS_RECOVERED;

  // Fill error descriptor
  MemRecord.Descriptor.Offset = sizeof (UEFI_ERROR_RECORD_HEADER) + sizeof (UEFI_ERROR_SECT_DESC);
  MemRecord.Descriptor.Length = sizeof (MEMORY_ERROR_SECTION);
  MemRecord.Descriptor.Revision = 0x0204;  // UEFI Spec version 2.4
  MemRecord.Descriptor.ValidBits = 0;
  MemRecord.Descriptor.Flags = 1;   // Primary
  CopyMem (&MemRecord.Descriptor.SectionType, &gErrRecordPlatformMemErrGuid, sizeof (EFI_GUID));
  MemRecord.Descriptor.Severity = EFI_ACPI_5_0_ERROR_SEVERITY_CORRECTED;

  // Fill error section body
  MemRecord.Section.ValidBits = PLATFORM_MEM_ERROR_STATUS_VALID;
  MemRecord.Section.Node = Node;
  MemRecord.Section.Card = Ch;
  MemRecord.Section.RankNumber = Rank;
  MemRecord.Section.CardHandle = mRasTopology->SystemInfo.SmbiosType16Handle; // Type 16 handle
  MemRecord.Section.ModuleHandle =
    mRasTopology->SystemInfo.SocketInfo[Socket].ChannelInfo[NODECH_TO_SKTCH(Node, Ch)].DimmInfo[Dimm].SmbiosType17Handle; // Type 17 handle
  MemRecord.Section.Module = Dimm;
  switch (SubType) {
    case MEM_LINK_ERROR:
      if ((mRasTopology->SystemInfo.RasModesEnabled & CH_MIRROR) == CH_MIRROR)
        MemRecord.Section.ErrorType = PLATFORM_MEM_ERROR_MIRROR_FAILED;
      else
        MemRecord.Section.ErrorType = PLATFORM_MEM_ERROR_UNKNOWN;
      MemRecord.Section.Card = Ch * MAX_CH;
      break;
    case MEM_LANE_FAILOVER:
      MemRecord.Section.ErrorType = PLATFORM_MEM_ERROR_UNKNOWN;
      MemRecord.Section.Card = Ch * MAX_CH;
      break;
    case MEM_ECC_ERROR:
      MemRecord.Section.ErrorType = PLATFORM_MEM_ERROR_SINGLEBIT_ECC;
      MemRecord.Section.ValidBits |= PLATFORM_MEM_MODULE_VALID | PLATFORM_RANK_NUMBER_VALID | PLATFORM_CARD_HANDLE_VALID | PLATFORM_Module_HANDLE_VALID;
      break;
    case MEM_DDR_PARITY_ERROR:
      MemRecord.Section.ErrorType = PLATFORM_MEM_ERROR_PARITY;
      MemRecord.Section.ValidBits |= PLATFORM_MEM_MODULE_VALID;
      MemRecord.Section.Card = Ch * MAX_CH;
      break;
    case MEM_CRC_ERROR:
      MemRecord.Section.ErrorType = PLATFORM_MEM_ERROR_UNKNOWN;
      break;
    case MEM_MIRROR_FAILOVER:
     MemRecord.Section.ErrorType = PLATFORM_MEM_ERROR_MIRROR_FAILED;
      break;
  }

  MemRecord.Section.ValidBits |= PLATFORM_MEM_NODE_VALID | PLATFORM_MEM_CARD_VALID | PLATFORM_MEM_ERROR_TYPE_VALID;

  ElogMcExecuteCorrectableErrorHandlingPolicies (&MemRecord);

  if (SubType == MEM_MIRROR_FAILOVER) {
    GetPhyAddr = TRUE;

    if (GetPhyAddr) {
      Mc5ShadowStatus.Data = mCpuCsrAccess->ReadMcCpuCsr (Socket, Mc, MCI_STATUS_SHADOW_N1_M2MEM_MAIN_REG);
      if (Mc5ShadowStatus.Bits.valid && Mc5ShadowStatus.Bits.addrv) {
        Mc5Shadow0Addr.Data = mCpuCsrAccess->ReadMcCpuCsr (Socket, Mc, MCI_ADDR_SHADOW0_M2MEM_MAIN_REG);
        Mc5Shadow1Addr.Data = mCpuCsrAccess->ReadMcCpuCsr (Socket, Mc, MCI_ADDR_SHADOW1_M2MEM_MAIN_REG);
        MemRecord.Section.PhysAddr = ((UINT64)Mc5Shadow1Addr.Data << 32) | (Mc5Shadow0Addr.Data);
        MemRecord.Section.PhysAddr &= (0x3fffffffffc0); // 46bit address space, bits 45:6.
      }

      if (MemRecord.Section.PhysAddr != 0) {
        Status = mMemRas->SystemAddressToDimmAddress (MemRecord.Section.PhysAddr, &TA);

        if(Status == EFI_SUCCESS) {
          Socket = TA.SocketId;
          Mc = TA.MemoryControllerId;
          MemRecord.Section.Module = TA.DimmSlot;
          MemRecord.Section.ValidBits |= PLATFORM_MEM_PHY_ADDRESS_VALID | PLATFORM_MEM_CARD_VALID | PLATFORM_MEM_NODE_VALID | PLATFORM_MEM_MODULE_VALID;
          if(TA.DimmType != ddrtdimmType) {
            MemRecord.Section.RankNumber = TA.PhysicalRankId;
            MemRecord.Section.Bank = TA.Bank;
            MemRecord.Section.Row = (UINT16) TA.Row;
            MemRecord.Section.Col = (UINT16) TA.Col;

            MemRecord.Section.ValidBits |= PLATFORM_MEM_ROW_VALID | PLATFORM_MEM_COLUMN_VALID | PLATFORM_RANK_NUMBER_VALID | PLATFORM_MEM_BANK_VALID;


            if ((TA.Row >> 16) > 0) {
              MemRecord.Section.ValidBits &= ~PLATFORM_MEM_ROW_VALID;         // Row number will not fit -> set old row number invalid
              MemRecord.Section.ValidBits |= PLATFORM_MEM_EXTENDED_ROW_VALID; // Set row overflow valid bit 18
              MemRecord.Section.Extended = (UINT8)(TA.Row >> 16) & 0x3;
            }
            if (TA.DimmType == ddr4dimmType) {
              MemRecord.Section.Bank = (UINT16)(TA.BankGroup << 8) | TA.Bank;                 // Drop-in bits 15:8
              MemRecord.Section.ValidBits |= (PLATFORM_MEM_BANK_GROUP_VALID | PLATFORM_MEM_BANK_ADDRESS_VALID);
              MemRecord.Section.ValidBits &= ~PLATFORM_MEM_BANK_VALID;
              MemRecord.Section.Extended |= (UINT8)(TA.ChipSelect << 5);   // Merge extended row bits and chip select
              MemRecord.Section.ValidBits |= PLATFORM_MEM_CHIP_IDENTIFICATION_VALID;
            }
          }
        }
      }

      MemRecord.Section.CardHandle = mRasTopology->SystemInfo.SmbiosType16Handle; // Type 16 handle;
      MemRecord.Section.ModuleHandle = mRasTopology->SystemInfo.SocketInfo[Socket].ChannelInfo[NODECH_TO_SKTCH(Node, Ch)].DimmInfo[TA.DimmSlot].SmbiosType17Handle; // Type 17 handle
      MemRecord.Section.ValidBits |= PLATFORM_CARD_HANDLE_VALID | PLATFORM_Module_HANDLE_VALID;
    }
  }

  mPlatformErrorHandlingProtocol->LogReportedError (&MemRecord.Header);

}

/**
  This function reads the CMOS location.

  @param[in]  Address        - cmos address to read

  @retval cmos address value
**/
static
UINT8
RtcRead (
  IN        UINT8   Address
  )
{
  UINT8             data8;

  //
  // Port 0x74 is alias of port 0x70 which is write only
  //
  data8 = IoRead8 (0x74);
  data8 &= 0x80;
  data8 |= Address;
  IoWrite8 (0x70, data8);

  //
  // Data is at port 0x71
  //
  data8 = IoRead8 (0x71);
  return data8;
}

/**
  Waits until RTC register A and D show data is valid.

  @param[in]  Timeout        - Maximum time to wait

  @retval EFI_DEVICE_ERROR   RTC device error.
          EFI_SUCCESS        RTC is updated and ready.
**/
EFI_STATUS
RtcWaitToUpdate (
  IN        UINTN   Timeout
  )
{
  RTC_REGISTER_A    RegisterA;
  RTC_REGISTER_D    RegisterD;

  //
  // See if the RTC is functioning correctly
  //
  RegisterD.Data = RtcRead (RTC_ADDRESS_REGISTER_D);

  if (RegisterD.Bits.Vrt == 0) {
    return EFI_DEVICE_ERROR;
  }
  //
  // Wait for up to 0.1 seconds for the RTC to be ready.
  //
  Timeout = (Timeout / 10) + 1;
  RegisterA.Data = RtcRead (RTC_ADDRESS_REGISTER_A);
  while (RegisterA.Bits.Uip == 1 && Timeout > 0) {
    MicroSecondDelay (10);
    RegisterA.Data = RtcRead (RTC_ADDRESS_REGISTER_A);
    Timeout--;
  }

  RegisterD.Data = RtcRead (RTC_ADDRESS_REGISTER_D);
  if (Timeout == 0 || RegisterD.Bits.Vrt == 0) {
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

/**
  This function provides the time stamp information.

  @param[in]  Time        - The time stamp

  @retval The time
**/
EFI_STATUS
EfiSmmGetTime (
  IN  OUT   TIME_STAMP    *Time
  )
{
  EFI_STATUS  Status;
  UINT8       data8;

  Status = EFI_SUCCESS;

  //
  // Wait for up to 0.1 seconds for the RTC to be updated
  //
  Status = RtcWaitToUpdate (PcdGet32 (PcdRealTimeClockUpdateTimeout));
  if (EFI_ERROR (Status)) {
    return Status;
  }

  data8 = RtcRead (RTC_ADDRESS_YEAR);
  Time->Year = BCD2DEC (data8);
  data8 = RtcRead (RTC_ADDRESS_MONTH);
  Time->Month = BCD2DEC (data8);
  data8 = RtcRead (RTC_ADDRESS_DAY_OF_THE_MONTH);
  Time->Day = BCD2DEC (data8);
  data8 = RtcRead (RTC_ADDRESS_HOURS);
  Time->Hour = BCD2DEC (data8);
  data8 = RtcRead (RTC_ADDRESS_MINUTES);
  Time->Minute = BCD2DEC (data8);
  data8 = RtcRead (RTC_ADDRESS_SECONDS);
  Time->Second = BCD2DEC (data8);

  return Status;
}

/**
  This function gets the number of hours passed.

  @param[in]  PrevTime  - The previous time stamp

  @retval The time
**/
UINT32
EfiSmmGetNoofHoursPast (
  IN  OUT   TIME_STAMP  *PrevTime
  )
{
  TIME_STAMP  CurTime;
  UINT32      CurHours;
  UINT32      PrevHours;
  UINT32      HoursPast;

  EfiSmmGetTime (&CurTime);
  HoursPast = 0;
  CurHours = (CurTime.Year * 365 * 24) + (CurTime.Month * 30 * 24) + (CurTime.Day * 24) + (CurTime.Hour);
  PrevHours = (PrevTime->Year * 365 * 24) + (PrevTime->Month * 30 * 24) + (PrevTime->Day * 24) + (PrevTime->Hour);
  if (CurHours > PrevHours)
    HoursPast = (CurHours - PrevHours);

  return HoursPast;
}

/**
  Throttle SMI if the rate is greater than the threshold.

  @param[in]  MemoryErrRecord      - Pointer to the memory error record.

  @retval None
**/
VOID
CorrectedErrorSMIThrottling (
  IN        UEFI_MEM_ERROR_RECORD   *MemoryErrRecord
  )
{
  UINT8   Skt;
  UINT8   Mc;
  UINT16  failedNode;
  UINT16  failedChannel;
  UINT16  failedRank;
  UINT32  ExtData[2];

  if (((~MemoryErrRecord->Section.ValidBits) & (PLATFORM_MEM_NODE_VALID | PLATFORM_MEM_CARD_VALID | PLATFORM_RANK_NUMBER_VALID)) == 0) {
    failedNode = MemoryErrRecord->Section.Node;
    failedChannel = MemoryErrRecord->Section.Card;
    failedRank = MemoryErrRecord->Section.RankNumber;

    //
    // Compute rank error rate and disable corrected SMIs if rate > threshold
    //

    //
    // Init timestamp, if first time
    //
    if (mCorrectedErrorRankTimeStamp[failedNode][failedChannel][failedRank].Year == 0) {
      EfiSmmGetTime (&mCorrectedErrorRankTimeStamp[failedNode][failedChannel][failedRank]);
      return;
    }

    if (EfiSmmGetNoofHoursPast (&mCorrectedErrorRankTimeStamp[failedNode][failedChannel][failedRank])
                               <= PLATFORM_PER_RANK_THRESHOLD_DURATION_HOURS) {
      mEccErrorSmiCount[failedNode]++;

      //
      // Disable SMI when number of rank failures per TIME_DURATION hours exceeds threshold
      //
      if (mEccErrorSmiCount[failedNode] > PLATFORM_ERROR_COUNT_THRESHOLD) {
        Skt = NODE_TO_SKT ((UINT8)failedNode);
        Mc = NODE_TO_MC (failedNode);
        if (mRasTopology->SystemInfo.SocketInfo[Skt].imcEnabled[Mc] != 0)
          ExtData[0] = Skt;
          ExtData[1] = Mc;
          ProcMemDisableReporting(0, 0, ExtData);
      }
    } else {
      //
      // Reset beginning of time period for rank
      //
      EfiSmmGetTime (&mCorrectedErrorRankTimeStamp[failedNode][failedChannel][failedRank]);
    }
  }
  return;
}

/**
  Executes based on error handling policies.

  @param[in]  MemoryErrRecord      - Pointer to the memory error record.

  @retval None
**/
VOID
ElogMcExecuteCorrectableErrorHandlingPolicies (
  IN        UEFI_MEM_ERROR_RECORD   *MemoryErrRecord
  )
{
  //
  // 1. Check for SMI throttling for corrected errors
  //
  CorrectedErrorSMIThrottling (MemoryErrRecord);

  return;
}

////////////////////////////////// Initialization routines //////////////////////////////////

/**
  Enables or disables the memory and HA specifc error reporting based on the SYSTEM_INFO structure.

  @retval EFI_SUCCESS if the call is succeed

**/
EFI_STATUS
EnableElogMemory (
  VOID
  )
{
  UINT8                             Socket;
  UINT8                             Mc;
  UINT8                             Node;
  UINT8                             Ch;
  UINT8                             NodeCh;
  UINT8                             Rank;

  if (mRasSetup->Memory.MemErrEn) {
    // Used for corrected error SMI throttling
    for (Node = 0; Node < MC_MAX_NODE; Node++) {
      mEccErrorSmiCount[Node] = 0;
    }

    for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
      // Skip if socket not enabled
      if (mRasTopology->SystemInfo.SocketInfo[Socket].Valid == 0) continue;

      for (Mc = 0; Mc < MAX_IMC; Mc++) {
        // Skip if Mc is not enabled
        if (mRasTopology->SystemInfo.SocketInfo[Socket].imcEnabled[Mc] == 0) continue;

        // Init timestamps for corrected error rate SMI throttling
        for (NodeCh = 0; NodeCh < MAX_MC_CH; NodeCh++) {
          Ch = NODECH_TO_SKTCH(SKTMC_TO_NODE(Socket, Mc), NodeCh);
          for (Rank = 0; Rank < MAX_RANK_CH; Rank++) {
            mCorrectedErrorRankTimeStamp[Socket][Ch][Rank].Year = 0;
          }
        } // for (NodeCh)

      } // for (Mc)
    } // Socket
  }
  return EFI_SUCCESS;
}

