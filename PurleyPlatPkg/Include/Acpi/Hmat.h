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

#ifndef _HMAT_HMAT_H_
#define _HMAT_HMAT_H_

#include <IndustryStandard/Acpi.h>
#include "Platform.h"
#include "Msars.h"
//#include "Sllbis.h" // LBIS not supported in CR 1.0
#include "Mscis.h"

#define EFI_ACPI_HETEROGENEOUS_MEMORY_ATTRIBUTE_TABLE_REVISION 0x01
#define EFI_ACPI_OEM_HMAT_REVISION  0x00000001

#define EFI_HETEROGENEOUS_MEMORY_ATTRIBUTE_TABLE_SIGNATURE  SIGNATURE_32('H', 'M', 'A', 'T')
#define MEMORY_SUBSYSTEM_ADDRESS_RANGE_STRUCTURE_TYPE 0
#define SYSTEM_LOCALITY_LATENCY_BANDWIDTH_INFORMATION_STRUCTURE_TYPE 1
#define MEMORY_SIDE_CACHE_INFORMATION_STRUCTURE_TYPE 2

#define EFI_ACPI_HMAT_MSARS_COUNT  (MC_MAX_NODE * MAX_CRS_ENTRIES_PER_NODE)
#define EFI_ACPI_HMAT_LBIS_COUNT   0 // LBIS not supported in CR 1.0
#define EFI_ACPI_HMAT_MSCIS_COUNT  (MC_MAX_NODE * MAX_CRS_ENTRIES_PER_NODE)

#define EFI_ACPI_HMAT_NUMBER_OF_PROCESSOR_DOMAINS MC_MAX_NODE
#define EFI_ACPI_HMAT_NUMBER_OF_MEMORY_DOMAINS    (MC_MAX_NODE * MAX_CRS_ENTRIES_PER_NODE)

#pragma pack(1)
typedef struct {
  UINT64  MemMapIndexMap;
  UINT8   Valid;
  UINT8   Cacheable;
  UINT64  MemorySideCacheSize;
} MEMORY_DOMAIN_LIST_INFO;

typedef struct {
  UINT32                   ProcessorDomainNumber;
  UINT32                   MemoryDomainNumber;
  UINT16                   ProcessorDomainList [EFI_ACPI_HMAT_NUMBER_OF_PROCESSOR_DOMAINS];
  MEMORY_DOMAIN_LIST_INFO  MemoryDomainList [EFI_ACPI_HMAT_NUMBER_OF_MEMORY_DOMAINS];
  UINT8                    SncEnabled2Clusters;
} HMAT_PROXIMITY_DOMAIN_DATA_STRUCTURE;


typedef struct {
  EFI_ACPI_DESCRIPTION_HEADER Header;
  UINT32 Reserved; // To make the structures 8 byte aligned
} EFI_ACPI_HETEROGENEOUS_MEMORY_ATTRIBUTE_TABLE_HEADER;

typedef struct {
  EFI_ACPI_HETEROGENEOUS_MEMORY_ATTRIBUTE_TABLE_HEADER HmatHeader;
  MEMORY_SUBSYSTEM_ADDRESS_RANGE_STRUCTURE Msars[EFI_ACPI_HMAT_MSARS_COUNT];
  //LATENCY_BANDWIDTH_INFO_STRUCTURE Lbis[EFI_ACPI_HMAT_LBIS_COUNT];  // LBIS not supported in CR 1.0
  MEMORY_SIDE_CACHE_INFORMATION_STRUCTURE MemSideCache[EFI_ACPI_HMAT_MSCIS_COUNT];
} EFI_ACPI_HETEROGENEOUS_MEMORY_ATTRIBUTE_TABLE;
#pragma pack()

#endif /* _HMAT_H_ */
