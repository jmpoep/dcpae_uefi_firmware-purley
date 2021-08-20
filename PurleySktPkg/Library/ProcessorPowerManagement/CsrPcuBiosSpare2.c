//
// This file contains an 'Intel Peripheral Driver' and is      
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may 
// be modified by the user, subject to additional terms of the 
// license agreement                                           
//
/** @file

  Code File for CPU Power Management

  Copyright (c) 2006 - 2012, Intel Corporation. All rights reserved.<BR>
  This software and associated documentation (if any) is furnished
  under a license and may only be used or copied in accordance
  with the terms of the license. Except as permitted by such
  license, no part of this software or documentation may be
  reproduced, stored in a retrieval system, or transmitted in any
  form or by any means without the express written consent of
  Intel Corporation.

  Module Name:  CsrPcuBiosSpare2.c

**/

#include "CpuPpmIncludes.h"
#include "PCU_FUN3.h"


/**
  This routine is called to program PCU_BIOS_SPARE2. It will be call multiple time passing any the SocketNumber to be programmed.

  @param PPMPolicy Pointer to PPM Policy protocol instance

  @retval EFI_SUCCESS

**/
VOID
ProgramCsrPcuBiosSpare2 (
  EFI_CPU_PM_STRUCT   *ppm
  )
{
  // This should be programmed by Memory reference code.
  return;
}

