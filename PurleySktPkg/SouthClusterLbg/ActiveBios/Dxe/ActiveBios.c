/** @file
  Source file for the ActiveBios ActiveBios protocol implementation

@copyright
 Copyright (c) 2005 - 2015 Intel Corporation. All rights reserved
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
#include "ActiveBios.h"

//
// Prototypes for our ActiveBios protocol functions
//
static
EFI_STATUS
EFIAPI
SetState (
        IN PCH_ACTIVE_BIOS_PROTOCOL *This,
        IN PCH_ACTIVE_BIOS_STATE    DesiredState,
        IN UINTN                    Key
        );

static
EFI_STATUS
EFIAPI
LockState (
  IN PCH_ACTIVE_BIOS_PROTOCOL *This,
  IN BOOLEAN                  Lock,
  IN OUT UINTN                *Key
  );

//
// Function implementations
//

/**
  Change the current active BIOS settings to the requested state.
  The caller is responsible for requesting a supported state from
  the PCH_ACTIVE_BIOS_STATE selections.
  This will fail if someone has locked the interface and the correct key is
  not provided.

  @param[in] This                 Pointer to the PCH_ACTIVE_BIOS_PROTOCOL instance.
  @param[in] DesiredState         The requested state to configure the system for.
  @param[in] Key                  If the interface is locked, Key must be the Key
                                  returned from the LockState function call.

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_ACCESS_DENIED       The interface is currently locked.
**/
static
EFI_STATUS
EFIAPI
SetState (
  IN PCH_ACTIVE_BIOS_PROTOCOL *This,
  IN PCH_ACTIVE_BIOS_STATE    DesiredState,
  IN UINTN                    Key
  )
{
  PCH_SERIES PchSeries;
  UINTN      PciSpiBase;
  UINTN      LpcPciBase;

  PciSpiBase = MmPciBase (
                      DEFAULT_PCI_BUS_NUMBER_PCH,
                      PCI_DEVICE_NUMBER_PCH_SPI,
                      PCI_FUNCTION_NUMBER_PCH_SPI
                      );
  LpcPciBase = MmPciBase (
                 DEFAULT_PCI_BUS_NUMBER_PCH,
                 PCI_DEVICE_NUMBER_PCH_LPC,
                 PCI_FUNCTION_NUMBER_PCH_LPC
                 );
  
  PchSeries = GetPchSeries();
  //
  // Verify requested state is allowed
  //
  ASSERT (DesiredState < ActiveBiosStateMax);

  //
  // Check if the interface is locked by another
  //
  if (mPrivateData.Locked && Key != mPrivateData.CurrentKey) {
    return EFI_ACCESS_DENIED;
  }

  if ((MmioRead8 (PciSpiBase + R_PCH_SPI_BC) & B_PCH_SPI_BC_BILD) == B_PCH_SPI_BC_BILD) {
    return EFI_ACCESS_DENIED;
  }
  //
  // Set the requested state
  //
  switch (DesiredState) {

    case ActiveBiosStateSpi:
        //
        // LPC/eSPI
        //
        MmioAndThenOr8 (
          (UINTN) (LpcPciBase + R_PCH_LPC_BC),
          (UINT8) ~B_PCH_LPC_BC_BBS,
          (UINT8) (V_PCH_LPC_BC_BBS_SPI << N_PCH_LPC_BC_BBS)
                );
        //
        // SPI
        //
        MmioAndThenOr8 (
                (UINTN) (PciSpiBase + R_PCH_SPI_BC),
                (UINT8) ~B_PCH_SPI_BC_BBS,
                (UINT8) (V_PCH_SPI_BC_BBS_SPI << N_PCH_SPI_BC_BBS)
                );
        //
        // Set PCR[DMI] + 274Ch[10] = 0b for SPI
        //
        PchPcrAndThenOr8 (PID_DMI, R_PCH_PCR_DMI_GCS + 1, (UINT8) (~(B_PCH_PCR_DMI_BBS >> 8)), 0);
      break;

    case ActiveBiosStateLpc:
        //
        // LPC/eSPI
        //
        MmioAndThenOr8 (
                (UINTN) (LpcPciBase + R_PCH_LPC_BC),
                (UINT8) ~B_PCH_LPC_BC_BBS,
                (UINT8) (V_PCH_LPC_BC_BBS_LPC << N_PCH_LPC_BC_BBS)
                );
        //
        // SPI
        //
        MmioAndThenOr8 (
                (UINTN) (PciSpiBase + R_PCH_SPI_BC),
                (UINT8) ~B_PCH_SPI_BC_BBS,
                (UINT8) (V_PCH_SPI_BC_BBS_LPC << N_PCH_SPI_BC_BBS)
                );
        //
        // Set PCR[DMI] + 274C[10] = 1b for LPC/eSPI
        //
        PchPcrAndThenOr8 (PID_DMI, R_PCH_PCR_DMI_GCS + 1, 0xFF, (UINT8) (B_PCH_PCR_DMI_BBS >> 8));

    default:
      //
      // This is an invalid use of the protocol
      // See definition, but caller must call with valid value
      //
      ASSERT (!EFI_UNSUPPORTED);
      break;
  }
  //
  // Read state back
  // This ensures the chipset MMIO was flushed and updates the protocol state
  //
  MmioRead8 (PciSpiBase + R_PCH_SPI_BC);
  MmioRead8 (LpcPciBase + R_PCH_LPC_BC);

  //
  // Record current state
  //
  mPrivateData.ActiveBiosProtocol.State = DesiredState;

  return EFI_SUCCESS;
}

/**
  Lock or unlock the current active BIOS state.
  Key is a simple incrementing number.

  @param[in] This                 Pointer to the PCH_ACTIVE_BIOS_PROTOCOL instance.
  @param[in] Lock                 TRUE to lock the current state, FALSE to unlock.
  @param[in, out] Key             If Lock is TRUE, then a key will be returned.  If
                                  Lock is FALSE, the key returned from the prior call
                                  to lock the protocol must be provided to unlock the
                                  protocol.  The value of Key is undefined except that
                                  it cannot be 0.

  @retval EFI_SUCCESS             Command succeed.
  @exception EFI_UNSUPPORTED      The function is not supported.
  @retval EFI_ACCESS_DENIED       The interface is currently locked.
**/
static
EFI_STATUS
EFIAPI
LockState (
  IN PCH_ACTIVE_BIOS_PROTOCOL *This,
  IN BOOLEAN                  Lock,
  IN OUT UINTN                *Key
  )
{
  //
  // Check if lock or unlock requesed
  //
  if (Lock) {
    //
    // Check if already locked
    //
    if (mPrivateData.Locked) {
      return EFI_ACCESS_DENIED;
    }
    //
    // Lock the interface
    //
    mPrivateData.Locked = TRUE;

    //
    // Increment the key
    //
    mPrivateData.CurrentKey++;

    //
    // Update the caller's copy
    //
    *Key = mPrivateData.CurrentKey;
  } else {
    //
    // Verify caller "owns" the current lock
    //
    if (*Key == mPrivateData.CurrentKey) {
      mPrivateData.Locked = FALSE;
    } else {
      return EFI_ACCESS_DENIED;
    }
  }

  return EFI_SUCCESS;
}

/**
  Initialization function for the ActiveBios protocol implementation.

  @param[in] This                 Pointer to the protocol

  @retval EFI_SUCCESS             The function completed successfully
**/
EFI_STATUS
ActiveBiosProtocolConstructor (
  IN PCH_ACTIVE_BIOS_PROTOCOL *This
  )
{
  EFI_STATUS Status;
  EFI_HANDLE Handle;
  PCH_SERIES PchSeries;
  UINTN      PciSpiBase;
  
  PchSeries = GetPchSeries();

  PciSpiBase = MmPciBase (
                      DEFAULT_PCI_BUS_NUMBER_PCH,
                      PCI_DEVICE_NUMBER_PCH_SPI,
                      PCI_FUNCTION_NUMBER_PCH_SPI
                      );

  //
  // Read current state from the PCH
  //
  switch ((MmioRead8 (PciSpiBase + R_PCH_SPI_BC) & B_PCH_SPI_BC_BBS) >> N_PCH_SPI_BC_BBS) {

    case V_PCH_SPI_BC_BBS_SPI:
            mPrivateData.ActiveBiosProtocol.State = ActiveBiosStateSpi;
            break;

    case V_PCH_SPI_BC_BBS_LPC:
            mPrivateData.ActiveBiosProtocol.State = ActiveBiosStateLpc;
            break;

        default:
      //
      // This is an invalid use of the protocol
      // See definition, but caller must call with valid value
      //
            ASSERT (!EFI_UNSUPPORTED);
            break;
    }

  mPrivateData.ActiveBiosProtocol.SetState  = SetState;
  mPrivateData.ActiveBiosProtocol.LockState = LockState;
  mPrivateData.CurrentKey                   = 1;
  mPrivateData.Locked                       = FALSE;

  //
  // Install the protocol
  //
  Handle = NULL;
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &Handle,
                  &gPchActiveBiosProtocolGuid,
                  &mPrivateData.ActiveBiosProtocol,
                  NULL
                  );
  return Status;
}
