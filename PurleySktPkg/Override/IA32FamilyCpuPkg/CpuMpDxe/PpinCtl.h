//
// This file contains an 'Intel Peripheral Driver' and is
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may
// be modified by the user, subject to additional terms of the
// license agreement
//
/** @file

  Include file for PPIN lock control

  Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
  This software and associated documentation (if any) is furnished
  under a license and may only be used or copied in accordance
  with the terms of the license. Except as permitted by such
  license, no part of this software or documentation may be
  reproduced, stored in a retrieval system, or transmitted in any
  form or by any means without the express written consent of
  Intel Corporation.

  Module Name:  PpinCtl.h

**/

#ifndef _CPU_PPIN_CTL_H_
#define _CPU_PPIN_CTL_H_

#include "Cpu.h"
#include <Include/CpuHost.h>

#define PPIN_LOCK_DISABLE     1;
#define PPIN_UNLOCK_ENABLE    2;

/**
  Detect capability of PpinCtl feature for specified processor.

  This function detects capability of PpinCtl feature for specified processor.

  @param  ProcessorNumber   The handle number of specified processor.

**/
VOID
PpinCtlDetect (
  UINTN   ProcessorNumber
  );

/**
  Configures Processor Feature Lists for PpinCtl feature for all processors.

  This function configures Processor Feature Lists for PpinCtl feature for all processors.

**/
VOID
PpinCtlConfigFeatureList (
  VOID
  );

/**
  Produces entry of PpinCtl feature in Register Table for specified processor.

  This function produces entry of PpinCtl feature in Register Table for specified processor.

  @param  ProcessorNumber   The handle number of specified processor.
  @param  Attribute         Pointer to the attribute

**/
VOID
PpinCtlReg (
  UINTN      ProcessorNumber,
  VOID       *Attribute
  );

#endif
