/**
@file  Implementation of the OEM RAS LIBRARY.

@copyright
  Copyright (c) 2011 - 2014 Intel Corporation. All rights reserved
  This software and associated documentation (if any) is furnished
  under a license and may only be used or copied in accordance
  with the terms of the license. Except as permitted by such
  license, no part of this software or documentation may be
  reproduced, stored in a retrieval system, or transmitted in any
  form or by any means without the express written consent of
  Intel Corporation.
**/


#include "OemRasLib.h"

/**
  Override Ras Topology Structure

  This function overrides Ras Topology structure.

  @param[in out] *Topology          A structure pointer of Topology may need to be overridden

  @retval Status.

**/
EFI_STATUS
OemOverrideRasTopologyStructure (
  IN OUT   EFI_RAS_SYSTEM_TOPOLOGY *Topology
  )
{
  EFI_STATUS          Status;
  
  Status = EFI_SUCCESS;
  
  return Status;
}

/**
  Override Ras Capability Structure

  This function overrides Ras Capability structure.

  @param[in out] *Capability        A pointer of Capability structure may need to be overridden

  @retval Status.

**/
EFI_STATUS
OemOverrideRasCapabilityStructure (
  IN OUT      PLATFORM_RAS_CAPABILITY *Capability
  )
{
  EFI_STATUS          Status;
 
  Status = EFI_SUCCESS;
 
 
  return Status;
}


/**
  Override Pcie Error Logging 

  This function overrides Pcie Error Logging for a Pcie device

  @param[in out] *PciDevice         A pointer of PciDevice may need to be overridden

  @retval Status.

**/
EFI_STATUS
OemOverridePcieErrorLogging (
  IN OUT    PCIE_DEVICE_OVERRIDE *PciDevice
  )
{
  EFI_STATUS          Status;
  
  Status = EFI_SUCCESS;
  
  return Status;
}

/**
  Override Error Handling 

  This function overrides OS Notification

  @param[in] ErrorSeverity          Error Severity

  @retval Status.

**/
EFI_STATUS
OemNotifyOs (
  IN      UINT8     ErrorSeverity,
     OUT  BOOLEAN   *OsNotified
  )
{
  EFI_STATUS          Status;
  
  Status = EFI_SUCCESS;
  
  return Status;
}
  
/**
  Override RAS Setup Structure  

  This function overrides Setup Structure
  @param[in out] *SetupStructure         A pointer of Setup Stucture may need to be overridden

  @retval Status.

**/
EFI_STATUS
OemOverrideRasSetupStructure (
  IN OUT    SYSTEM_RAS_SETUP *SetupStructure
  )
{
  EFI_STATUS          Status;
  // bit c4 (Master Abort Error Enable) should be masked because of a known issue.
  SetupStructure->Iio.IioCoreErrorBitMap = (SetupStructure->Iio.IioCoreErrorBitMap) & 0xffffffef;
  Status = EFI_SUCCESS;
  
  return Status;
}