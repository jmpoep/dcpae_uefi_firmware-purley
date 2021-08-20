/** @file
  This driver module produces IDE_CONTROLLER_INIT protocol for serial ATA
  driver and will be used by ATA Bus driver to support chipset dependent timing
  information, config SATA control/status registers. This driver
  is responsible for early initialization of serial ATA controller.

  Serial ATA spec requires SATA controller compatible with parallel IDE
  controller. That's why lots of code here is the same with IDE controller
  driver. However, We need this driver to optimize timing settings for SATA
  device and set SATA config/error/status registers.

@copyright
 Copyright (c) 1999 - 2018 Intel Corporation. All rights reserved
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
#include "SataController.h"
#include <Library/PcdLib.h>

///
///  EFI_DRIVER_BINDING_PROTOCOL instance
///
GLOBAL_REMOVE_IF_UNREFERENCED EFI_DRIVER_BINDING_PROTOCOL mSataControllerDriverBinding = {
  SataControllerSupported,
  SataControllerStart,
  SataControllerStop,
  0x10,
  NULL,
  NULL
};

//
// Internal function definitions
//
EFI_STATUS
CalculateBestPioMode (
  IN  EFI_IDENTIFY_DATA      * IdentifyData,
  IN  UINT16                 *DisPioMode OPTIONAL,
  OUT UINT16                 *SelectedMode
  );

EFI_STATUS
CalculateBestUdmaMode (
  IN  EFI_IDENTIFY_DATA      * IdentifyData,
  IN  UINT16                 *DisUDmaMode OPTIONAL,
  OUT UINT16                 *SelectedMode
  );

/**
  <b>SataController DXE Module Entry Point</b>\n
  - <b>Introduction</b>\n
    The SataController module is a DXE EFI 1.1 Driver Model driver which abstracts
    the timings calculation and algorithms of the SATA controller. These algorithms
    are described in PCH <I>BIOS specification or EDS</I>. It is consumed by the ATA bus
    driver to configure SATA devices. <b>This module only supports PCH internal SATA controller.</b>

  - @result
    The SataController driver produces EFI_IDE_CONTROLLER_INIT_PROTOCOL
      - Documented in the IDE Controller section in PI 1.2 Specification

  - <b>Porting Recommendations</b>\n
    This module does not load legacy Option ROM in RAID or AHCI mode. If needed, platform
    code needs to perform legacy Option ROM loading.
    This module supports Framework native AHCI driver (in AHCI mode).

  @param[in] ImageHandle          While the driver image loaded be the ImageLoader(),
                                  an image handle is assigned to this driver binary,
                                  all activities of the driver is tied to this ImageHandle
  @param[in] SystemTable          A pointer to the system table, for all BS(Boo Services) and
                                  RT(Runtime Services)

  @retval EFI_SUCCESS             Function completes successfully
**/
EFI_STATUS
EFIAPI
InitializeSataControllerDriver (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS              Status;
  Status = EfiLibInstallDriverBindingComponentName2 (
             ImageHandle,
             SystemTable,
             &mSataControllerDriverBinding,
             ImageHandle,
             &mSataControllerName,
             &mSataControllerName2
             );
  ASSERT_EFI_ERROR (Status);
  return Status;
}

/**
  This function checks to see if the driver supports a device specified by
  "Controller handle" parameter. It is called by DXE Core StartImage() or
  ConnectController() routines. The driver uses 'device path' and/or
  'services' from the Bus I/O abstraction attached to the controller handle
  to determine if the driver support this controller handle.
  Note: In the BDS (Boot Device Selection) phase, the DXE core enumerate all
  devices (or, controller) and assigns GUIDs to them.

  @param[in] This                 A pointer points to the Binding Protocol instance
  @param[in] Controller           The handle of controller to be tested.
  @param[in] RemainingDevicePath  A pointer to the device path. Ignored by device
                                  driver but used by bus driver

  @retval EFI_SUCCESS             The device is supported
  @exception EFI_UNSUPPORTED      The device is not supported
**/
EFI_STATUS
EFIAPI
SataControllerSupported (
  IN EFI_DRIVER_BINDING_PROTOCOL *This,
  IN EFI_HANDLE                  Controller,
  IN EFI_DEVICE_PATH_PROTOCOL    *RemainingDevicePath
  )
{
  EFI_STATUS                Status;
  UINT32                    SataDeviceIdFound;
  EFI_DEVICE_PATH_PROTOCOL  *ParentDevicePath;
  EFI_PCI_IO_PROTOCOL       *PciIo;
  PCI_TYPE00                PciData;

  ///
  /// SATA Controller is a device driver, and should ingore the
  /// "RemainingDevicePath" according to EFI spec
  ///
  Status = gBS->OpenProtocol (
                  Controller,
                  &gEfiDevicePathProtocolGuid,
                  (VOID *) &ParentDevicePath,
                  This->DriverBindingHandle,
                  Controller,
                  EFI_OPEN_PROTOCOL_BY_DRIVER
                  );
  if (EFI_ERROR (Status)) {
    ///
    /// EFI_ALREADY_STARTED is also an error
    ///
    return Status;
  }
  ///
  /// Close the protocol because we don't use it here
  ///
  gBS->CloseProtocol (
        Controller,
        &gEfiDevicePathProtocolGuid,
        This->DriverBindingHandle,
        Controller
        );

  ///
  /// Now test the EfiPciIoProtocol
  ///
  Status = gBS->OpenProtocol (
                  Controller,
                  &gEfiPciIoProtocolGuid,
                  (VOID **) &PciIo,
                  This->DriverBindingHandle,
                  Controller,
                  EFI_OPEN_PROTOCOL_BY_DRIVER
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }
  ///
  /// Now further check the PCI header: Base class (offset 0x0B) and
  /// Sub Class (offset 0x0A). This controller should be an SATA controller
  ///
  Status = PciIo->Pci.Read (
                        PciIo,
                        EfiPciIoWidthUint8,
                        0,
                        sizeof (PciData),
                        &PciData
                        );
  if (EFI_ERROR (Status)) {
    gBS->CloseProtocol (
          Controller,
          &gEfiPciIoProtocolGuid,
          This->DriverBindingHandle,
          Controller
          );
    return EFI_UNSUPPORTED;
  }
  ///
  /// Since we already got the PciData, we can close protocol to avoid to carry it on for multiple exit points.
  ///
  gBS->CloseProtocol (
        Controller,
        &gEfiPciIoProtocolGuid,
        This->DriverBindingHandle,
        Controller
        );

  ///
  /// Examine SATA PCI Configuration table fields
  ///
  SataDeviceIdFound = FALSE;
  ///
  /// When found is storage device and provided by Intel then detect for right device Ids
  ///
  if (PciData.Hdr.VendorId == V_PCH_SATA_VENDOR_ID) {
    if (PciData.Hdr.ClassCode[2] == PCI_CLASS_MASS_STORAGE) {
      if (PciData.Hdr.ClassCode[1] == V_PCH_SATA_SUB_CLASS_CODE_AHCI) {
        if (IS_PCH_SATA_AHCI_DEVICE_ID (PciData.Hdr.DeviceId)
            ) {
          SataDeviceIdFound = TRUE;
        }
      }
      if (FALSE == PcdGetBool (PcdUefiOptimizedBoot)) {
        if (PciData.Hdr.ClassCode[1] == V_PCH_SATA_SUB_CLASS_CODE_RAID) {
          if (IS_PCH_SATA_RAID_DEVICE_ID (PciData.Hdr.DeviceId)) {
            SataDeviceIdFound = TRUE;
          }
        }
      }
    }
  }

  if (!SataDeviceIdFound) {
    return EFI_UNSUPPORTED;
  }

  return Status;
}

/**
  This routine is called right after the .Supported() is called and returns
  EFI_SUCCESS. Notes: The supported protocols are checked but the Protocols
  are closed.

  @param[in] This                 A pointer points to the Binding Protocol instance
  @param[in] Controller           The handle of controller to be tested. Parameter
                                  passed by the caller
  @param[in] RemainingDevicePath  A pointer to the device path. Should be ignored by
                                  device driver

  @retval EFI_SUCCESS             The device is started
  @retval Other values            Something error happened
**/
EFI_STATUS
EFIAPI
SataControllerStart (
  IN EFI_DRIVER_BINDING_PROTOCOL    *This,
  IN EFI_HANDLE                     Controller,
  IN EFI_DEVICE_PATH_PROTOCOL       *RemainingDevicePath
  )
{
  EFI_STATUS                        Status;
  EFI_PCI_IO_PROTOCOL               *PciIo;
  PCH_SATA_CONTROLLER_PRIVATE_DATA  *SataPrivateData;
  PCI_TYPE00                        PciData;
  UINTN                             SegNum;
  UINTN                             BusNum;
  UINTN                             DevNum;
  UINTN                             FuncNum;
  UINT64                            CommandVal;

  DEBUG ((DEBUG_INFO, "SataControllerStart() Start\n"));

  SataPrivateData = NULL;
  ///
  /// Now test and open the EfiPciIoProtocol
  ///
  Status = gBS->OpenProtocol (
                  Controller,
                  &gEfiPciIoProtocolGuid,
                  (VOID **) &PciIo,
                  This->DriverBindingHandle,
                  Controller,
                  EFI_OPEN_PROTOCOL_BY_DRIVER
                  );
  ///
  /// Status == 0 - A normal execution flow, SUCCESS and the program proceeds.
  /// Status == ALREADY_STARTED - A non-zero Status code returned. It indicates
  ///           that the protocol has been opened and should be treated as a
  ///           normal condition and the program proceeds. The Protocol will not
  ///           opened 'again' by this call.
  /// Status != ALREADY_STARTED - Error status, terminate program execution
  ///
  if (EFI_ERROR (Status)) {
    ///
    /// EFI_ALREADY_STARTED is also an error
    ///
    return Status;
  }
  ///
  /// Allocate SATA private data structure
  ///
  SataPrivateData = AllocatePool (sizeof (PCH_SATA_CONTROLLER_PRIVATE_DATA));
  if (SataPrivateData == NULL) {
    DEBUG ((DEBUG_ERROR, "SATA Controller START ERROR: Allocating pool for IdePrivateData failed!\n"));
    Status = EFI_OUT_OF_RESOURCES;
    goto Done;
  }
  ///
  /// Initialize SATA private data
  ///
  ZeroMem (SataPrivateData, sizeof (PCH_SATA_CONTROLLER_PRIVATE_DATA));
  SataPrivateData->Signature              = SATA_CONTROLLER_SIGNATURE;
  SataPrivateData->PciIo                  = PciIo;
  SataPrivateData->IdeInit.GetChannelInfo = IdeInitGetChannelInfo;
  SataPrivateData->IdeInit.NotifyPhase    = IdeInitNotifyPhase;
  SataPrivateData->IdeInit.SubmitData     = IdeInitSubmitData;
  SataPrivateData->IdeInit.DisqualifyMode = IdeInitDisqualifyMode;
  SataPrivateData->IdeInit.CalculateMode  = IdeInitCalculateMode;
  SataPrivateData->IdeInit.SetTiming      = IdeInitSetTiming;
  SataPrivateData->IdeInit.EnumAll        = PCH_SATA_ENUMER_ALL;

  Status = PciIo->GetLocation (
                    PciIo,
                    &SegNum,
                    &BusNum,
                    &DevNum,
                    &FuncNum
                    );
  ASSERT_EFI_ERROR (Status);

  Status = PciIo->Pci.Read (
                        PciIo,
                        EfiPciIoWidthUint8,
                        0,
                        sizeof (PciData),
                        &PciData
                        );
  ASSERT_EFI_ERROR (Status);

  ///
  /// Get device capabilities
  ///
  Status = PciIo->Attributes (
                    PciIo,
                    EfiPciIoAttributeOperationSupported,
                    0,
                    &CommandVal
                    );
  ASSERT_EFI_ERROR (Status);

  ///
  /// Enable Command Register
  ///
  Status = PciIo->Attributes (
                    PciIo,
                    EfiPciIoAttributeOperationEnable,
                    CommandVal & EFI_PCI_DEVICE_ENABLE,
                    NULL
                    );
  ASSERT_EFI_ERROR (Status);
  ///
  /// Default MAX port number
  ///
  SataPrivateData->IdeInit.ChannelCount = GetPchMaxSataPortNum ();
  ///
  /// Install IDE_CONTROLLER_INIT protocol & private data to this instance
  ///
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &Controller,
                  &gSataControllerDriverGuid,
                  SataPrivateData,
                  &gEfiIdeControllerInitProtocolGuid,
                  &(SataPrivateData->IdeInit),
                  NULL
                  );

Done:

  if (EFI_ERROR (Status)) {

    gBS->CloseProtocol (
          Controller,
          &gEfiPciIoProtocolGuid,
          This->DriverBindingHandle,
          Controller
          );
    FreePool (SataPrivateData);
  }

  DEBUG ((DEBUG_INFO, "SataControllerStart() End\n"));

  return Status;
}

/**
  Stop managing the target device

  @param[in] This                 A pointer pointing to the Binding Protocol instance
  @param[in] Controller           The handle of controller to be stopped
  @param[in] NumberOfChildren     Number of child devices
  @param[in] ChildHandleBuffer    Buffer holding child device handles

  @retval EFI_SUCCESS             The target device is stopped
**/
EFI_STATUS
EFIAPI
SataControllerStop (
  IN  EFI_DRIVER_BINDING_PROTOCOL     *This,
  IN  EFI_HANDLE                      Controller,
  IN  UINTN                           NumberOfChildren,
  IN  EFI_HANDLE                      *ChildHandleBuffer
  )
{
  EFI_STATUS                        Status;
  PCH_SATA_CONTROLLER_PRIVATE_DATA  *SataPrivateData;

  DEBUG ((DEBUG_INFO, "SataControllerStop() Start\n"));

  ///
  /// Get private data
  ///
  Status = gBS->OpenProtocol (
                  Controller,
                  &gSataControllerDriverGuid,
                  (VOID **) &SataPrivateData,
                  This->DriverBindingHandle,
                  Controller,
                  EFI_OPEN_PROTOCOL_GET_PROTOCOL
                  );

  if (!EFI_ERROR (Status)) {
    Status = gBS->UninstallMultipleProtocolInterfaces (
                    Controller,
                    &gSataControllerDriverGuid,
                    SataPrivateData,
                    &gEfiIdeControllerInitProtocolGuid,
                    &(SataPrivateData->IdeInit),
                    NULL
                    );
    if (EFI_ERROR (Status)) {
      return Status;
    }
  }
  ///
  /// Close protocols opened by SATA controller driver
  ///
  gBS->CloseProtocol (
        Controller,
        &gEfiPciIoProtocolGuid,
        This->DriverBindingHandle,
        Controller
        );

  FreePool (SataPrivateData);

  DEBUG ((DEBUG_INFO, "SataControllerStop() End\n"));

  return EFI_SUCCESS;
}

//
// Interface functions of IDE_CONTROLLER_INIT protocol
//

/**
  This function can be used to obtain information about a specified channel.
  It's usually used by ATA Bus driver during enumeration process.

  @param[in] This                 The EFI_IDE_CONTROLLER_INIT_PROTOCOL instance.
  @param[in] Channel              Channel number (0 based)
  @param[out] Enabled             TRUE if the channel is enabled. If the channel is disabled,
                                  then it will no be enumerated.
  @param[out] MaxDevices          The Max number of SATA devices that the bus driver can expect
                                  on this channel. For Intel PCH SATA contoller, this number
                                  always be 1 because Port Multiplier is not support.

  @retval EFI_SUCCESS             Function completes successfully
  @retval Other Values            Something error happened
  @retval EFI_INVALID_PARAMETER   The Channel parameter is invalid
**/
EFI_STATUS
EFIAPI
IdeInitGetChannelInfo (
  IN   EFI_IDE_CONTROLLER_INIT_PROTOCOL    *This,
  IN   UINT8                               Channel,
  OUT  BOOLEAN                             *Enabled,
  OUT  UINT8                               *MaxDevices
  )
{
  ///
  /// Channel number (0 based, either 0 or 1)
  ///
  PCH_SATA_CONTROLLER_PRIVATE_DATA  *SataPrivateData;

  SataPrivateData = SATA_CONTROLLER_PRIVATE_DATA_FROM_THIS (This);
  ASSERT (SataPrivateData);

  if (Channel < This->ChannelCount) {
    *Enabled    = TRUE;
    *MaxDevices = PCH_SATA_MAX_DEVICES_PER_PORT;
    return EFI_SUCCESS;
  } else {
    *Enabled = FALSE;
    return EFI_INVALID_PARAMETER;
  }
}

/**
  This function is called by ATA Bus driver before executing certain actions.
  This allows IDE Controller Init to prepare for each action.

  @param[in] This                 The EFI_IDE_CONTROLLER_INIT_PROTOCOL instance.
  @param[in] Phase                phase indicator defined by IDE_CONTROLLER_INIT protocol
  @param[in] Channel              Channel number (0 based)

  @retval EFI_SUCCESS             Function completes successfully
  @retval EFI_INVALID_PARAMETER   Channel parameter is out of range
  @exception EFI_UNSUPPORTED      Phase is not supported
**/
EFI_STATUS
EFIAPI
IdeInitNotifyPhase (
  IN  EFI_IDE_CONTROLLER_INIT_PROTOCOL  *This,
  IN  EFI_IDE_CONTROLLER_ENUM_PHASE     Phase,
  IN  UINT8                             Channel
  )
{
  if (Channel >= This->ChannelCount) {
    return EFI_INVALID_PARAMETER;
  }

  switch (Phase) {

  case EfiIdeBeforeChannelEnumeration:
  case EfiIdeAfterChannelEnumeration:
  case EfiIdeBeforeChannelReset:
  case EfiIdeAfterChannelReset:
  case EfiIdeBusBeforeDevicePresenceDetection:
  case EfiIdeBusAfterDevicePresenceDetection:
  case EfiIdeResetMode:
    ///
    /// Do nothing at present
    ///
    break;

  default:
    return EFI_UNSUPPORTED;
    break;
  }

  return EFI_SUCCESS;
}

/**
  This function is called by ATA Bus driver to submit EFI_IDENTIFY_DATA data structure
  obtained from SATA deivce.

  @param[in] This                 The EFI_IDE_CONTROLLER_INIT_PROTOCOL instance.
  @param[in] Channel              Channel number (0 based)
  @param[in] Device               Device number, it will always be 0 since IDE is not support.
  @param[in] IdentifyData         A pointer to EFI_IDENTIFY_DATA data structure

  @retval EFI_SUCCESS             Function completes successfully
  @retval EFI_INVALID_PARAMETER   Channel or Device parameter is out of range
**/
EFI_STATUS
EFIAPI
IdeInitSubmitData (
  IN  EFI_IDE_CONTROLLER_INIT_PROTOCOL    *This,
  IN  UINT8                               Channel,
  IN  UINT8                               Device,
  IN  EFI_IDENTIFY_DATA                   *IdentifyData
  )
{
  PCH_SATA_CONTROLLER_PRIVATE_DATA  *SataPrivateData;
  SataPrivateData = SATA_CONTROLLER_PRIVATE_DATA_FROM_THIS (This);
  ASSERT (SataPrivateData);

  if ((Channel >= This->ChannelCount) || (Device != 0)) {
    return EFI_INVALID_PARAMETER;
  }
  ///
  /// Make a local copy of device's IdentifyData and mark the valid flag
  ///
  if (IdentifyData != NULL) {
    CopyMem (
      &(SataPrivateData->IdentifyData[Channel]),
      IdentifyData,
      sizeof (EFI_IDENTIFY_DATA)
      );

    SataPrivateData->IdentifyValid[Channel] = TRUE;
  } else {
    SataPrivateData->IdentifyValid[Channel] = FALSE;
  }

  return EFI_SUCCESS;
}

/**
  This function is called by ATA Bus driver to disqualify unsupported operation
  mode on specfic SATA device

  @param[in] This                 The EFI_IDE_CONTROLLER_INIT_PROTOCOL instance.
  @param[in] Channel              Channel number (0 based)
  @param[in] Device               Device number, it will always be 0 since IDE is not support.
  @param[in] BadModes             Operation mode indicator

  @retval EFI_SUCCESS             Function completes successfully
  @retval EFI_INVALID_PARAMETER   Channel parameter or Devicde parameter is out of range,
                                  or BadModes is NULL
**/
EFI_STATUS
EFIAPI
IdeInitDisqualifyMode (
  IN  EFI_IDE_CONTROLLER_INIT_PROTOCOL    *This,
  IN  UINT8                               Channel,
  IN  UINT8                               Device,
  IN  EFI_ATA_COLLECTIVE_MODE             *BadModes
  )
{
  PCH_SATA_CONTROLLER_PRIVATE_DATA  *SataPrivateData;
  SataPrivateData = SATA_CONTROLLER_PRIVATE_DATA_FROM_THIS (This);
  ASSERT (SataPrivateData);

  if ((Channel >= This->ChannelCount) || (BadModes == NULL) || (Device != 0)) {
    return EFI_INVALID_PARAMETER;
  }
  ///
  /// Record the disqualified modes per channel per device. From ATA/ATAPI spec,
  /// if a mode is not supported, the modes higher than it is also not
  /// supported
  ///
  CopyMem (
    &(SataPrivateData->DisqulifiedModes[Channel]),
    BadModes,
    sizeof (EFI_ATA_COLLECTIVE_MODE)
    );

  return EFI_SUCCESS;
}

/**
  This function is called by ATA Bus driver to calculate the best operation mode
  supported by specific SATA device

  @param[in] This                 The EFI_IDE_CONTROLLER_INIT_PROTOCOL instance.
  @param[in] Channel              Channel number (0 based)
  @param[in] Device               Device number, it will always be 0 since IDE is not support.
  @param[in, out] SupportedModes  Modes collection supported by IDE device

  @retval EFI_SUCCESS             Function completes successfully
  @retval EFI_INVALID_PARAMETER   Channel parameter or Device parameter is out of range;
                                  Or SupportedModes is NULL
  @retval EFI_NOT_READY           Identify data is not valid
  @retval EFI_OUT_OF_RESOURCES    SupportedModes is out of range
**/
EFI_STATUS
EFIAPI
IdeInitCalculateMode (
  IN  EFI_IDE_CONTROLLER_INIT_PROTOCOL       *This,
  IN  UINT8                                  Channel,
  IN  UINT8                                  Device,
  IN OUT EFI_ATA_COLLECTIVE_MODE             **SupportedModes
  )
{
  PCH_SATA_CONTROLLER_PRIVATE_DATA  *SataPrivateData;
  EFI_IDENTIFY_DATA                 *IdentifyData;
  BOOLEAN                           IdentifyValid;
  EFI_ATA_COLLECTIVE_MODE           *DisqulifiedModes;
  UINT16                            SelectedMode;
  EFI_STATUS                        Status;

  SataPrivateData = SATA_CONTROLLER_PRIVATE_DATA_FROM_THIS (This);
  ASSERT (SataPrivateData);

  if ((Channel >= This->ChannelCount) || (SupportedModes == NULL) || (Device != 0)) {
    return EFI_INVALID_PARAMETER;
  }

  IdentifyData      = &(SataPrivateData->IdentifyData[Channel]);
  DisqulifiedModes  = &(SataPrivateData->DisqulifiedModes[Channel]);
  IdentifyValid     = SataPrivateData->IdentifyValid[Channel];

  ///
  /// Make sure we've got the valid identify data of the device from SubmitData()
  ///
  if (!IdentifyValid) {
    return EFI_NOT_READY;
  }

  *SupportedModes = AllocateZeroPool (sizeof (EFI_ATA_COLLECTIVE_MODE));
  if (*SupportedModes == NULL) {
    ASSERT (FALSE);
    return EFI_OUT_OF_RESOURCES;
  }

  Status = CalculateBestPioMode (
            IdentifyData,
            (DisqulifiedModes->PioMode.Valid ? ((UINT16 *) &(DisqulifiedModes->PioMode.Mode)) : NULL),
            &SelectedMode
            );
  if (!EFI_ERROR (Status)) {
    (*SupportedModes)->PioMode.Valid  = TRUE;
    (*SupportedModes)->PioMode.Mode   = SelectedMode;

  } else {
    (*SupportedModes)->PioMode.Valid = FALSE;
  }

  Status = CalculateBestUdmaMode (
            IdentifyData,
            (DisqulifiedModes->UdmaMode.Valid ? ((UINT16 *) &(DisqulifiedModes->UdmaMode.Mode)) : NULL),
            &SelectedMode
            );

  if (!EFI_ERROR (Status)) {
    (*SupportedModes)->UdmaMode.Valid = TRUE;
    (*SupportedModes)->UdmaMode.Mode  = SelectedMode;

  } else {
    (*SupportedModes)->UdmaMode.Valid = FALSE;
  }
  ///
  /// The modes other than PIO and UDMA are not supported by SATA controller
  ///
  return EFI_SUCCESS;
}

/**
  This function is called by ATA Bus driver to set appropriate timing on SATA
  controller according supported operation mode

  @param[in] This                 The EFI_IDE_CONTROLLER_INIT_PROTOCOL instance.
  @param[in] Channel              Channel number (0 based)
  @param[in] Device               Device number, it will always be 0 since IDE is not support.
  @param[in] Modes                Operation modes

  @retval EFI_SUCCESS             This function always returns EFI_SUCCESS
**/
EFI_STATUS
EFIAPI
IdeInitSetTiming (
  IN  EFI_IDE_CONTROLLER_INIT_PROTOCOL    *This,
  IN  UINT8                               Channel,
  IN  UINT8                               Device,
  IN  EFI_ATA_COLLECTIVE_MODE             *Modes
  )
{
  return EFI_SUCCESS;
}

/**
  This function is used to calculate the best PIO mode supported by
  specific SATA device

  @param[in] IdentifyData         The identify data of specific IDE device
  @param[in] DisPioMode           Disqualified PIO modes collection
  @param[out] SelectedMode        Available PIO modes collection

  @retval EFI_SUCCESS             Function completes successfully
  @exception EFI_UNSUPPORTED      Some invalid condition
**/
EFI_STATUS
CalculateBestPioMode (
  IN  EFI_IDENTIFY_DATA    * IdentifyData,
  IN  UINT16               *DisPioMode OPTIONAL,
  OUT UINT16               *SelectedMode
  )
{
  UINT16  PioMode;
  UINT16  AdvancedPioMode;
  UINT16  Temp;
  UINT16  Index;
  UINT16  MinimumPioCycleTime;

  Temp    = 0xff;

  PioMode = (UINT8) (((ATA5_IDENTIFY_DATA *) IdentifyData)->pio_cycle_timing >> 8);
  ///
  /// see whether Identify Data word 64 - 70 are valid
  ///
  if ((IdentifyData->AtaData.field_validity & 0x02) == 0x02) {

    AdvancedPioMode = IdentifyData->AtaData.advanced_pio_modes;

    for (Index = 0; Index < 8; Index++) {
      if ((AdvancedPioMode & 0x01) != 0) {
        Temp = Index;
      }

      AdvancedPioMode >>= 1;
    }
    ///
    /// if Temp is modified, meant the advanced_pio_modes is not zero;
    /// if Temp is not modified, meant the no advanced PIO Mode is supported,
    /// the best PIO Mode is the value in pio_cycle_timing.
    ///
    if (Temp != 0xff) {
      AdvancedPioMode = (UINT16) (Temp + 3);
    } else {
      AdvancedPioMode = PioMode;
    }
    ///
    /// Limit the PIO mode to at most PIO4.
    ///
    PioMode             = (UINT16) (AdvancedPioMode < 4 ? AdvancedPioMode : 4);

    MinimumPioCycleTime = IdentifyData->AtaData.min_pio_cycle_time_with_flow_control;

    if (MinimumPioCycleTime <= 120) {
      PioMode = (UINT16) (4 < PioMode ? 4 : PioMode);
    } else if (MinimumPioCycleTime <= 180) {
      PioMode = (UINT16) (3 < PioMode ? 3 : PioMode);
    } else if (MinimumPioCycleTime <= 240) {
      PioMode = (UINT16) (2 < PioMode ? 2 : PioMode);
    } else {
      PioMode = 0;
    }
    ///
    /// Degrade the PIO mode if the mode has been disqualified
    ///
    if (DisPioMode != NULL) {

      if (*DisPioMode < 2) {
        return EFI_UNSUPPORTED;
        ///
        /// no mode below ATA_PIO_MODE_BELOW_2
        ///
      }

      if (PioMode >= *DisPioMode) {
        PioMode = (UINT16) (*DisPioMode - 1);
      }
    }

    if (PioMode < 2) {
      *SelectedMode = 1;
      ///
      /// ATA_PIO_MODE_BELOW_2;
      ///
    } else {
      *SelectedMode = PioMode;
      ///
      /// ATA_PIO_MODE_2 to ATA_PIO_MODE_4;
      ///
    }

  } else {
    ///
    /// Identify Data word 64 - 70 are not valid
    /// Degrade the PIO mode if the mode has been disqualified
    ///
    if (DisPioMode != NULL) {

      if (*DisPioMode < 2) {
        return EFI_UNSUPPORTED;
        ///
        /// no mode below ATA_PIO_MODE_BELOW_2
        ///
      }

      if (PioMode == *DisPioMode) {
        PioMode--;
      }
    }

    if (PioMode < 2) {
      *SelectedMode = 1;
      ///
      /// ATA_PIO_MODE_BELOW_2;
      ///
    } else {
      *SelectedMode = 2;
      ///
      /// ATA_PIO_MODE_2;
      ///
    }

  }

  DEBUG ((DEBUG_ERROR, "CalculateBestPioMode() End\n"));

  return EFI_SUCCESS;
}

/**
  This function is used to calculate the best UDMA mode supported by
  specific SATA device

  @param[in] IdentifyData         The identify data of specific SATA device
  @param[in] DisUDmaMode          Disqualified UDMA modes collection
  @param[out] SelectedMode        Available UMDA modes collection

  @retval EFI_SUCCESS             Function completes successfully
  @exception EFI_UNSUPPORTED      Some invalid condition
**/
EFI_STATUS
CalculateBestUdmaMode (
  IN  EFI_IDENTIFY_DATA    * IdentifyData,
  IN  UINT16               *DisUDmaMode OPTIONAL,
  OUT UINT16               *SelectedMode
  )
{
  UINT16  TempMode;
  UINT16  DeviceUDmaMode;

  DeviceUDmaMode = 0;
  ///
  /// flag for 'Udma mode is not supported'
  ///
  /// Check whether the WORD 88 (supported UltraDMA by drive) is valid
  ///
  if ((IdentifyData->AtaData.field_validity & 0x04) == 0x00) {
    return EFI_UNSUPPORTED;
  }

  DeviceUDmaMode = IdentifyData->AtaData.ultra_dma_mode;
  DeviceUDmaMode &= 0x3f;
  TempMode = 0;
  ///
  /// initialize it to UDMA-0
  ///
  while ((DeviceUDmaMode >>= 1) != 0) {
    TempMode++;
  }
  ///
  /// Degrade the UDMA mode if the mode has been disqualified
  ///
  if (DisUDmaMode != NULL) {
    if (*DisUDmaMode == 0) {
      *SelectedMode = 0;
      return EFI_UNSUPPORTED;
      ///
      /// no mode below ATA_UDMA_MODE_0
      ///
    }

    if (TempMode >= *DisUDmaMode) {
      TempMode = (UINT16) (*DisUDmaMode - 1);
    }
  }
  ///
  /// Possible returned mode is between ATA_UDMA_MODE_0 and ATA_UDMA_MODE_5
  ///
  *SelectedMode = TempMode;

  return EFI_SUCCESS;
}
