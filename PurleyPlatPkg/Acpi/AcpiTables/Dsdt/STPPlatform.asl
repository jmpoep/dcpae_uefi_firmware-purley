//
// This file contains 'Framework Code' and is licensed as such
// under the terms of your license agreement with Intel or your
// vendor.  This file may not be modified, except as allowed by
// additional terms of your license agreement.
//
/*++

  Copyright (c)  2001 - 2016 Intel Corporation.  All rights reserved.

  This software and associated documentation (if any) is furnished under
  a license and may only be used or copied in accordance with the terms
  of the license.  Except as permitted by such license, no part of this
  software or documentation may be reproduced, stored in a retrieval
  system, or transmitted in any form or by any means without the express
  written consent of Intel Corporation.

  Module Name:

    STPPlatform.asl

  Abstract:

    Contains root level name space objects for the platform

--*/

DefinitionBlock ("STPPlatform.asl","DSDT",2,"INTEL","PLATSTP ",3)
{

  #include "CommonPlatform.asi"
  #include "PlatformPciTree_STP.asi"
  #include "AMLUPD.asl"
  #include "DSDT.ASL"
  #include "Pch.asl"       //This is in another package (PchPkg)
  #include "Platform.asl"
  #include "PlatformGpe.asi"
  #include "PcieSeg.asi"
 
  Scope (\_SB.PC00.XHCI.RHUB) {



    //
    // Method for creating generic _PLD buffers
    // _PLD contains lots of data, but for purpose of internal validation we care only about
    // ports' visibility and pairing (this requires group position)
    // so these are the only 2 configurable parameters (User Visible, Group Position)
    //
    Method(GPLD, 2, Serialized) {
      Name(PCKG, Package() { Buffer(0x10) {} } )
      CreateField(DerefOf(Index(PCKG,0)), 0, 7, REV)
      Store(1,REV)
      CreateField(DerefOf(Index(PCKG,0)), 64, 1, VISI)
      Store(Arg0, VISI)
      CreateField(DerefOf(Index(PCKG,0)), 87, 8, GPOS)
      Store(Arg1, GPOS)


      return (PCKG)
    }

    //
    // Method for creating generic _UPC buffers
    // Similar to _PLD, for internal testing we only care about 1 parameter (port connectable)
    //
    Method(GUPC, 1, Serialized) {
      Name(PCKG, Package(4) { 0, 0xFF, 0, 0 } )
      Store(Arg0,Index(PCKG,0))


      return (PCKG)
    }



  } //end scope RHUB

  Scope (\_SB.PC00.XHCI.RHUB.HS01) {
    Method(_UPC) { Return (GUPC(1)) }
    Method(_PLD) { Return (GPLD(1,1)) } //Back Panel A  - Left - [J8A2] usb2 port 
  }

  Scope (\_SB.PC00.XHCI.RHUB.HS02) {
    Method(_UPC) { Return (GUPC(1)) }
    Method(_PLD) { Return (GPLD(1,2)) } //Back Panel B  - Right - [J7A1] usb2 port
  }

  Scope (\_SB.PC00.XHCI.RHUB.HS03) {
    Method(_UPC) { Return (GUPC(1)) }
    Method(_PLD) { Return (GPLD(1,3)) } //Internal (BMC 1.1)
  }

  Scope (\_SB.PC00.XHCI.RHUB.HS04) {
    Method(_UPC) { Return (GUPC(1)) }
    Method(_PLD) { Return (GPLD(0,4)) } //Internal A1 [J1E3] usb2 port
  }

  Scope (\_SB.PC00.XHCI.RHUB.HS05) {
    Method(_UPC) { Return (GUPC(0)) }
    Method(_PLD) { Return (GPLD(0,0)) } //N/A
  }

  Scope (\_SB.PC00.XHCI.RHUB.HS06) {
    Method(_UPC) { Return (GUPC(0)) }
    Method(_PLD) { Return (GPLD(0,0)) } //N/A
  }

  Scope (\_SB.PC00.XHCI.RHUB.HS07) {
    Method(_UPC) { Return (GUPC(1)) }
    Method(_PLD) { Return (GPLD(1,7)) } //Internal (BMC 2.0)
  }

  Scope (\_SB.PC00.XHCI.RHUB.HS08) {
    Method(_UPC) { Return (GUPC(1)) }
    Method(_PLD) { Return (GPLD(1,8)) } //Back Panel B  - Right - [J7A1] usb2 port
  }

  Scope (\_SB.PC00.XHCI.RHUB.HS09) {
    Method(_UPC) { Return (GUPC(0)) }
    Method(_PLD) { Return (GPLD(0,0)) } //N/A
  }

  Scope (\_SB.PC00.XHCI.RHUB.HS10) {
    Method(_UPC) { Return (GUPC(1)) }
    Method(_PLD) { Return (GPLD(1,10)) } //Front Panel A  - Right - [J1J4] usb2 port
  }

  Scope (\_SB.PC00.XHCI.RHUB.HS11) {
    Method(_UPC) { Return (GUPC(1)) }
    Method(_PLD) { Return (GPLD(1,11)) } //Back Panel A  - Left - [J8A2] usb2 port
  }

  Scope (\_SB.PC00.XHCI.RHUB.HS12) {
    Method(_UPC) { Return (GUPC(1)) }
    Method(_PLD) { Return (GPLD(1,12)) } //Front Panel A  - Left - [J1J4] usb2 port
  }

  Scope (\_SB.PC00.XHCI.RHUB.HS13) {
    Method(_UPC) { Return (GUPC(0)) }
    Method(_PLD) { Return (GPLD(0,0)) } //N/A
  }

  Scope (\_SB.PC00.XHCI.RHUB.HS14) {
    Method(_UPC) { Return (GUPC(0)) }
    Method(_PLD) { Return (GPLD(0,0)) } //N/A
  }

  Scope (\_SB.PC00.XHCI.RHUB.USR1) {
    Method(_UPC) { Return (GUPC(0)) }
    Method(_PLD) { Return (GPLD(0,0)) }
  }

  Scope (\_SB.PC00.XHCI.RHUB.USR2) {
    Method(_UPC) { Return (GUPC(0)) }
    Method(_PLD) { Return (GPLD(0,0)) } 
  }

  Scope (\_SB.PC00.XHCI.RHUB.SS01) {
    Method(_UPC) { Return (GUPC(0)) }
    Method(_PLD) { Return (GPLD(0,0)) } //N/A 
  }

  Scope (\_SB.PC00.XHCI.RHUB.SS02) {
    Method(_UPC) { Return (GUPC(1)) }
    Method(_PLD) { Return (GPLD(1,2)) } //Back Panel B  - Right - [J7A1] usb3 port
  }

  Scope (\_SB.PC00.XHCI.RHUB.SS03) {
    Method(_UPC) { Return (GUPC(1)) }
    Method(_PLD) { Return (GPLD(1,8)) } //Back Panel B  - Right - [J7A1] usb3 port
  }

  Scope (\_SB.PC00.XHCI.RHUB.SS04) {
    Method(_UPC) { Return (GUPC(1)) }
    Method(_PLD) { Return (GPLD(0,4)) } //Internal A1 [J1E3] usb3 port
  }

  Scope (\_SB.PC00.XHCI.RHUB.SS05) {
    Method(_UPC) { Return (GUPC(1)) }
    Method(_PLD) { Return (GPLD(1,10)) } //Front Panel A  - Right - [J1J4] usb3 port
  }

  Scope (\_SB.PC00.XHCI.RHUB.SS06) {
    Method(_UPC) { Return (GUPC(1)) }
    Method(_PLD) { Return (GPLD(1,12)) } //Front Panel A  - Left - [J1J4] usb3 port
  }
  Scope (\_SB.PC00.XHCI.RHUB.SS07) {
    Method(_UPC) { Return (GUPC(0)) }
    Method(_PLD) { Return (GPLD(0,0)) } //N/A
  }

  Scope (\_SB.PC00.XHCI.RHUB.SS08) {
    Method(_UPC) { Return (GUPC(0)) }
    Method(_PLD) { Return (GPLD(0,0)) } //N/A
  }

  Scope (\_SB.PC00.XHCI.RHUB.SS09) {
    Method(_UPC) { Return (GUPC(0)) }
    Method(_PLD) { Return (GPLD(0,0)) } //N/A
  }

  Scope (\_SB.PC00.XHCI.RHUB.SS10) {
    Method(_UPC) { Return (GUPC(0)) }
    Method(_PLD) { Return (GPLD(0,0)) } //N/A
  }
  
} // end of DSDT
