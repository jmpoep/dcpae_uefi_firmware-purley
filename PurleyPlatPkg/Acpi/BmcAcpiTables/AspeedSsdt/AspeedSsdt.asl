/** @file
  ACPI SSDT table

@copyright
 Copyright (c) 2015 Intel Corporation. All rights reserved
 This software and associated documentation (if any) is furnished
 under a license and may only be used or copied in accordance
 with the terms of the license. Except as permitted by the
 license, no part of this software or documentation may be
 reproduced, stored in a retrieval system, or transmitted in any
 form or by any means without the express written consent of
 Intel Corporation.
 This file contains a 'Sample Driver' and is licensed as such
 under the terms of your license agreement with Intel or your
 vendor. This file may be modified by the user, subject to
 the additional terms of the license agreement.

@par Specification Reference:
**/

DefinitionBlock (
    "AspeedSsdt.aml",
    "SSDT",
    0x02,         // SSDT revision.
                  // A Revision field value greater than or equal to 2 signifies that integers 
                  // declared within the Definition Block are to be evaluated as 64-bit values
    "INTEL",      // OEM ID (6 byte string)
    "ASPEEDTL",   // OEM table ID  (8 byte string)
    0x0           // OEM version of DSDT table (4 byte Integer)
    )
{

  External(\_SB.PC00.LPC0, DeviceObj)
  
  Scope (\_SB.PC00.LPC0) 
  {
    #include "Aspd.asi"
  } 

}
