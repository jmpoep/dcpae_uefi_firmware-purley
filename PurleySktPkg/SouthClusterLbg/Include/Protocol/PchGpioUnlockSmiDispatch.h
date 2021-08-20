/** @file
  APIs of PCH GPIO UNLOCK SMI Dispatch Protocol.

@copyright
 Copyright (c) 2014 Intel Corporation. All rights reserved
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
#ifndef _PCH_GPIO_UNLOCK_SMI_DISPATCH_PROTOCOL_H_
#define _PCH_GPIO_UNLOCK_SMI_DISPATCH_PROTOCOL_H_

//
// Extern the GUID for protocol users.
//
extern EFI_GUID                         gPchGpioUnlockSmiDispatchProtocolGuid;

//
// Forward reference for ANSI C compatibility
//
typedef struct _PCH_GPIO_UNLOCK_SMI_DISPATCH_PROTOCOL       PCH_GPIO_UNLOCK_SMI_DISPATCH_PROTOCOL;

//
// Member functions
//

/**
  Callback function for an PCH GPIO UNLOCK SMI handler dispatch.

  @param[in] DispatchHandle             The unique handle assigned to this handler by register function.

**/
typedef
VOID
(EFIAPI *PCH_GPIO_UNLOCK_SMI_DISPATCH_CALLBACK) (
  IN EFI_HANDLE                         DispatchHandle
  );

/**
  Register a child SMI source dispatch function for specific PCH GPIO UNLOCK SMI dispatch event.

  @param[in] This                       Protocol instance pointer.
  @param[in] DispatchFunction           Pointer to dispatch function to be invoked for
                                        this SMI source
  @param[out] DispatchHandle            Handle of dispatch function, for when interfacing
                                        with the parent SMM driver.

  @retval EFI_SUCCESS                   The dispatch function has been successfully
                                        registered and the SMI source has been enabled.
  @retval EFI_DEVICE_ERROR              The driver was unable to enable the SMI source.
  @retval EFI_OUT_OF_RESOURCES          Not enough memory (system or SMM) to manage this child.
  @retval EFI_ACCESS_DENIED             Return access denied if the EndOfDxe event has been triggered
**/
typedef
EFI_STATUS
(EFIAPI *PCH_GPIO_UNLOCK_SMI_DISPATCH_REGISTER) (
  IN  PCH_GPIO_UNLOCK_SMI_DISPATCH_PROTOCOL       *This,
  IN  PCH_GPIO_UNLOCK_SMI_DISPATCH_CALLBACK       DispatchFunction,
  OUT EFI_HANDLE                                  *DispatchHandle
  );

/**
  Unregister a child SMI source dispatch function with a parent GPIO UNLOCK SMM driver

  @param[in] This                       Protocol instance pointer.
  @param[in] DispatchHandle             Handle of dispatch function to deregister.

  @retval EFI_SUCCESS                   The dispatch function has been successfully
                                        unregistered and the SMI source has been disabled
                                        if there are no other registered child dispatch
                                        functions for this SMI source.
  @retval EFI_INVALID_PARAMETER         Handle is invalid.
  @retval EFI_ACCESS_DENIED             Return access denied if the EndOfDxe event has been triggered
**/
typedef
EFI_STATUS
(EFIAPI *PCH_GPIO_UNLOCK_SMI_DISPATCH_UNREGISTER) (
  IN  PCH_GPIO_UNLOCK_SMI_DISPATCH_PROTOCOL       *This,
  IN  EFI_HANDLE                                  DispatchHandle
  );

/**
  Interface structure for PCH GPIOUNLOCK SMI Dispatch Protocol
  The PCH GPIO UNLOCK SMI DISPATCH PROTOCOL provides the ability to dispatch function for
  PCH gpio unlock SMIs.
**/
struct _PCH_GPIO_UNLOCK_SMI_DISPATCH_PROTOCOL {
  /**
    This member specifies the revision of this structure. This field is used to
    indicate backwards compatible changes to the protocol.
  **/
  UINT8                                           Revision;
  /**
    Smi unregister function for PCH GPIO UNLOCK SMI DISPATCH PROTOCOL.
  **/
  PCH_GPIO_UNLOCK_SMI_DISPATCH_UNREGISTER         UnRegister;
  /**
    GpioUnlock
    The event is triggered if the GPIO registers lockdown logic is requesting an SMI#.
  **/
  PCH_GPIO_UNLOCK_SMI_DISPATCH_REGISTER           Register;
};

/**
  PCH GPIO UNLOCK SMI dispatch revision number

  Revision 1:   Initial version
**/
#define PCH_GPIO_UNLOCK_SMI_DISPATCH_REVISION     1

#endif
