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

#ifndef _MSARS_H_
#define _MSARS_H_

#include <Uefi/UefiBaseType.h>

#pragma pack(1)
typedef struct {
  UINT16 ProcessorDomainValid:1;
  UINT16 MemoryDomainValid:1;
  UINT16 ReservationHint:1;
  UINT16 Reserved_15_3:13;
} MSARS_FLAGS_BITS;

typedef union {
  UINT16 Value;
  MSARS_FLAGS_BITS Bits;
} MSARS_FLAGS;

typedef struct {
  UINT16 Type;
  UINT16 Reserved_2_4;
  UINT32 Length;
  MSARS_FLAGS Flags;
  UINT16 Reserved_10_12;
  UINT32 ProcessorProximityDomain;
  UINT32 MemoryProximityDomain;
  UINT32 Reserved_20_24;
  UINT64 AddrBase; //System Physical Address Range Base
  UINT64 AddrLength; //System Physical Address Range Length
} MEMORY_SUBSYSTEM_ADDRESS_RANGE_STRUCTURE;
#pragma pack()

// MSARS_INIT Macro
// Used to initialize MEMORY_SUBSYSTEM_ADDRESS_RANGE_STRUCTURE
#define MSARS_INIT() {                                   \
  (UINT16) MEMORY_SUBSYSTEM_ADDRESS_RANGE_STRUCTURE_TYPE,     \
  (UINT16) 0,       \
  (UINT32) sizeof(MEMORY_SUBSYSTEM_ADDRESS_RANGE_STRUCTURE),  \
  {(UINT16) 0},     \
  (UINT16) 0,   \
  (UINT32) 0,       \
  (UINT32) 0,       \
  (UINT32) 0,       \
  (UINT64) 0,       \
  (UINT64) 0,       \
  },

#endif /* _MSARS_H_ */
