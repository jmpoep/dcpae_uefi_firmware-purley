/** @file
  ACPI DSDT table

@copyright
 Copyright (c) 2011 - 2014 Intel Corporation. All rights reserved
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
  


// Use this information when determining the Possible IRQs that can be
// used in a given system.
//
// The following IRQs are always in use by legacy devices:
//    0  = System Timer
//    2  = 8259 PIC
//    8  = RTC
//    9  = SCI Interrupt (It may be used, we choose not to)
//    13 = Co-processor Error
//
// The following may be in use by legacy devices:
//    1  = If using PS/2 Keyboard
//    3  = If COMx Port Enabled and IRQ = 3
//    4  = If COMx Port Enabled and IRQ = 4
//    5  = If LPT Port Enabled and IRQ = 5
//    6  = If FDC Enabled
//    7  = If LPT Port Enabled and IRQ = 7
//    12 = If using PS/2 Mouse
//    14 = Primary IDE (If populated and in Compatibility Mode)
//    15 = Secondary IDE (If populated and in Compatibility Mode)
//
// The following will never be in use by legacy devices:
//    10 = Assign to PARC, PCRC, PERC, PGRC
//    11 = Assign to PBRC, PDRC, PFRC, PHRC

Device(LNKA)        // PARC Routing Resource
{
  Name(_HID,EISAID("PNP0C0F"))  // PCI Interrupt Link Device
  
  Name(_UID,1)      // Unique to other Link Devices

  // Disable the PCI IRQ.

  Method(_DIS,0,Serialized)
  {
    Or(\_SB.PARC,0x80,\_SB.PARC)
  }

  // Possible IRQ Resource Setting.

  Method (_PRS, 0, Serialized)
  {
    return (PRSA)
  }

  // Current IRQ Resource Setting.

  Method(_CRS,0,Serialized)
  {
    Name(RTLA,ResourceTemplate()
    {
      IRQ(Level,ActiveLow,Shared) {}
    })

    // Point to specific byte.

    CreateWordField(RTLA,1,IRQ0)
        
    // Zero out IRQ mask bits 0-15

    Store(Zero,IRQ0)

    ShiftLeft(1,And(\_SB.PARC,0x0F),IRQ0)

    Return(RTLA)
  }

  // Set IRQ Resource Setting.

  Method(_SRS,1,Serialized) 
  {
    // Point to the specific byte passed in
      
    CreateWordField(Arg0,1,IRQ0)

    // Determine the IRQ bit to set and store it

    FindSetRightBit(IRQ0,Local0) 
    Decrement(Local0)
    Store(Local0,\_SB.PARC)
  }

  // PCI IRQ Status.

  Method(_STA,0,Serialized)
  {
    If(And(\_SB.PARC,0x80))
    {
      Return(0x0009)
    }
    Else
    {
      Return(0x000B)
    }
  }
}

Device(LNKB)        // PBRC Routing Resource
{
  Name(_HID,EISAID("PNP0C0F"))

  Name(_UID,2)

  // Disable the PCI IRQ.

  Method(_DIS,0,Serialized)
  {
    Or(\_SB.PBRC,0x80,\_SB.PBRC)
  }

  // Possible IRQ Resource Setting.

  Method (_PRS, 0, Serialized)
  {
    return (PRSB)
  }

  // Current IRQ Resource Setting.

  Method(_CRS,0,Serialized)
  {
    Name(RTLB,ResourceTemplate()
    {
      IRQ(Level,ActiveLow,Shared) {}
    })

    // Point to specific byte.

    CreateWordField(RTLB,1,IRQ0)
        
    // Zero out IRQ mask bits 0-15

    Store(Zero,IRQ0)

    ShiftLeft(1,And(\_SB.PBRC,0x0F),IRQ0)

    Return(RTLB)
  }

  // Set IRQ Resource Setting.

  Method(_SRS,1,Serialized) 
  {
    // Point to the specific byte passed in.
      
    CreateWordField(Arg0,1,IRQ0)

    // Determine the IRQ bit to set and store it,

    FindSetRightBit(IRQ0,Local0) 
    Decrement(Local0)
    Store(Local0,\_SB.PBRC)
  }

  // PCI IRQ Status.

  Method(_STA,0,Serialized)
  {
    If(And(\_SB.PBRC,0x80))
    {
      Return(0x0009)
    }
    Else
    {
      Return(0x000B)
    }
  }
}

Device(LNKC)        // PCRC Routing Resource
{    
  Name(_HID,EISAID("PNP0C0F"))

  Name(_UID,3)

  // Disable the PCI IRQ.

  Method(_DIS,0,Serialized)
  {
    Or(\_SB.PCRC,0x80,\_SB.PCRC)
  }

  // Possible IRQ Resource Setting.

  Method (_PRS, 0, Serialized)
  {
    return (PRSC)
  }

  // Current IRQ Resource Setting.

  Method(_CRS,0,Serialized)
  {
    Name(RTLC,ResourceTemplate()
    {
      IRQ(Level,ActiveLow,Shared) {}
    })

    // Point to specific byte.

    CreateWordField(RTLC,1,IRQ0)
        
    // Zero out IRQ mask bits 0-15

    Store(Zero,IRQ0)

    ShiftLeft(1,And(\_SB.PCRC,0x0F),IRQ0)

    Return(RTLC)
  }

  // Set IRQ Resource Setting.

  Method(_SRS,1,Serialized) 
  {
    // Point to the specific byte passed in.
      
    CreateWordField(Arg0,1,IRQ0)

    // Determine the IRQ bit to set and store it,

    FindSetRightBit(IRQ0,Local0) 
    Decrement(Local0)
    Store(Local0,\_SB.PCRC)
  }
        
  // PCI IRQ Status.

  Method(_STA,0,Serialized)
  {
    If(And(\_SB.PCRC,0x80))
    {
      Return(0x0009)
    }
    Else
    {
      Return(0x000B)
    }
  }
}

Device(LNKD)        // PDRC Routing Resource
{    
  Name(_HID,EISAID("PNP0C0F"))

  Name(_UID,4)

  // Disable the PCI IRQ.

  Method(_DIS,0,Serialized)
  {
    Or(\_SB.PDRC,0x80,\_SB.PDRC)
  }
                   
  // Possible IRQ Resource Setting.

  Method (_PRS, 0, Serialized)
  {
    return (PRSD)
  }

  // Current IRQ Resource Setting.

  Method(_CRS,0,Serialized)
  {
    Name(RTLD,ResourceTemplate()
    {
      IRQ(Level,ActiveLow,Shared) {}
    })

    // Point to specific byte.

    CreateWordField(RTLD,1,IRQ0)
        
    // Zero out IRQ mask bits 0-15

    Store(Zero,IRQ0)

    ShiftLeft(1,And(\_SB.PDRC,0x0F),IRQ0)

    Return(RTLD)
  }

  // Set IRQ Resource Setting.

  Method(_SRS,1,Serialized) 
  {
    // Point to the specific byte passed in.
      
    CreateWordField(Arg0,1,IRQ0)

    // Determine the IRQ bit to set and store it,

    FindSetRightBit(IRQ0,Local0) 
    Decrement(Local0)
    Store(Local0,\_SB.PDRC)
  }
        
  // PCI IRQ Status.

  Method(_STA,0,Serialized)
  { 
    If(And(\_SB.PDRC,0x80))
    {
      Return(0x0009)
    }
    Else
    {
      Return(0x000B)
    }
  }
}

Device(LNKE)        // PERC Routing Resource
{    
  Name(_HID,EISAID("PNP0C0F"))

  Name(_UID,5)

  // Disable the PCI IRQ.

  Method(_DIS,0,Serialized)
  {
    Or(\_SB.PERC,0x80,\_SB.PERC)
  }
                   
  // Possible IRQ Resource Setting.

  Method (_PRS, 0, Serialized)
  {
    return (PRSE)
  }

  // Current IRQ Resource Setting.

  Method(_CRS,0,Serialized)
  {
    Name(RTLE,ResourceTemplate()
    {
      IRQ(Level,ActiveLow,Shared) {}
    })

    // Point to specific byte.

    CreateWordField(RTLE,1,IRQ0)
        
    // Zero out IRQ mask bits 0-15

    Store(Zero,IRQ0)

    ShiftLeft(1,And(\_SB.PERC,0x0F),IRQ0)

    Return(RTLE)
  }

  // Set IRQ Resource Setting.

  Method(_SRS,1,Serialized) 
  {
    // Point to the specific byte passed in
      
    CreateWordField(Arg0,1,IRQ0)

    // Determine the IRQ bit to set and store it

    FindSetRightBit(IRQ0,Local0) 
    Decrement(Local0)
    Store(Local0,\_SB.PERC)
  }
        
  // PCI IRQ Status.

  Method(_STA,0,Serialized)
  { 
    If(And(\_SB.PERC,0x80))
    {
      Return(0x0009)
    }
    Else
    {
      Return(0x000B)
    }
  }
}

Device(LNKF)        // PFRC Routing Resource
{
  Name(_HID,EISAID("PNP0C0F"))

  Name(_UID,6)

  // Disable the PCI IRQ.

  Method(_DIS,0,Serialized)
  {
    Or(\_SB.PFRC,0x80,\_SB.PFRC)
  }
                   
  // Possible IRQ Resource Setting.

  Method (_PRS, 0, Serialized)
  {
    return (PRSF)
  }

  // Current IRQ Resource Setting.

  Method(_CRS,0,Serialized)
  {
    Name(RTLF,ResourceTemplate()
    {
      IRQ(Level,ActiveLow,Shared) {}
    })

    // Point to specific byte.

    CreateWordField(RTLF,1,IRQ0)
        
    // Zero out IRQ mask bits 0-15

    Store(Zero,IRQ0)

    ShiftLeft(1,And(\_SB.PFRC,0x0F),IRQ0)

    Return(RTLF)
  }

  // Set IRQ Resource Setting.

  Method(_SRS,1,Serialized) 
  {
    // Point to the specific byte passed in.
      
    CreateWordField(Arg0,1,IRQ0)

    // Determine the IRQ bit to set and store it,

    FindSetRightBit(IRQ0,Local0) 
    Decrement(Local0)
    Store(Local0,\_SB.PFRC)
  }
        
  // PCI IRQ Status.

  Method(_STA,0,Serialized)
  { 
    If(And(\_SB.PFRC,0x80))
    {
      Return(0x0009)
    }
    Else
    {
      Return(0x000B)
    }
  }
}

Device(LNKG)        // PGRC Routing Resource
{    
  Name(_HID,EISAID("PNP0C0F"))

  Name(_UID,7)

  // Disable the PCI IRQ.

  Method(_DIS,0,Serialized)
  {
    Or(\_SB.PGRC,0x80,\_SB.PGRC)
  }
                   
  // Possible IRQ Resource Setting.

  Method (_PRS, 0, Serialized)
  {
    return (PRSG)
  }

  // Current IRQ Resource Setting.

  Method(_CRS,0,Serialized)
  {
    Name(RTLG,ResourceTemplate()
    {
      IRQ(Level,ActiveLow,Shared) {}
    })

    // Point to specific byte.

    CreateWordField(RTLG,1,IRQ0)
        
    // Zero out IRQ mask bits 0-15

    Store(Zero,IRQ0)

    ShiftLeft(1,And(\_SB.PGRC,0x0F),IRQ0)

    Return(RTLG)
  }

  // Set IRQ Resource Setting.

  Method(_SRS,1,Serialized) 
  {
    // Point to the specific byte passed in.
      
    CreateWordField(Arg0,1,IRQ0)

    // Determine the IRQ bit to set and store it,

    FindSetRightBit(IRQ0,Local0) 
    Decrement(Local0)
    Store(Local0,\_SB.PGRC)
  }
        
  // PCI IRQ Status.

  Method(_STA,0,Serialized)
  { 
    If(And(\_SB.PGRC,0x80))
    {
      Return(0x0009)
    }
    Else
    {
      Return(0x000B)
    }
  }
}

Device(LNKH)        // PHRC Routing Resource
{    
  Name(_HID,EISAID("PNP0C0F"))

  Name(_UID,8)

  // Disable the PCI IRQ.

  Method(_DIS,0,Serialized)
  {
    Or(\_SB.PHRC,0x80,\_SB.PHRC)
  }
                   
  // Possible IRQ Resource Setting.

  Method (_PRS, 0, Serialized)
  {
    return (PRSH)
  }

  // Current IRQ Resource Setting.

  Method(_CRS,0,Serialized)
  {
    Name(RTLH,ResourceTemplate()
    {
      IRQ(Level,ActiveLow,Shared) {}
    })

    // Point to specific byte.

    CreateWordField(RTLH,1,IRQ0)
        
    // Zero out IRQ mask bits 0-15

    Store(Zero,IRQ0)

    ShiftLeft(1,And(\_SB.PHRC,0x0F),IRQ0)

    Return(RTLH)
  }

  // Set IRQ Resource Setting.

  Method(_SRS,1,Serialized) 
  {
    // Point to the specific byte passed in.
      
    CreateWordField(Arg0,1,IRQ0)

    // Determine the IRQ bit to set and store it,

    FindSetRightBit(IRQ0,Local0) 
    Decrement(Local0)
    Store(Local0,\_SB.PHRC)
  }
        
  // PCI IRQ Status.

  Method(_STA,0,Serialized)
  { 
    If(And(\_SB.PHRC,0x80))
    {
      Return(0x0009)
    }
    Else
    {
      Return(0x000B)
    }
  }
}
