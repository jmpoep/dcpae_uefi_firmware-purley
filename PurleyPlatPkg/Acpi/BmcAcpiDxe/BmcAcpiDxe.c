/**
  This file contains an 'Intel Peripheral Driver' and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement
**/
/**

Copyright (c)  1999 - 2016, Intel Corporation. All rights reserved
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

#include "BmcAcpiDxe.h"
#include <SioRegs.h>

extern EFI_GUID gEfiAcpiSupportProtocolGuid;

// {A381A73E-23C9-4939-8F88-30B544DC4023}
EFI_GUID gEfiBmcAcpiTablesGuid = { 0xa381a73e, 0x23c9, 0x4939, { 0x8f, 0x88, 0x30, 0xb5, 0x44, 0xdc, 0x40, 0x23 } };


UINT32  
IsSioExist (
  VOID
);

/**

  Locate the first instance of a protocol.  If the protocol requested is an
  FV protocol, then it will return the first FV that contains the ACPI table
  storage file.

  @param Protocol  -  The protocol to find.
  Instance  -  Return pointer to the first instance of the protocol.
  Type      -  The type of protocol to locate.

  @retval EFI_SUCCESS           -  The function completed successfully.
  @retval EFI_NOT_FOUND         -  The protocol could not be located.
  @retval EFI_OUT_OF_RESOURCES  -  There are not enough resources to find the protocol.

**/
EFI_STATUS
LocateSupportProtocol (
  IN   EFI_GUID       *Protocol,
  OUT  VOID           **Instance,
  IN   UINT32         Type
  )
{
  EFI_STATUS              Status;
  EFI_HANDLE              *HandleBuffer;
  UINTN                   NumberOfHandles;
  EFI_FV_FILETYPE         FileType;
  UINT32                  FvStatus = 0;
  EFI_FV_FILE_ATTRIBUTES  Attributes;
  UINTN                   Size;
  UINTN                   Index;

  Status = gBS->LocateHandleBuffer (ByProtocol, Protocol, NULL, &NumberOfHandles, &HandleBuffer);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  //
  // Looking for FV with ACPI storage file
  //
  for (Index = 0; Index < NumberOfHandles; Index++) {
    Status = gBS->HandleProtocol (HandleBuffer[Index], Protocol, Instance);
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
                                                              &gEfiBmcAcpiTablesGuid,
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

  gBS->FreePool (HandleBuffer);
  return Status;
}


EFI_STATUS
UpdateDeviceSsdtTable (
  IN OUT EFI_ACPI_COMMON_HEADER     *Table
  )
{
  EFI_ACPI_DESCRIPTION_HEADER               *TableHeader = NULL;
  UINT64                                    TempOemId, TempOemTableId;

  UINT32                                    SioExist = 0;
  UINT64                                    PilotSignature = SIGNATURE_64 ('P', 'I', 'L', 'O', 'T', 'S', 'D', 'T');
  UINT64                                    AspeedSignature = SIGNATURE_64 ('A', 'S', 'P', 'E', 'E', 'D', 'T', 'L');

  SioExist    = IsSioExist ();
  TableHeader = (EFI_ACPI_DESCRIPTION_HEADER *) Table;

  if ((SioExist & PILOTIV_EXIST)  && (TableHeader->OemTableId == PilotSignature)) {
    DEBUG ((EFI_D_INFO, "[BmcAcpi] found Pilot BMC, install ACPI table.\n"));

  } else if ((SioExist & PC8374_EXIST)  && (TableHeader->OemTableId == PilotSignature)) {
    DEBUG ((EFI_D_INFO, "[BmcAcpi] found PC8374 BMC, install ACPI table.\n"));
    
  } else if ((SioExist & ASPEED2500_EXIST)  && (TableHeader->OemTableId == AspeedSignature)) {
    DEBUG ((EFI_D_INFO, "[BmcAcpi] found ASPEED BMC, install ACPI table.\n"));
    
  } else {
    return EFI_NOT_FOUND;
  }

  //
  // Update the OEMID and OEM Table ID.
  //
  TempOemId       = PcdGet64(PcdAcpiOemId);
  TempOemTableId  = PcdGet64(PcdAcpiTableOemId);

  CopyMem (&TableHeader->OemId, &TempOemId, 6);
  CopyMem (&TableHeader->OemTableId, &TempOemTableId, 8);
  TableHeader->CreatorId        = EFI_ACPI_CREATOR_ID;
  TableHeader->CreatorRevision  = EFI_ACPI_CREATOR_REVISION;
  
  return EFI_SUCCESS;
}

VOID
BmcAcpiUpdateChecksum (
  IN UINT8      *Buffer,
  IN UINTN      Size
  )
{
  UINTN ChecksumOffset;

  ChecksumOffset = OFFSET_OF (EFI_ACPI_DESCRIPTION_HEADER, Checksum);

  //
  // Set checksum to 0 first
  //
  Buffer[ChecksumOffset] = 0;

  //
  // Update checksum value
  //
  Buffer[ChecksumOffset] = CalculateCheckSum8(Buffer, Size);
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
BmcAcpiEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS                    Status;
  EFI_STATUS                    AcpiStatus;

  EFI_FIRMWARE_VOLUME2_PROTOCOL  *FwVol;
  INTN                          Instance = 0;
  EFI_ACPI_COMMON_HEADER        *CurrentTable = NULL;
  UINTN                         TableHandle = 0;
  UINT32                        FvStatus;
  UINT32                        Size;

  EFI_ACPI_TABLE_PROTOCOL        *AcpiTable;
  UINTN                          TableSize;

  //
  // Find the AcpiTable protocol
  //
  Status = gBS->LocateProtocol (&gEfiAcpiTableProtocolGuid, NULL, (VOID**)&AcpiTable);
  if (EFI_ERROR (Status)) {
    return EFI_ABORTED;
  }

  //
  // Locate the firmware volume protocol
  //
  Status = LocateSupportProtocol (&gEfiFirmwareVolume2ProtocolGuid, &FwVol, 1);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status    = EFI_SUCCESS;
  Instance  = 0;

  //
  // Read tables from the storage file.
  //
  while (!EFI_ERROR (Status)) {
    CurrentTable = NULL;

    Status = FwVol->ReadSection (FwVol, &gEfiBmcAcpiTablesGuid, EFI_SECTION_RAW, Instance, &CurrentTable, (UINTN *) &Size, &FvStatus);
    if (!EFI_ERROR (Status)) {
      //
      // Perform any table specific updates.
      //
      AcpiStatus = UpdateDeviceSsdtTable (CurrentTable);
      if (!EFI_ERROR (AcpiStatus)) {

        TableHandle = 0;
        TableSize = ((EFI_ACPI_DESCRIPTION_HEADER *) CurrentTable)->Length;
        ASSERT (Size >= TableSize);

        BmcAcpiUpdateChecksum ((UINT8*)CurrentTable, TableSize);

        Status = AcpiTable->InstallAcpiTable (
                              AcpiTable,
                              CurrentTable,
                              TableSize,
                              &TableHandle
                              );

        ASSERT_EFI_ERROR (Status);
      }

      //
      // Increment the instance
      //
      Instance++;
    }
  }

  return EFI_SUCCESS;
}
