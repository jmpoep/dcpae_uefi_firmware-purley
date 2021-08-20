/** @file
    SklPchLp Ax HSIO C File

@copyright
 Copyright (c) 2014 Intel Corporation. All rights reserved
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
#include <PchAccess.h>
#include <IncludePrivate/PchHsio.h>

UINT8 PchLpChipsetInitTable_Ax_Mipi_A[] = {
  0x9B, 0x92, //CRC-16
  0x22,       //Version
  0x80,       //Product/Stepping
  // Phy Data Group Table (Fixed table with 16 Entries)
  0x00, 0x00,
  0x01, 0x03,
  0x0C, 0x54,
  0x0C, 0xF5,
  0x00, 0x0F,
  0x00, 0x00,
  0x00, 0x00,
  0x00, 0x00,
  0x00, 0x00,
  0x00, 0x00,
  0x00, 0x00,
  0x00, 0x00,
  0x00, 0x00,
  0x00, 0x00,
  0x00, 0x00,
  0x00, 0x00,
  // HSIO Command Table
  0x28, 0xE9, 0x07, 0x00, 0x0F, 0x00, 0x08, 0x81, 0x1C, 0x01, 0x1F, 0x00, //  {0xE9, 0x001F011C, 0x8108, PCH_LANE_OWN_COMMON, LANE},
  0x28, 0xE9, 0x07, 0x00, 0x0F, 0x00, 0x10, 0x81, 0x04, 0x08, 0x01, 0x08, //  {0xE9, 0x08010804, 0x8110, PCH_LANE_OWN_COMMON, LANE},
  0x28, 0xE9, 0x07, 0x00, 0x0F, 0x00, 0x28, 0x81, 0x12, 0x00, 0x0F, 0x00, //  {0xE9, 0x000F0012, 0x8128, PCH_LANE_OWN_COMMON, LANE},
  0x28, 0xE9, 0x07, 0x00, 0x0F, 0x00, 0x2C, 0x81, 0x0A, 0x03, 0x00, 0x00, //  {0xE9, 0x0000030A, 0x812C, PCH_LANE_OWN_COMMON, LANE},
  0x28, 0xE9, 0x07, 0x00, 0x0F, 0x00, 0x30, 0x81, 0x00, 0x3C, 0x00, 0x78, //  {0xE9, 0x78003C00, 0x8130, PCH_LANE_OWN_COMMON, LANE},
  0x0F, 0x00, 0x00, 0x00,                                                 //  End Table Marker
  0x28, 0xA9, 0x07, 0x00, 0x0F, 0x00, 0x40, 0x83, 0x58, 0xCF, 0x05, 0x01, //  {0xA9, 0x0105CF58, 0x8340, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI, MULCAST},
  0x28, 0xA9, 0x07, 0x00, 0x0F, 0x00, 0x44, 0x83, 0x48, 0xD5, 0x05, 0x01, //  {0xA9, 0x0105D548, 0x8344, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI, MULCAST},
  0x28, 0xA9, 0x07, 0x00, 0x0F, 0x00, 0x94, 0x82, 0x06, 0x2A, 0x29, 0x28, //  {0xA9, 0x28292A06, 0x8294, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI, MULCAST},
  0x28, 0xA9, 0x07, 0x00, 0x0F, 0x00, 0x70, 0x83, 0x5A, 0x25, 0x40, 0x33, //  {0xA9, 0x3340255A, 0x8370, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI, MULCAST},
  0x28, 0xA9, 0x07, 0x00, 0x0F, 0x00, 0x18, 0x83, 0x3F, 0x40, 0x00, 0x00, //  {0xA9, 0x0000403F, 0x8318, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI, MULCAST},
  0x28, 0xA9, 0x07, 0x00, 0x0F, 0x00, 0x78, 0x83, 0x60, 0x40, 0x83, 0x40, //  {0xA9, 0x40834060, 0x8378, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI, MULCAST},
  0x28, 0xA9, 0x07, 0x00, 0x0F, 0x00, 0x04, 0x82, 0x64, 0x00, 0x00, 0x00, //  {0xA9, 0x00000064, 0x8204, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI, MULCAST},
  0x28, 0xA9, 0x07, 0x00, 0x0F, 0x00, 0x8C, 0x83, 0x58, 0x88, 0x06, 0x00, //  {0xA9, 0x00068858, 0x838C, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI, MULCAST},
  0x28, 0xA9, 0x07, 0x00, 0x0F, 0x00, 0xD8, 0x82, 0xFF, 0x0C, 0x40, 0x80, //  {0xA9, 0x80400CFF, 0x82D8, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI, MULCAST},
  0x28, 0xA9, 0x07, 0x00, 0x0F, 0x00, 0x80, 0x83, 0x60, 0x07, 0xA0, 0x08, //  {0xA9, 0x08A00760, 0x8380, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI, MULCAST},
  0x28, 0xA9, 0x07, 0x00, 0x0F, 0x00, 0x00, 0x83, 0x10, 0x50, 0x52, 0x09, //  {0xA9, 0x09525010, 0x8300, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI, MULCAST},
  0x28, 0xA9, 0x07, 0x00, 0x0F, 0x00, 0x0C, 0x83, 0x1F, 0x9B, 0xD2, 0x00, //  {0xA9, 0x00D29B1F, 0x830C, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI, MULCAST},
  0x28, 0xA9, 0x07, 0x00, 0x0F, 0x00, 0x84, 0x83, 0x00, 0x40, 0x17, 0xE0, //  {0xA9, 0xE0174000, 0x8384, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI, MULCAST},
  0x28, 0xEA, 0x07, 0x00, 0x0F, 0x00, 0x40, 0x83, 0x58, 0xCF, 0x05, 0x01, //  {0xEA, 0x0105CF58, 0x8340, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI, MULCAST},
  0x28, 0xEA, 0x07, 0x00, 0x0F, 0x00, 0x44, 0x83, 0x48, 0xD5, 0x05, 0x01, //  {0xEA, 0x0105D548, 0x8344, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI, MULCAST},
  0x28, 0xEA, 0x07, 0x00, 0x0F, 0x00, 0x94, 0x88, 0x02, 0x2A, 0x29, 0x28, //  {0xEA, 0x28292A02, 0x8894, V_PCH_PCR_FIA_LANE_OWN_USB3, MULCAST},
  0x28, 0xEA, 0x07, 0x00, 0x0F, 0x00, 0x94, 0x82, 0x06, 0x2A, 0x29, 0x28, //  {0xEA, 0x28292A06, 0x8294, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI, MULCAST},
  0x28, 0xEA, 0x07, 0x00, 0x0F, 0x00, 0xA0, 0x89, 0xEC, 0x10, 0x00, 0x30, //  {0xEA, 0x300010EC, 0x89A0, V_PCH_PCR_FIA_LANE_OWN_USB3, MULCAST},
  0x28, 0xEA, 0x07, 0x00, 0x0F, 0x00, 0xA4, 0x89, 0x30, 0x30, 0x30, 0x00, //  {0xEA, 0x00303030, 0x89A4, V_PCH_PCR_FIA_LANE_OWN_USB3, MULCAST},
  0x28, 0xEA, 0x07, 0x00, 0x0F, 0x00, 0x40, 0x89, 0x4F, 0xC9, 0x1A, 0x03, //  {0xEA, 0x031AC94F, 0x8940, V_PCH_PCR_FIA_LANE_OWN_USB3, MULCAST},
  0x28, 0xEA, 0x07, 0x00, 0x0F, 0x00, 0x14, 0x89, 0x1F, 0xC3, 0x30, 0x03, //  {0xEA, 0x0330C31F, 0x8914, V_PCH_PCR_FIA_LANE_OWN_USB3, MULCAST},
  0x28, 0xEA, 0x07, 0x00, 0x0F, 0x00, 0x70, 0x83, 0x5A, 0x25, 0x40, 0x33, //  {0xEA, 0x3340255A, 0x8370, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI, MULCAST},
  0x28, 0xEA, 0x07, 0x00, 0x0F, 0x00, 0x70, 0x89, 0x5A, 0x25, 0x40, 0x33, //  {0xEA, 0x3340255A, 0x8970, V_PCH_PCR_FIA_LANE_OWN_USB3, MULCAST},
  0x28, 0xEA, 0x07, 0x00, 0x0F, 0x00, 0x18, 0x89, 0x3F, 0x00, 0x00, 0x00, //  {0xEA, 0x0000003F, 0x8918, V_PCH_PCR_FIA_LANE_OWN_USB3, MULCAST},
  0x28, 0xEA, 0x07, 0x00, 0x0F, 0x00, 0x18, 0x83, 0x3F, 0x40, 0x00, 0x00, //  {0xEA, 0x0000403F, 0x8318, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI, MULCAST},
  0x28, 0xEA, 0x07, 0x00, 0x0F, 0x00, 0x78, 0x83, 0x60, 0x40, 0x83, 0x40, //  {0xEA, 0x40834060, 0x8378, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI, MULCAST},
  0x28, 0xEA, 0x07, 0x00, 0x0F, 0x00, 0x78, 0x89, 0x60, 0x40, 0x83, 0x40, //  {0xEA, 0x40834060, 0x8978, V_PCH_PCR_FIA_LANE_OWN_USB3, MULCAST},
  0x28, 0xEA, 0x07, 0x00, 0x0F, 0x00, 0x04, 0x89, 0x89, 0x09, 0x55, 0x00, //  {0xEA, 0x00550989, 0x8904, V_PCH_PCR_FIA_LANE_OWN_USB3, MULCAST},
  0x28, 0xEA, 0x07, 0x00, 0x0F, 0x00, 0xC8, 0x89, 0x50, 0x88, 0x4D, 0x60, //  {0xEA, 0x604D8850, 0x89C8, V_PCH_PCR_FIA_LANE_OWN_USB3, MULCAST},
  0x28, 0xEA, 0x07, 0x00, 0x0F, 0x00, 0x04, 0x82, 0x64, 0x00, 0x00, 0x00, //  {0xEA, 0x00000064, 0x8204, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI, MULCAST},
  0x28, 0xEA, 0x07, 0x00, 0x0F, 0x00, 0x8C, 0x89, 0x28, 0x80, 0x06, 0x00, //  {0xEA, 0x00068028, 0x898C, V_PCH_PCR_FIA_LANE_OWN_USB3, MULCAST},
  0x28, 0xEA, 0x07, 0x00, 0x0F, 0x00, 0x8C, 0x83, 0x58, 0x88, 0x06, 0x00, //  {0xEA, 0x00068858, 0x838C, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI, MULCAST},
  0x28, 0xEA, 0x07, 0x00, 0x0F, 0x00, 0xE0, 0x89, 0x02, 0x00, 0x00, 0x08, //  {0xEA, 0x08000002, 0x89E0, V_PCH_PCR_FIA_LANE_OWN_USB3, MULCAST},
  0x28, 0xEA, 0x07, 0x00, 0x0F, 0x00, 0xD8, 0x82, 0xFF, 0x0C, 0x40, 0x80, //  {0xEA, 0x80400CFF, 0x82D8, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI, MULCAST},
  0x28, 0xEA, 0x07, 0x00, 0x0F, 0x00, 0xD8, 0x88, 0xFF, 0x0C, 0x40, 0x80, //  {0xEA, 0x80400CFF, 0x88D8, V_PCH_PCR_FIA_LANE_OWN_USB3, MULCAST},
  0x28, 0xEA, 0x07, 0x00, 0x0F, 0x00, 0x24, 0x88, 0x0F, 0x0C, 0x0C, 0x84, //  {0xEA, 0x840C0C0F, 0x8824, V_PCH_PCR_FIA_LANE_OWN_USB3, MULCAST},
  0x28, 0xEA, 0x07, 0x00, 0x0F, 0x00, 0x80, 0x83, 0x60, 0x07, 0xA0, 0x08, //  {0xEA, 0x08A00760, 0x8380, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI, MULCAST},
  0x28, 0xEA, 0x07, 0x00, 0x0F, 0x00, 0x00, 0x89, 0x10, 0x40, 0x52, 0x09, //  {0xEA, 0x09524010, 0x8900, V_PCH_PCR_FIA_LANE_OWN_USB3, MULCAST},
  0x28, 0xEA, 0x07, 0x00, 0x0F, 0x00, 0x00, 0x83, 0x10, 0x50, 0x52, 0x09, //  {0xEA, 0x09525010, 0x8300, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI, MULCAST},
  0x28, 0xEA, 0x07, 0x00, 0x0F, 0x00, 0x2C, 0x89, 0x0A, 0x10, 0x0A, 0x00, //  {0xEA, 0x000A100A, 0x892C, V_PCH_PCR_FIA_LANE_OWN_USB3, MULCAST},
  0x28, 0xEA, 0x07, 0x00, 0x0F, 0x00, 0x9C, 0x89, 0x00, 0x00, 0xB0, 0x00, //  {0xEA, 0x00B00000, 0x899C, V_PCH_PCR_FIA_LANE_OWN_USB3, MULCAST},
  0x28, 0xEA, 0x07, 0x00, 0x0F, 0x00, 0x20, 0x88, 0x26, 0x16, 0x07, 0x0D, //  {0xEA, 0x0D071626, 0x8820, V_PCH_PCR_FIA_LANE_OWN_USB3, MULCAST},
  0x28, 0xEA, 0x07, 0x00, 0x0F, 0x00, 0x0C, 0x83, 0x1F, 0x9B, 0xD2, 0x00, //  {0xEA, 0x00D29B1F, 0x830C, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI, MULCAST},
  0x28, 0xEA, 0x07, 0x00, 0x0F, 0x00, 0x3C, 0x89, 0xE0, 0x00, 0x00, 0x00, //  {0xEA, 0x000000E0, 0x893C, V_PCH_PCR_FIA_LANE_OWN_USB3, MULCAST},
  0x28, 0xEA, 0x07, 0x00, 0x0F, 0x00, 0x84, 0x83, 0x00, 0x40, 0x17, 0xE0, //  {0xEA, 0xE0174000, 0x8384, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI, MULCAST},
  0x28, 0xEA, 0x07, 0x00, 0x0F, 0x00, 0x84, 0x89, 0x00, 0x40, 0x17, 0xE0, //  {0xEA, 0xE0174000, 0x8984, V_PCH_PCR_FIA_LANE_OWN_USB3, MULCAST},
  0x28, 0xEA, 0x07, 0x00, 0x0F, 0x00, 0x80, 0x89, 0x60, 0x07, 0xE8, 0x00, //  {0xEA, 0x00E80760, 0x8980, V_PCH_PCR_FIA_LANE_OWN_USB3, MULCAST},
  0x28, 0xE9, 0x07, 0x00, 0x0F, 0x00, 0x40, 0x83, 0x58, 0xCF, 0x05, 0x01, //  {0xE9, 0x0105CF58, 0x8340, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI, MULCAST},
  0x28, 0xE9, 0x07, 0x00, 0x0F, 0x00, 0x44, 0x83, 0x48, 0xD5, 0x05, 0x01, //  {0xE9, 0x0105D548, 0x8344, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI, MULCAST},
  0x28, 0xE9, 0x07, 0x00, 0x0F, 0x00, 0x94, 0x82, 0x06, 0x2A, 0x29, 0x28, //  {0xE9, 0x28292A06, 0x8294, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI, MULCAST},
  0x28, 0xE9, 0x07, 0x00, 0x0F, 0x00, 0x70, 0x83, 0x5A, 0x25, 0x40, 0x33, //  {0xE9, 0x3340255A, 0x8370, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI, MULCAST},
  0x28, 0xE9, 0x07, 0x00, 0x0F, 0x00, 0x18, 0x83, 0x3F, 0x40, 0x00, 0x00, //  {0xE9, 0x0000403F, 0x8318, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI, MULCAST},
  0x28, 0xE9, 0x07, 0x00, 0x0F, 0x00, 0x78, 0x83, 0x60, 0x40, 0x83, 0x40, //  {0xE9, 0x40834060, 0x8378, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI, MULCAST},
  0x28, 0xE9, 0x07, 0x00, 0x0F, 0x00, 0x04, 0x82, 0x64, 0x00, 0x00, 0x00, //  {0xE9, 0x00000064, 0x8204, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI, MULCAST},
  0x28, 0xE9, 0x07, 0x00, 0x0F, 0x00, 0x8C, 0x83, 0x58, 0x88, 0x06, 0x00, //  {0xE9, 0x00068858, 0x838C, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI, MULCAST},
  0x28, 0xE9, 0x07, 0x00, 0x0F, 0x00, 0xD8, 0x82, 0xFF, 0x0C, 0x40, 0x80, //  {0xE9, 0x80400CFF, 0x82D8, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI, MULCAST},
  0x28, 0xE9, 0x07, 0x00, 0x0F, 0x00, 0x80, 0x83, 0x60, 0x07, 0xA0, 0x08, //  {0xE9, 0x08A00760, 0x8380, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI, MULCAST},
  0x28, 0xE9, 0x07, 0x00, 0x0F, 0x00, 0x00, 0x83, 0x10, 0x50, 0x52, 0x09, //  {0xE9, 0x09525010, 0x8300, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI, MULCAST},
  0x28, 0xE9, 0x07, 0x00, 0x0F, 0x00, 0x0C, 0x83, 0x1F, 0x9B, 0xD2, 0x00, //  {0xE9, 0x00D29B1F, 0x830C, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI, MULCAST},
  0x28, 0xE9, 0x07, 0x00, 0x0F, 0x00, 0x84, 0x83, 0x00, 0x40, 0x17, 0xE0, //  {0xE9, 0xE0174000, 0x8384, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI, MULCAST},
  0x0F, 0x00, 0x00, 0x00,                                                 //  End Table Marker
  0x29, 0xEC, 0x07, 0x00, 0x0F, 0x00, 0x44, 0x03, 0xF6, 0x4C, 0x1C, 0x30, //  {0xEC, 0x301C4CF6, 0x344, OPIPHY, NA, LANE},
  0x29, 0xEC, 0x07, 0x00, 0x0F, 0x00, 0x48, 0x03, 0x00, 0xE0, 0xBF, 0x1F, //  {0xEC, 0x1FBFE000, 0x348, OPIPHY, NA, LANE},
  0x29, 0xEC, 0x07, 0x00, 0x0F, 0x00, 0x40, 0x03, 0x00, 0x00, 0x00, 0x14, //  {0xEC, 0x14000000, 0x340, OPIPHY, NA, LANE},
  0x0F, 0x00, 0x00, 0x00,                                                 //  End Table Marker
  0x28, 0xAE, 0x07, 0x00, 0x0F, 0x00, 0x54, 0x03, 0x00, 0x00, 0xC0, 0x06, //  {0xAE, 0x06C00000, 0x354, GPIOPHY, NA, LANE},
  0x0F, 0x00, 0x00, 0x00,                                                 //  End Table Marker
};

PCH_SBI_HSIO_TABLE_STRUCT PchLpHsio_Ax_Mipi_A[] = {
  {0xA8, 0x00300401, 0x8030, PCH_LANE_OWN_COMMON}, //LANE
  {0xA8, 0x00000006, 0x803C, PCH_LANE_OWN_COMMON}, //LANE
  {0xA8, 0x0A030804, 0x8110, PCH_LANE_OWN_COMMON}, //LANE
  {0xA8, 0x001F000A, 0x8128, PCH_LANE_OWN_COMMON}, //LANE
  {0xA8, 0x0000030B, 0x812C, PCH_LANE_OWN_COMMON}, //LANE
  {0xA8, 0x78003C00, 0x8130, PCH_LANE_OWN_COMMON}, //LANE
  {0xA9, 0x00300401, 0x8030, PCH_LANE_OWN_COMMON}, //LANE
  {0xA9, 0x00000006, 0x803C, PCH_LANE_OWN_COMMON}, //LANE
  {0xA9, 0x0A010601, 0x8110, PCH_LANE_OWN_COMMON}, //LANE
  {0xA9, 0x001F0006, 0x8128, PCH_LANE_OWN_COMMON}, //LANE
  {0xA9, 0x0000030B, 0x812C, PCH_LANE_OWN_COMMON}, //LANE
  {0xA9, 0x78003C00, 0x8130, PCH_LANE_OWN_COMMON}, //LANE
  {0xE9, 0x00300401, 0x8030, PCH_LANE_OWN_COMMON}, //LANE
  {0xE9, 0x00000006, 0x803C, PCH_LANE_OWN_COMMON}, //LANE
  {0xEA, 0x00300401, 0x8030, PCH_LANE_OWN_COMMON}, //LANE
  {0xEA, 0x00000006, 0x803C, PCH_LANE_OWN_COMMON}, //LANE
  {0xEA, 0x0000010B, 0x812C, PCH_LANE_OWN_COMMON}, //LANE
  {0xEA, 0x78003C00, 0x8130, PCH_LANE_OWN_COMMON}, //LANE
  {0xEA, 0x010201E0, 0xC028, PCH_LANE_BDCAST}, //BDCAST
  {0xEA, 0x4B030540, 0xC1E4, PCH_LANE_BDCAST}, //BDCAST
  {0xEA, 0xA40B0A10, 0xC024, PCH_LANE_BDCAST}, //BDCAST
  {0xE9, 0x010201E0, 0xC028, PCH_LANE_BDCAST}, //BDCAST
  {0xE9, 0x4B030540, 0xC1E4, PCH_LANE_BDCAST}, //BDCAST
  {0xE9, 0xA40B0A10, 0xC024, PCH_LANE_BDCAST}, //BDCAST
  {0xA9, 0x010201E0, 0xC028, PCH_LANE_BDCAST}, //BDCAST
  {0xA9, 0x4B030540, 0xC1E4, PCH_LANE_BDCAST}, //BDCAST
  {0xA9, 0xA40B0A10, 0xC024, PCH_LANE_BDCAST}, //BDCAST
  {0xA9, 0x01051158, 0x8B40, V_PCH_PCR_FIA_LANE_OWN_SATA}, //MULCAST
  {0xA9, 0x01051548, 0x8B44, V_PCH_PCR_FIA_LANE_OWN_SATA}, //MULCAST
  {0xA9, 0x0105154C, 0x8B48, V_PCH_PCR_FIA_LANE_OWN_SATA}, //MULCAST
  {0xA9, 0x0105194C, 0x8748, V_PCH_PCR_FIA_LANE_OWN_GBE}, //MULCAST
  {0xA9, 0x0105D35A, 0x834C, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI}, //MULCAST
  {0xA9, 0x0132A009, 0x8B58, V_PCH_PCR_FIA_LANE_OWN_SATA}, //MULCAST
  {0xA9, 0x151F2800, 0x8A98, V_PCH_PCR_FIA_LANE_OWN_SATA}, //MULCAST
  {0xA9, 0x24743020, 0x8388, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI}, //MULCAST
  {0xA9, 0x282D2D06, 0x8A94, V_PCH_PCR_FIA_LANE_OWN_SATA}, //MULCAST
  {0xA9, 0x3340255A, 0x8770, V_PCH_PCR_FIA_LANE_OWN_GBE}, //MULCAST
  {0xA9, 0x3340255A, 0x8B70, V_PCH_PCR_FIA_LANE_OWN_SATA}, //MULCAST
  {0xA9, 0x00384008, 0x8B1C, V_PCH_PCR_FIA_LANE_OWN_SATA}, //MULCAST
  {0xA9, 0x04000045, 0x8A04, V_PCH_PCR_FIA_LANE_OWN_SATA}, //MULCAST
  {0xA9, 0x40834060, 0x8778, V_PCH_PCR_FIA_LANE_OWN_GBE}, //MULCAST
  {0xA9, 0x40834060, 0x8B78, V_PCH_PCR_FIA_LANE_OWN_SATA}, //MULCAST
  {0xA9, 0x043F3F3F, 0x8350, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI}, //MULCAST
  {0xA9, 0x053F3F3F, 0x8B50, V_PCH_PCR_FIA_LANE_OWN_SATA}, //MULCAST
  {0xA9, 0x00006000, 0x8B20, V_PCH_PCR_FIA_LANE_OWN_SATA}, //MULCAST
  {0xA9, 0x80400CFF, 0x86D8, V_PCH_PCR_FIA_LANE_OWN_GBE}, //MULCAST
  {0xA9, 0x80400CFF, 0x8AD8, V_PCH_PCR_FIA_LANE_OWN_SATA}, //MULCAST
  {0xA9, 0x80C00002, 0x8354, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI}, //MULCAST
  {0xA9, 0x80C00200, 0x8754, V_PCH_PCR_FIA_LANE_OWN_GBE}, //MULCAST
  {0xA9, 0x80C00205, 0x8B54, V_PCH_PCR_FIA_LANE_OWN_SATA}, //MULCAST
  {0xA9, 0x00868058, 0x878C, V_PCH_PCR_FIA_LANE_OWN_GBE}, //MULCAST
  {0xA9, 0x00868858, 0x8B8C, V_PCH_PCR_FIA_LANE_OWN_SATA}, //MULCAST
  {0xA9, 0x0008887C, 0x838C, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI}, //MULCAST
  {0xA9, 0x09521410, 0x8700, V_PCH_PCR_FIA_LANE_OWN_GBE}, //MULCAST
  {0xA9, 0x09525110, 0x8300, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI}, //MULCAST
  {0xA9, 0x09525410, 0x8B00, V_PCH_PCR_FIA_LANE_OWN_SATA}, //MULCAST
  {0xA9, 0x000A100A, 0x832C, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI}, //MULCAST
  {0xA9, 0x00B00000, 0x839C, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI}, //MULCAST
  {0xA9, 0x000000BF, 0x8B18, V_PCH_PCR_FIA_LANE_OWN_SATA}, //MULCAST
  {0xA9, 0x000000E0, 0x833C, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI}, //MULCAST
  {0xA9, 0xE0174000, 0x8784, V_PCH_PCR_FIA_LANE_OWN_GBE}, //MULCAST
  {0xA9, 0xE0174000, 0x8B84, V_PCH_PCR_FIA_LANE_OWN_SATA}, //MULCAST
  {0xA9, 0xFF00FF00, 0x8240, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI}, //MULCAST
  {0xA9, 0xFFE01800, 0x8308, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI}, //MULCAST
  {0xEA, 0x0105174C, 0x8D44, PCH_LANE_OWN_MIPI_A}, //MULCAST
  {0xEA, 0x0105194C, 0x8D48, PCH_LANE_OWN_MIPI_A}, //MULCAST
  {0xEA, 0x0105D35A, 0x834C, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI}, //MULCAST
  {0xEA, 0x1F1F0000, 0x885C, V_PCH_PCR_FIA_LANE_OWN_USB3}, //MULCAST
  {0xEA, 0x00001FDD, 0x8DC4, PCH_LANE_OWN_MIPI_A}, //MULCAST
  {0xEA, 0x24743020, 0x8388, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI}, //MULCAST
  {0xEA, 0x3340255A, 0x8D70, PCH_LANE_OWN_MIPI_A}, //MULCAST
  {0xEA, 0x40834060, 0x8D78, PCH_LANE_OWN_MIPI_A}, //MULCAST
  {0xEA, 0x043F3F3F, 0x8350, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI}, //MULCAST
  {0xEA, 0x80400CFF, 0x8CD8, PCH_LANE_OWN_MIPI_A}, //MULCAST
  {0xEA, 0x80C00002, 0x8354, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI}, //MULCAST
  {0xEA, 0x80C0020A, 0x8D54, PCH_LANE_OWN_MIPI_A}, //MULCAST
  {0xEA, 0x00868858, 0x8D8C, PCH_LANE_OWN_MIPI_A}, //MULCAST
  {0xEA, 0x0008887C, 0x838C, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI}, //MULCAST
  {0xEA, 0x09521410, 0x8D00, PCH_LANE_OWN_MIPI_A}, //MULCAST
  {0xEA, 0x09525110, 0x8300, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI}, //MULCAST
  {0xEA, 0x000A100A, 0x832C, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI}, //MULCAST
  {0xEA, 0xA40C0C0F, 0x8824, V_PCH_PCR_FIA_LANE_OWN_USB3}, //MULCAST
  {0xEA, 0x00B00000, 0x839C, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI}, //MULCAST
  {0xEA, 0x000000E0, 0x833C, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI}, //MULCAST
  {0xEA, 0xE0174000, 0x8D84, PCH_LANE_OWN_MIPI_A}, //MULCAST
  {0xEA, 0xFF00FF00, 0x8240, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI}, //MULCAST
  {0xEA, 0xFFE01800, 0x8308, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI}, //MULCAST
  {0xE9, 0x0105194C, 0x8748, V_PCH_PCR_FIA_LANE_OWN_GBE}, //MULCAST
  {0xE9, 0x0105D35A, 0x834C, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI}, //MULCAST
  {0xE9, 0x24743020, 0x8388, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI}, //MULCAST
  {0xE9, 0x3340255A, 0x8770, V_PCH_PCR_FIA_LANE_OWN_GBE}, //MULCAST
  {0xE9, 0x40834060, 0x8778, V_PCH_PCR_FIA_LANE_OWN_GBE}, //MULCAST
  {0xE9, 0x043F3F3F, 0x8350, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI}, //MULCAST
  {0xE9, 0x80400CFF, 0x86D8, V_PCH_PCR_FIA_LANE_OWN_GBE}, //MULCAST
  {0xE9, 0x80C00002, 0x8354, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI}, //MULCAST
  {0xE9, 0x80C00200, 0x8754, V_PCH_PCR_FIA_LANE_OWN_GBE}, //MULCAST
  {0xE9, 0x00868058, 0x878C, V_PCH_PCR_FIA_LANE_OWN_GBE}, //MULCAST
  {0xE9, 0x0008887C, 0x838C, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI}, //MULCAST
  {0xE9, 0x09521410, 0x8700, V_PCH_PCR_FIA_LANE_OWN_GBE}, //MULCAST
  {0xE9, 0x09525110, 0x8300, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI}, //MULCAST
  {0xE9, 0x000A100A, 0x832C, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI}, //MULCAST
  {0xE9, 0x00B00000, 0x839C, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI}, //MULCAST
  {0xE9, 0x000000E0, 0x833C, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI}, //MULCAST
  {0xE9, 0xE0174000, 0x8784, V_PCH_PCR_FIA_LANE_OWN_GBE}, //MULCAST
  {0xE9, 0xFF00FF00, 0x8240, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI}, //MULCAST
  {0xE9, 0xFFE01800, 0x8308, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI}, //MULCAST
};

UINT8 PchLpChipsetInitTable_Ax_Mipi_B[] = {
  0xBE, 0x3A, //CRC-16
  0x22,       //Version
  0x80,       //Product/Stepping
  // Phy Data Group Table (Fixed table with 16 Entries)
  0x00, 0x00,
  0x01, 0x03,
  0x0C, 0x54,
  0x0C, 0xF5,
  0x00, 0x0F,
  0x00, 0x00,
  0x00, 0x00,
  0x00, 0x00,
  0x00, 0x00,
  0x00, 0x00,
  0x00, 0x00,
  0x00, 0x00,
  0x00, 0x00,
  0x00, 0x00,
  0x00, 0x00,
  0x00, 0x00,
  // HSIO Command Table
  0x28, 0xE9, 0x07, 0x00, 0x0F, 0x00, 0x08, 0x81, 0x1C, 0x01, 0x1F, 0x00, //  {0xE9, 0x001F011C, 0x8108, PCH_LANE_OWN_COMMON, LANE},
  0x28, 0xE9, 0x07, 0x00, 0x0F, 0x00, 0x10, 0x81, 0x04, 0x08, 0x01, 0x08, //  {0xE9, 0x08010804, 0x8110, PCH_LANE_OWN_COMMON, LANE},
  0x28, 0xE9, 0x07, 0x00, 0x0F, 0x00, 0x28, 0x81, 0x12, 0x00, 0x0F, 0x00, //  {0xE9, 0x000F0012, 0x8128, PCH_LANE_OWN_COMMON, LANE},
  0x28, 0xE9, 0x07, 0x00, 0x0F, 0x00, 0x2C, 0x81, 0x0A, 0x03, 0x00, 0x00, //  {0xE9, 0x0000030A, 0x812C, PCH_LANE_OWN_COMMON, LANE},
  0x28, 0xE9, 0x07, 0x00, 0x0F, 0x00, 0x30, 0x81, 0x00, 0x3C, 0x00, 0x78, //  {0xE9, 0x78003C00, 0x8130, PCH_LANE_OWN_COMMON, LANE},
  0x0F, 0x00, 0x00, 0x00,                                                 //  End Table Marker
  0x28, 0xA9, 0x07, 0x00, 0x0F, 0x00, 0x40, 0x83, 0x58, 0xCF, 0x05, 0x01, //  {0xA9, 0x0105CF58, 0x8340, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI, MULCAST},
  0x28, 0xA9, 0x07, 0x00, 0x0F, 0x00, 0x44, 0x83, 0x48, 0xD5, 0x05, 0x01, //  {0xA9, 0x0105D548, 0x8344, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI, MULCAST},
  0x28, 0xA9, 0x07, 0x00, 0x0F, 0x00, 0x94, 0x82, 0x06, 0x2A, 0x29, 0x28, //  {0xA9, 0x28292A06, 0x8294, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI, MULCAST},
  0x28, 0xA9, 0x07, 0x00, 0x0F, 0x00, 0x70, 0x83, 0x5A, 0x25, 0x40, 0x33, //  {0xA9, 0x3340255A, 0x8370, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI, MULCAST},
  0x28, 0xA9, 0x07, 0x00, 0x0F, 0x00, 0x18, 0x83, 0x3F, 0x40, 0x00, 0x00, //  {0xA9, 0x0000403F, 0x8318, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI, MULCAST},
  0x28, 0xA9, 0x07, 0x00, 0x0F, 0x00, 0x78, 0x83, 0x60, 0x40, 0x83, 0x40, //  {0xA9, 0x40834060, 0x8378, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI, MULCAST},
  0x28, 0xA9, 0x07, 0x00, 0x0F, 0x00, 0x04, 0x82, 0x64, 0x00, 0x00, 0x00, //  {0xA9, 0x00000064, 0x8204, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI, MULCAST},
  0x28, 0xA9, 0x07, 0x00, 0x0F, 0x00, 0x8C, 0x83, 0x58, 0x88, 0x06, 0x00, //  {0xA9, 0x00068858, 0x838C, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI, MULCAST},
  0x28, 0xA9, 0x07, 0x00, 0x0F, 0x00, 0xD8, 0x82, 0xFF, 0x0C, 0x40, 0x80, //  {0xA9, 0x80400CFF, 0x82D8, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI, MULCAST},
  0x28, 0xA9, 0x07, 0x00, 0x0F, 0x00, 0x80, 0x83, 0x60, 0x07, 0xA0, 0x08, //  {0xA9, 0x08A00760, 0x8380, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI, MULCAST},
  0x28, 0xA9, 0x07, 0x00, 0x0F, 0x00, 0x00, 0x83, 0x10, 0x50, 0x52, 0x09, //  {0xA9, 0x09525010, 0x8300, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI, MULCAST},
  0x28, 0xA9, 0x07, 0x00, 0x0F, 0x00, 0x0C, 0x83, 0x1F, 0x9B, 0xD2, 0x00, //  {0xA9, 0x00D29B1F, 0x830C, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI, MULCAST},
  0x28, 0xA9, 0x07, 0x00, 0x0F, 0x00, 0x84, 0x83, 0x00, 0x40, 0x17, 0xE0, //  {0xA9, 0xE0174000, 0x8384, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI, MULCAST},
  0x28, 0xEA, 0x07, 0x00, 0x0F, 0x00, 0x40, 0x83, 0x58, 0xCF, 0x05, 0x01, //  {0xEA, 0x0105CF58, 0x8340, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI, MULCAST},
  0x28, 0xEA, 0x07, 0x00, 0x0F, 0x00, 0x44, 0x83, 0x48, 0xD5, 0x05, 0x01, //  {0xEA, 0x0105D548, 0x8344, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI, MULCAST},
  0x28, 0xEA, 0x07, 0x00, 0x0F, 0x00, 0x94, 0x88, 0x02, 0x2A, 0x29, 0x28, //  {0xEA, 0x28292A02, 0x8894, V_PCH_PCR_FIA_LANE_OWN_USB3, MULCAST},
  0x28, 0xEA, 0x07, 0x00, 0x0F, 0x00, 0x94, 0x82, 0x06, 0x2A, 0x29, 0x28, //  {0xEA, 0x28292A06, 0x8294, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI, MULCAST},
  0x28, 0xEA, 0x07, 0x00, 0x0F, 0x00, 0xA0, 0x89, 0xEC, 0x10, 0x00, 0x30, //  {0xEA, 0x300010EC, 0x89A0, V_PCH_PCR_FIA_LANE_OWN_USB3, MULCAST},
  0x28, 0xEA, 0x07, 0x00, 0x0F, 0x00, 0xA4, 0x89, 0x30, 0x30, 0x30, 0x00, //  {0xEA, 0x00303030, 0x89A4, V_PCH_PCR_FIA_LANE_OWN_USB3, MULCAST},
  0x28, 0xEA, 0x07, 0x00, 0x0F, 0x00, 0x40, 0x89, 0x4F, 0xC9, 0x1A, 0x03, //  {0xEA, 0x031AC94F, 0x8940, V_PCH_PCR_FIA_LANE_OWN_USB3, MULCAST},
  0x28, 0xEA, 0x07, 0x00, 0x0F, 0x00, 0x14, 0x89, 0x1F, 0xC3, 0x30, 0x03, //  {0xEA, 0x0330C31F, 0x8914, V_PCH_PCR_FIA_LANE_OWN_USB3, MULCAST},
  0x28, 0xEA, 0x07, 0x00, 0x0F, 0x00, 0x70, 0x83, 0x5A, 0x25, 0x40, 0x33, //  {0xEA, 0x3340255A, 0x8370, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI, MULCAST},
  0x28, 0xEA, 0x07, 0x00, 0x0F, 0x00, 0x70, 0x89, 0x5A, 0x25, 0x40, 0x33, //  {0xEA, 0x3340255A, 0x8970, V_PCH_PCR_FIA_LANE_OWN_USB3, MULCAST},
  0x28, 0xEA, 0x07, 0x00, 0x0F, 0x00, 0x18, 0x89, 0x3F, 0x00, 0x00, 0x00, //  {0xEA, 0x0000003F, 0x8918, V_PCH_PCR_FIA_LANE_OWN_USB3, MULCAST},
  0x28, 0xEA, 0x07, 0x00, 0x0F, 0x00, 0x18, 0x83, 0x3F, 0x40, 0x00, 0x00, //  {0xEA, 0x0000403F, 0x8318, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI, MULCAST},
  0x28, 0xEA, 0x07, 0x00, 0x0F, 0x00, 0x78, 0x83, 0x60, 0x40, 0x83, 0x40, //  {0xEA, 0x40834060, 0x8378, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI, MULCAST},
  0x28, 0xEA, 0x07, 0x00, 0x0F, 0x00, 0x78, 0x89, 0x60, 0x40, 0x83, 0x40, //  {0xEA, 0x40834060, 0x8978, V_PCH_PCR_FIA_LANE_OWN_USB3, MULCAST},
  0x28, 0xEA, 0x07, 0x00, 0x0F, 0x00, 0x04, 0x89, 0x89, 0x09, 0x55, 0x00, //  {0xEA, 0x00550989, 0x8904, V_PCH_PCR_FIA_LANE_OWN_USB3, MULCAST},
  0x28, 0xEA, 0x07, 0x00, 0x0F, 0x00, 0xC8, 0x89, 0x50, 0x88, 0x4D, 0x60, //  {0xEA, 0x604D8850, 0x89C8, V_PCH_PCR_FIA_LANE_OWN_USB3, MULCAST},
  0x28, 0xEA, 0x07, 0x00, 0x0F, 0x00, 0x04, 0x82, 0x64, 0x00, 0x00, 0x00, //  {0xEA, 0x00000064, 0x8204, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI, MULCAST},
  0x28, 0xEA, 0x07, 0x00, 0x0F, 0x00, 0x8C, 0x89, 0x28, 0x80, 0x06, 0x00, //  {0xEA, 0x00068028, 0x898C, V_PCH_PCR_FIA_LANE_OWN_USB3, MULCAST},
  0x28, 0xEA, 0x07, 0x00, 0x0F, 0x00, 0x8C, 0x83, 0x58, 0x88, 0x06, 0x00, //  {0xEA, 0x00068858, 0x838C, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI, MULCAST},
  0x28, 0xEA, 0x07, 0x00, 0x0F, 0x00, 0xE0, 0x89, 0x02, 0x00, 0x00, 0x08, //  {0xEA, 0x08000002, 0x89E0, V_PCH_PCR_FIA_LANE_OWN_USB3, MULCAST},
  0x28, 0xEA, 0x07, 0x00, 0x0F, 0x00, 0xD8, 0x82, 0xFF, 0x0C, 0x40, 0x80, //  {0xEA, 0x80400CFF, 0x82D8, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI, MULCAST},
  0x28, 0xEA, 0x07, 0x00, 0x0F, 0x00, 0xD8, 0x88, 0xFF, 0x0C, 0x40, 0x80, //  {0xEA, 0x80400CFF, 0x88D8, V_PCH_PCR_FIA_LANE_OWN_USB3, MULCAST},
  0x28, 0xEA, 0x07, 0x00, 0x0F, 0x00, 0x24, 0x88, 0x0F, 0x0C, 0x0C, 0x84, //  {0xEA, 0x840C0C0F, 0x8824, V_PCH_PCR_FIA_LANE_OWN_USB3, MULCAST},
  0x28, 0xEA, 0x07, 0x00, 0x0F, 0x00, 0x80, 0x83, 0x60, 0x07, 0xA0, 0x08, //  {0xEA, 0x08A00760, 0x8380, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI, MULCAST},
  0x28, 0xEA, 0x07, 0x00, 0x0F, 0x00, 0x00, 0x89, 0x10, 0x40, 0x52, 0x09, //  {0xEA, 0x09524010, 0x8900, V_PCH_PCR_FIA_LANE_OWN_USB3, MULCAST},
  0x28, 0xEA, 0x07, 0x00, 0x0F, 0x00, 0x00, 0x83, 0x10, 0x50, 0x52, 0x09, //  {0xEA, 0x09525010, 0x8300, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI, MULCAST},
  0x28, 0xEA, 0x07, 0x00, 0x0F, 0x00, 0x2C, 0x89, 0x0A, 0x10, 0x0A, 0x00, //  {0xEA, 0x000A100A, 0x892C, V_PCH_PCR_FIA_LANE_OWN_USB3, MULCAST},
  0x28, 0xEA, 0x07, 0x00, 0x0F, 0x00, 0x9C, 0x89, 0x00, 0x00, 0xB0, 0x00, //  {0xEA, 0x00B00000, 0x899C, V_PCH_PCR_FIA_LANE_OWN_USB3, MULCAST},
  0x28, 0xEA, 0x07, 0x00, 0x0F, 0x00, 0x20, 0x88, 0x26, 0x16, 0x07, 0x0D, //  {0xEA, 0x0D071626, 0x8820, V_PCH_PCR_FIA_LANE_OWN_USB3, MULCAST},
  0x28, 0xEA, 0x07, 0x00, 0x0F, 0x00, 0x0C, 0x83, 0x1F, 0x9B, 0xD2, 0x00, //  {0xEA, 0x00D29B1F, 0x830C, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI, MULCAST},
  0x28, 0xEA, 0x07, 0x00, 0x0F, 0x00, 0x3C, 0x89, 0xE0, 0x00, 0x00, 0x00, //  {0xEA, 0x000000E0, 0x893C, V_PCH_PCR_FIA_LANE_OWN_USB3, MULCAST},
  0x28, 0xEA, 0x07, 0x00, 0x0F, 0x00, 0x84, 0x83, 0x00, 0x40, 0x17, 0xE0, //  {0xEA, 0xE0174000, 0x8384, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI, MULCAST},
  0x28, 0xEA, 0x07, 0x00, 0x0F, 0x00, 0x84, 0x89, 0x00, 0x40, 0x17, 0xE0, //  {0xEA, 0xE0174000, 0x8984, V_PCH_PCR_FIA_LANE_OWN_USB3, MULCAST},
  0x28, 0xEA, 0x07, 0x00, 0x0F, 0x00, 0x80, 0x89, 0x60, 0x07, 0xE8, 0x00, //  {0xEA, 0x00E80760, 0x8980, V_PCH_PCR_FIA_LANE_OWN_USB3, MULCAST},
  0x28, 0xE9, 0x07, 0x00, 0x0F, 0x00, 0x40, 0x83, 0x58, 0xCF, 0x05, 0x01, //  {0xE9, 0x0105CF58, 0x8340, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI, MULCAST},
  0x28, 0xE9, 0x07, 0x00, 0x0F, 0x00, 0x44, 0x83, 0x48, 0xD5, 0x05, 0x01, //  {0xE9, 0x0105D548, 0x8344, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI, MULCAST},
  0x28, 0xE9, 0x07, 0x00, 0x0F, 0x00, 0x94, 0x82, 0x06, 0x2A, 0x29, 0x28, //  {0xE9, 0x28292A06, 0x8294, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI, MULCAST},
  0x28, 0xE9, 0x07, 0x00, 0x0F, 0x00, 0x70, 0x83, 0x5A, 0x25, 0x40, 0x33, //  {0xE9, 0x3340255A, 0x8370, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI, MULCAST},
  0x28, 0xE9, 0x07, 0x00, 0x0F, 0x00, 0x18, 0x83, 0x3F, 0x40, 0x00, 0x00, //  {0xE9, 0x0000403F, 0x8318, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI, MULCAST},
  0x28, 0xE9, 0x07, 0x00, 0x0F, 0x00, 0x78, 0x83, 0x60, 0x40, 0x83, 0x40, //  {0xE9, 0x40834060, 0x8378, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI, MULCAST},
  0x28, 0xE9, 0x07, 0x00, 0x0F, 0x00, 0x04, 0x82, 0x64, 0x00, 0x00, 0x00, //  {0xE9, 0x00000064, 0x8204, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI, MULCAST},
  0x28, 0xE9, 0x07, 0x00, 0x0F, 0x00, 0x8C, 0x83, 0x58, 0x88, 0x06, 0x00, //  {0xE9, 0x00068858, 0x838C, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI, MULCAST},
  0x28, 0xE9, 0x07, 0x00, 0x0F, 0x00, 0xD8, 0x82, 0xFF, 0x0C, 0x40, 0x80, //  {0xE9, 0x80400CFF, 0x82D8, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI, MULCAST},
  0x28, 0xE9, 0x07, 0x00, 0x0F, 0x00, 0x80, 0x83, 0x60, 0x07, 0xA0, 0x08, //  {0xE9, 0x08A00760, 0x8380, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI, MULCAST},
  0x28, 0xE9, 0x07, 0x00, 0x0F, 0x00, 0x00, 0x83, 0x10, 0x50, 0x52, 0x09, //  {0xE9, 0x09525010, 0x8300, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI, MULCAST},
  0x28, 0xE9, 0x07, 0x00, 0x0F, 0x00, 0x0C, 0x83, 0x1F, 0x9B, 0xD2, 0x00, //  {0xE9, 0x00D29B1F, 0x830C, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI, MULCAST},
  0x28, 0xE9, 0x07, 0x00, 0x0F, 0x00, 0x84, 0x83, 0x00, 0x40, 0x17, 0xE0, //  {0xE9, 0xE0174000, 0x8384, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI, MULCAST},
  0x0F, 0x00, 0x00, 0x00,                                                 //  End Table Marker
  0x29, 0xEC, 0x07, 0x00, 0x0F, 0x00, 0x44, 0x03, 0xF6, 0x4C, 0x1C, 0x30, //  {0xEC, 0x301C4CF6, 0x344, OPIPHY, NA, LANE},
  0x29, 0xEC, 0x07, 0x00, 0x0F, 0x00, 0x48, 0x03, 0x00, 0xE0, 0xBF, 0x1F, //  {0xEC, 0x1FBFE000, 0x348, OPIPHY, NA, LANE},
  0x29, 0xEC, 0x07, 0x00, 0x0F, 0x00, 0x40, 0x03, 0x00, 0x00, 0x00, 0x14, //  {0xEC, 0x14000000, 0x340, OPIPHY, NA, LANE},
  0x0F, 0x00, 0x00, 0x00,                                                 //  End Table Marker
  0x28, 0xAE, 0x07, 0x00, 0x0F, 0x00, 0x54, 0x03, 0x00, 0x00, 0xC0, 0x06, //  {0xAE, 0x06C00000, 0x354, GPIOPHY, NA, LANE},
  0x0F, 0x00, 0x00, 0x00,                                                 //  End Table Marker
};

PCH_SBI_HSIO_TABLE_STRUCT PchLpHsio_Ax_Mipi_B[] = {
  {0xA8, 0x00300401, 0x8030, PCH_LANE_OWN_COMMON}, //LANE
  {0xA8, 0x00000006, 0x803C, PCH_LANE_OWN_COMMON}, //LANE
  {0xA8, 0x0A030804, 0x8110, PCH_LANE_OWN_COMMON}, //LANE
  {0xA8, 0x001F000A, 0x8128, PCH_LANE_OWN_COMMON}, //LANE
  {0xA8, 0x0000030B, 0x812C, PCH_LANE_OWN_COMMON}, //LANE
  {0xA8, 0x78003C00, 0x8130, PCH_LANE_OWN_COMMON}, //LANE
  {0xA9, 0x00300401, 0x8030, PCH_LANE_OWN_COMMON}, //LANE
  {0xA9, 0x00000006, 0x803C, PCH_LANE_OWN_COMMON}, //LANE
  {0xA9, 0x0A010601, 0x8110, PCH_LANE_OWN_COMMON}, //LANE
  {0xA9, 0x001F0006, 0x8128, PCH_LANE_OWN_COMMON}, //LANE
  {0xA9, 0x0000030B, 0x812C, PCH_LANE_OWN_COMMON}, //LANE
  {0xA9, 0x78003C00, 0x8130, PCH_LANE_OWN_COMMON}, //LANE
  {0xE9, 0x00300401, 0x8030, PCH_LANE_OWN_COMMON}, //LANE
  {0xE9, 0x00000006, 0x803C, PCH_LANE_OWN_COMMON}, //LANE
  {0xEA, 0x00300401, 0x8030, PCH_LANE_OWN_COMMON}, //LANE
  {0xEA, 0x00000006, 0x803C, PCH_LANE_OWN_COMMON}, //LANE
  {0xEA, 0x78003C00, 0x8130, PCH_LANE_OWN_COMMON}, //LANE
  {0xEA, 0x010201E0, 0xC028, PCH_LANE_BDCAST}, //BDCAST
  {0xEA, 0x4B030540, 0xC1E4, PCH_LANE_BDCAST}, //BDCAST
  {0xEA, 0xA40B0A10, 0xC024, PCH_LANE_BDCAST}, //BDCAST
  {0xE9, 0x010201E0, 0xC028, PCH_LANE_BDCAST}, //BDCAST
  {0xE9, 0x4B030540, 0xC1E4, PCH_LANE_BDCAST}, //BDCAST
  {0xE9, 0xA40B0A10, 0xC024, PCH_LANE_BDCAST}, //BDCAST
  {0xA9, 0x010201E0, 0xC028, PCH_LANE_BDCAST}, //BDCAST
  {0xA9, 0x4B030540, 0xC1E4, PCH_LANE_BDCAST}, //BDCAST
  {0xA9, 0xA40B0A10, 0xC024, PCH_LANE_BDCAST}, //BDCAST
  {0xA9, 0x01051158, 0x8B40, V_PCH_PCR_FIA_LANE_OWN_SATA}, //MULCAST
  {0xA9, 0x01051548, 0x8B44, V_PCH_PCR_FIA_LANE_OWN_SATA}, //MULCAST
  {0xA9, 0x0105154C, 0x8B48, V_PCH_PCR_FIA_LANE_OWN_SATA}, //MULCAST
  {0xA9, 0x0105194C, 0x8748, V_PCH_PCR_FIA_LANE_OWN_GBE}, //MULCAST
  {0xA9, 0x0105D35A, 0x834C, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI}, //MULCAST
  {0xA9, 0x0132A009, 0x8B58, V_PCH_PCR_FIA_LANE_OWN_SATA}, //MULCAST
  {0xA9, 0x151F2800, 0x8A98, V_PCH_PCR_FIA_LANE_OWN_SATA}, //MULCAST
  {0xA9, 0x24743020, 0x8388, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI}, //MULCAST
  {0xA9, 0x282D2D06, 0x8A94, V_PCH_PCR_FIA_LANE_OWN_SATA}, //MULCAST
  {0xA9, 0x3340255A, 0x8770, V_PCH_PCR_FIA_LANE_OWN_GBE}, //MULCAST
  {0xA9, 0x3340255A, 0x8B70, V_PCH_PCR_FIA_LANE_OWN_SATA}, //MULCAST
  {0xA9, 0x00384008, 0x8B1C, V_PCH_PCR_FIA_LANE_OWN_SATA}, //MULCAST
  {0xA9, 0x04000045, 0x8A04, V_PCH_PCR_FIA_LANE_OWN_SATA}, //MULCAST
  {0xA9, 0x40834060, 0x8778, V_PCH_PCR_FIA_LANE_OWN_GBE}, //MULCAST
  {0xA9, 0x40834060, 0x8B78, V_PCH_PCR_FIA_LANE_OWN_SATA}, //MULCAST
  {0xA9, 0x043F3F3F, 0x8350, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI}, //MULCAST
  {0xA9, 0x053F3F3F, 0x8B50, V_PCH_PCR_FIA_LANE_OWN_SATA}, //MULCAST
  {0xA9, 0x00006000, 0x8B20, V_PCH_PCR_FIA_LANE_OWN_SATA}, //MULCAST
  {0xA9, 0x80400CFF, 0x86D8, V_PCH_PCR_FIA_LANE_OWN_GBE}, //MULCAST
  {0xA9, 0x80400CFF, 0x8AD8, V_PCH_PCR_FIA_LANE_OWN_SATA}, //MULCAST
  {0xA9, 0x80C00002, 0x8354, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI}, //MULCAST
  {0xA9, 0x80C00200, 0x8754, V_PCH_PCR_FIA_LANE_OWN_GBE}, //MULCAST
  {0xA9, 0x80C00205, 0x8B54, V_PCH_PCR_FIA_LANE_OWN_SATA}, //MULCAST
  {0xA9, 0x00868058, 0x878C, V_PCH_PCR_FIA_LANE_OWN_GBE}, //MULCAST
  {0xA9, 0x00868858, 0x8B8C, V_PCH_PCR_FIA_LANE_OWN_SATA}, //MULCAST
  {0xA9, 0x0008887C, 0x838C, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI}, //MULCAST
  {0xA9, 0x09521410, 0x8700, V_PCH_PCR_FIA_LANE_OWN_GBE}, //MULCAST
  {0xA9, 0x09525110, 0x8300, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI}, //MULCAST
  {0xA9, 0x09525410, 0x8B00, V_PCH_PCR_FIA_LANE_OWN_SATA}, //MULCAST
  {0xA9, 0x000A100A, 0x832C, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI}, //MULCAST
  {0xA9, 0x00B00000, 0x839C, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI}, //MULCAST
  {0xA9, 0x000000BF, 0x8B18, V_PCH_PCR_FIA_LANE_OWN_SATA}, //MULCAST
  {0xA9, 0x000000E0, 0x833C, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI}, //MULCAST
  {0xA9, 0xE0174000, 0x8784, V_PCH_PCR_FIA_LANE_OWN_GBE}, //MULCAST
  {0xA9, 0xE0174000, 0x8B84, V_PCH_PCR_FIA_LANE_OWN_SATA}, //MULCAST
  {0xA9, 0xFF00FF00, 0x8240, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI}, //MULCAST
  {0xA9, 0xFFE01800, 0x8308, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI}, //MULCAST
  {0xEA, 0x0105174C, 0x8D44, PCH_LANE_OWN_MIPI_B}, //MULCAST
  {0xEA, 0x0105194C, 0x8D48, PCH_LANE_OWN_MIPI_B}, //MULCAST
  {0xEA, 0x0105D35A, 0x834C, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI}, //MULCAST
  {0xEA, 0x1F1F0000, 0x885C, V_PCH_PCR_FIA_LANE_OWN_USB3}, //MULCAST
  {0xEA, 0x00001FDD, 0x8DC4, PCH_LANE_OWN_MIPI_B}, //MULCAST
  {0xEA, 0x24743020, 0x8388, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI}, //MULCAST
  {0xEA, 0x3340255A, 0x8D70, PCH_LANE_OWN_MIPI_B}, //MULCAST
  {0xEA, 0x40834060, 0x8D78, PCH_LANE_OWN_MIPI_B}, //MULCAST
  {0xEA, 0x043F3F3F, 0x8350, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI}, //MULCAST
  {0xEA, 0x80400CFF, 0x8CD8, PCH_LANE_OWN_MIPI_B}, //MULCAST
  {0xEA, 0x80C00002, 0x8354, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI}, //MULCAST
  {0xEA, 0x80C0030A, 0x8D54, PCH_LANE_OWN_MIPI_B}, //MULCAST
  {0xEA, 0x00868858, 0x8D8C, PCH_LANE_OWN_MIPI_B}, //MULCAST
  {0xEA, 0x0008887C, 0x838C, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI}, //MULCAST
  {0xEA, 0x09521410, 0x8D00, PCH_LANE_OWN_MIPI_B}, //MULCAST
  {0xEA, 0x09525110, 0x8300, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI}, //MULCAST
  {0xEA, 0x000A100A, 0x832C, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI}, //MULCAST
  {0xEA, 0xA40C0C0F, 0x8824, V_PCH_PCR_FIA_LANE_OWN_USB3}, //MULCAST
  {0xEA, 0x00B00000, 0x839C, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI}, //MULCAST
  {0xEA, 0xC0000100, 0x8C00, PCH_LANE_OWN_MIPI_B}, //MULCAST
  {0xEA, 0x000000E0, 0x833C, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI}, //MULCAST
  {0xEA, 0xE0174000, 0x8D84, PCH_LANE_OWN_MIPI_B}, //MULCAST
  {0xEA, 0xFF00FF00, 0x8240, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI}, //MULCAST
  {0xEA, 0xFFE01800, 0x8308, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI}, //MULCAST
  {0xE9, 0x0105194C, 0x8748, V_PCH_PCR_FIA_LANE_OWN_GBE}, //MULCAST
  {0xE9, 0x0105D35A, 0x834C, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI}, //MULCAST
  {0xE9, 0x24743020, 0x8388, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI}, //MULCAST
  {0xE9, 0x3340255A, 0x8770, V_PCH_PCR_FIA_LANE_OWN_GBE}, //MULCAST
  {0xE9, 0x40834060, 0x8778, V_PCH_PCR_FIA_LANE_OWN_GBE}, //MULCAST
  {0xE9, 0x043F3F3F, 0x8350, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI}, //MULCAST
  {0xE9, 0x80400CFF, 0x86D8, V_PCH_PCR_FIA_LANE_OWN_GBE}, //MULCAST
  {0xE9, 0x80C00002, 0x8354, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI}, //MULCAST
  {0xE9, 0x80C00200, 0x8754, V_PCH_PCR_FIA_LANE_OWN_GBE}, //MULCAST
  {0xE9, 0x00868058, 0x878C, V_PCH_PCR_FIA_LANE_OWN_GBE}, //MULCAST
  {0xE9, 0x0008887C, 0x838C, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI}, //MULCAST
  {0xE9, 0x09521410, 0x8700, V_PCH_PCR_FIA_LANE_OWN_GBE}, //MULCAST
  {0xE9, 0x09525110, 0x8300, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI}, //MULCAST
  {0xE9, 0x000A100A, 0x832C, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI}, //MULCAST
  {0xE9, 0x00B00000, 0x839C, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI}, //MULCAST
  {0xE9, 0x000000E0, 0x833C, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI}, //MULCAST
  {0xE9, 0xE0174000, 0x8784, V_PCH_PCR_FIA_LANE_OWN_GBE}, //MULCAST
  {0xE9, 0xFF00FF00, 0x8240, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI}, //MULCAST
  {0xE9, 0xFFE01800, 0x8308, V_PCH_PCR_FIA_LANE_OWN_PCIEDMI}, //MULCAST
};
