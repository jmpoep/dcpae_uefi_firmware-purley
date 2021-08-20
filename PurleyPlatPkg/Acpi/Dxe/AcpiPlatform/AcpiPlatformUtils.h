//
// This file contains an 'Intel Peripheral Driver' and is
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may
// be modified by the user, subject to additional terms of the
// license agreement
//
/**

Copyright (c)  2017 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


  @file AcpiPlatformUtils.h

--*/

#ifndef _ACPI_PLATFORM_UTILS_H_
#define _ACPI_PLATFORM_UTILS_H_

extern EFI_GUID gEfiAcpiTableStorageGuid;

/**
  Function initialize and install ACPI Platform Protocol

  @param None

  @retval EFI_SUCCESS Operation completed successfully

**/
EFI_STATUS
InstallAcpiPlatformProtocol (
  VOID
  );

/**
  Function checks if ACPI Platform Protocol is ready to install

  @param None

  @retval TRUE  ACPI Platform Protocol can be installed,
          FALSE ACPI Platform Protocol not ready yet

**/
BOOLEAN
IsAcpiPlatformProtocolReadyForInstall (
  VOID
  );

/**
  Function checks if ACPI Platform Protocol is already installed

  @param None

  @retval TRUE  ACPI Platform Protocol is installed,
          FALSE ACPI Platform Protocol not installed yet

**/
BOOLEAN
IsAcpiPlatformProtocolInstalled (
  VOID
  );

/**

  Locate the first instance of a protocol.  If the protocol requested is an
  FV protocol, then it will return the first FV that contains the ACPI table
  storage file.

  @param[in]  Protocol            - The protocol to find.
  @param[in]  EfiAcpiStorageGuid  - EFI ACPI tables storage guid
  @param[out] Instance            - Return pointer to the first instance of the protocol.
  @param[in]  Type                - The type of protocol to locate.

  @retval EFI_SUCCESS           -  The function completed successfully.
  @retval EFI_NOT_FOUND         -  The protocol could not be located.
  @retval EFI_OUT_OF_RESOURCES  -  There are not enough resources to find the protocol.

**/
EFI_STATUS
LocateSupportProtocol (
  IN   EFI_GUID       *Protocol,
  IN   EFI_GUID       EfiAcpiStorageGuid,
  OUT  VOID           **Instance,
  IN   UINT32         Type
  );

/**

  Updates ACPI platform protocol data related with given ACPI platform table

  @param[in]  *Table            - Pointer to the ACPI platform table
  @param[in]  IsActive          - Indicates whether ACPI platform table is active and will be installed

  @retval EFI_SUCCESS           - The function completed successfully
  @retval EFI_INVALID_PARAMETER - The ACPI platform table pointer invalid
  @retval EFI_NOT_FOUND         - No data to update

**/
EFI_STATUS
UpdateAcpiPlatformProtocolData (
  IN EFI_ACPI_COMMON_HEADER     *Table,
  IN BOOLEAN                    IsActive
  );

#endif // _ACPI_PLATFORM_UTILS_H_
