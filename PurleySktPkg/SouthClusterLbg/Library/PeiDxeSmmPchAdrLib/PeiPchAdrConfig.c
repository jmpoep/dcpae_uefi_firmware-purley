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

#include <Uefi/UefiBaseType.h>
#include <PiPei.h>
#include <Library/DebugLib.h>
#include <Library/PeiServicesLib.h>
#include <Ppi/PchPolicy.h>

/**
  Retrieves pointer to PCH_ADR_CONFIG from PCH policy.

  @param[out] PchAdrConfig Pointer to pointer to PCH_ADR_CONFIG.
**/
VOID
PchAdrGetConfig (
  OUT PCH_ADR_CONFIG **PchAdrConfig
  )
{
  EFI_STATUS     Status;
  PCH_POLICY_PPI *PchPolicyPpi;

  Status = PeiServicesLocatePpi (
             &gPchPlatformPolicyPpiGuid,
             0,
             NULL,
             (VOID **)&PchPolicyPpi
             );
  ASSERT_EFI_ERROR (Status);
  *PchAdrConfig = &PchPolicyPpi->AdrConfig;
}