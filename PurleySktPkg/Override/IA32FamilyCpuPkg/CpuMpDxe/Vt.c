/*++
  This file contains an 'Intel Peripheral Driver' and is        
  licensed for Intel CPUs and chipsets under the terms of your  
  license agreement with Intel or your vendor.  This file may   
  be modified by the user, subject to additional terms of the   
  license agreement                                             
--*/
/** @file

  Code for Vanderpool Technology and LaGrande Technology

  Copyright (c) 2006 - 2016, Intel Corporation. All rights reserved.<BR>
  This software and associated documentation (if any) is furnished
  under a license and may only be used or copied in accordance
  with the terms of the license. Except as permitted by such
  license, no part of this software or documentation may be
  reproduced, stored in a retrieval system, or transmitted in any
  form or by any means without the express written consent of
  Intel Corporation.

  Module Name:  Vt.c

**/

#include "Vt.h"

BOOLEAN   *mLtSupported;
UINT64    *mMsrValue;

/**
  Allocate buffer for VT/LT feature configuration.
  
**/
VOID
VtLtAllocateBuffer (
  VOID
  )
{
  mLtSupported = (BOOLEAN *)AllocateZeroPool (
                              sizeof (BOOLEAN) * mCpuConfigContextBuffer.NumberOfProcessors
                              );
  ASSERT (mLtSupported != NULL);

  mMsrValue = (UINT64 *)AllocatePool (
                          sizeof (UINT64) * mCpuConfigContextBuffer.NumberOfProcessors
                          );
  ASSERT (mMsrValue != NULL);
}

/**
  Detect capability of VT and LT feature for specified processor.
  
  This function detects capability of VT and LT feature for specified processor.

  @param  ProcessorNumber   The handle number of specified processor.

**/
VOID
VtLtDetect (
  UINTN   ProcessorNumber
  )
{
  EFI_CPUID_REGISTER  *CpuidRegisters;

  //
  // Check CPUID(1).ECX[5] for VT capability
  //
  CpuidRegisters = GetProcessorCpuid (ProcessorNumber, EFI_CPUID_VERSION_INFO);
  ASSERT (CpuidRegisters != NULL);

  if (BitFieldRead32 (CpuidRegisters->RegEcx, N_CPUID_VMX_SUPPORT, N_CPUID_VMX_SUPPORT) == 1) {
    //
    // If VT is supported, check CPUID(1).ECX[6] for LT capability
    //
    if (BitFieldRead32 (CpuidRegisters->RegEcx, N_CPUID_SMX_SUPPORT, N_CPUID_SMX_SUPPORT) == 1) {
      mLtSupported[ProcessorNumber] = TRUE;
    }

    SetProcessorFeatureCapability (ProcessorNumber, Vt, &mLtSupported[ProcessorNumber]);

    //
    // Get the current value of Feature Control MSR
    //
    mMsrValue[ProcessorNumber] = AsmReadMsr64 (MSR_IA32_FEATURE_CONTROL);
  }
}

/**
  Configures Processor Feature Lists for VT and LT feature for all processors.
  
  This function configures Processor Feature Lists for VT and LT feature for all processors.

**/
VOID
VtLtConfigFeatureList (
  VOID
  )
{
  UINTN                 ProcessorNumber;
  BOOLEAN               VtSupported;
  BOOLEAN               LtSupported;
  BOOLEAN               *Attribute;
  UINT64                NewMsrValue;
  EFI_STATUS            Status;

  VtSupported = TRUE;
  LtSupported = TRUE;

  //
  // Check whether all logical processors support VT
  //
  for (ProcessorNumber = 0; ProcessorNumber < mCpuConfigContextBuffer.NumberOfProcessors; ProcessorNumber++) {
    if (!GetProcessorFeatureCapability (ProcessorNumber, Vt, (VOID **) &Attribute)) {
      //
      // If VT is not supported by this processor, then VT will be disabled on all logical processors.
      // Therefore, LT will also be disabled on all logical processors.
      //
      VtSupported = FALSE;
      LtSupported = FALSE;
      break;
    }
    if (LtSupported) {
      //
      // If LT is not supported by this processor, then LT will be disabled on all logical processors.
      //
      if (!*Attribute) {
        LtSupported = FALSE;
      }
    }
  }

  //
  // The new value would be assigned to all Feature Control MSRs that are not locked.
  // At lease, it would be locked.
  //
//
// PURLEY_OVERRIDE_BEGIN
//
  NewMsrValue = 0;
//
// PURLEY_OVERRIDE_END
//  
  if (VtSupported) {
    //
    // Set the bit of VT capability
    //
    Status = PcdSet32S (PcdCpuProcessorFeatureCapability, PcdGet32 (PcdCpuProcessorFeatureCapability) | PCD_CPU_VT_BIT);
    ASSERT_EFI_ERROR (Status);

    if (LtSupported) {
      //
      // Set the bit of LT capability
      //
      Status = PcdSet32S (PcdCpuProcessorFeatureCapability, PcdGet32 (PcdCpuProcessorFeatureCapability) | PCD_CPU_LT_BIT);
      ASSERT_EFI_ERROR (Status);
    }

    //
    // Check whether MSR is locked. If locked, we cannot modify Feature Control MSR, and just use current
    // status as the final configuration status.
    // Since VtSupported is TRUE, all logical processors support VT and have Feature Control MSR.
    // Since Feature Control MSR setting is uniform, so we just check the status of processor 0.
    //
    if (BitFieldRead64(mMsrValue[0], N_MSR_FEATURE_CONTROL_LOCK, N_MSR_FEATURE_CONTROL_LOCK) == 1) {
      if (BitFieldRead64(mMsrValue[0], N_MSR_ENABLE_VMX_OUTSIDE_SMX, N_MSR_ENABLE_VMX_OUTSIDE_SMX) == 1) {
        Status = PcdSet32S (PcdCpuProcessorFeatureSetting, PcdGet32 (PcdCpuProcessorFeatureSetting) | PCD_CPU_VT_BIT);
        ASSERT_EFI_ERROR (Status);
      }
      if (BitFieldRead64(mMsrValue[0], N_MSR_ENABLE_VMX_INSIDE_SMX, N_MSR_ENABLE_VMX_INSIDE_SMX) == 1) {
        Status = PcdSet32S (PcdCpuProcessorFeatureSetting, PcdGet32 (PcdCpuProcessorFeatureSetting) | PCD_CPU_LT_BIT);
        ASSERT_EFI_ERROR (Status);
      }
    } else {
      //
      // If not locked, checkes whether user indicates to enable VT.
      //
      if ((PcdGet32 (PcdCpuProcessorFeatureUserConfiguration) & PCD_CPU_VT_BIT) != 0) {
        //
        // Set bit 2 of new MSR value
        //
        NewMsrValue = BitFieldWrite64 (NewMsrValue, N_MSR_ENABLE_VMX_OUTSIDE_SMX, N_MSR_ENABLE_VMX_OUTSIDE_SMX, 1);
        //
        // Set the bit of current setting for VT
        //
        Status = PcdSet32S (PcdCpuProcessorFeatureSetting, PcdGet32 (PcdCpuProcessorFeatureSetting) | PCD_CPU_VT_BIT);
        ASSERT_EFI_ERROR (Status);

//
// PURLEY_OVERRIDE_BEGIN
//
#ifdef LT_FLAG
//
// PURLEY_OVERRIDE_END
//
        //
        // After VT has been enabled, check if LT can be enabled.
        //
        if (LtSupported && ((PcdGet32 (PcdCpuProcessorFeatureUserConfiguration) & PCD_CPU_LT_BIT) != 0)) {
          //
          // Set bit 1 to enable VMX inside SMX
          //
          NewMsrValue = BitFieldWrite64 (NewMsrValue, N_MSR_ENABLE_VMX_INSIDE_SMX, N_MSR_ENABLE_VMX_INSIDE_SMX, 0x01);
          //
          // Set bit field 8:15 of MSR_IA32_FEATURE_CONTROL for LT
          //
          NewMsrValue = BitFieldWrite64 (NewMsrValue, N_MSR_SENTER_LOCAL_FUNC_ENABLE_START, N_MSR_SENTER_GLOBAL_ENABLE, 0xff);
          //
          // Set the bit of current setting for LT
          //
          Status = PcdSet32S (PcdCpuProcessorFeatureSetting, PcdGet32 (PcdCpuProcessorFeatureSetting) | PCD_CPU_LT_BIT);
          ASSERT_EFI_ERROR (Status);
        }
//
// PURLEY_OVERRIDE_BEGIN
//
#endif
//
// PURLEY_OVERRIDE_END
//
      }
    }    
  }

  //
  // For all logical processors supporting VT whose Feature Control MSR is not locked, we must operate on it.
  //
  for (ProcessorNumber = 0; ProcessorNumber < mCpuConfigContextBuffer.NumberOfProcessors; ProcessorNumber++) {
    if (GetProcessorFeatureCapability (ProcessorNumber, Vt, NULL)) {
      if (BitFieldRead64(mMsrValue[ProcessorNumber], N_MSR_FEATURE_CONTROL_LOCK, N_MSR_FEATURE_CONTROL_LOCK) == 0) {
        mMsrValue[ProcessorNumber] = NewMsrValue;
        AppendProcessorFeatureIntoList (ProcessorNumber, Vt, &(mMsrValue[ProcessorNumber]));
      }
    }
  }
}

/**
  Produces entry of VT and LT feature in Register Table for specified processor.
  
  This function produces entry of VT and LT feature in Register Table for specified processor.

  @param  ProcessorNumber   The handle number of specified processor.
  @param  Attribute         Pointer to the attribute

**/
VOID
VtLtReg (
  UINTN      ProcessorNumber,
  VOID       *Attribute
  )
{
  UINT64    *Value;

  Value = (UINT64 *) Attribute;

  WriteRegisterTable (
    ProcessorNumber,
    Msr,
    MSR_IA32_FEATURE_CONTROL,
    0,
    63,
    *Value
    );
}
