/** @file
  Implementation of the CLV PCI Express Error Handler.

  Copyright (c) 2009-2016 Intel Corporation.
  All rights reserved. This software and associated documentation
  (if any) is furnished under a license and may only be used or
  copied in accordance with the terms of the license.  Except as
  permitted by such license, no part of this software or
  documentation may be reproduced, stored in a retrieval system, or
  transmitted in any form or by any means without the express
  written consent of Intel Corporation.

**/
#include <Setup/IioUniversalData.h>
#include <IndustryStandard/Acpi.h>
#include <IndustryStandard/WheaDefs.h>
#include <Cpu/CpuBaseLib.h>
#include <Protocol/ErrorHandlingProtocol.h>
#include <Protocol/PlatformErrorHandlingProtocol.h>
#include <Protocol/PlatformRasPolicyProtocol.h>
#include <Protocol/SmmBase2.h>
#include <Protocol/SmmSwDispatch2.h>
#include <PciExpress.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/PciExpressLib.h>
#include <Library/IoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/mpsyncdatalib.h>


#include "PcieErrorHandler.h"
#include "ClvPcieErrorHandler.h"

RAS_CLV_PCIE_ERROR_HANDLING_PROTOCOL mRasClvPcieErrorHandlingProtocol;

/**
  Entry point for the Clv PCI Express Error Handler. 

  This function initializes the error handling and enables PCI
  Express error sources.

  @param[in] ImageHandle          Image handle of this driver.
  @param[in] SystemTable          Global system service table.

  @retval Status.
**/
EFI_STATUS
EFIAPI
ClvInitializePcieErrHandler (
  IN      EFI_HANDLE          ImageHandle,
  IN      EFI_SYSTEM_TABLE    *SystemTable
  )
{
  EFI_STATUS                      Status;

  // Call the original entry point
  Status = InitializePcieErrHandler (ImageHandle, SystemTable);
  ClvPcieErrHandlingInitializeHooks ();

  return Status;
}

/**
  Install Protocol for PCIe Error Handler Validation Purposes.

  @retval None.
**/
VOID
ClvPcieErrHandlingInitializeHooks(
  VOID
  )
{
  EFI_HANDLE    Handle = NULL;
  EFI_STATUS    Status;

  // Exported functions
  mRasClvPcieErrorHandlingProtocol.PcieErrIsSupported                       = PciErrIsSupported;
  mRasClvPcieErrorHandlingProtocol.IIOErrorEnable                           = IIOErrorEnable;
  mRasClvPcieErrorHandlingProtocol.PciErrLibEnablePciSerrPerrGeneration     = PciErrLibEnablePciSerrPerrGeneration;
  mRasClvPcieErrorHandlingProtocol.PciErrLibEnableBridgeControl             = PciErrLibEnableBridgeControl;
  mRasClvPcieErrorHandlingProtocol.PciErrCheckStatus                        = PciErrCheckStatus;
  mRasClvPcieErrorHandlingProtocol.ClearIioErrors                           = ClearIioErrors;
  mRasClvPcieErrorHandlingProtocol.ProcessIioErrors                         = ProcessIioErrors;
  mRasClvPcieErrorHandlingProtocol.EnableElogPCH                            = EnableElogPCH;
  mRasClvPcieErrorHandlingProtocol.ElogPchPciEnable                         = ElogPchPciEnable;
  mRasClvPcieErrorHandlingProtocol.ElogPchRootPortEnable                    = ElogPchRootPortEnable;
  mRasClvPcieErrorHandlingProtocol.ProcessPchErrors                         = ProcessPchErrors;

  // Exported variables
  mRasClvPcieErrorHandlingProtocol.HandledSeverity            = &mSeverity;
  mRasClvPcieErrorHandlingProtocol.EnablePerr                 = &mNmiOnPerr;
  mRasClvPcieErrorHandlingProtocol.EnableSerr                 = &mNmiOnSerr;

  if (gSmst == NULL) {
    ASSERT (FALSE);
  }
  Status = gSmst->SmmInstallProtocolInterface (
    &Handle,
    &gRasClvPcieErrHandProtocol,
    EFI_NATIVE_INTERFACE,
    &mRasClvPcieErrorHandlingProtocol
  );
  ASSERT_EFI_ERROR (Status);
}
