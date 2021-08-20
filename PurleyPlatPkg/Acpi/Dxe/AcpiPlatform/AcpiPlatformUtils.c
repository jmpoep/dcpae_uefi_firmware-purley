  /**
  This file contains an 'Intel Peripheral Driver' and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement
**/
/**

Copyright (c) 2017, Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


  @file AcpiPlatformUtils.c

  ACPI Platform Utilities

**/

#include "AcpiPlatformUtils.h"
#include <IndustryStandard/Acpi.h>
#include <Protocol/AcpiSystemDescriptionTable.h>
#include <Protocol/AcpiPlatformProtocol.h>
#include <Protocol/FirmwareVolume2.h>
#include <Library/BaseLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/HobLib.h>
#include <Library/UefiLib.h>
#include <Cpu/CpuRegs.h>
#include <Acpi/Mcfg.h>
#include <Acpi/Hpet.h>
#include <Acpi/Srat.h>
#include <Acpi/Pcct.h>
#include <Acpi/Slit.h>
#include <Acpi/Migt.h>
#include <Acpi/Svos.h>
#include <Acpi/Msct.h>
#include <Acpi/Bdat.h>

static ACPI_PLATFORM_PROTOCOL   mAcpiPlatformProtocol;
static BOOLEAN                  mAcpiPlatformProtocolInstalled;

struct _ACPI_PLATFORM_UTILS_MEM_AFF_DATA {
  EFI_ACPI_6_1_MEMORY_AFFINITY_STRUCTURE AcpiMemAffData[EFI_ACPI_MEMORY_AFFINITY_STRUCTURE_COUNT];
  BOOLEAN                                AcpiMemAffDataValid;
} mAcpiPlatformMemAffData;

/**
  Function retrieves selected data of ACPI SRAT Memory Affinity Structures
  (please note that data will not be available until SRAT table installation)

  @param[out] *MemAffData         ACPI Memory Affinity Data
  @param[out] *MemAffDataLength   ACPI Memory Affinity Data Length

  @retval EFI_SUCCESS             ACPI Memory Affinity Data retrieved successfully
  @retval EFI_NOT_FOUND           ACPI Memory Affinity Data not found (SRAT ACPI table was not published)
  @retval EFI_INVALID_PARAMETER   One or more of input arguments is NULL
**/
EFI_STATUS
GetAcpiMemoryAffinityData (
  OUT ACPI_MEMORY_AFFINITY_DATA **MemAffData,
  OUT UINTN                     *MemAffDataLength
  )
{
  if (MemAffData == NULL || MemAffDataLength == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (!mAcpiPlatformMemAffData.AcpiMemAffDataValid) {
    return EFI_NOT_FOUND;
  }

  *MemAffData = mAcpiPlatformMemAffData.AcpiMemAffData;
  *MemAffDataLength = (UINT8)EFI_ACPI_MEMORY_AFFINITY_STRUCTURE_COUNT;

  return EFI_SUCCESS;
}

/**
  Function initialize and install ACPI Platform Protocol

  @param None

  @retval EFI_SUCCESS Operation completed successfully

**/
EFI_STATUS
InstallAcpiPlatformProtocol (
  VOID
  )
{
  EFI_HANDLE Handle = NULL;

  ZeroMem(&mAcpiPlatformProtocol, sizeof(mAcpiPlatformProtocol));
  mAcpiPlatformProtocol.GetAcpiMemoryAffinityData = GetAcpiMemoryAffinityData;

  return gBS->InstallProtocolInterface (&Handle, &gAcpiPlatformProtocolGuid, EFI_NATIVE_INTERFACE, &mAcpiPlatformProtocol);
}

/**

  Updates ACPI platform protocol data related with given ACPI platform table

  @param[in]  *Table            - Pointer to the ACPI platform table
  @param[in]  Installed         - Indicates whether ACPI platform table was published

  @retval EFI_SUCCESS           - The function completed successfully
  @retval EFI_INVALID_PARAMETER - The ACPI platform table pointer invalid
  @retval EFI_NOT_FOUND         - No data to update

**/
EFI_STATUS
UpdateAcpiPlatformProtocolData (
  IN EFI_ACPI_COMMON_HEADER     *Table,
  IN BOOLEAN                    Installed
  )
{
  EFI_ACPI_4_0_STATIC_RESOURCE_AFFINITY_TABLE *SratTable;

  if (Table == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  switch (Table->Signature) {
    case SIGNATURE_32('S', 'R', 'A', 'T'):
      if (Installed) {
        DEBUG ((DEBUG_INFO, "ACPI Platform Protocol - Memory Affinity Data updated\n"));
        ZeroMem(&mAcpiPlatformMemAffData, sizeof(mAcpiPlatformMemAffData));
        SratTable = (EFI_ACPI_4_0_STATIC_RESOURCE_AFFINITY_TABLE *)Table;
        CopyMem(mAcpiPlatformMemAffData.AcpiMemAffData, SratTable->Memory, sizeof(SratTable->Memory));
        mAcpiPlatformMemAffData.AcpiMemAffDataValid = TRUE;
      } else {
        DEBUG ((DEBUG_INFO, "ACPI Platform Protocol - SRAT table will not be published - Memory Affinity Data not updated\n"));
        mAcpiPlatformMemAffData.AcpiMemAffDataValid = FALSE;
      }
      break;

    default:
      // Currently ACPI Platform Protocol is used only to share SRAT ACPI Platform Table data
      break;
  }

  return EFI_SUCCESS;
}

/**

  Locate the first instance of a protocol.  If the protocol requested is an
  FV protocol, then it will return the first FV that contains the ACPI table
  storage file.

  @param[in]  Protocol            - The protocol to find.
  @param[in]  EfiAcpiStorageGuid  - EFI ACPI tables storage guid
  @param[out] Instance            -  Return pointer to the first instance of the protocol.
  @param[in]  Type                -  The type of protocol to locate.

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
  )
{
  EFI_STATUS              Status;
  EFI_HANDLE              *HandleBuffer;
  UINTN                   NumberOfHandles;
  EFI_FV_FILETYPE         FileType;
  UINT32                  FvStatus;
  EFI_FV_FILE_ATTRIBUTES  Attributes;
  UINTN                   Size;
  UINTN                   Index;

  FvStatus = 0;
  //
  // Locate protocol.
  //
  Status = gBS->LocateHandleBuffer (
                                    ByProtocol,
                                    Protocol,
                                    NULL,
                                    &NumberOfHandles,
                                    &HandleBuffer
                                    );
  if (EFI_ERROR (Status)) {
    //
    // Defined errors at this time are not found and out of resources.
    //
    return Status;
  }
  //
  // Looking for FV with ACPI storage file
  //
  for (Index = 0; Index < NumberOfHandles; Index++) {
    //
    // Get the protocol on this handle
    // This should not fail because of LocateHandleBuffer
    //
    Status = gBS->HandleProtocol (
                                  HandleBuffer[Index],
                                  Protocol,
                                  Instance
                                  );
    ASSERT (!EFI_ERROR (Status));

    if (!Type) {
      //
      // Not looking for the FV protocol, so find the first instance of the
      // protocol.  There should not be any errors because our handle buffer
      // should always contain at least one or LocateHandleBuffer would have
      // returned not found.
      //
      break;
    }
    //
    // See if it has the ACPI storage file
    //
    Status = ((EFI_FIRMWARE_VOLUME2_PROTOCOL *) (*Instance))->ReadFile (
                                                                        *Instance,
                                                                        &EfiAcpiStorageGuid,
                                                                        NULL,
                                                                        &Size,
                                                                        &FileType,
                                                                        &Attributes,
                                                                        &FvStatus
                                                                        );

    //
    // If we found it, then we are done
    //
    if (!EFI_ERROR (Status)) {
      break;
    }
  }
  //
  // Our exit status is determined by the success of the previous operations
  // If the protocol was found, Instance already points to it.
  //
  //
  // Free any allocated buffers
  //
  gBS->FreePool (HandleBuffer);

  return Status;
}
