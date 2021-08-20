  /**
  This file contains an 'Intel Peripheral Driver' and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement
**/
/**

Copyright (c)  1999 - 2018, Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


  @file AcpiPlatform.c

  ACPI Platform Driver

**/

#include "AcpiPlatform.h"
#include "AcpiPlatformUtils.h"
#include "AcpiPlatformHooks.h"

#pragma optimize("", off)

extern BIOS_ACPI_PARAM                                *mAcpiParameter;
extern SOCKET_IIO_CONFIGURATION                       mSocketIioConfiguration;
extern SOCKET_POWERMANAGEMENT_CONFIGURATION           mSocketPowermanagementConfiguration;
extern SOCKET_PROCESSORCORE_CONFIGURATION             mSocketProcessorCoreConfiguration;
extern EFI_ACPI_SERIAL_PORT_CONSOLE_REDIRECTION_TABLE *mSpcrTable;
extern EFI_IIO_UDS_PROTOCOL                           *mIioUds;

BOOLEAN               mFirstNotify;
SYSTEM_CONFIGURATION  mSystemConfiguration;
PCH_RC_CONFIGURATION  mPchRcConfiguration;
UINT8                 mKBPresent = 0;
UINT8                 mMousePresent = 0;
UINT8                 mPStateEnable = 0;

/**
  Executes ACPI Platform actions related with ready to boot event

  @param Event   - not used
  @param Context - not used

  @retval None
**/
STATIC
VOID
EFIAPI
AcpiPlatformOnReadyToBoot (
  IN      EFI_EVENT                 Event,
  IN      VOID                      *Context
  )
{
  EFI_STATUS                  Status;
  EFI_ACPI_DESCRIPTION_HEADER Table = {0};
  EFI_ACPI_TABLE_VERSION      TableVersion;
  UINTN                       TableHandle;
  EFI_ACPI_TABLE_PROTOCOL     *AcpiTable;
  EFI_CPUID_REGISTER          CpuidRegisters;
  SETUP_DATA                  SetupData;
  UINT8                       ARIForward;

  if (mFirstNotify) {
    return ;
  }

  mFirstNotify = TRUE;

  Status = GetEntireConfig (&SetupData);
  ASSERT_EFI_ERROR (Status);
  CopyMem (&mSystemConfiguration, &(SetupData.SystemConfig), sizeof(SYSTEM_CONFIGURATION));
  CopyMem (&mSocketIioConfiguration, &(SetupData.SocketConfig.IioConfig), sizeof(SOCKET_IIO_CONFIGURATION));
  CopyMem (&mSocketPowermanagementConfiguration, &(SetupData.SocketConfig.PowerManagementConfig), sizeof(SOCKET_POWERMANAGEMENT_CONFIGURATION));
  CopyMem (&mSocketProcessorCoreConfiguration, &(SetupData.SocketConfig.SocketProcessorCoreConfiguration), sizeof(SOCKET_PROCESSORCORE_CONFIGURATION));
  CopyMem (&mPchRcConfiguration, &(SetupData.PchRcConfig), sizeof(PCH_RC_CONFIGURATION));

  mAcpiParameter->TpmEnable = mSystemConfiguration.TpmEnable;

  // CpuPm.Asl: External (CSEN, FieldUnitObj)
  mAcpiParameter->CStateEnable = !mSocketPowermanagementConfiguration.ProcessorAutonomousCstateEnable;
  // CpuPm.Asl: External (ACEN, FieldUnitObj)
  mAcpiParameter->AutoCstate = mSocketPowermanagementConfiguration.ProcessorAutonomousCstateEnable;
  // CpuPm.Asl: External (C3EN, FieldUnitObj)
  mAcpiParameter->C3Enable     = mSocketPowermanagementConfiguration.C3Enable;
  // CpuPm.Asl: External (C6EN, FieldUnitObj)
  if (mSocketPowermanagementConfiguration.C6Enable == PPM_AUTO || mAcpiParameter->AutoCstate) {
    mAcpiParameter->C6Enable     = 1;  //POR Default = Enabled
  } else {
    mAcpiParameter->C6Enable     = mSocketPowermanagementConfiguration.C6Enable;
  }
  if(mAcpiParameter->C6Enable && mAcpiParameter->C3Enable) {  //C3 and C6 enable are exclusive
    mAcpiParameter->C6Enable = 1;
    mAcpiParameter->C3Enable = 0;
  }  
  
  // CpuPm.Asl: External (C7EN, FieldUnitObj)
  mAcpiParameter->C7Enable     = 0;
  // CpuPm.Asl: External (OSCX, FieldUnitObj)
  // If ACC enabled, leave OSCX set to 0 (C2)
  if(!mAcpiParameter->AutoCstate) {
    mAcpiParameter->OSCX         = mSocketPowermanagementConfiguration.OSCx;
  }

  //
  // SKX HSD 5355146 - Disable Monitor/MWAIT for 2LM  4S and 8S
  //
  if ((mIioUds->IioUdsPtr->SystemStatus.VolMemMode == VOL_MEM_MODE_2LM) &&
      ((mIioUds->IioUdsPtr->SystemStatus.MinimumCpuStepping == B0_REV_CLX) || (mIioUds->IioUdsPtr->SystemStatus.MinimumCpuStepping == B1_REV_CLX)) &&
      (mIioUds->IioUdsPtr->SystemStatus.numCpus >= 4) && (mIioUds->IioUdsPtr->SystemStatus.AepDimmPresent) && (mIioUds->IioUdsPtr->SystemStatus.MonitorMwaitEnabled == 0)) {
    //
    // Apply WA only when enabled (Monitor/MWAIT knob set as Auto/Disabled) and 2LM 4S/8S system detected
    //
    DEBUG ((DEBUG_INFO, "ACPI Platform - Disable Monitor/MWAIT for 2LM  4S and 8S for 2LM and 4S/8S\n"));
    // CpuPm.Asl: External (MWOS, FieldUnitObj)
    mAcpiParameter->MonitorMwaitEnable = 0;
  } else {
    // CpuPm.Asl: External (MWOS, FieldUnitObj)
    mAcpiParameter->MonitorMwaitEnable = 1;
  }
  //
  // SKX HSD 5355146 - Disable Monitor/MWAIT for 2LM  4S and 8S
  //

  // CpuPm.Asl: External (PSEN, FieldUnitObj)
  mAcpiParameter->PStateEnable = mPStateEnable;
  // CpuPm.Asl: External (HWAL, FieldUnitObj)
  mAcpiParameter->HWAllEnable = mSocketPowermanagementConfiguration.ProcessorEistPsdFunc;

  if (mIioUds->IioUdsPtr->SystemStatus.CpuPCPSInfo & B_PCPS_SPD_ENABLE) {
    if ((mSocketPowermanagementConfiguration.ProcessorSinglePCTLEn) && (!mSocketPowermanagementConfiguration.PStateDomain)) {
      mAcpiParameter->HWAllEnable = 2;  //force SW_ANY
    }

    if (mSocketPowermanagementConfiguration.ProcessorSinglePCTLEn == 2) {
      mAcpiParameter->HWAllEnable = 1;  //force SW_ALL
    }
  }

  mAcpiParameter->PBFEnable = 0;  // PBFE
  if (mIioUds->IioUdsPtr->SystemStatus.PbfCapable && mSocketPowermanagementConfiguration.ProcessorActivePbf) {
    mAcpiParameter->PBFEnable = 1;  // PBFE
  }

  mAcpiParameter->KBPresent    = mKBPresent;
  mAcpiParameter->MousePresent = mMousePresent;
  mAcpiParameter->TStateEnable = mSocketPowermanagementConfiguration.TStateEnable;
  //Fine grained T state
  AsmCpuid (EFI_CPUID_POWER_MANAGEMENT_PARAMS,  &CpuidRegisters.RegEax, &CpuidRegisters.RegEbx, &CpuidRegisters.RegEcx, &CpuidRegisters.RegEdx);
  if ((CpuidRegisters.RegEax & EFI_FINE_GRAINED_CLOCK_MODULATION) && mSocketPowermanagementConfiguration.TStateEnable){
    mAcpiParameter->TStateFineGrained = 1;
  }
  if(CpuidRegisters.RegEax & B_CPUID_POWER_MANAGEMENT_EAX_HWP_LVT_INTERRUPT_SUPPORT) {
    mAcpiParameter->HwpInterrupt = 1;
  }
  // CpuPm.Asl: External (HWEN, FieldUnitObj)
  mAcpiParameter->HWPMEnable = DetectHwpFeature();

  mAcpiParameter->EmcaEn    = mSystemConfiguration.EmcaEn;
  mAcpiParameter->WheaSupportEn  = mSystemConfiguration.WheaSupportEn;

  mAcpiParameter->PcieAcpiHotPlugEnable = (UINT8) (BOOLEAN) (mSocketIioConfiguration.PcieAcpiHotPlugEnable != 0);
  //
  // Initialize USB3 mode from setup data
  //
  // If mode != manual control
  //  just copy mode from setup
  if (mPchRcConfiguration.PchUsbManualMode != 1) {
    mAcpiParameter->XhciMode = mPchRcConfiguration.PchUsbManualMode;
  }
  if (IsSimicsPlatform() == TRUE) {
    // SKX_TODO: Windows cannot boot with MWAIT and Cstates enabled
    mAcpiParameter->CStateEnable = 0;
    mAcpiParameter->C6Enable     = 0;
    mAcpiParameter->MonitorMwaitEnable = 0;
  }

  // When X2APIC enabled and VTD support enabled, Enable ApicIdOverrided parameter to update ACPI table.
  if (mSocketIioConfiguration.VTdSupport == 1 && mSocketProcessorCoreConfiguration.ProcessorX2apic ==1){
    mAcpiParameter->ApicIdOverrided = 1;
  }

  //
  // Find the AcpiTable protocol
  //
  Status = LocateSupportProtocol (
            &gEfiAcpiTableProtocolGuid,
            gEfiAcpiTableStorageGuid,
            &AcpiTable,
            FALSE
            );

  ASSERT_EFI_ERROR (Status);

  TableVersion    = EFI_ACPI_TABLE_VERSION_2_0;
  Table.Signature = EFI_ACPI_2_0_SERIAL_PORT_CONSOLE_REDIRECTION_TABLE_SIGNATURE;
  Status = PlatformUpdateTables ((EFI_ACPI_COMMON_HEADER *)&Table, &TableVersion);
  if(!EFI_ERROR (Status)) {
    //
    // Add SPCR table
    //
    if (mSpcrTable != NULL) {
      DEBUG ((DEBUG_INFO, "mSpcrTable->Header.Length=%d\n", mSpcrTable->Header.Length));
      DEBUG ((DEBUG_INFO, "install=%x\n", &(AcpiTable->InstallAcpiTable)));
      DEBUG ((DEBUG_INFO, "acpit=%x\n",   AcpiTable));
      DEBUG ((DEBUG_INFO, "mSpcr=%x\n",   mSpcrTable));
      DEBUG ((DEBUG_INFO, "len   =%d\n",  mSpcrTable->Header.Length));

      TableHandle = 0;
      Status = AcpiTable->InstallAcpiTable (
        AcpiTable,
        mSpcrTable,
        mSpcrTable->Header.Length,
        &TableHandle
        );
      ASSERT_EFI_ERROR (Status);
    } else {
      DEBUG ((DEBUG_INFO, "Warning: mSpcrTable is NULL\n"));
    }
  }
  if (mSpcrTable != NULL) {
    gBS->FreePool (mSpcrTable);
  }

  VtdAcpiTablesUpdateFn();

  Status = GetOptionData (&gEfiSetupVariableGuid,  OFFSET_OF(SYSTEM_CONFIGURATION, ARIForward), &ARIForward, sizeof(UINT8));
  ASSERT_EFI_ERROR (Status);
  if (!ARIForward) {
    DisableAriForwarding();
  }

}

/**
  Installs ACPI Platform tables

  @param None

  @retval EFI_SUCCESS -  Operation completed successfully.
**/
STATIC
EFI_STATUS
EFIAPI
AcpiPlatformEarlyAcpiTablesInstall(
   VOID
   )
{
  EFI_STATUS                    Status;
  EFI_STATUS                    AcpiStatus;
  BOOLEAN                       Installed;
  EFI_ACPI_TABLE_PROTOCOL       *AcpiTable;
  EFI_FIRMWARE_VOLUME2_PROTOCOL  *FwVol;
  INTN                          Instance = 0;
  EFI_ACPI_COMMON_HEADER        *CurrentTable;
  EFI_ACPI_TABLE_VERSION        TableVersion;
  UINTN                         TableHandle;
  UINT32                        FvStatus;
  UINT32                        Size;

  //
  // Find the AcpiTable protocol
  //
  Status = LocateSupportProtocol (
            &gEfiAcpiTableProtocolGuid,
            gEfiAcpiTableStorageGuid,
            &AcpiTable,
            FALSE
            );

  ASSERT_EFI_ERROR (Status);

  //
  // Locate the firmware volume protocol
  //
  Status = LocateSupportProtocol (
            &gEfiFirmwareVolume2ProtocolGuid,
            gEfiAcpiTableStorageGuid,
            &FwVol,
            TRUE
            );
  ASSERT_EFI_ERROR (Status);

  //
  // Read tables from the storage file.
  //
  while (!EFI_ERROR (Status)) {

    CurrentTable = NULL;
    TableVersion = EFI_ACPI_TABLE_VERSION_NONE;
    TableHandle = 0;
    Installed = FALSE;

    Status = FwVol->ReadSection (
                      FwVol,
                      &gEfiAcpiTableStorageGuid,
                      EFI_SECTION_RAW,
                      Instance,
                      &CurrentTable,
                      (UINTN *) &Size,
                      &FvStatus
                      );

    if (!EFI_ERROR (Status)) {
      //
      // Check if table should be processed or will be updated later
      //
      if (CurrentTable->Signature != NVDIMM_FW_INTERFACE_TABLE_SIGNATURE) {
        //
        // Allow platform specific code to reject the table or update it
        //
        AcpiStatus = AcpiPlatformHooksIsActiveTable (CurrentTable); //SystemBoard);
        if (!EFI_ERROR (AcpiStatus)) {
          //
          // Perform any table specific updates.
          //
          AcpiStatus = PlatformUpdateTables (CurrentTable, &TableVersion);
          if (!EFI_ERROR (AcpiStatus)) {
            //
            // Add the table
            //
            if (TableVersion != EFI_ACPI_TABLE_VERSION_NONE) {
              //
              // Install the table
              //
              AcpiStatus = AcpiTable->InstallAcpiTable (AcpiTable, CurrentTable, CurrentTable->Length, &TableHandle);
              if (!EFI_ERROR (AcpiStatus)) {
                Installed = TRUE;
              }
              ASSERT_EFI_ERROR (AcpiStatus);
            }
          }
        }
        //
        // Update ACPI Platform Protocol data related with table
        //
        AcpiStatus = UpdateAcpiPlatformProtocolData (CurrentTable, Installed);
        ASSERT_EFI_ERROR (AcpiStatus);
      }
      //
      // Increment the instance
      //
      Instance++;
    }
  }

  return EFI_SUCCESS;
}

/**
  Installs ACPI Platform tables that wasn't installed in the early phase

  @param None

  @retval EFI_SUCCESS -  Operation completed successfully.
**/
STATIC
EFI_STATUS
EFIAPI
AcpiPlatformLateAcpiTablesInstall(
   VOID
   )
{
  //
  // InstallNfitAcpiTable;
  //
  InstallNfitAcpiTable ();

  //
  // Install xHCI ACPI Table
  //
  InstallXhciAcpiTable ();

  return EFI_SUCCESS;
}


/**
  Entry point for Acpi platform driver.

  @param ImageHandle  -  A handle for the image that is initializing this driver.
  @param SystemTable  -  A pointer to the EFI system table.

  @retval EFI_SUCCESS           -  Driver initialized successfully.
  @retval EFI_LOAD_ERROR        -  Failed to Initialize or has been loaded.
  @retval EFI_OUT_OF_RESOURCES  -  Could not allocate needed resources.
**/
EFI_STATUS
EFIAPI
AcpiPlatformEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS  Status;
  EFI_EVENT   Event;
  VOID        *HobList;

  mFirstNotify = FALSE;

  //
  // Report Status Code to indicate Acpi Init
  //
  REPORT_STATUS_CODE (
    EFI_PROGRESS_CODE,
    (EFI_SOFTWARE_DXE_CORE | EFI_SW_DXE_BS_ACPI_INIT)
    );

  //
  // Update HOB variable for PCI resource information
  // Get the HOB list.  If it is not present, then ASSERT.
  //
  Status = EfiGetSystemConfigurationTable (&gEfiHobListGuid, &HobList);
  ASSERT_EFI_ERROR (Status);

  //
  // Initialize Platform Hooks
  //
  PlatformHookInit ();
  UpdateVtdIntRemapFlag(mSocketIioConfiguration.VTdSupport, mSocketIioConfiguration.InterruptRemap);

  //
  // Install ACPI PLatform Tables
  //
  Status = AcpiPlatformEarlyAcpiTablesInstall ();
  ASSERT_EFI_ERROR (Status);

  //
  //
  // Install ACPI Platform Protocol
  //
  Status = InstallAcpiPlatformProtocol();
  ASSERT_EFI_ERROR (Status);

  //
  // Install ACPI PLatform Tables that wasn't installed yet (NFIT/xHCI)
  //
  Status = AcpiPlatformLateAcpiTablesInstall ();
  ASSERT_EFI_ERROR (Status);

  //
  // Register ready to boot event handler
  //
  Status = EfiCreateEventReadyToBootEx (TPL_NOTIFY, AcpiPlatformOnReadyToBoot, NULL, &Event);
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}
