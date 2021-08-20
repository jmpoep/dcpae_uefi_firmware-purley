/*++
  This file contains an 'Intel Peripheral Driver' and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement
--*/
/** @file

  Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
  This software and associated documentation (if any) is furnished
  under a license and may only be used or copied in accordance
  with the terms of the license. Except as permitted by such
  license, no part of this software or documentation may be
  reproduced, stored in a retrieval system, or transmitted in any
  form or by any means without the express written consent of
  Intel Corporation.

**/

#include "PpinCtl.h"

BOOLEAN    mEnablePpinCtl = FALSE;

/**
  Detect capability of PpinCtl feature for specified processor.

  This function detects capability of PpinCtl feature for specified processor.

  @param  ProcessorNumber   The handle number of specified processor.

**/
VOID
PpinCtlDetect (
  UINTN   ProcessorNumber
  )
{
  UINT64        PlatformInfoMsrData;

  PlatformInfoMsrData = AsmReadMsr64 (MSR_PLATFORM_INFO);

  //
  // If PPIN supported
  //
  if ((PlatformInfoMsrData & BIT23) == BIT23) {
    SetProcessorFeatureCapability (ProcessorNumber, PpinCtl, NULL);
  }
}

/**
  Configures Processor Feature Lists for PpinCtl feature for all processors.

  This function configures Processor Feature Lists for PpinCtl feature for all processors.

**/
VOID
PpinCtlConfigFeatureList (
  VOID
  )
{
  UINTN                 ProcessorNumber;
  EFI_STATUS            Status;

  if ((PcdGet32 (PcdCpuProcessorFeatureUserConfigurationEx1) & PCD_CPU_PPIN_CTL_BIT) != 0) {
    mEnablePpinCtl = TRUE;
  }

  for (ProcessorNumber = 0; ProcessorNumber < mCpuConfigContextBuffer.NumberOfProcessors; ProcessorNumber++) {
    //
    // Check whether this logical processor supports PpinCtl
    //
    if (GetProcessorFeatureCapability (ProcessorNumber, PpinCtl, NULL)) {
      if (ProcessorNumber == mCpuConfigContextBuffer.BspNumber) {
        //
        // Set the bit of PpinCtl capability according to BSP capability.
        //
        Status = PcdSet32S (PcdCpuProcessorFeatureCapabilityEx1, PcdGet32 (PcdCpuProcessorFeatureCapabilityEx1) | PCD_CPU_PPIN_CTL_BIT);
        ASSERT_EFI_ERROR (Status);
        //
        // Set the bit of PpinCtl setting according to BSP setting.
        //
        if (mEnablePpinCtl) {
          Status = PcdSet32S (PcdCpuProcessorFeatureSettingEx1, PcdGet32 (PcdCpuProcessorFeatureSettingEx1) | PCD_CPU_PPIN_CTL_BIT);
          ASSERT_EFI_ERROR (Status);
        }
      }

      //
      // If this logical processor supports PpinCtl, then add feature into feature list for operation
      // on corresponding register.
      //
      AppendProcessorFeatureIntoList (ProcessorNumber, PpinCtl, &mEnablePpinCtl);
    }
  }
}

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
  )
{
  BOOLEAN    *Enable;
  UINT64     Value;

  //
  // The scope of the MSR_PPIN_CTL is Package, only program MSR_PPIN_CTL once for each package.
  //
  if (mCpuConfigContextBuffer.CollectedDataBuffer[ProcessorNumber].PackageBsp) {
    //
    // If Attribute is TRUE, then unlock and enable.
    // Otherwise, lock and disable.
    //
    Enable = (BOOLEAN *) Attribute;
    Value  = PPIN_LOCK_DISABLE;
    if (*Enable) {
      Value = PPIN_UNLOCK_ENABLE;
    }

    WriteRegisterTable (
      ProcessorNumber,
      Msr,
      MSR_PPIN_CTL,
      0,
      2,
      Value
      );
  }
}
