/** @file
  Header file for the PCH SPI Runtime Driver.

@copyright
 Copyright (c) 2004 - 2014 Intel Corporation. All rights reserved
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
#ifndef _PCH_SPI_H_
#define _PCH_SPI_H_

#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiRuntimeLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Guid/EventGroup.h>
#include <Library/MmPciBaseLib.h>
#include <PchAccess.h>
#include <Protocol/Spi.h>
#include <IncludePrivate/Library/PchSpiCommonLib.h>

#define EFI_INTERNAL_POINTER  0x00000004

//
// Function prototypes used by the SPI protocol.
//

/**
  Fixup internal data pointers so that the services can be called in virtual mode.

  @param[in] Event                The event registered.
  @param[in] Context              Event context. Not used in this event handler.

**/
VOID
EFIAPI
PchSpiVirtualAddressChangeEvent (
  IN EFI_EVENT              Event,
  IN VOID                   *Context
  );
#endif
