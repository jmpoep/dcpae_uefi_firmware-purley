//
// This file contains 'Framework Code' and is licensed as such
// under the terms of your license agreement with Intel or your
// vendor.  This file may not be modified, except as allowed by
// additional terms of your license agreement.
//
/*++
  
  Copyright (c)  2001 - 2015 Intel Corporation.  All rights reserved.

  This software and associated documentation (if any) is furnished under 
  a license and may only be used or copied in accordance with the terms 
  of the license.  Except as permitted by such license, no part of this 
  software or documentation may be reproduced, stored in a retrieval 
  system, or transmitted in any form or by any means without the express 
  written consent of Intel Corporation.
 
  Module Name:

    EXRPPlatform.asl

  Abstract:

    Contains root level name space objects for the platform

--*/

DefinitionBlock ("EXRPPlatform.asl","DSDT",2,"INTEL","PLATEXRP",3)
{
  #include "CommonPlatform.asi"
  #include "PlatformPciTree_EXRP.asi"
  #include "AMLUPD.asl"
  #include "DSDT.ASL"
  #include "Pch.asl"       //This is in another package (PchPkg)
  #include "Platform.asl"
  #include "PlatformGpe.asi"
  #include "PcieSeg.asi"
} // end of DSDT
