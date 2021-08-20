/** @file
  PCH ADR setup library

@copyright
 Copyright (c) 2017 Intel Corporation. All rights reserved
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

#ifndef _PCH_ADR_LIB_H_
#define _PCH_ADR_LIB_H_

#include <Uefi/UefiBaseType.h>
#include <PchAccess.h>

#define PCH_ADR_TIMER_0US        0
#define PCH_ADR_TIMER_25US      25
#define PCH_ADR_TIMER_50US      50
#define PCH_ADR_TIMER_100US    100
#define PCH_ADR_TIMER_400US    400
#define PCH_ADR_TIMER_600US    600
#define PCH_ADR_TIMER_1200US  1200
#define PCH_ADR_TIMER_2400US  2400
#define PCH_ADR_TIMER_9600US  9600

#define PCH_ADR_SOURCE_NONE      0
#define PCH_ADR_SOURCE_DDR    BIT0
#define PCH_ADR_SOURCE_DDRT   BIT1

#define PCH_ADR_CFG_PMC       BIT0
#define PCH_ADR_CFG_SRC       BIT1
#define PCH_ADR_CFG_TIMER     BIT2

/**
  PCH ADR Enable master switch.

  Enables/disables PCH-side ADR.
  This function must always be called by MRC even if ADR is not being enabled (with Enable parameter equal to FALSE).
  This is to allow PCH DFX overrides to work.

  @param Enable TRUE:  Enables PCH-side ADR
                FALSE: Disables PCH-side ADR
**/
VOID
PchAdrEnableAdr (
  BOOLEAN Enable
  );

/**
  Configures PCH-side ADR.

  This function must always be called by MRC even if ADR is not being enabled.
  This is to allow PCH DFX overrides to work.

  @param ConfigMask    PCH ADR configuration mask - indicates what needs to be configured (PMC/Timer/Sources)
  @param TimerVal      Timer value in microseconds (us).
                       A finite set of values is supported (see PCH_ADR_TIMER_ macros).
  @param SrcConfigMask PCH ADR sources configuration type mask

**/
VOID
PchAdrConfigure (
  UINT32 ConfigMask,
  UINT32 TimerVal,
  UINT32 SrcConfigMask
  );

/**
  Clears PCH ADR Reset Status
**/
VOID
PchAdrClearAdrResetStatus (
  VOID
  );

/**
  Gets PCH ADR Reset Status

  @retval TRUE  ADR flow was executed during a prior global reset entry.
  @retval FALSE ADR flow was *not* executed during a prior global reset entry.

**/
BOOLEAN
PchAdrGetAdrResetStatus (
  VOID
  );

#endif // _PCH_ADR_LIB_H_
