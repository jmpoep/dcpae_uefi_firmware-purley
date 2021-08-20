/** @file
  This is the FSP driver that initializes the Intel PCH.

@copyright
  Copyright (c) 2015 Intel Corporation. All rights reserved
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
#include "PchInit.h"

/**
  PCH initialization before Boot Sript Table is closed

  @param[in]  PeiServices       An indirect pointer to the EFI_PEI_SERVICES table published by the PEI Foundation
  @param[in]  NotifyDescriptor  Address of the notification descriptor data structure.
  @param[in]  Ppi               Address of the PPI that was installed.

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_OUT_OF_RESOURCES    Do not have enough resources to initialize the driver
**/
EFI_STATUS
EFIAPI
PchOnEndOfDxeFsp (
  IN  EFI_PEI_SERVICES            **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR   *NotifyDescriptor,
  IN  VOID                        *Ppi
  );

STATIC EFI_PEI_NOTIFY_DESCRIPTOR  mPchInitNotifyList[] = {
  {
    EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
    &gEfiEventReadyToBootGuid,
    PchOnEndOfDxeFsp
  }
};

/**
  <b>FSP PchInit Module Entry Point for FSP</b>\n

  @param[in] FileHandle      PEIM's file handle
  @param[in] PeiServices     An indirect pointer to the EFI_PEI_SERVICES table published by the PEI Foundation

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_OUT_OF_RESOURCES    Do not have enough resources to initialize the driver
**/
EFI_STATUS
EFIAPI
PchInitEntryPointFsp (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS  Status;

  DEBUG ((DEBUG_INFO, "Fsp PchInitEntryPoint() Start\n"));

  PchInitEntryPoint();

  //
  // Performing Pch early init after PchPlatfromPolicy PPI produced
  //
  Status = PeiServicesNotifyPpi (mPchInitNotifyList);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "Fsp PchInitEntryPoint() End\n"));

  return Status;
}

/**
  Fsp PCH initialization before Boot Sript Table is closed

  @param[in]  PeiServices       An indirect pointer to the EFI_PEI_SERVICES table published by the PEI Foundation
  @param[in]  NotifyDescriptor  Address of the notification descriptor data structure.
  @param[in]  Ppi               Address of the PPI that was installed.

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_OUT_OF_RESOURCES    Do not have enough resources to initialize the driver
**/
EFI_STATUS
EFIAPI
PchOnEndOfDxeFsp (
  IN  EFI_PEI_SERVICES            **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR   *NotifyDescriptor,
  IN  VOID                        *Ppi
  )
{
  DEBUG ((DEBUG_INFO, "Fsp PchFspOnEndOfDxe() Start\n"));

  PchOnEndOfDxe ();

  DEBUG ((DEBUG_INFO, "Fsp PchFspOnEndOfDxe() End\n"));

  return EFI_SUCCESS;
}
