/** @file
  Driver Binding functions for PCI Bus module.

  Single PCI bus driver instance will manager all PCI Root Bridges in one EFI based firmware,
  since all PCI Root Bridges' resources need to be managed together.
  Supported() function will try to get PCI Root Bridge IO Protocol.
  Start() function will get PCI Host Bridge Resource Allocation Protocol to manage all
  PCI Root Bridges. So it means platform needs install PCI Root Bridge IO protocol for each
  PCI Root Bus and install PCI Host Bridge Resource Allocation Protocol.

Copyright (c) 2006 - 2017, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials
are licensed and made available under the terms and conditions of the BSD License
which accompanies this distribution.  The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "PciBus.h"

//
// PCI Bus Driver Global Variables
//
EFI_DRIVER_BINDING_PROTOCOL                   gPciBusDriverBinding = {
  PciBusDriverBindingSupported,
  PciBusDriverBindingStart,
  PciBusDriverBindingStop,
  0xa,
  NULL,
  NULL
};

EFI_HANDLE                                    gPciHostBrigeHandles[PCI_MAX_HOST_BRIDGE_NUM];
EFI_INCOMPATIBLE_PCI_DEVICE_SUPPORT_PROTOCOL  *gIncompatiblePciDeviceSupport = NULL;
UINTN                                         gPciHostBridgeNumber = 0;
BOOLEAN                                       gFullEnumeration     = TRUE;
UINT64                                        gAllOne              = 0xFFFFFFFFFFFFFFFFULL;
UINT64                                        gAllZero             = 0;

EFI_PCI_PLATFORM_PROTOCOL                     *gPciPlatformProtocol;
EFI_PCI_OVERRIDE_PROTOCOL                     *gPciOverrideProtocol;


GLOBAL_REMOVE_IF_UNREFERENCED EFI_PCI_HOTPLUG_REQUEST_PROTOCOL mPciHotPlugRequest = {
  PciHotPlugRequestNotify
};

VOID
EFIAPI
PciBusInstallPendingOproms (
  IN EFI_EVENT                Event,
  IN VOID                     *Context
  )
{
  UINTN                         HandleCount;
  EFI_HANDLE                    *HandleBuffer;
  EFI_STATUS                    Status;
  UINTN                         Index;
  EFI_PCI_IO_PROTOCOL           *PciIoProtocol;
  PCI_IO_DEVICE                 *PciIoDevice;
  BOOLEAN                       AllOpRomProcessed;
  BOOLEAN                       PciContainEfiImage;

  DEBUG((EFI_D_INFO, "Installing all pending UEFI Option ROMs...\n"));
  HandleBuffer = NULL;
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiPciIoProtocolGuid,
                  NULL,
                  &HandleCount,
                  &HandleBuffer
                  );
  ASSERT_EFI_ERROR(Status);

  for (Index = 0; Index < HandleCount; Index++) {
    Status = gBS->HandleProtocol (HandleBuffer[Index], &gEfiPciIoProtocolGuid, (VOID **)&PciIoProtocol);
    ASSERT_EFI_ERROR(Status);
    PciIoDevice = PCI_IO_DEVICE_FROM_PCI_IO_THIS(PciIoProtocol);
    AllOpRomProcessed = PciIoDevice->AllOpRomProcessed;
    PciContainEfiImage = ContainEfiImage(PciIoDevice->PciIo.RomImage, PciIoDevice->PciIo.RomSize);
    if (!AllOpRomProcessed && PciContainEfiImage) {
      PciIoDevice->AllOpRomProcessed = TRUE;
      //
      // Start the OpRom image
      //
      DEBUG((EFI_D_INFO, "Installing Option ROM for B0x%x D0x%x F0x%x\n", PciIoDevice->BusNumber, PciIoDevice->DeviceNumber, PciIoDevice->FunctionNumber));
      ProcessOpRomImage(PciIoDevice);

      if (PciIoDevice->BusOverride) {
        //
        // Install Bus Specific Driver Override Protocol
        //
        Status = gBS->InstallMultipleProtocolInterfaces (
                        &PciIoDevice->Handle,
                        &gEfiBusSpecificDriverOverrideProtocolGuid,
                        &PciIoDevice->PciDriverOverride,
                        NULL
                        );
        if (EFI_ERROR (Status)) {
          gBS->UninstallMultipleProtocolInterfaces (
                 &PciIoDevice->Handle,
                 &gEfiDevicePathProtocolGuid,
                 PciIoDevice->DevicePath,
                 &gEfiPciIoProtocolGuid,
                 &PciIoDevice->PciIo,
                 NULL
                 );
          if (PciContainEfiImage) {
            gBS->UninstallMultipleProtocolInterfaces (
                   &PciIoDevice->Handle,
                   &gEfiLoadFile2ProtocolGuid,
                   &PciIoDevice->LoadFile2,
                   NULL
                   );
          }
        }
      }
    }
  }

  if (HandleBuffer != NULL) {
    gBS->FreePool(HandleBuffer);
  }
  DEBUG((EFI_D_INFO, "All pending Option ROMs installed!\n")); 
  return; 
}

/**
  The Entry Point for PCI Bus module. The user code starts with this function.

  Installs driver module protocols and. Creates virtual device handles for ConIn,
  ConOut, and StdErr. Installs Simple Text In protocol, Simple Text In Ex protocol,
  Simple Pointer protocol, Absolute Pointer protocol on those virtual handlers.
  Installs Graphics Output protocol and/or UGA Draw protocol if needed.

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.
  @param[in] SystemTable    A pointer to the EFI System Table.

  @retval EFI_SUCCESS       The entry point is executed successfully.
  @retval other             Some error occurred when executing this entry point.

**/
EFI_STATUS
EFIAPI
PciBusEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS  Status;
  EFI_HANDLE  Handle;
  EFI_EVENT   Registration;

  //
  // Initializes PCI devices pool
  //
  InitializePciDevicePool ();

  //
  // Install driver model protocol(s).
  //
  Status = EfiLibInstallDriverBindingComponentName2 (
             ImageHandle,
             SystemTable,
             &gPciBusDriverBinding,
             ImageHandle,
             &gPciBusComponentName,
             &gPciBusComponentName2
             );
  ASSERT_EFI_ERROR (Status);

  if (FeaturePcdGet (PcdPciBusHotplugDeviceSupport)) {
    //
    // If Hot Plug is supported, install EFI PCI Hot Plug Request protocol.
    //
    Handle = NULL;
    Status = gBS->InstallProtocolInterface (
                    &Handle,
                    &gEfiPciHotPlugRequestProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    &mPciHotPlugRequest
                    );
  }
  Status = gBS->CreateEventEx(
      EVT_NOTIFY_SIGNAL,
      TPL_CALLBACK,
      PciBusInstallPendingOproms,
      NULL,
      &gEfiAfterPlatformLocksEventGuid,
      &Registration
      );

  return Status;
}

/**
  Test to see if this driver supports ControllerHandle. Any ControllerHandle
  than contains a gEfiPciRootBridgeIoProtocolGuid protocol can be supported.

  @param  This                Protocol instance pointer.
  @param  Controller          Handle of device to test.
  @param  RemainingDevicePath Optional parameter use to pick a specific child
                              device to start.

  @retval EFI_SUCCESS         This driver supports this device.
  @retval EFI_ALREADY_STARTED This driver is already running on this device.
  @retval other               This driver does not support this device.

**/
EFI_STATUS
EFIAPI
PciBusDriverBindingSupported (
  IN EFI_DRIVER_BINDING_PROTOCOL    *This,
  IN EFI_HANDLE                     Controller,
  IN EFI_DEVICE_PATH_PROTOCOL       *RemainingDevicePath
  )
{
  EFI_STATUS                      Status;
  EFI_DEVICE_PATH_PROTOCOL        *ParentDevicePath;
  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL *PciRootBridgeIo;
  EFI_DEV_PATH_PTR                Node;
//
// PURLEY_OVERRIDE_BEGIN
//
  EFI_PCI_IO_PROTOCOL             *PciIoProtocol;
  PCI_IO_DEVICE                   *PciIoDevice;
  BOOLEAN                         AllOpRomProcessed;
  BOOLEAN                         PciContainEfiImage;


  //
  // Check if Pci IO protocol is installed by this driver
  //
  Status = gBS->OpenProtocol (
                  Controller,
                  &gEfiPciIoProtocolGuid,
                  (VOID **) &PciIoProtocol,
                  This->DriverBindingHandle,
                  Controller,
                  EFI_OPEN_PROTOCOL_BY_DRIVER
                  );
  if (!EFI_ERROR(Status)) {
    //
    // If PciIo exists, and isn't already open, check to see if device contains OpRom
    // Return success if OpRom hasn't been launched yet.  OpRom will be launched in Start()
    //
    PciIoDevice = PCI_IO_DEVICE_FROM_PCI_IO_THIS(PciIoProtocol);
    AllOpRomProcessed = PciIoDevice->AllOpRomProcessed;
    PciContainEfiImage = ContainEfiImage(PciIoDevice->PciIo.RomImage, PciIoDevice->PciIo.RomSize);
    //
    // Close the I/O Abstraction(s) used to perform the supported test
    //
    gBS->CloseProtocol(
        Controller,
        &gEfiPciIoProtocolGuid,
        This->DriverBindingHandle,
        Controller
        );
    if (!AllOpRomProcessed && PciContainEfiImage) {
      return EFI_SUCCESS;
    }
  }
//
// PURLEY_OVERRIDE_END
//
  //
  // Check RemainingDevicePath validation
  //
  if (RemainingDevicePath != NULL) {
    //
    // Check if RemainingDevicePath is the End of Device Path Node, 
    // if yes, go on checking other conditions
    //
    if (!IsDevicePathEnd (RemainingDevicePath)) {
      //
      // If RemainingDevicePath isn't the End of Device Path Node,
      // check its validation
      //
      Node.DevPath = RemainingDevicePath;
      if (Node.DevPath->Type != HARDWARE_DEVICE_PATH ||
          Node.DevPath->SubType != HW_PCI_DP         ||
          DevicePathNodeLength(Node.DevPath) != sizeof(PCI_DEVICE_PATH)) {
        return EFI_UNSUPPORTED;
      }
    }
  }

  //
  // Check if Pci Root Bridge IO protocol is installed by platform
  //
  Status = gBS->OpenProtocol (
                  Controller,
                  &gEfiPciRootBridgeIoProtocolGuid,
                  (VOID **) &PciRootBridgeIo,
                  This->DriverBindingHandle,
                  Controller,
                  EFI_OPEN_PROTOCOL_BY_DRIVER
                  );
  if (Status == EFI_ALREADY_STARTED) {
    return EFI_SUCCESS;
  }

  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Close the I/O Abstraction(s) used to perform the supported test
  //
  gBS->CloseProtocol (
        Controller,
        &gEfiPciRootBridgeIoProtocolGuid,
        This->DriverBindingHandle,
        Controller
        );

  //
  // Open the EFI Device Path protocol needed to perform the supported test
  //
  Status = gBS->OpenProtocol (
                  Controller,
                  &gEfiDevicePathProtocolGuid,
                  (VOID **) &ParentDevicePath,
                  This->DriverBindingHandle,
                  Controller,
                  EFI_OPEN_PROTOCOL_BY_DRIVER
                  );
  if (Status == EFI_ALREADY_STARTED) {
    return EFI_SUCCESS;
  }

  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Close protocol, don't use device path protocol in the Support() function
  //
  gBS->CloseProtocol (
        Controller,
        &gEfiDevicePathProtocolGuid,
        This->DriverBindingHandle,
        Controller
        );

  return EFI_SUCCESS;
}

/**
  Start this driver on ControllerHandle and enumerate Pci bus and start
  all device under PCI bus.

  @param  This                 Protocol instance pointer.
  @param  Controller           Handle of device to bind driver to.
  @param  RemainingDevicePath  Optional parameter use to pick a specific child
                               device to start.

  @retval EFI_SUCCESS          This driver is added to ControllerHandle.
  @retval EFI_ALREADY_STARTED  This driver is already running on ControllerHandle.
  @retval other                This driver does not support this device.

**/
EFI_STATUS
EFIAPI
PciBusDriverBindingStart (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   Controller,
  IN EFI_DEVICE_PATH_PROTOCOL     *RemainingDevicePath
  )
{
  EFI_STATUS                Status;
  EFI_DEVICE_PATH_PROTOCOL  *ParentDevicePath;
//
// PURLEY_OVERRIDE_BEGIN
//
  EFI_PCI_IO_PROTOCOL       *PciIoProtocol;
  PCI_IO_DEVICE             *PciIoDevice;
  BOOLEAN                   AllOpRomProcessed;
  BOOLEAN                   PciContainEfiImage;

  //
  // Check if Pci IO protocol is installed by this driver
  //
  Status = gBS->OpenProtocol (
                  Controller,
                  &gEfiPciIoProtocolGuid,
                  (VOID **) &PciIoProtocol,
                  This->DriverBindingHandle,
                  Controller,
                  EFI_OPEN_PROTOCOL_BY_DRIVER
                  );
  if (!EFI_ERROR(Status)) {
    //
    // If PciIo exists, and isn't already open, check to see if device contains OpRom
    // Return success if OpRom hasn't been launched yet.  OpRom will be launched in Start()
    //
    PciIoDevice = PCI_IO_DEVICE_FROM_PCI_IO_THIS(PciIoProtocol);
    AllOpRomProcessed = PciIoDevice->AllOpRomProcessed;
    PciContainEfiImage = ContainEfiImage(PciIoDevice->PciIo.RomImage, PciIoDevice->PciIo.RomSize);
    //
    // Close the I/O Abstraction(s) used to perform the supported test
    //
    gBS->CloseProtocol(
        Controller,
        &gEfiPciIoProtocolGuid,
        This->DriverBindingHandle,
        Controller
        );
      if (!AllOpRomProcessed && PciContainEfiImage) {
        PciIoDevice->AllOpRomProcessed = TRUE;
        //
        // Start the OpRom image
        //
        ProcessOpRomImage(PciIoDevice);
        //
        // Call ConnectController to now have OpRom driver start on this handle
        //
        return gBS->ConnectController(Controller, NULL, RemainingDevicePath, FALSE);
      }
  }
//
// PURLEY_OVERRIDE_END
//
  //
  // Check RemainingDevicePath validation
  //
  if (RemainingDevicePath != NULL) {
    //
    // Check if RemainingDevicePath is the End of Device Path Node, 
    // if yes, return EFI_SUCCESS
    //
    if (IsDevicePathEnd (RemainingDevicePath)) {
      return EFI_SUCCESS;
    }
  }

  gBS->LocateProtocol (
         &gEfiIncompatiblePciDeviceSupportProtocolGuid,
         NULL,
         (VOID **) &gIncompatiblePciDeviceSupport
         );

  //
  // If PCI Platform protocol is available, get it now.
  // If the platform implements this, it must be installed before BDS phase
  //
  gPciPlatformProtocol = NULL;
  gBS->LocateProtocol (
        &gEfiPciPlatformProtocolGuid,
        NULL,
        (VOID **) &gPciPlatformProtocol
        );

  //
  // If PCI Platform protocol doesn't exist, try to Pci Override Protocol.
  //
  if (gPciPlatformProtocol == NULL) { 
    gPciOverrideProtocol = NULL;
    gBS->LocateProtocol (
          &gEfiPciOverrideProtocolGuid,
          NULL,
          (VOID **) &gPciOverrideProtocol
          );
  }  

  if (PcdGetBool (PcdPciDisableBusEnumeration)) {
    gFullEnumeration = FALSE;
  } else {
    gFullEnumeration = (BOOLEAN) ((SearchHostBridgeHandle (Controller) ? FALSE : TRUE));
  }

  //
  // Open Device Path Protocol for PCI root bridge
  //
  Status = gBS->OpenProtocol (
                  Controller,
                  &gEfiDevicePathProtocolGuid,
                  (VOID **) &ParentDevicePath,
                  This->DriverBindingHandle,
                  Controller,
                  EFI_OPEN_PROTOCOL_GET_PROTOCOL
                  );  
  ASSERT_EFI_ERROR (Status);

  //
  // Report Status Code to indicate PCI bus starts
  //
  REPORT_STATUS_CODE_WITH_DEVICE_PATH (
    EFI_PROGRESS_CODE,
    (EFI_IO_BUS_PCI | EFI_IOB_PC_INIT),
    ParentDevicePath
    );

  //
  // Enumerate the entire host bridge
  // After enumeration, a database that records all the device information will be created
  //
  //
  Status = PciEnumerator (Controller);

  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Start all the devices under the entire host bridge.
  //
  StartPciDevices (Controller);

  return EFI_SUCCESS;
}

/**
  Stop this driver on ControllerHandle. Support stopping any child handles
  created by this driver.

  @param  This              Protocol instance pointer.
  @param  Controller        Handle of device to stop driver on.
  @param  NumberOfChildren  Number of Handles in ChildHandleBuffer. If number of
                            children is zero stop the entire bus driver.
  @param  ChildHandleBuffer List of Child Handles to Stop.

  @retval EFI_SUCCESS       This driver is removed ControllerHandle.
  @retval other             This driver was not removed from this device.

**/
EFI_STATUS
EFIAPI
PciBusDriverBindingStop (
  IN  EFI_DRIVER_BINDING_PROTOCOL   *This,
  IN  EFI_HANDLE                    Controller,
  IN  UINTN                         NumberOfChildren,
  IN  EFI_HANDLE                    *ChildHandleBuffer
  )
{
  EFI_STATUS  Status;
  UINTN       Index;
  BOOLEAN     AllChildrenStopped;

  if (NumberOfChildren == 0) {
    //
    // Close the bus driver
    //
    gBS->CloseProtocol (
          Controller,
          &gEfiDevicePathProtocolGuid,
          This->DriverBindingHandle,
          Controller
          );
    gBS->CloseProtocol (
          Controller,
          &gEfiPciRootBridgeIoProtocolGuid,
          This->DriverBindingHandle,
          Controller
          );

    DestroyRootBridgeByHandle (
      Controller
      );

    return EFI_SUCCESS;
  }

  //
  // Stop all the children
  //

  AllChildrenStopped = TRUE;

  for (Index = 0; Index < NumberOfChildren; Index++) {

    //
    // De register all the pci device
    //
    Status = DeRegisterPciDevice (Controller, ChildHandleBuffer[Index]);

    if (EFI_ERROR (Status)) {
      AllChildrenStopped = FALSE;
    }
  }

  if (!AllChildrenStopped) {
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

