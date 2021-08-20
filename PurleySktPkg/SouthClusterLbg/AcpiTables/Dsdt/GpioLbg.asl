/**@file

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

 Module Name:

 GpioLbg.asl

@par Specification Reference:
**/
#ifdef HALO_BUILD
/**  This ASL module has been implemented to support Lewisburg PCH GPIO Communities only for the ADG Halo Creek Program
     Please do not alter this code as it has been encapsulated with HALO_BUILD flag.
     Contact  mike.malvestuto@intel.com if you wish to reuse or make changes to this module.
**/

#define LBG_SERIAL_IO_GPIO_HID "INT3536"

 Scope (\_SB.PC00)
    {
        Device (GPIO)
        {
            Method (_HID)
            {
               Return(LBG_SERIAL_IO_GPIO_HID)
            }

            Method (_CRS, 0, NotSerialized)
            {
                Name (RBUF, ResourceTemplate() {
                    /*
                     * All communities except community 2 (Deep Sleep Well)
                     * which will not be exposed to the OS.
                     */
                    Memory32Fixed(ReadWrite, 0x00000000, 0x0001000, COM0) // Community 0
                    Memory32Fixed(ReadWrite, 0x00000000, 0x0001000, COM1) // Community 1
                    Memory32Fixed(ReadWrite, 0x00000000, 0x0001000, COM2) // Community 2
                    Memory32Fixed(ReadWrite, 0x00000000, 0x0001000, COM3) // Community 3
                    Memory32Fixed(ReadWrite, 0x00000000, 0x0001000, COM4) // Community 4
                    Memory32Fixed(ReadWrite, 0x00000000, 0x0001000, COM5) // Community 5
                    Interrupt (ResourceConsumer, Level, ActiveLow,Shared, , , IRQ   ) { 14 }
                })

                // Assign base addresses for each GPIO community
                CreateDWordField(RBUF, COM0._BAS, C0BS)
                CreateDWordField(RBUF, COM1._BAS, C1BS)
                CreateDWordField(RBUF, COM2._BAS, C2BS)
                CreateDWordField(RBUF, COM3._BAS, C3BS)
                CreateDWordField(RBUF, COM4._BAS, C4BS)
                CreateDWordField(RBUF, COM5._BAS, C5BS)

                Store( Add(SBRG,PCH_GPIO_COM0), C0BS)
                Store( Add(SBRG,PCH_GPIO_COM1), C1BS)
                Store( Add(SBRG,PCH_GPIO_COM2), C2BS)
                Store( Add(SBRG,PCH_GPIO_COM3), C3BS)
                Store( Add(SBRG,PCH_GPIO_COM4), C4BS)
                Store( Add(SBRG,PCH_GPIO_COM5), C5BS)

                CreateDWordField(RBUF,IRQ._INT,IRQN)

                Return (RBUF)
            }

            Method (_STA, 0x0, NotSerialized) 
            {
                If(LEqual(SBRG, 0))   { Return(0x0) } // has BAR for this device been programmed by the OS?
                Return(0xF)
            }

        }

    }  // END Scope(\_SB.PC00)

#endif
