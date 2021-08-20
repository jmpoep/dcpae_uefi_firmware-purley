/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2018 Intel Corporation. <BR>

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary    and
  confidential information of Intel Corporation and its suppliers and licensors,
  and is protected by worldwide copyright and trade secret laws and treaty
  provisions. No part of the Material may be used, copied, reproduced, modified,
  published, uploaded, posted, transmitted, distributed, or disclosed in any way
  without Intel's prior express written permission.

  No license under any patent, copyright, trade secret or other intellectual
  property right is granted to or conferred upon you by disclosure or delivery
  of the Materials, either expressly, by implication, inducement, estoppel or
  otherwise. Any license under such intellectual property rights must be
  express and approved by Intel in writing.

  Unless otherwise agreed by Intel in writing, you may not remove or alter
  this notice or any other notice embedded in Materials by Intel or
  Intel's suppliers or licensors in any way.
**/

#ifndef _MSCIS_H_
#define _MSCIS_H_

#include <Uefi/UefiBaseType.h>

#define EFI_ACPI_HMAT_CACHE_LEVEL_NONE  0
#define EFI_ACPI_HMAT_ONE_LEVEL_CACHE   1
#define EFI_ACPI_HMAT_TWO_LEVEL_CACHE   2
#define EFI_ACPI_HMAT_THREE_LEVEL_CACHE 3

#define EFI_ACPI_HMAT_CACHE_ASSOCIATIVITY_NONE                    0
#define EFI_ACPI_HMAT_CACHE_ASSOCIATIVITY_DIRECT_MAPPED           1
#define EFI_ACPI_HMAT_CACHE_ASSOCIATIVITY_COIMPLEX_CACHE_INDEXING 2

#define EFI_ACPI_HMAT_WRITE_POLICY_NONE 0
#define EFI_ACPI_HMAT_WRITE_POLICY_WB   1
#define EFI_ACPI_HMAT_WRITE_POLICY_WT   2

#define DDR4_CACHE_LINE_SIZE  64

#pragma pack(1)
typedef struct {
  UINT32 TotalCacheLevels:4;
  UINT32 CacheLevel:4;
  UINT32 CacheAssociativity:4;
  UINT32 WritePolicy:4;
  UINT32 CacheLineSize:16;
} CACHE_ATTRIBUTES_BITS;

typedef union {
  UINT32 Data;
  CACHE_ATTRIBUTES_BITS Bits;
} HMAT_CACHE_ATTRIBUTES;

typedef struct {
  UINT16 Type;
  UINT16 Reserved_2_4;
  UINT32 Length;
  UINT32 MemoryProximityDomain;
  UINT32 Reserved_12_16;
  UINT64 MemorySideCacheSize;
  HMAT_CACHE_ATTRIBUTES CacheAttributes;
  UINT16 Reserved_28_30;
  UINT16 NumSmbiosHandles;
} MEMORY_SIDE_CACHE_INFORMATION_STRUCTURE;
#pragma pack()

// MSCIS_INIT Macro
// Used to initialize MEMORY_SIDE_CACHE_INFORMATION_STRUCTURE
#define MSCIS_INIT() {                                       \
  (UINT16) MEMORY_SIDE_CACHE_INFORMATION_STRUCTURE_TYPE,     \
  (UINT16) 0,   \
  (UINT32) sizeof(MEMORY_SIDE_CACHE_INFORMATION_STRUCTURE),  \
  (UINT32) 0,   \
  (UINT32) 0,   \
  (UINT64) 0,   \
  (UINT32) 0,   \
  (UINT16) 0,   \
  (UINT16) 0,   \
  },

#endif /* _MSCIS_H_ */
