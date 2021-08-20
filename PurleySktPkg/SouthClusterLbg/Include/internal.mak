#
# This file contains 'Framework Code' and is licensed as such
# under the terms of your license agreement with Intel or your
# vendor.  This file may not be modified, except as allowed by
# additional terms of your license agreement.
#
#**************************************************************************
#*                                                                        *
#*      Intel Restricted Secret                                           *
#*                                                                        *
#*      Reference Code                                                    *
#*                                                                        *
#*      ESS - Enterprise Silicon Software                                 *
#*                                                                        *
#*      Copyright (c) 2014 Intel Corp.                                    *
#*                                                                        *
#*      This program has been developed by Intel Corporation.             *
#*      Licensee has Intel's permission to incorporate this source code   *
#*      into their product, royalty free.  This source code may NOT be    *
#*      redistributed to anyone without Intel's written permission.       *
#*                                                                        *
#*      Intel specifically disclaims all warranties, express or           *
#*      implied, and all liability, including consequential and other     *
#*      indirect damages, for the use of this code, including liability   *
#*      for infringement of any proprietary rights, and including the     *
#*      warranties of merchantability and fitness for a particular        *
#*      purpose.  Intel does not assume any responsibility for any        *
#*      errors which may appear in this code nor any responsibility to    *
#*      update it.                                                        *
#*                                                                        *
#**************************************************************************
#**************************************************************************
#*                                                                        *
#*  PURPOSE:                                                              *
#*                                                                        *
#*      Makefile for Reference Code.                                      *
#*                                                                        *
#**************************************************************************

!INCLUDE "..\..\BuildOptions.mak"

#--------------------------------------------------------------------
EXT_FILES = \
        PchReservedResources.h        \
        Register/PchRegsDmi.h         \
        Register/PchRegsPcr.h         \
        Register/PchRegsPmc.h         \
        Register/PchRegsPcie.h


INT_FILES =  

#--------------------------------------------------------------------
INT_HY_RELEASE: $(EXT_FILES) $(INT_FILES)
        @!$(MINITOOLS)unifdef $(UNIFDEF_INTHY_FLAGS) $** > $(TARGET_DIR)\$**

INT_EMU_RELEASE: $(EXT_FILES) $(INT_FILES)
        @!$(MINITOOLS)unifdef $(UNIFDEF_INTEMU_FLAGS) $** > $(TARGET_DIR)\$**

INT_RC_RELEASE: $(EXT_FILES) $(INT_FILES)
        @!$(MINITOOLS)unifdef $(UNIFDEF_INT_FLAGS) $** > $(TARGET_DIR)\$**

EXT_IDT_RELEASE         \
EXT_INPHI_RELEASE       \
EXT_RC_RELEASE: $(EXT_FILES)
        @!$(MINITOOLS)unifdef $(UNIFDEF_EXT_FLAGS) $** > $(TARGET_DIR)\$**

