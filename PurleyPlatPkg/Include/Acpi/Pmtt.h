//
// This file contains 'Framework Code' and is licensed as such
// under the terms of your license agreement with Intel or your
// vendor.  This file may not be modified, except as allowed by
// additional terms of your license agreement.
//
/*++

  Copyright (c) 2018 Intel Corporation.  All rights reserved.

  This software and associated documentation (if any) is furnished under
  a license and may only be used or copied in accordance with the terms
  of the license.  Except as permitted by such license, no part of this
  software or documentation may be reproduced, stored in a retrieval
  system, or transmitted in any form or by any means without the express
  written consent of Intel Corporation.

Abstract:

  This file describes the contents of the ACPI Memory Topology Table
  (MPST).  Some additional ACPI values are defined in Acpi1_0.h, Acpi2_0.h, and Acpi3_0.h
  All changes to the PMTT contents should be done in this file.

--*/

#ifndef _ACPI_PMTT_H_
#define _ACPI_PMTT_H_

//
// Includes
//
#include <IndustryStandard/Acpi.h>
#include <IndustryStandard/SmBios.h>
#include "Platform.h"

//
// PMTT Definitions
//
#define ACPI_PMTT_TABLE_SIGNATURE     0x54544D50
#define ACPI_PMTT_TABLE_REVISION      1
#define ACPI_PMTT_OEM_REVISION        1

#define TOP_LEVEL_AGGREGATOR_DEVICE                   0x0001
#define PHYSICAL_ELEMENT_OF_TOPOLOGY                  0x0002
#define AEP_DIMM                                      0x0004

#define EFI_ACPI_TOP_LEVEL_SOCKET                     0x00
#define EFI_ACPI_TOP_LEVEL_IMC                        0x01
#define PHYSICAL_COMPONENT_IDENTIFIER_TYPE_DIMM       0x02

typedef struct {
  UINT8                                     Type;
  UINT8                                     Rsvd;
  UINT16                                    Length;
  UINT16                                    Flag;
  UINT16                                    Rsvd1;
  UINT16                                    PhyCompIdentifier;
  UINT16                                    Rsvd2;
  UINT32                                    Size;
  UINT32                                    SmbiosHandle;
} ACPI_PMTT_DIMM_DEVICE;

typedef struct {
  UINT8                                     Type;
  UINT8                                     Rsvd;
  UINT16                                    Length;
  UINT16                                    Flag;
  UINT16                                    Rsvd1;
  UINT32                                    ReadLatency;
  UINT32                                    WriteLatency;
  UINT32                                    ReadBandwith;
  UINT32                                    WriteBandwith;
  UINT16                                    OptimalAccessUnit;
  UINT16                                    OptimalAccessAlignment;
  UINT16                                    Rsvd2;
  UINT16                                    NumProxDomains;
  UINT32                                    ProxDomain;      //  Proximity domain set to socket ID.
  ACPI_PMTT_DIMM_DEVICE                     Dimm[MAX_MC_CH * MAX_DIMM];
} ACPI_PMTT_IMC_DEVICE;

typedef struct {
  UINT8                                     Type;
  UINT8                                     Rsvd;
  UINT16                                    Length;
  UINT16                                    Flag;
  UINT16                                    Rsvd1;
  UINT16                                    SckIdent;
  UINT16                                    Rsvd2;
  ACPI_PMTT_IMC_DEVICE                      Imc[MAX_IMC];
} ACPI_PMTT_SOCKET_DEVICE;

typedef struct {
  EFI_ACPI_DESCRIPTION_HEADER               Header;
  UINT8                                     rsvd[4];
  ACPI_PMTT_SOCKET_DEVICE                   Socket[MAX_SOCKET];
} ACPI_PLATFORM_MEMORY_TOPOLOGY_TABLE;

#endif // _ACPI_PMTT_H_

