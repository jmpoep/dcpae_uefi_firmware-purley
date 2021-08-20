#
# This file contains an 'Intel Peripheral Driver' and is      
# licensed for Intel CPUs and chipsets under the terms of your
# license agreement with Intel or your vendor.  This file may 
# be modified by the user, subject to additional terms of the 
# license agreement                                           
#
#******************************************************************************
#*
# Copyright (c) 2006 - 2017, Intel Corporation. All rights reserved.<BR>
# This software and associated documentation (if any) is furnished
# under a license and may only be used or copied in accordance
# with the terms of the license. Except as permitted by such
# license, no part of this software or documentation may be
# reproduced, stored in a retrieval system, or transmitted in any
# form or by any means without the express written consent of
# Intel Corporation.

#*
#******************************************************************************

.equ                   VacantFlag,                0x0
.equ                   NotVacantFlag,             0xff
.equ                   StartupApSignal,           0x6E750000
#
# PURLEY_OVERRIDE_BEGIN
#
.equ                   MonitorFilterSize,         0x40
#
# PURLEY_OVERRIDE_END
#
.equ                   ApInHltLoop,               1
.equ                   ApInMwaitLoop,             2
.equ                   ApInRunLoop,               3

.equ                   LockLocation,              RendezvousFunnelProcEnd - RendezvousFunnelProcStart
.equ                   StackStartAddressLocation, RendezvousFunnelProcEnd - RendezvousFunnelProcStart + 0x08
.equ                   StackSizeLocation,         RendezvousFunnelProcEnd - RendezvousFunnelProcStart + 0x10
.equ                   CProcedureLocation,        RendezvousFunnelProcEnd - RendezvousFunnelProcStart + 0x18
.equ                   GdtrLocation,              RendezvousFunnelProcEnd - RendezvousFunnelProcStart + 0x20
.equ                   IdtrLocation,              RendezvousFunnelProcEnd - RendezvousFunnelProcStart + 0x2A
.equ                   BufferStartLocation,       RendezvousFunnelProcEnd - RendezvousFunnelProcStart + 0x34
.equ                   Cr3OffsetLocation,         RendezvousFunnelProcEnd - RendezvousFunnelProcStart + 0x38
.equ                   InitFlagLocation,          RendezvousFunnelProcEnd - RendezvousFunnelProcStart + 0x3C
.equ                   ApCountLocation,           RendezvousFunnelProcEnd - RendezvousFunnelProcStart + 0x40
.equ                   DcuModeSelectFlagLocation, RendezvousFunnelProcEnd - RendezvousFunnelProcStart + 0x44
.equ                   DcuModeLocation,           RendezvousFunnelProcEnd - RendezvousFunnelProcStart + 0x45
.equ                   ApLoopModeLocation,        RendezvousFunnelProcEnd - RendezvousFunnelProcStart + 0x48
.equ                   EnableExecuteDisable,      RendezvousFunnelProcEnd - RendezvousFunnelProcStart + 0x4C
.equ                   BistBuffer,                RendezvousFunnelProcEnd - RendezvousFunnelProcStart + 0x50

#-------------------------------------------------------------------------------
