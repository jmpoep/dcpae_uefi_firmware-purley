/** @file
  Register names for PCH Integrated Sensor Hub (ISH3.0)

  Conventions:
  
  - Prefixes:
    Definitions beginning with "R_" are registers
    Definitions beginning with "B_" are bits within registers
    Definitions beginning with "V_" are meaningful values within the bits
    Definitions beginning with "S_" are register sizes
    Definitions beginning with "N_" are the bit position
  - In general, PCH registers are denoted by "_PCH_" in register names
  - Registers / bits that are different between PCH generations are denoted by
    "_PCH_[generation_name]_" in register/bit names.
  - Registers / bits that are specific to PCH-H denoted by "_H_" in register/bit names.
    Registers / bits that are specific to PCH-LP denoted by "_LP_" in register/bit names.
    e.g., "_PCH_H_", "_PCH_LP_"
    Registers / bits names without _H_ or _LP_ apply for both H and LP.
  - Registers / bits that are different between SKUs are denoted by "_[SKU_name]"
    at the end of the register/bit names
  - Registers / bits of new devices introduced in a PCH generation will be just named
    as "_PCH_" without [generation_name] inserted.

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
#ifndef _PCH_REGS_ISH_H_
#define _PCH_REGS_ISH_H_

//
// ISH Controller Registers (D19:F0)
//
// PCI Configuration Space Registers
#define PCI_DEVICE_NUMBER_PCH_ISH             19
#define PCI_FUNCTION_NUMBER_PCH_ISH           0
#define V_PCH_ISH_VENDOR_ID                   V_PCH_INTEL_VENDOR_ID
#define V_PCH_H_ISH_DEVICE_ID                 0xA135
#define V_PCH_LP_ISH_DEVICE_ID                0x9D35

#define R_PCH_ISH_BAR0_LOW                    0x10
#define R_PCH_ISH_BAR0_HIGH                   0x14
#define V_PCH_ISH_BAR0_SIZE                   0x100000
#define N_PCH_ISH_BAR0_ALIGNMENT              20
#define R_PCH_ISH_BAR1_LOW                    0x18
#define R_PCH_ISH_BAR1_HIGH                   0x1C
#define V_PCH_ISH_BAR1_SIZE                   0x1000
#define N_PCH_ISH_BAR1_ALIGNMENT              12

//
// ISH Private Configuration Space Registers (IOSF2OCP)
// (PID:ISH)
//
#define R_PCH_PCR_ISH_PMCTL                   0x1D0                         ///< Power Management
#define R_PCH_PCR_ISH_PCICFGCTRL              0x200                         ///< PCI Configuration Control
#define B_PCH_PCR_ISH_PCICFGCTR_PCI_IRQ       0x0FF00000                    ///< PCI IRQ number
#define N_PCH_PCR_ISH_PCICFGCTR_PCI_IRQ       20
#define B_PCH_PCR_ISH_PCICFGCTR_ACPI_IRQ      0x000FF000                    ///< ACPI IRQ number
#define N_PCH_PCR_ISH_PCICFGCTR_ACPI_IRQ      12
#define B_PCH_PCR_ISH_PCICFGCTR_IPIN1         (BIT11 | BIT10 | BIT9 | BIT8) ///< Interrupt Pin
#define N_PCH_PCR_ISH_PCICFGCTR_IPIN1         8
#define B_PCH_PCR_ISH_PCICFGCTRL_BAR1DIS      BIT7                          ///< BAR1 Disable

//
// Number of pins used by ISH controllers
//
#define PCH_ISH_PINS_PER_I2C_CONTROLLER               2
#define PCH_ISH_PINS_PER_UART_CONTROLLER              4
#define PCH_ISH_PINS_PER_SPI_CONTROLLER               4

#endif
