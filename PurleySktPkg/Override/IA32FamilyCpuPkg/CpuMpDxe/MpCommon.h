//
// This file contains an 'Intel Peripheral Driver' and is
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may
// be modified by the user, subject to additional terms of the
// license agreement
//
/** @file

  Include files of common functions for CPU DXE module.

  Copyright (c) 2006 - 2017, Intel Corporation. All rights reserved.<BR>
  This software and associated documentation (if any) is furnished
  under a license and may only be used or copied in accordance
  with the terms of the license. Except as permitted by such
  license, no part of this software or documentation may be
  reproduced, stored in a retrieval system, or transmitted in any
  form or by any means without the express written consent of
  Intel Corporation.

  Module Name:  MpCommon.h

**/

#ifndef _MP_COMMON_H_
#define _MP_COMMON_H_

#include "ArchSpecificDef.h"
#include <AcpiCpuData.h>
#include <IndustryStandard/SmBios.h>

#include <PiPei.h>
#include <Ppi/SecPlatformInformation.h>
#include <Ppi/SecPlatformInformation2.h>

#include <Protocol/Smbios.h>
#include <Protocol/MpService.h>
#include <Protocol/LegacyBios.h>
#include <Protocol/GenericMemoryTest.h>
#include <Protocol/Cpu.h>
#include <Protocol/SmmConfiguration.h>
#include <Protocol/Timer.h>
#include <Protocol/TcgService.h>
#include <Protocol/DxeSmmReadyToLock.h>

#include <Guid/StatusCodeDataTypeId.h>
#include <Guid/HtBistHob.h>
#include <Guid/EventGroup.h>
#include <Guid/IdleLoopEvent.h>
#include <Guid/IA32FamilyCpuPkgTokenSpace.h>

#include <Library/BaseLib.h>
#include <Library/SynchronizationLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/HobLib.h>
#include <Library/HiiLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/ReportStatusCodeLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/PcdLib.h>
#include <Library/IoLib.h>
#include <Library/CpuConfigLib.h>
#include <Library/CpuLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/TimerLib.h>
#include <Library/CpuOnlyResetLib.h>
#include <Library/UefiCpuLib.h>
#include <Library/MtrrLib.h>
#include <Library/SocketLga775Lib.h>
#include <Library/DebugAgentLib.h>
#include <Library/SocketLga1156Lib.h>
#include <Library/LocalApicLib.h>
#include <Library/PrintLib.h>
//
// PURLEY_OVERRIDE_BEGIN
//
#include <Library/PlatformHooksLib.h>
#include "BdeSpecificHooks.h"
//
// PURLEY_OVERRIDE_END
//
#define INTERRUPT_HANDLER_MACHINE_CHECK       0x12

#define VACANT_FLAG           0x00
#define NOT_VACANT_FLAG       0xff

#define MICROSECOND         10

#define RSIZE_MAX  (PcdGet32 (PcdMaximumUnicodeStringLength))
#define ASCII_RSIZE_MAX  (PcdGet32 (PcdMaximumUnicodeStringLength))

#pragma pack(1)

typedef  struct {
  UINT16  LimitLow;
  UINT16  BaseLow;
  UINT8   BaseMiddle;
  UINT8   Attributes1;
  UINT8	  Attributes2;
  UINT8   BaseHigh;
} SEGMENT_DESCRIPTOR;

#pragma pack()

#define STARTUP_AP_SIGNAL        0x6E750000
//
// PURLEY_OVERRIDE_BEGIN
//
//
// The MONITOR_FILTER_SIZE should be equal or larger thatn 
// sizeof (MONITOR_MWAIT_DATA) and should be mulitple of 16 bytes,
// to make sure AP's stack is 16-byte alignment
//
#define MONITOR_FILTER_SIZE      0x40
//
// PURLEY_OVERRIDE_END
//
typedef enum {
  ApInHltLoop   = 1,
  ApInMwaitLoop = 2,
  ApInRunLoop   = 3
} AP_LOOP_MODE;

typedef struct {
  UINT32                    StartupApSignal;
  UINT32                    MwaitTargetCstate;
  AP_LOOP_MODE              ApLoopMode;
  BOOLEAN                   ReadyToBootFlag;
  BOOLEAN                   CStateEnable;
} MONITOR_MWAIT_DATA;
//
// PURLEY_OVERRIDE_BEGIN
//
//
// The MONITOR_FILTER_SIZE be mulitple of 16 bytes,
// to make sure AP's stack is 16-byte alignment on x64 arch.
//
//#define MONITOR_FILTER_SIZE      sizeof(MONITOR_MWAIT_DATA)
//
// PURLEY_OVERRIDE_END
//

typedef struct {
  UINT32                    ApicId;
  UINT32                    Bist;
} BIST_INFO;

//
// MTRR table definitions
//
typedef struct {
  UINT16  Index;
  UINT64  Value;
} EFI_MTRR_VALUES;

typedef
VOID
(EFIAPI *AP_PROC_ENTRY) (
  IN UINTN  ProcessorNumber
  );

typedef
VOID
(*AP_PROCEDURE) (
  IN UINTN  ProcessorNumber
  );

//
// Note that this structure is located immediately after the AP startup code
// in the wakeup buffer. this structure is not always writeable if the wakeup
// buffer is in the legacy region.
//
typedef struct {
  UINTN             Lock;
  VOID              *StackStart;
  UINTN             StackSize;
  AP_PROC_ENTRY     ApFunction;
  IA32_DESCRIPTOR   GdtrProfile;
  IA32_DESCRIPTOR   IdtrProfile;
  UINT32            BufferStart;
  UINT32            Cr3;
  UINT32            InitFlag;
  UINT32            ApCount;
  BOOLEAN           DcuModeSelection;
  UINT8             DcuMode;
  AP_LOOP_MODE      ApLoopMode;
  BOOLEAN           EnableExecuteDisable;
  //
  // The length of BIST_INFO array is PcdGet32 (PcdCpuMaxLogicalProcessorNumber).
  //
  BIST_INFO         BistBuffer[1];
} MP_CPU_EXCHANGE_INFO;

extern MP_CPU_EXCHANGE_INFO        *mExchangeInfo;
extern AP_PROCEDURE                mApFunction;
extern CPU_CONFIG_CONTEXT_BUFFER   mCpuConfigContextBuffer;
extern EFI_PHYSICAL_ADDRESS        mApMachineCheckHandlerBase;
extern UINT32                      mApMachineCheckHandlerSize;
extern BOOLEAN                     mEnableCState;
extern BOOLEAN                     mEnableMonitorMwait;
extern UINTN                       *mMaxCStateValue;
extern UINTN                       mLocalApicTimerDivisor;
extern UINT32                      mLocalApicTimerInitialCount;

/**
  The function will check if BSP Execute Disable is enabled.
  DxeIpl may have enabled Execute Disable for BSP,
  APs need to get the status and sync up the settings.

  @retval TRUE      BSP Execute Disable is enabled.
  @retval FALSE     BSP Execute Disable is not enabled.

**/
BOOLEAN
IsBspExecuteDisableEnabled (
  VOID
  );

/**
  Allocates startup vector for APs.

  This function allocates Startup vector for APs.

  @param  Size  The size of startup vector.

**/
VOID
AllocateStartupVector (
  UINTN   Size
  );

/**
  Creates a copy of GDT and IDT for all APs.

  This function creates a copy of GDT and IDT for all APs.

  @param  Gdtr   Base and limit of GDT for AP
  @param  Idtr   Base and limit of IDT for AP

**/
VOID
PrepareGdtIdtForAP (
  OUT IA32_DESCRIPTOR  *Gdtr,
  OUT IA32_DESCRIPTOR  *Idtr
  );

/**
  Allocate aligned ACPI NVS memory below 4G.

  This function allocates aligned ACPI NVS memory below 4G.

  @param  Size       Size of memory region to allocate
  @param  Alignment  Alignment in bytes

  @return Base address of the allocated region

**/
VOID*
AllocateAlignedAcpiNvsMemory (
  IN  UINTN         Size,
  IN  UINTN         Alignment
  );

/**
  Prepares Startup Vector for APs.

  This function prepares Startup Vector for APs.

**/
VOID
PrepareAPStartupVector (
  VOID
  );

/**
  Sets specified IDT entry with given function pointer.

  This function sets specified IDT entry with given function pointer.

  @param  FunctionPointer  Function pointer for IDT entry.
  @param  IdtEntry         The IDT entry to update.

  @return The original IDT entry value.

**/
UINTN
SetIdtEntry (
  IN  UINTN                       FunctionPointer,
  OUT INTERRUPT_GATE_DESCRIPTOR   *IdtEntry
  );

/**
  Allocate EfiACPIMemoryNVS below 4G memory address.

  This function allocates EfiACPIMemoryNVS below 4G memory address.

  @param  Size         Size of memory to allocate.

  @return Allocated address for output.

**/
VOID*
AllocateAcpiNvsMemoryBelow4G (
  IN   UINTN   Size
  );

/**
  Sends INIT-SIPI-SIPI to specific AP, and make it work on procedure specified by ApFunction.

  This function sends INIT-SIPI-SIPI to specific AP, and make it work on procedure specified by ApFunction.

  @param  Broadcast   If TRUE, broadcase IPI to all APs; otherwise, send to specified AP.
  @param  ApicID      The Local APIC ID of the specified AP.
  @param  ApFunction  The procedure for all APs to work on.

**/
VOID
SendInitSipiSipiIpis (
  IN BOOLEAN            Broadcast,
  IN UINT32             ApicID,
  IN AP_PROCEDURE       ApFunction
  );

/**
  Dispatches task to AP.

  This function dispatches task to AP. The BSP waits until specified APs have finished.

  @param  Broadcast   If TRUE, send task to all APs; otherwise, send to specified AP.
  @param  ApicID      The Local APIC ID of the specified AP. If Broadcast is TRUE, it is ignored.
  @param  Procedure   The procedure for AP to work on.

**/
VOID
DispatchAPAndWait (
  IN BOOLEAN             Broadcast,
  IN UINT32              ApicID,
  IN AP_PROCEDURE        Procedure
  );

/**
  Installs MP Services Protocol and register related timer event.

  This function installs MP Services Protocol and register related timer event.

**/
VOID
InstallMpServicesProtocol (
  VOID
  );

/**
  Protocol notification that is fired when LegacyBios protocol is installed.

  Re-allocate a wakeup buffer from E/F segment because the previous wakeup buffer
  under 640K won't be preserved by the legacy OS.

  @param  Event                 The triggered event.
  @param  Context               Context for this event.
**/
VOID
EFIAPI
ReAllocateMemoryForAP (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  );

/**
  Fixup jump instructions in the AP startup code.

  @param AddressMap    Pointer to MP_ASSEMBLY_ADDRESS_MAP.
  @param TargetBuffer  Target address of the startup vector.
  @param StartupVector AP startup vector address.
**/
VOID
RedirectFarJump (
  IN MP_ASSEMBLY_ADDRESS_MAP *AddressMap,
  IN EFI_PHYSICAL_ADDRESS    TargetBuffer,
  IN EFI_PHYSICAL_ADDRESS    StartupVector
  );

/**
  Protocol notification that will wake up and place AP to the suitable state
  before booting to OS.

  @param  Event                 The triggered event.
  @param  Context               Context for this event.
**/
VOID
EFIAPI
ChangeApLoopModeCallBack (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  );

/**
  Get mointor data address for specified processor.

  @param  ProcessorNumber    Handle number of specified logical processor.

  @return Pointer to monitor data.
**/
MONITOR_MWAIT_DATA *
EFIAPI
GetMonitorDataAddress (
  IN UINTN  ProcessorNumber
  );

#endif
