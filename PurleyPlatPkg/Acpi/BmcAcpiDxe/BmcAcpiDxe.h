//
// This file contains an 'Intel Peripheral Driver' and is
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may
// be modified by the user, subject to additional terms of the
// license agreement
//
/**

Copyright (c)  1999 - 2015 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


  @file BmcAcpiDxe.h

--*/

#ifndef _ACPI_BMC_H_
#define _ACPI_BMC_H_

//
// Statements that include other header files
//
#include <PiDxe.h>

#include <IndustryStandard/Acpi.h>
#include <Protocol/AcpiSystemDescriptionTable.h>
#include <Protocol/FirmwareVolume2.h>
#include <Protocol/AcpiTable.h>

#include <Library/BaseLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiLib.h>

#ifndef EFI_ACPI_CREATOR_ID
#define EFI_ACPI_CREATOR_ID       SIGNATURE_32 ('M', 'S', 'F', 'T')
#endif
#ifndef EFI_ACPI_CREATOR_REVISION
#define EFI_ACPI_CREATOR_REVISION 0x0100000D
#endif


//
// Protocol private structure definition
//


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
BmcAcpiEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  );

EFI_STATUS
LocateSupportProtocol (
  IN   EFI_GUID       *Protocol,
  OUT  VOID           **Instance,
  IN   UINT32         Type
  );

#endif
