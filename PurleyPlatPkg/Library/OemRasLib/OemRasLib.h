/**
@file  Implementation of the OEM RAS LIBRARY.

@copyright
  Copyright (c) 2011 - 2016 Intel Corporation. All rights
  reserved This software and associated documentation (if any)
  is furnished under a license and may only be used or copied in
  accordance with the terms of the license. Except as permitted
  by such license, no part of this software or documentation may
  be reproduced, stored in a retrieval system, or transmitted in
  any form or by any means without the express written consent
  of Intel Corporation.
**/

#ifndef _OEM_RAS_LIB_H_
#define _OEM_RAS_LIB_H_

#include <PiDxe.h>
#include <Setup/IioUniversalData.h>
#include <Protocol/PlatformRasPolicyProtocol.h>
#include <Library/DebugLib.h>

typedef struct {
  UINT8	    Bus;
  UINT8	    Device;   
  UINT8	    Function;
  BOOLEAN	IsRp;
  UINT8	    RpBus;
  UINT8	    RpDev;
  UINT8	    RpFunction;
  UINT32	VidDid;
  BOOLEAN	AerSupport;
  UINT32	ErrorConfigOverride;
} PCIE_DEVICE_OVERRIDE;

///
/// Function Prototypes
///

EFI_STATUS
OemOverrideRasTopologyStructure (
  IN OUT    EFI_RAS_SYSTEM_TOPOLOGY *Topology
  );  

EFI_STATUS
OemOverrideRasCapabilityStructure (
  IN OUT    PLATFORM_RAS_CAPABILITY *Capability
  );
  
EFI_STATUS
OemOverridePcieErrorLogging (
  IN OUT    PCIE_DEVICE_OVERRIDE *PciDevice
  );
  
EFI_STATUS
OemNotifyOs (
  IN      UINT8     ErrorSeverity,
     OUT  BOOLEAN   *OsNotified
  );
  
EFI_STATUS
OemOverrideRasSetupStructure (
  IN OUT    SYSTEM_RAS_SETUP *SetupStructure
  );


#endif
