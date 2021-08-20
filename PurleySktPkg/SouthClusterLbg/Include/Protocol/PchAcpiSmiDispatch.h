/** @file
  APIs of PCH ACPI SMI Dispatch Protocol.

@copyright
 Copyright (c) 2014 Intel Corporation. All rights reserved
 This software and associated documentation (if any) is furnished
 under a license and may only be used or copied in accordance
 with the terms of the license. Except as permitted by the
 license, no part of this software or documentation may be
 reproduced, stored in a retrieval system, or transmitted in any
 form or by any means without the express written consent of
 Intel Corporation.
 This file contains an 'Intel Peripheral Driver' and is uniquely
 identified as "Intel Reference Module" and is licensed for Intel
 CPUs and chipsets under the terms of your license agreement with
 Intel or your vendor. This file may be modified by the user, subject
 to additional terms of the license agreement.

@par Specification Reference:
**/
#ifndef _PCH_ACPI_SMI_DISPATCH_PROTOCOL_H_
#define _PCH_ACPI_SMI_DISPATCH_PROTOCOL_H_

//
// Extern the GUID for protocol users.
//
extern EFI_GUID                         gPchAcpiSmiDispatchProtocolGuid;

//
// Forward reference for ANSI C compatibility
//
typedef struct _PCH_ACPI_SMI_DISPATCH_PROTOCOL    PCH_ACPI_SMI_DISPATCH_PROTOCOL;

//
// Member functions
//

/**
  Callback function for an PCH ACPI SMI handler dispatch.

  @param[in] DispatchHandle             The unique handle assigned to this handler by register function.

**/
typedef
VOID
(EFIAPI *PCH_ACPI_SMI_DISPATCH_CALLBACK) (
  IN EFI_HANDLE                         DispatchHandle
  );

/**
  Register a child SMI source dispatch function for PCH ACPI SMI events.

  @param[in] This                       Protocol instance pointer.
  @param[in] DispatchFunction           Pointer to dispatch function to be invoked for
                                        this SMI source
  @param[out] DispatchHandle            Handle of dispatch function, for when interfacing
                                        with the parent SMM driver.

  @retval EFI_SUCCESS                   The dispatch function has been successfully
                                        registered and the SMI source has been enabled.
  @retval EFI_DEVICE_ERROR              The driver was unable to enable the SMI source.
  @retval EFI_OUT_OF_RESOURCES          Not enough memory (system or SMM) to manage this child.
  @retval EFI_ACCESS_DENIED             Return access denied if the EndOfDxe event has been triggered
**/
typedef
EFI_STATUS
(EFIAPI *PCH_ACPI_SMI_DISPATCH_REGISTER) (
  IN  PCH_ACPI_SMI_DISPATCH_PROTOCOL    *This,
  IN  PCH_ACPI_SMI_DISPATCH_CALLBACK    DispatchFunction,
  OUT EFI_HANDLE                        *DispatchHandle
  );

/**
  Unregister a child SMI source dispatch function with a parent ACPI SMM driver

  @param[in] This                       Protocol instance pointer.
  @param[in] DispatchHandle             Handle of dispatch function to deregister.

  @retval EFI_SUCCESS                   The dispatch function has been successfully
                                        unregistered and the SMI source has been disabled
                                        if there are no other registered child dispatch
                                        functions for this SMI source.
  @retval EFI_INVALID_PARAMETER         Handle is invalid.
  @retval EFI_ACCESS_DENIED             Return access denied if the EndOfDxe event has been triggered
**/
typedef
EFI_STATUS
(EFIAPI *PCH_ACPI_SMI_DISPATCH_UNREGISTER) (
  IN  PCH_ACPI_SMI_DISPATCH_PROTOCOL    *This,
  IN  EFI_HANDLE                        DispatchHandle
  );

/**
  Interface structure for PCH ACPI SMIs Dispatch Protocol
  The PCH ACPI SMI DISPATCH PROTOCOL provides the ability to dispatch function for PCH ACPI related SMIs.
  It contains SMI types of Pme, RtcAlarm, PmeB0, and Time overflow.
**/
struct _PCH_ACPI_SMI_DISPATCH_PROTOCOL {
  /**
    This member specifies the revision of this structure. This field is used to
    indicate backwards compatible changes to the protocol.
  **/
  UINT8                                 Revision;
  /**
    Smi unregister function for PCH ACPI SMI DISPATCH PROTOCOL.
  **/
  PCH_ACPI_SMI_DISPATCH_UNREGISTER      UnRegister;
  /**
    Pme
    The event is triggered by hardware when the PME# signal goes active.
    Additionally, the event is only triggered when SCI_EN is not set.
  **/
  PCH_ACPI_SMI_DISPATCH_REGISTER        PmeRegister;
  /**
    PmeB0
    The event is triggered PCH when any internal device with PCI Power Management
    capabilities on bus 0 asserts the equivalent of the PME# signal.
    Additionally, the event is only triggered when SCI_EN is not set.
    The following are internal devices which can set this bit:
    Intel HD Audio, Intel Management Engine "maskable" wake events, Integrated LAN,
    SATA, xHCI, Intel SST
  **/
  PCH_ACPI_SMI_DISPATCH_REGISTER        PmeB0Register;
  /**
    RtcAlarm
    The event is triggered by hardware when the RTC generates an alarm
    (assertion of the IRQ8# signal).
  **/
  PCH_ACPI_SMI_DISPATCH_REGISTER        RtcAlarmRegister;
  /**
    TmrOverflow
    The event is triggered any time bit 22 of the 24-bit timer goes high
    (bits are numbered from 0 to 23).
    This will occur every 2.3435 seconds. When the TMROF_EN bit (ABASE + 02h, bit 0) is set,
    then the setting of the TMROF_STS bit will additionally generate an SMI#
    Additionally, the event is only triggered when SCI_EN is not set.
  **/
  PCH_ACPI_SMI_DISPATCH_REGISTER        TmrOverflowRegister;
};

/**
  PCH ACPI SMI dispatch revision number

  Revision 1:   Initial version
**/
#define PCH_ACPI_SMI_DISPATCH_REVISION            1

#endif
