//
// This file contains an 'Intel Peripheral Driver' and is      
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may 
// be modified by the user, subject to additional terms of the 
// license agreement                                           
//
/** @file

  Include file for CPU DXE Module

  Copyright (c) 2006 - 2018, Intel Corporation. All rights reserved.<BR>
  This software and associated documentation (if any) is furnished
  under a license and may only be used or copied in accordance
  with the terms of the license. Except as permitted by such
  license, no part of this software or documentation may be
  reproduced, stored in a retrieval system, or transmitted in any
  form or by any means without the express written consent of
  Intel Corporation.

  Module Name:  Cpu.h

**/

#ifndef _CPU_DXE_H_
#define _CPU_DXE_H_

#include "MpCommon.h"

#define PLATFORM_DESKTOP           0
#define PLATFORM_MOBILE            1
#define PLATFORM_SERVER            2

#define EFI_CPUID_CORE_TOPOLOGY    0x0B

//
// The definitions below follow the naming rules.
//   Definitions beginning with "B_" are bits within registers
//   Definitions beginning with "N_" are the bit position
//   Definitions with "_CPUID_" are CPUID bit fields
//   Definitions with "_MSR_" are MSR bit fields
//   Definitions with "N_*_START" are the bit start position
//   Definitions with "N_*_STOP" are the bit stop position
//   Definitions with "B_*_MASK" are the bit mask for the register values
//

//
// Bit definitions for CPUID EAX = 1
//
// ECX
#define N_CPUID_MONITOR_MWAIT_SUPPORT               3
#define N_CPUID_VMX_SUPPORT                         5
#define N_CPUID_SMX_SUPPORT                         6
#define N_CPUID_EIST_SUPPORT                        7
#define N_CPUID_TM2_SUPPORT                         8
//
// PURLEY_OVERRIDE_BEGIN
//
#define N_CPUID_DEBUG_FEATURE_SUPPORT               11	// Haswell Processor ONLY!
//
// PURLEY_OVERRIDE_END
//
#define N_CPUID_DCA_SUPPORT                         18
#define N_CPUID_X2APIC_SUPPORT                      21
#define N_CPUID_AESNI_SUPPORT                       25
// EDX
#define N_CPUID_MCE_SUPPORT                         7
#define N_CPUID_MCA_SUPPORT                         14
#define N_CPUID_TM_SUPPORT                          29
#define N_CPUID_PBE_SUPPORT                         31

//
// Bit definitions for CPUID EAX = 6
//
// EAX
#define N_CPUID_TURBO_MODE_AVAILABLE                1

//
// Bit definitions for CPUID EAX = 80000001h
//
// EDX
#define N_CPUID_XD_BIT_AVAILABLE                    20

//
// Bit definitions for MSR_IA32_APIC_BASE (ECX = 1Bh)
//
#define N_MSR_BSP_FLAG                              8
#define B_MSR_ENABLE_X2APIC_MODE                    BIT10
#define N_MSR_ENABLE_X2APIC_MODE                    10
#define N_MSR_APIC_GLOBAL_ENABLE                    11

//
// Bit definitions for MSR_EBC_SOFT_POWERON (ECX = 2Bh)
//
#define N_MSR_INITIATOR_MCERR_DISABLE               4
#define N_MSR_INTERNAL_MCERR_DISABLE                5
#define N_MSR_BINIT_DRIVER_DISABLE                  6

//
// Bit definitions for MSR_EBC_FREQUENCY_ID (ECX = 2Ch)
//
#define N_MSR_SCALABLE_BUS_SPEED_START              16
#define N_MSR_SCALABLE_BUS_SPEED_STOP               18

//
// Bit definitions for MSR_PIC_MSG_CONTROL (ECX = 2Eh)
//
#define N_MSR_APIC_TPR_UPD_MSG_DISABLE              10

//
// Bit definitions for MSR_IA32_FEATURE_CONTROL (ECX = 3Ah)
//
#define N_MSR_FEATURE_CONTROL_LOCK                  0
#define N_MSR_ENABLE_VMX_INSIDE_SMX                 1
#define N_MSR_ENABLE_VMX_OUTSIDE_SMX                2
#define N_MSR_SENTER_LOCAL_FUNC_ENABLE_START        8
#define N_MSR_SENTER_LOCAL_FUNC_ENABLE_STOP         14
#define N_MSR_SENTER_GLOBAL_ENABLE                  15

//
// Bit definitions for MSR_VLW_CONTROL (ECX = 4Bh)
//
#define N_MSR_A20M_DISABLE                          1

//
// Bit definitions for MSR_PLATFORM_INFO (ECX = 0CEh)
//
#define N_MSR_MAX_NON_TURBO_RATIO_START             8
#define N_MSR_MAX_NON_TURBO_RATIO_STOP              15
#define B_MSR_PROGRAMMABLE_RATIO_LIMIT_TURBO_MODE   BIT28
#define B_MSR_PROGRAMMABLE_TDP_LIMIT_TURBO_MODE     BIT29
#define B_MSR_PROGRAMMABLE_TCC_ACTIVATION_OFFSET    BIT30
#define N_MSR_MAX_EFFICIENCY_RATIO_START            40
#define N_MSR_MAX_EFFICIENCY_RATIO_STOP             47
#define B_MSR_NO_OF_CONFIG_TDP_LEVELS               (BIT34 | BIT33) 

//
// Bit definitions for MSR_PKG_CST_CONFIG_CONTROL (ECX = 0E2h)
//
#define B_MSR_PACKAGE_C_STATE_LIMIT_MASK            0x7
#define B_MSR_IO_MWAIT_REDIRECTION_ENABLE           BIT10
#define N_MSR_EIST_HARDWARE_COORDINATION_DISABLE    11
#define B_MSR_C3_STATE_AUTO_DEMOTION_ENABLE         BIT25
#define B_MSR_C1_STATE_AUTO_DEMOTION_ENABLE         BIT26
#define B_MSR_C3_STATE_AUTO_UNDEMOTION_ENABLE       BIT27
#define B_MSR_C1_STATE_AUTO_UNDEMOTION_ENABLE       BIT28

//
// Bit definitions for MSR_POWER_MISC (ECX = 120h)
//
#define B_MSR_POWER_MISC_IA_UNTRUSTED               BIT6
#define N_MSR_POWER_MISC_IA_UNTRUSTED               6

//
// Bit definitions for MSR_FEATURE_CONFIG (ECX = 13Ch)
//
#define N_MSR_FEATURE_CONFIG_LOCK                   0
#define B_MSR_FEATURE_CONFIG_LOCK                   BIT0
#define N_MSR_AESNI_DISABLE                         1

//
// Bit definitions for MSR_IA32_MCG_CAP (ECX = 179h)
//
#define N_MSR_MCG_COUNT_START                       0
#define N_MSR_MCG_COUNT_STOP                        7
#define N_MSR_MCG_CTL_P                             8

//
// Bit definitions for MSR_IA32_CLOCK_MODULATION (ECX = 19Ah)
//
#define N_MSR_CLOCK_MODULATION_DUTY_CYCLE_START     0
#define N_MSR_CLOCK_MODULATION_DUTY_CYCLE_STOP      3
#define B_MSR_CLOCK_MODULATION_DUTY_CYCLE_MASK      0xF
#define N_MSR_CLOCK_MODULATION_ENABLE               4

//
// Bit definitions for MSR_IA32_MISC_ENABLE (ECX = 1A0h)
//
#define N_MSR_FAST_STRINGS_ENABLE                   0
#define N_MSR_AUTOMATIC_TCC_ENABLE                  3
#define N_MSR_HW_PREFETCHER_DISABLE                 9
#define N_MSR_FERR_MULTIPLEXING_ENABLE              10
#define N_MSR_TM2_ENABLE                            13
#define N_MSR_EIST_ENABLE                           16
#define N_MSR_ENABLE_MONITOR_FSM                    18
#define N_MSR_ADJACENT_CACHE_LINE_PREFETCH_DISABLE  19
#define N_MSR_EIST_SELECT_LOCK                      20
#define N_MSR_LIMIT_CPUID_MAXVAL                    22
#define N_MSR_XD_BIT_DISABLE                        34
#define N_MSR_DCU_PREFETCHER_DISABLE                37
#define N_MSR_TURBO_MODE_DISABLE                    38
#define N_MSR_IP_PREFETCHER_DISABLE                 39

//
// Bit definitions for MSR_TEMPERATURE_TARGET (ECX = 1A2h)
//
#define N_MSR_TCC_ACTIVATION_OFFSET_START           24
#define N_MSR_TCC_ACTIVATION_OFFSET_STOP            27

//
// Bit definitions for MSR_MISC_FEATURE_CONTROL (ECX = 1A4h)
//
#define N_MSR_MLC_STREAMER_PREFETCHER_DISABLE       0
#define N_MSR_MLC_SPATIAL_PREFETCHER_DISABLE        1
#define N_MSR_DCU_STREAMER_PREFETCHER_DISABLE       2
#define N_MSR_DCU_IP_PREFETCHER_DISABLE             3
#define N_MSR_THREE_STRIKE_COUNTER_DISABLE          11
//
// For Knights Landing processor
//
#define N_MSR_L1_DATA_PREFETCHER_DISABLE            0
#define N_MSR_L2_PREFETCHER_DISABLE                 1
#define N_MSR_DISABLE_THREE_STRIKE_CNT              2

//
// Bit definitions for MSR_MISC_PWR_MGMT (ECX = 1AAh)
//
#define N_MSR_EIST_HW_COORDINATION_DISABLE          0
#define N_MSR_LOCK_TM_INTERRUPT                     22

//
// Bit definitions for MSR_TURBO_POWER_CURRENT_LIMIT (ECX = 1ACh)
//
#define N_MSR_TDP_LIMIT_START                       0
#define N_MSR_TDP_LIMIT_STOP                        14
#define N_MSR_TDC_LIMIT_START                       16
#define N_MSR_TDC_LIMIT_STOP                        30

//
// Bit definitions for MSR_TURBO_RATIO_LIMIT (ECX = 1ADh)
//
#define N_MSR_MAX_RATIO_LIMIT_1C_START              0
#define N_MSR_MAX_RATIO_LIMIT_1C_STOP               7

//
// Bit definitions for MSR_IA32_ENERGY_PERFORMANCE_BIAS (ECX = 1B0h)
//
#define N_MSR_POWER_POLICY_PREFERENCE_START         0
#define N_MSR_POWER_POLICY_PREFERENCE_STOP          3

//
// Bit definitions for MSR_IA32_PLATFORM_DCA_CAP (ECX = 1F8h)
//
#define N_MSR_DCA_TYPE0_ENABLE                      0

//
// Bit definitions for MSR_IA32_CPU_DCA_CAP (ECX = 1F9h)
//
#define B_MSR_DCA_TYPE0_SUPPORTED                   BIT0

//
// Bit definitions for MSR_IA32_DCA_0_CAP (ECX = 1FAh)
//
#define N_MSR_SW_BLOCK                              24

//
// Bit definitions for MSR_POWER_CTL (ECX = 1FCh)
//
#define N_MSR_BI_DIRECTIONAL_PROCHOT_ENABLE         0
#define N_MSR_C1E_ENABLE                            1
#define N_MSR_IA32_ENERGY_PERF_BIAS_ACCESS_ENABLE   18

//
// Bit definitions for MSR_PACKAGE_POWER_SKU_UNIT (ECX = 606h)
//
#define N_MSR_POWER_UNIT_START                      0
#define N_MSR_POWER_UNIT_STOP                       3

//
// Bit definitions for MSR_PACKAGE_POWER_LIMIT (ECX = 610h)
//
#define N_MSR_POWER_LIMIT_1_START                   0
#define N_MSR_POWER_LIMIT_1_STOP                    14

//
// Bit definitions for MSR_IACORE_RATIOS (ECX = 66Ah)
//
#define N_MSR_LFM_RATIO_START                       8
#define N_MSR_LFM_RATIO_STOP                        13
#define N_MSR_GUAR_RATIO_START                      16
#define N_MSR_GUAR_RATIO_STOP                       21

//
// Bit definitions for MSR_IACORE_VIDS (ECX = 66Bh)
//
#define N_MSR_LFM_VID_START                         8
#define N_MSR_LFM_VID_STOP                          14
#define N_MSR_GUAR_VID_START                        16
#define N_MSR_GUAR_VID_STOP                         22

//
// Bit definitions for MSR_IACORE_TURBO_RATIOS (ECX = 66Ch)
//
#define M_MSR_MAX_RATIO_1C_MASK                     0x3F

//
// Bit definitions for MSR_IACORE_TURBO_VIDS (ECX = 66Dh)
//
#define B_MSR_MAX_VID_1C_MASK                       0x7F

//
// Bit definitions for Config TDP related MSRs
//
//
// PURLEY_OVERRIDE_BEGIN
//
#define MSR_CONFIG_TDP_NOMINAL                0x648
#define B_CONFIG_TDP_NOMINAL_RATIO            0xFF
//
// PURLEY_OVERRIDE_END
//
#define MSR_CONFIG_TDP_LVL1                         0x649
#define MSR_CONFIG_TDP_LVL2                         0x64A
#define N_CONFIG_TDP_PKG_MIN_PWR                    47
#define B_CONFIG_TDP_PKG_MIN_PWR                    0xFFFF         // bits 62:47
#define N_CONFIG_TDP_PKG_MAX_PWR                    32
#define B_CONFIG_TDP_PKG_MAX_PWR                    0x7FFF         // bits 46:32
#define N_CONFIG_TDP_LVL_RATIO                      16
#define B_CONFIG_TDP_LVL_RATIO                      0xFF
#define B_CONFIG_TDP_LVL_PKG_TDP                    0x7FFF

#define MSR_CONFIG_TDP_CONTROL                      0x64B
#define B_CONFIG_TDP_CONTROL_LOCK                   (1 << 31)
#define B_CONFIG_TDP_CONTROL_LVL                    0x3
#define V_CONFIG_TDP_NOMINAL                        0
#define V_CONFIG_TDP_LEVEL1                         1
#define V_CONFIG_TDP_LEVEL2                         2

//
// PURLEY_OVERRIDE_BEGIN
//
// IA32_DEBUG_INTERFACE_MSR
#define MSR_IA32_DEBUG_INTERFACE              0x00000C80
#define   B_ENABLE_DEBUG_FEATURES             BIT0
#define   B_DEBUG_INTERFACE_LOCK              BIT30

#define MSR_TRACE_HUB_STH_ACPIBAR_BASE                                0x00000080

// CBO_SLICE0_CR_IIO_LLC_WAYS
#define MSR_CBO_SLICE0_CR_IIO_LLC_WAYS        0x0C8B
#define   MASK_CBO_SLICE0_CR_IIO_LLC_WAYS     0x00000000007fffff  // lower 23-bits are R/W
#define MSR_CBO_SLICE0_CR_INGRESS_SPARE       0x005F
#define   MASK_CBO_SLICE0_CR_INGRESS_SPARE    0x00000000000007ff  // Lower 11 bits are R/W
#define   MASK_CBO_SLICE0_CR_REMOTE_WAYS      0x0000000007ff0000  // bits [26:16] are R/W

#define MSR_RRQ_VIRTUAL_MSR                   0x0063
#define   MASK_RRQ_VIRTUAL_MSR_RRQ_CT         0x000000000000003E  // bits [5:1] are R/W
#define   B_RRQ_VIRTUAL_MSR_LOCK              BIT0

// VIRTUAL_MSR_CR_QLRU_CONFIG
#define MSR_VIRTUAL_MSR_CR_QLRU_CONFIG        0x0C8C
//#define   MASK_VIRTUAL_MSR_CR_QLRU_CONFIG     ((0x00fff << 44) | 0x3fffffff)   // [55:44], [29:0]
#define   MASK_VIRTUAL_MSR_CR_QLRU_CONFIG     0x00fff0003fffffff   // [55:44], [29:0]

#define MSR_VIRTUAL_MSR_LLC_PREFETCH          0x972
#define   N_VIRTUAL_MSR_LLC_PREFETCH_DISABLE  0
#define   N_ENABLE_DBP_FOR_F                  5
#define   N_BI_2IFU_4_F_VICTIMS_EN            6
#define   B_ENABLE_DBP_FOR_F                  BIT5

//
// PURLEY_OVERRIDE_END
//
extern EFI_PHYSICAL_ADDRESS        mStartupVector;

extern BOOLEAN                     mRestoreSettingAfterInit;
extern UINT8                       mPlatformType;
//
// PURLEY_OVERRIDE_BEGIN
//
extern UINT8                       mFUSE_TJ_MAX_OFFSET;
//
// PURLEY_OVERRIDE_END
//
extern ACPI_CPU_DATA               *mAcpiCpuData;

/**
  Collects basic processor data for calling processor.

  This function collects basic processor data for calling processor.

  @param  ProcessorNumber    Handle number of specified logical processor.

**/
VOID
CollectBasicProcessorData (
  IN UINTN  ProcessorNumber
  );

/**
  Early MP Initialization.
  
  This function does early MP initialization, including MTRR sync and first time microcode load.

  @param  ProcessorNumber    Handle number of specified logical processor.

**/
VOID
EarlyMpInit (
  IN UINTN  ProcessorNumber
  );

/**
  Collects data from all logical processors.

  This function collects data from all logical processors

**/
VOID
DataCollectionPhase (
  VOID
  );

/**
  Produces register table according to output of Data Collection phase.
  
  This function produces register table according to output of Data Collection phase.

**/
VOID
AnalysisPhase (
  VOID
  );

/**
  Programs processor registers according to register tables.

  This function programs processor registers according to register tables.

**/
VOID
SettingPhase (
  VOID
  );

/**
  Produces Pre-SMM-Init Register Tables for all processors.

  This function produces Pre-SMM-Init Register Tables for all processors.

**/
VOID
ProducePreSmmInitRegisterTable (
  VOID
  );

/**
  Wakes up APs for the first time to count their number and collect BIST data.

  This function wakes up APs for the first time to count their number and collect BIST data.

**/
VOID
WakeupAPAndCollectBist (
  VOID
  );

/**
  Configures all logical processors with three-phase architecture.
  
  This function configures all logical processors with three-phase architecture.

**/
VOID
ProcessorConfigurationuration (
  VOID
  );

/**
  Select least-feature processor as BSP.
  
  This function selects least-feature processor as BSP.

**/
VOID
SelectLfpAsBsp (
  VOID
  );

/**
  Add SMBIOS Processor Type and Cache Type tables for the CPU.
**/
VOID
AddCpuSmbiosTables (
  VOID
  );

/**
  Prepare ACPI NVS memory below 4G memory for use of S3 resume.
  
  This function allocates ACPI NVS memory below 4G memory for use of S3 resume,
  and saves data into the memory region.

  @param  Context   The Context save the info.
  
**/
VOID
SaveCpuS3Data (
  VOID    *Context
  );

/**
  Programs registers for the calling processor.

  This function programs registers for the calling processor.

  @param  ProcessorNumber    Handle number of specified logical processor.

**/
VOID
SetProcessorRegister (
  IN UINTN  ProcessorNumber
  );

//
// PURLEY_OVERRIDE_BEGIN
//
VOID 
EarlyMpInitPbsp (
  UINTN	   ProcessorNumber
);
//
// PURLEY_OVERRIDE_END
//
/**
  Programs registers before SMM initialization for the calling processor.

  This function programs registers before SMM initialization for the calling processor.

  @param  ProcessorNumber    Handle number of specified logical processor.

**/
VOID
SetPreSmmInitProcessorRegister (
  IN UINTN  ProcessorNumber
  );

/**
  Collects processor microcode revision of the processor installed in the system.

  This function collects processor microcode revision of the processor installed in the system.

  @param  ProcessorNumber    Handle number of specified logical processor

**/
VOID
CollectMicrocodeRevision (
  UINTN    ProcessorNumber
  );

/**
  Label of start of AP machine check handler.

  This is just a label of start of AP machine check handler.

**/
VOID
EFIAPI
ApMachineCheckHandler (
  VOID
  );

/**
  Label of end of AP machine check handler.

  This is just a label of end of AP machine check handler.

**/
VOID
EFIAPI
ApMachineCheckHandlerEnd (
  VOID
  );

/**
  This function gets Package ID/Core ID/Thread ID of the processor.

  The algorithm below assumes the target system has symmetry across physical package boundaries
  with respect to the number of logical processors per package, number of cores per package.

  @param  InitialApicId  Initial APIC ID for determing processor topology.
  @param  Location       Pointer to EFI_CPU_PHYSICAL_LOCATION structure.
  @param  ThreadIdBits   Number of bits occupied by Thread ID portion.
  @param  CoreIdBits     Number of bits occupied by Core ID portion.

**/
VOID
ExtractProcessorLocation (
  IN  UINT32                    InitialApicId,
  OUT EFI_CPU_PHYSICAL_LOCATION *Location,
  OUT UINTN                     *ThreadIdBits,
  OUT UINTN                     *CoreIdBits
  );

//
// PURLEY_OVERRIDE_BEGIN
//
BOOLEAN  IsHsxCpu();
//
// PURLEY_OVERRIDE_END
//

//BDE Override for s5330552 start
/**
  Get Trace Hub Acpi Base address for BSP
**/
VOID
GetTraceHubAcpiBaseAddress (
  VOID
  );

/**
  Set Trace Hub Acpi Base address for AP
**/
VOID
SetTraceHubAcpiBaseAddress (
  UINTN      ProcessorNumber
  );
//BDE Override for s5330552 end
#endif
