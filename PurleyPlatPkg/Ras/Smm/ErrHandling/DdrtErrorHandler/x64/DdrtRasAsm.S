#
# This file contains an 'Intel Peripheral Driver' and is
# licensed for Intel CPUs and chipsets under the terms of your
# license agreement with Intel or your vendor.  This file may
# be modified by the user, subject to additional terms of the
# license agreement
#
#------------------------------------------------------------------------------
#
# Copyright (c)  2004 - 2017 Intel Corporation. All rights reserved
# This software and associated documentation (if any) is furnished
# under a license and may only be used or copied in accordance
# with the terms of the license. Except as permitted by such
# license, no part of this software or documentation may be
# reproduced, stored in a retrieval system, or transmitted in any
# form or by any means without the express written consent of
# Intel Corporation.`
#
# Module Name:
#
#  DdrtRasAsm.S
#
# Abstract:
#
#  DDRT RAS ASM Code 
#



#------------------------------------------------------------------------------
# VOID *
# EFIAPI 
# AsmWriteFlushCacheLine (
#   IN      VOID                      *LinearAddress
#   );
#------------------------------------------------------------------------------
ASM_GLOBAL ASM_PFX(AsmWriteFlushCacheLine)
ASM_PFX(AsmWriteFlushCacheLine):
    movb    $0, (%rcx)
    clflush (%rcx)
    mov     %rcx, %rax
    ret

       