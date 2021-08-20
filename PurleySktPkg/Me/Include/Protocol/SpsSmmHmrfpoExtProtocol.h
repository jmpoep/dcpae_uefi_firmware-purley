/*++
  This file contains a 'Sample Driver' and is licensed as such
  under the terms of your license agreement with Intel or your
  vendor.  This file may be modified by the user, subject to
  the additional terms of the license agreement.
--*/
/*++
Copyright (c) 2019 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

Module Name:

 SpsSmmHmrfpoExtProtocol.h

Abstract:
--*/
#ifndef _SPS_SMM_HMRFPO_EXT_PROTOCOL_H_
#define _SPS_SMM_HMRFPO_EXT_PROTOCOL_H_

#include <Sps.h>

extern EFI_GUID gSpsSmmHmrfpoExtProtocolGuid;

typedef enum {
  HmrfpoLockType0     // Full ME Region lock without region validation
} HMRFPO_EXT_LOCK_TYPE;

typedef enum {
  HmrfpoStatusDisabled  = 0x00,
  HmrfpoStatusLocked    = 0x01,
  HmrfpoStatusEnabled   = 0x02
} HMRFPO_EXT_STATUS;

typedef enum {
  HmrfpoEnableRegionIntelMe   = 0x00
} HMRFPO_EXT_REGION_SELECT;

typedef enum {
  HmrfpoLegacyReset           = 0x00
} HMRFPO_EXT_RESET_TYPE;


typedef EFI_STATUS (EFIAPI *HMRFPO_EXT_LOCK_FUNC) (
  VOID
  );

typedef EFI_STATUS (EFIAPI *HMRFPO_EXT_ENABLE_FUNC) (
  IN  HMRFPO_EXT_REGION_SELECT  RegionSelect,
  OUT UINT32                    *FactoryBase,
  OUT UINT32                    *FactoryLimit
  );

typedef EFI_STATUS (EFIAPI *HMRFPO_EXT_MERESET_FUNC) (
  IN HMRFPO_EXT_RESET_TYPE  ResetType
  );

typedef EFI_STATUS (EFIAPI *HMRFPO_EXT_LOCK_IMMEDIATELY_FUNC) (
  IN HMRFPO_EXT_LOCK_TYPE Type
  );

typedef EFI_STATUS (EFIAPI *HMRFPO_EXT_GET_STATUS_FUNC) (
  OUT HMRFPO_EXT_STATUS *ExtStatus
  );

typedef struct _SPS_SMM_HMRFPO_EXT_PROTOCOL_ {

  ME_FW_VERSION   ActiveFwVersion;     // Active operational firmware
  ME_FW_VERSION   RecoveryFwVersion;   // Recovery firmware
  ME_FW_VERSION   BackupFwVersion;     // Backup operational firmware (optional)

  HMRFPO_EXT_LOCK_FUNC              Lock;
  HMRFPO_EXT_ENABLE_FUNC            Enable;
  HMRFPO_EXT_MERESET_FUNC           MeReset;
  HMRFPO_EXT_LOCK_IMMEDIATELY_FUNC  LockImmediately;
  HMRFPO_EXT_GET_STATUS_FUNC        GetStatus;

} SPS_SMM_HMRFPO_EXT_PROTOCOL;

#endif // _SPS_SMM_HMRFPO_EXT_PROTOCOL_H_

