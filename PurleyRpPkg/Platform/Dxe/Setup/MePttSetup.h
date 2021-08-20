/**@file

@copyright
 Copyright (c) 2019 Intel Corporation. All rights reserved
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

#ifndef __ME_PTT_SETUP_H__
#define __ME_PTT_SETUP_H__

/**
  This function configures PTT on Setup enter

  @param[in out] MeRcConfiguration - ME FW RC Configuration Data
**/
VOID
SetPttOnSetupEnter (
 IN OUT ME_RC_CONFIGURATION *MeRcConfiguration
 );

/**
  This function updates PTT state

  @param[in out] MeRcConfiguration - ME FW RC Configuration Data
**/
VOID
UpdatePttState (
 IN OUT ME_RC_CONFIGURATION *MeRcConfiguration
 );

#endif // __ME_PTT_SETUP_H__
