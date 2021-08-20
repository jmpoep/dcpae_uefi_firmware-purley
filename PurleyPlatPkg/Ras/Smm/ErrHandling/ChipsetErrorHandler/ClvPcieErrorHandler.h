/** @file
  Contains definitions relevant to the CLV PCI Express Error Handler.

  Copyright (c) 2009-2017 Intel Corporation.
  All rights reserved. This software and associated documentation
  (if any) is furnished under a license and may only be used or
  copied in accordance with the terms of the license.  Except as
  permitted by such license, no part of this software or
  documentation may be reproduced, stored in a retrieval system, or
  transmitted in any form or by any means without the express
  written consent of Intel Corporation.

**/

#ifndef _CLV_PCIE_ERROR_HANDLER_H
#define _CLV_PCIE_ERROR_HANDLER_H

#include <Ras/RasCLVDefinitions.h>

extern UINT32     mSeverity;
extern BOOLEAN    mNmiOnSerr;
extern BOOLEAN    mNmiOnPerr;

BOOLEAN
EFIAPI
PciErrIsSupported (
  VOID
  );

BOOLEAN
EFIAPI
PciErrCheckStatus (
  VOID
  );

VOID
ClearIioErrors(
  VOID
  );

EFI_STATUS
IIOErrorEnable(
  IN      UINT8   RootBridge
  );

EFI_STATUS
ProcessIioErrors(
  VOID
  );

VOID
PciErrLibEnableBridgeControl (
  IN      UINT8   Socket,
  IN      UINT8   Bus,
  IN      UINT8   Device,
  IN      UINT8   Function
  );

EFI_STATUS
EFIAPI
InitializePcieErrHandler (
  IN      EFI_HANDLE          ImageHandle,
  IN      EFI_SYSTEM_TABLE    *SystemTable
  );

EFI_STATUS
EnableElogPCH (
  VOID
  );

VOID
ElogPchPciEnable (
  VOID
  );

VOID
ElogPchRootPortEnable (
  IN      UINT8   Bus
  );

EFI_STATUS
ProcessPchErrors (
  VOID
  );

VOID
ClvPcieErrHandlingInitializeHooks(
  VOID
  );

#endif
