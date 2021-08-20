/** @file
  Initializes the PCH HD Audio ACPI Tables.

@copyright
 Copyright (c) 2014 Intel Corporation. All rights reserved
 This software and associated documentation (if any) is furnished
 under a license and may only be used or copied in accordance
 with the terms of the license. Except as permitted by the
 license, no part of this software or documentation may be
 reproduced, stored in a retrieval system, or transmitted in any
 form or by any means without the express written consent of
 Intel Corporation.
 This file contains an 'Intel Peripheral Driver' and is uniquely
 identified as "Intel Reference Module" and is licensed for Intel
 CPUs and chipsets under the terms of your license agreement with
 Intel or your vendor. This file may be modified by the user, subject
 to additional terms of the license agreement.

@par Specification Reference:
**/

#include <PchInit.h>
#include <Guid/Acpi.h>
#include <Library/PcdLib.h>
#include <IncludePrivate/Library/PchHdaLib.h>
#include <SaRegs.h>
#include <Cpu/CpuRegs.h>

PCH_HDA_NHLT_ENDPOINTS mPchHdaNhltEndpoints[HdaEndpointMax] =
{
  {HdaDmicX2,     FALSE},
  {HdaDmicX4,     FALSE},
  {HdaBtRender,   FALSE},
  {HdaBtCapture,  FALSE},
  {HdaI2sRender,  FALSE},
  {HdaI2sCapture, FALSE}
};

/**
  Retrieves address of NHLT table from XSDT/RSDT.

  @retval NHLT_ACPI_TABLE*  Pointer to NHLT table if found
  @retval NULL              NHLT could not be found
**/
NHLT_ACPI_TABLE *
LocateNhltAcpiTable (
VOID
  )
{
  EFI_ACPI_3_0_ROOT_SYSTEM_DESCRIPTION_POINTER  *Rsdp;
  EFI_ACPI_DESCRIPTION_HEADER                   *Xsdt;
  NHLT_ACPI_TABLE                               *Nhlt;
  UINTN                                         Index;
  UINT32                                        Data32;
  EFI_STATUS                                    Status;
  Rsdp  = NULL;
  Xsdt  = NULL;
  Nhlt  = NULL;

  ///
  /// Find the AcpiSupport protocol returns RSDP (or RSD PTR) address.
  ///
  DEBUG ((DEBUG_INFO, "LocateNhltAcpiTable() Start\n"));

  Status = EfiGetSystemConfigurationTable (&gEfiAcpiTableGuid, (VOID *) &Rsdp);
  if (EFI_ERROR (Status) || (Rsdp == NULL)) {
    DEBUG ((DEBUG_ERROR, "EFI_ERROR or Rsdp == NULL\n"));
    return NULL;
  }

  Xsdt = (EFI_ACPI_DESCRIPTION_HEADER *)(UINTN) Rsdp->XsdtAddress;
  if (Xsdt == NULL || Xsdt->Signature != EFI_ACPI_5_0_EXTENDED_SYSTEM_DESCRIPTION_TABLE_SIGNATURE) {
    // If XSDT has not been found, check RSDT
    Xsdt = (EFI_ACPI_DESCRIPTION_HEADER *)(UINTN) Rsdp->RsdtAddress;
    if (Xsdt == NULL || Xsdt->Signature != EFI_ACPI_5_0_ROOT_SYSTEM_DESCRIPTION_TABLE_SIGNATURE) {
      DEBUG ((DEBUG_ERROR, "XSDT/RSDT == NULL or wrong signature\n"));
      return NULL;
    }
  }

  for (Index = sizeof (EFI_ACPI_DESCRIPTION_HEADER); Index < Xsdt->Length; Index = Index + sizeof (UINT32)) {
    Data32  = *(UINT32 *) ((UINT8 *) Xsdt + Index);
    Nhlt    = (NHLT_ACPI_TABLE *) (UINT32 *) (UINTN) Data32;
    if (Nhlt->Header.Signature == NHLT_ACPI_TABLE_SIGNATURE) {
      break;
    }
  }

  if (Nhlt == NULL || Nhlt->Header.Signature != NHLT_ACPI_TABLE_SIGNATURE) {
    DEBUG ((DEBUG_ERROR, "Nhlt == NULL or wrong signature\n"));
    return NULL;
  }

  DEBUG ((DEBUG_INFO, "Found NhltTable, Address = 0x%x\n", Nhlt));

  return Nhlt;
}

/**
  Constructs and installs NHLT table.

  @retval EFI_SUCCESS       ACPI Table installed successfully
  @retval EFI_UNSUPPORTED   ACPI Table protocol not found
**/
EFI_STATUS
PublishNhltAcpiTable (
  VOID
  )
{
  UINTN                                     AcpiTableKey;
  EFI_ACPI_TABLE_PROTOCOL                   *AcpiTable;
  NHLT_ACPI_TABLE                           *NhltTable;
  UINT32                                    TableLength;
  EFI_STATUS                                Status;

  AcpiTable = NULL;
  NhltTable = NULL;
  AcpiTableKey = 0;

  DEBUG ((DEBUG_INFO, "PublishNhltAcpiTable() Start\n"));

  //
  // Locate ACPI support protocol
  //
  Status = gBS->LocateProtocol (&gEfiAcpiTableProtocolGuid, NULL, (VOID **) &AcpiTable);
  if( EFI_ERROR(Status) || AcpiTable == NULL) {
    return EFI_UNSUPPORTED;
  }

  NhltConstructor (mPchHdaNhltEndpoints, &NhltTable, &TableLength);
  NhltAcpiHeaderConstructor (NhltTable, TableLength);

  Status = AcpiTable->InstallAcpiTable (AcpiTable, NhltTable, NhltTable->Header.Length, &AcpiTableKey);

  DEBUG ((DEBUG_INFO, "PublishNhltAcpiTable() End\n"));
  return Status;
}

/**
  Sets NVS ACPI variables for _DSM accordingly to policy.

  @param[in]                NhltAcpiTableAddress
  @param[in]                NhltAcpiTableLength
  @param[in]                DspFeatureMask
  @param[in]                DspPpModuleMask
**/
VOID
UpdateHdaAcpiData(
  IN      UINT32   NhltAcpiTableAddress,
  IN      UINT32   NhltAcpiTableLength,
  IN      UINT32   DspFeatureMask,
  IN      UINT32   DspPpModuleMask
  )
{
  DEBUG ((DEBUG_INFO, "UpdateHdaAcpiData():\n NHLT Address = 0x%08x, Length = 0x%08x\n", NhltAcpiTableAddress, NhltAcpiTableLength));
  DEBUG ((DEBUG_INFO, " FeatureMask = 0x%08x\n PostProcessingModuleMask = 0x%08x\n", DspFeatureMask, DspPpModuleMask));

  mPchNvsAreaProtocol.Area->NHLA = NhltAcpiTableAddress;
  mPchNvsAreaProtocol.Area->NHLL = NhltAcpiTableLength;
  mPchNvsAreaProtocol.Area->ADFM = DspFeatureMask;
  mPchNvsAreaProtocol.Area->ADPM = DspPpModuleMask;
}

/**
  Initialize and publish NHLT (Non-HDA Link Table), update NVS variables.

  @param[in]                    *HdaConfig

  @retval EFI_SUCCESS    The function completed successfully
**/
EFI_STATUS
SetHdaAcpiTable (
  IN CONST PCH_HDAUDIO_CONFIG            *HdaConfig
  )
{
  NHLT_ACPI_TABLE     *NhltTable;
  EFI_STATUS          Status;
  NhltTable = NULL;

  // DMIC X2
  mPchHdaNhltEndpoints[HdaDmicX2].Enable   = TRUE;

  if (GetPchSeries () == PchLp) {
    //
    // PCH-LP NHLT Endpoints configuration
    // Both BT and I2S enabled
    //
    mPchHdaNhltEndpoints[HdaBtRender].Enable   = TRUE;
    mPchHdaNhltEndpoints[HdaBtCapture].Enable  = TRUE;
    mPchHdaNhltEndpoints[HdaI2sRender].Enable  = TRUE;
    mPchHdaNhltEndpoints[HdaI2sCapture].Enable = TRUE;
  } else {
    //
    // PCH-H NHLT Endpoints configuration
    // BT enabled / I2S disabled
    //
    mPchHdaNhltEndpoints[HdaBtRender].Enable   = TRUE;
    mPchHdaNhltEndpoints[HdaBtCapture].Enable  = TRUE;
  }

  Status    = PublishNhltAcpiTable ();
  NhltTable = LocateNhltAcpiTable ();
  if(NhltTable == NULL) {
    return EFI_LOAD_ERROR;
  }

  UpdateHdaAcpiData ((UINT32)(UINTN)NhltTable, (UINT32)(NhltTable->Header.Length), HdaConfig->DspFeatureMask, HdaConfig->DspPpModuleMask);

  DEBUG_CODE ( NhltAcpiTableDump (NhltTable); );
  return Status;
}

/**
  Initialize Intel High Definition Audio ACPI Tables

  @retval EFI_SUCCESS        The function completed successfully
  @retval EFI_LOAD_ERROR     ACPI table cannot be installed
  @retval EFI_UNSUPPORTED    ACPI table not set because DSP is disabled
**/
EFI_STATUS
PchHdAudioAcpiInit (
  VOID
  )
{
  EFI_STATUS                    Status;
  CONST PCH_HDAUDIO_CONFIG      *HdaConfig;
  UINTN                         HdaPciBase;

  DEBUG ((DEBUG_INFO, "PchHdAudioAcpiInit() Start\n"));

  HdaConfig = &mPchPolicyHob->HdAudioConfig;

  HdaPciBase = MmPciBase (
                 DEFAULT_PCI_BUS_NUMBER_PCH,
                 PCI_DEVICE_NUMBER_PCH_HDA,
                 PCI_FUNCTION_NUMBER_PCH_HDA
                 );

  if((MmioRead16 (HdaPciBase + PCI_VENDOR_ID_OFFSET) == 0xFFFF) || (HdaConfig->DspEnable == FALSE)) {
    // Do not set ACPI tables if HDAudio is Function disabled or DSP is disabled
    DEBUG ((DEBUG_INFO, "AudioDSP disabled: Non-HDAudio ACPI Table (NHLT) not set!\n"));
    return EFI_UNSUPPORTED;
  }

  Status = SetHdaAcpiTable (HdaConfig);

  DEBUG ((DEBUG_INFO, "PchHdAudioAcpiInit() End - Status = %r\n", Status));
  return Status;
}

