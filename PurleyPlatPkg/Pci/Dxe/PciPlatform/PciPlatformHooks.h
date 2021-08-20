//
// This file contains an 'Intel Pre-EFI Module' and is licensed
// for Intel CPUs and Chipsets under the terms of your license 
// agreement with Intel or your vendor.  This file may be      
// modified by the user, subject to additional terms of the    
// license agreement                                           
//
/**

Copyright (c)  2004 - 2016 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


  @file PciPlatformHooks.h

  This code supports a the private implementation 
  of the Legacy BIOS Platform protocol

**/

#ifndef PCI_PLATFORM_HOOKS_H_
#define PCI_PLATFORM_HOOKS_H_

#include <Library/PlatformHooksLib.h>
#include <Library/IoLib.h>
#include <Library/MmPciBaseLib.h>

VOID
ChipsetCallback (
  IN  EFI_HANDLE                                    RootBridgeHandle,
  IN  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_PCI_ADDRESS   PciAddress,
  IN  EFI_PCI_ENUMERATION_PHASE                     Phase,
  EFI_PCI_CALLBACK_CONTEXT                          *Context
  );

EFI_STATUS
PciTreeTraverse (
  IN  UINT8                                        StartBus
  );

#endif
