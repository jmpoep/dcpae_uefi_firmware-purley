/** @file
  Header file for PCH Pci Express helps library implementation.

@copyright
 Copyright (c) 2008 - 2014 Intel Corporation. All rights reserved
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
#ifndef _PCH_PCI_EXPRESS_HELPERS_LIBRARY_H_
#define _PCH_PCI_EXPRESS_HELPERS_LIBRARY_H_

#include <Uefi/UefiBaseType.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <IndustryStandard/Pci30.h>
#include <PchPolicyCommon.h>
#include <PchAccess.h>
#include <Library/PchPcieRpLib.h>
#include <Library/PchPcrLib.h>
#include <Library/PchInfoLib.h>
#include <Library/MmPciBaseLib.h>
#include <Library/GpioNativeLib.h>
#include <IncludePrivate/Library/PchPciExpressHelpersLib.h>

#define LTR_VALUE_MASK (BIT0 + BIT1 + BIT2 + BIT3 + BIT4 + BIT5 + BIT6 + BIT7 + BIT8 + BIT9)
#define LTR_SCALE_MASK (BIT10 + BIT11 + BIT12)

  #define CONFIG_WRITE_LOOP_COUNT   100000

//
// LTR related macros
//
#define LTR_LATENCY_VALUE(x)           ((x) & LTR_VALUE_MASK)
#define LTR_SCALE_VALUE(x)             (((x) & LTR_SCALE_MASK) >> 10)
#define LTR_LATENCY_NS(x)              (LTR_LATENCY_VALUE(x) * (1 << (5 * LTR_SCALE_VALUE(x))))

#endif
