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

    WFPPlatform.asl

  Abstract:

    Contains root level name space objects for the platform

--*/

DefinitionBlock ("WFPPlatform.asl","DSDT",2,"INTEL","PLATWFR ",3)
{

  #include "CommonPlatform.asi"
  #include "PlatformPciTree_WFP.asi"
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
    Method(_PLD) { Return (GPLD(0,1)) } //BMC USB2.0 - internal - BMC USB2B
  }

  Scope (\_SB.PC00.XHCI.RHUB.HS02) {
    Method(_UPC) { Return (GUPC(1)) }
    Method(_PLD) { Return (GPLD(0,2)) } //BMC USB1.1 - internal - BMC USB2A
  }

  Scope (\_SB.PC00.XHCI.RHUB.HS03) {
    Method(_UPC) { Return (GUPC(1)) }
    Method(_PLD) { Return (GPLD(1,3)) } //BACK PANEL [CONN J3A1] - Upper - usb2 port 
  }

  Scope (\_SB.PC00.XHCI.RHUB.HS04) {
    Method(_UPC) { Return (GUPC(1)) }
    Method(_PLD) { Return (GPLD(1,4)) } //BACK PANEL [CONN J3A1] - Center - usb2 port
  }

  Scope (\_SB.PC00.XHCI.RHUB.HS05) {
    Method(_UPC) { Return (GUPC(1)) }
    Method(_PLD) { Return (GPLD(1,5)) } //BACK PANEL [CONN J3A1] - Bottom - usb2 port
  }

  Scope (\_SB.PC00.XHCI.RHUB.HS06) {
    Method(_UPC) { Return (GUPC(0)) }
    Method(_PLD) { Return (GPLD(0,0)) } //INTERNAL TYPE A USB3.0 [CONN J3B2] - internal - usb2 port (No BOM)
  }

  Scope (\_SB.PC00.XHCI.RHUB.HS07) {
    Method(_UPC) { Return (GUPC(0)) }
    Method(_PLD) { Return (GPLD(0,0)) } //
  }

  Scope (\_SB.PC00.XHCI.RHUB.HS08) {
    Method(_UPC) { Return (GUPC(1)) }
    Method(_PLD) { Return (GPLD(0,8)) } //INTERNAL TYPE A USB2.0 [CONN J3B3] - internal - usb2 port
  }

  Scope (\_SB.PC00.XHCI.RHUB.HS09) {
    Method(_UPC) { Return (GUPC(0)) }
    Method(_PLD) { Return (GPLD(0,0)) } //
  }

  Scope (\_SB.PC00.XHCI.RHUB.HS10) {
    Method(_UPC) { Return (GUPC(0)) }
    Method(_PLD) { Return (GPLD(0,0)) } //
  }

  Scope (\_SB.PC00.XHCI.RHUB.HS11) {
    Method(_UPC) { Return (GUPC(1)) }
    Method(_PLD) { Return (GPLD(1,11)) } //FRONT PANEL [CONN J8E2] - Front - usb2 port
  }

  Scope (\_SB.PC00.XHCI.RHUB.HS12) {
    Method(_UPC) { Return (GUPC(1)) }
    Method(_PLD) { Return (GPLD(1,12)) } //STORAGE FRONT PANEL BOARD [CONN J1D1] - Front - usb2 port
  }

  Scope (\_SB.PC00.XHCI.RHUB.HS13) {
    Method(_UPC) { Return (GUPC(1)) }
    Method(_PLD) { Return (GPLD(1,13)) } //FRONT PANEL [CONN J8E2] - Front - usb2 port
  }

  Scope (\_SB.PC00.XHCI.RHUB.HS14) {
    Method(_UPC) { Return (GUPC(1)) }
    Method(_PLD) { Return (GPLD(1,14)) } //STORAGE FRONT PANEL BOARD [CONN J1D1] - Front - usb2 port
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
    Method(_UPC) { Return (GUPC(1)) }
    Method(_PLD) { Return (GPLD(1,11)) } //FRONT PANEL [CONN J8E2] - Front - usb3 port 
  }

  Scope (\_SB.PC00.XHCI.RHUB.SS02) {
    Method(_UPC) { Return (GUPC(1)) }
    Method(_PLD) { Return (GPLD(1,13)) } //FRONT PANEL [CONN J8E2] - Front - usb3 port
  }

  Scope (\_SB.PC00.XHCI.RHUB.SS03) {
    Method(_UPC) { Return (GUPC(1)) }
    Method(_PLD) { Return (GPLD(1,3)) } //BACK PANEL [CONN J3A1] - Upper - usb3 port
  }

  Scope (\_SB.PC00.XHCI.RHUB.SS04) {
    Method(_UPC) { Return (GUPC(1)) }
    Method(_PLD) { Return (GPLD(1,4)) } //BACK PANEL [CONN J3A1] - Center - usb3 port
  }

  Scope (\_SB.PC00.XHCI.RHUB.SS05) {
    Method(_UPC) { Return (GUPC(1)) }
    Method(_PLD) { Return (GPLD(1,5)) } //BACK PANEL [CONN J3A1] - Bottom - usb3 port
  }

  Scope (\_SB.PC00.XHCI.RHUB.SS06) {
    Method(_UPC) { Return (GUPC(0)) }
    Method(_PLD) { Return (GPLD(0,0)) } //INTERNAL TYPE A USB3.0 [CONN J3B2] - internal - usb3 port (No BOM)
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
