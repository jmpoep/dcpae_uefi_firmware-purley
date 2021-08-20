![Intel Corporation](img/intel-logo.png)

# Purley-EP RAS Sample Code

# User&#39;s Guide

**_Dec. 2014_**

**_Revision 0.5_**

**Intel Confidential**

- - -

## Contents

[TOC]

- - -

## Revision History

Rev. No. | Description      					                   | Date
---------|-----------------------------------------------|---------
0.4      | Initial revision 					                   |
0.5      | Updates as per 0.5 RAS Reference Code release | Dec 2014

- - -

## Legal Notice

**Notice: This document contains information on products in the design phase
of development. The information here is subject to change without notice.
Do not finalize a design with thin information.**

Intel technologies&#39; features and benefits depend on system configuration
and may require enabled hardware, software, or service activation. Learn more
at intel.com, or from the OEM or retailer. No computer system can be absolutely
secure. Intel does not assume any liability for lost or stolen data or systems
or any damages resulting from such losses.

You may not use or facilitate the use of this document in connection with any
infringement or other legal analysis concerning Intel products described herein.
You agree to grant Intel a non-exclusive, royalty-free license to any patent
claim thereafter drafted which includes subject matter disclosed herein.

No license (express or implied, by estoppel or otherwise) to any intellectual
property rights is granted by this document.

The products described may contain design defects or errors known as errata
which may cause the product to deviate from published specifications. Current
characterized errata are available on request.

This document contains information on products, services and/or processes in
development. All information provided here is subject to change without notice.
Contact your Intel representative to obtain the latest Intel product specifications
and roadmaps.

Intel disclaims all express and implied warranties, including without limitation,
the implied warranties of merchantability, fitness for a particular purpose, and
non-infringement, as well as any warranty arising from course of performance, course
of dealing, or usage in trade.

Copies of documents which have an order number and are referenced in this document
may be obtained by calling 1-800-548-4725 or by visiting
[www.intel.com/design/literature.htm](http://www.intel.com/design/literature.htm).

Intel, the Intel logo, and Xeon are trademarks of Intel Corporation in the U.S. 
and/or other countries.

*Other names and brands may be claimed as the property of others.

_Copyright &copy; 2015, Intel Corporation. All Rights Reserved._

- - -

## Introduction

This document accompanies the RAS Sample Code. The primary purpose of
this document is to describe the contents of the RAS Sample code. In
addition this document describes the integration methods for Independent
BIOS Vendors (IBVs) and Intel OEM customers.

### RAS Sample Code package contents

The RAS Sample code package is a collection of modules that gives OEM a
reference on how the RAS sample code is arranged. This also gives an
idea on what are specific functionality of modules and files are
required for supporting RAS features.

This code is used directly in Intel validation and CRB BIOS, and it is
expected to be compatible with all platforms that follow the Platform
Design Guide. The code is platform and codebase dependent and is not
expected to be compatible with all platforms and customer codebase

- - -

## RAS Code Structure

RAS reference code is structured for modularity and configurability as
described in the following sections:

### Modularity

In the reference implementation, RAS code is divided into:

* Silicon code.
* Platform code.
* Common code.

For a given {CRB} platform, the Silicon specific code resides in
folder {CRB}SktPkg package folder. For example, in case of {Purley
CRB}, the Silicon RAS content resides in PurleySktPkg. It consists of
drivers and libraries which directly operate silicon registers and
generally require new development or porting for every new processor
generation.

The Platform specific code such as GPIO or User Setup is expected to
reside in {CRB}PlatPkg. For Purley, the platform specific code is in
PurleyPlatPkg.

The Common code is expected to be independent of either Silicon or
Platform. It interfaces with the silicon and platform content only
through published driver protocols or libraries residing in
{CRB}SktPkg and {CRB}PlatPkg respectively.

_(**Note:** In the current reference release of Purley, the Common
code is distributed across PlatPkg and SktPkg. In later platforms, the
Common code may move to a separate Common RAS Package.)_

### Figure RAS Code Modularity

The following _Figure 1: RAS Code Modularity_ illustrates the modular
design of RAS code:

![Intel Corporation](img/EP_RAS/Figure1.png)

### RAS interfaces and data structures

The modules described in previous Section: _RAS Code, Modularity_
interconnect through externally published interfaces as illustrated in:
_Figure 2: Module Interface (Boot Time)_ and _Figure 3: Module
Interface (Run Time)_, Topology and policies from platform code to
silicon code are passed through the RAS policy interfaces. Common and
silicon code libraries publish APIs for RAS primitives.

### Figure Module Interface (Boot Time)

![Intel Corporation](img/EP_RAS/Figure2.png)

### Figure Module Interface (Run Time)

![Intel Corporation](img/EP_RAS/Figure3.png)

### RAS Files Structure

RAS sample code consists of PEI/DXE drivers required for Boot Time
Initialization of RAS functions and SMM for Runtime handling.

The RAS driver module files are located across following packages:

* {CRB}**PlatPkg**
* {CRB}**SktPkg**

#### {CRB}PlatPkg\\Ras

This package consists of following folders:

##### Include

.\\Ras\\Include is a common folder, consisting of common definition
Include files used by different RAS modules.

##### Library

Ras\\Library consists of libraries required by other RAS modules.
Following are the current set of libraries:

* **MpSyncDataLib**: It contains multiprocessor (MP)
configuration procedures

* **Emcalib**: It contains implementation of library functions
required by EMCA driver such as `SignalCmciToOs()` and
`ReadCoreMcBankBitField()`.

##### Smm

This folder contains common SMI based Runtime error handling drivers:

* **Error handling**

It has drivers organized as per following directory structures:

* **ChipsetErrorHandler:** This driver module contains Common PCIe,
IIO, and PCH Error handlers.

* **MainErrorHandler:** It is then main Common error handler driver
module which is responsible for initialization and registration of all
the other runtime error handlers.

* **PlatformErrorHandler:** This module consists of various functions
which interface with the Platform dependencies such as extracting
platform specific Setup and topology info into RAS specific structures
for runtime access. It is also where the Platform specific error logging
functions can register themselves for custom logging.

* **ProcessorErrorHandler:** This module contains the error handlers,
which check and report processor and memory related errors conditions.
It consists of:

    * **Processor** Error Handler.
    * **Memory** Error Hander.
    * **MCA** Error Handler.
    * **FNV** Error Handler.

* **McBankErrorInjecion**

This driver provides MCBANK error injection capabilities by creating
necessary ACPI structures.

* **PartialMirrorHandler**

Partial Mirror Handler module contains code for handling Address based
Partial Mirror handling. It contains functions to initialize partial
Mirror SMI based handler, set Mirror configurations and assert GPI.

* **RasInit**

This module contains code for initializing basic framework required for
the support of RAS Advanced features, including CPU, Memory and IIO
online.

##### WHEA

* RAS WHEA folder contains code related to WHEA Error reporting. It contains
code to initialize WHEA Error logger and WHEA OS notification hooks.

##### EMCA

This module contains functions to initialize EMCA Error handling and
logging errors:

* **EmcaErrorLog.c**

    * **InitializeEmcaErrorLogger** This function initializes the
    Common EMCA error handling and logging

    * **LogErrorToElog** This function is responsible for logging
    errors via the Elog interface.

* **EmcaErrorLog.h**

    * Prototypes for functions defined by EmcsErrorLog.c

This module is supported by library module Ras\\Library\\emcalib and
Ras\\Library\\mpsyncdatalib

#### {CRB}SktPkg

This package contains the following folders:

##### MemRas

MemRas folder contains modules which supports following memory RAS
features:

* ADDDC Sparing detect, enable and runtime event handling
* Memory RAS event registration and event handling
* Rank Sparing check and event handling
* Patrol Scrub enabling and event handing

##### Library\\ChipsetErrReporting

This {CRB}SktPkg library contains silicon specific procedures to
support the common Chipset Error Handler. It contains procedures to
configure, detect and clear IIO PCIe errors.

##### Smm\\Ras

This module folder contains silicon specific SMM Runtime libraries.

- - -

## Memory RAS

### SDDC Sparing

* **DramDeviceCorrection.c**

    * **InitializeSddcSparing** Initializes all SDDC sparing
    related data structures

    * **HandleSddcSparingEventDone** Handles SDDC sparing done
    event. Reports success if DDDC sparing operation completes without any
    failures

    * **HandleNewSddcSparingEventsIfAny** Checks for new SDDC sparing
    events and handles them accordingly. Ranks are checked for Rank Sparing
    compatibility and Spare copy initiated

* **DramDeviceCorrection.h**

    * Prototypes for functions defined by DramDeviceCorrection.c

* **SparingEngine.c**

    * **AcquireSparingEngine** Checks the internal status and takes
    ownership of the sparing engine

    * **ReleaseSparingEngine** Checks the internal status and releases
    ownership of the sparing engine. It is expected that only caller who
    has successfully acquired ownership using AcquireSparingEngine calls
    this function

    * **StartSparing** Programs the hardware registers for DDDC or
    Rank Sparing and starts spare copy operation

    * **IsCopyComplete** Checks the hardware registers for spare
    copy completion and returns status

* **SparingEngine.h**

    * Definitions and declarations used in SparingEngine.c

### ADDC Sparing

* **AddcSparing.c**

    * **InitializeDbdcSparing** Initialize all double Bank ADDDC
    related data structures for specified Node.

    * **IsFailureOccuredOnSameBank** This routine returns TRUE if there
    is another failure on the same Bank, else it returns FALSE.

    * **IsAvailableFreeAdddcRegion**  This routine returns TRUE if an
    ADDDC Region is available, otherwise it returns false.

    * **CheckandHandleNewAdddcSparingEvent**  Checks for new DBDC events
    and handle it including whether it was first, second or third hard
    failure.

    * **HandleDbdcSparingDoneEvent:** Handles DBDC sparing event done
    event. Clears error count and moves Rank status to Spare done from an
    in-progress status.

    * **IsAdddcAvailable:** Checks if a Rank can support ADDDC
    sparing.

### Memory RAS Driver

* **MemRas.c**

    * **InitializeMemRas** Initializes Memory RAS Support module and
    required protocols. This routine is entry point of the Memory Ras
    driver and installs protocols to support functions such as Sparing,
    Patrol Scrub, Address translation, Dimm translation.

* **MemRas.h**

    * Definitions / function prototypes for MemRasRoutines.c

* **MemRasRoutines.c**

    * **SktHaToHaNodeId**  Supporting routine to get HA Node Id from
    Socket ID and MC.
    
    * **InitializeMemRasData** Initializes all the global data
    structures/variables required for MemRas routines.
    
    * **SaveErrInfo** Saves the DDRCH/DIMM/Rank information for last
    error.
    
    * **GetLastErrInfo** Return DDRCH/DIMM/Rank that had the most
    recent error.

    * **IsErrorExcdForThisRank** Checks if the specified rank has
    error overflow.

    * **ClearErrorsForThisRank** Clears error counters and over flow
    status for specified rank in CPU.

    * **GetBitmapOfNodeWithErrors** Checks the error status
    registers and returns a bitmap of Memory NodeIDs that have errors. A
    copy of the bitmap is also stored for internal use.

    * **GetBitMapOfNodeWithEventInProgress** Returns a bitmap of
    Memory NodeIDs with event handling already in progress.

    * **GetOrSetErrorHandlingStatus** Updates / Gets error handling
    status for specified NodeID.

    * **OpenPamRegion** Open PAM region for DRAM RW access.

    * **RestorePamRegion** Restore PAM region with previously saved
    settings.
    
    * **IsMemNodeEnabled** Return if specified memory Node is
    enabled / disabled.
    
    * **EnableErrorSignallingtoBIOS** Setup registers to signal
    errors to BIOS.
    
    * **GetCurrentMemoryNodeState** Get the current state of a
    memory Node.

    * **TranslateRIR**  Determine RankAddress, DIMM Slot, DIMM Rank and
    Physical Rank ID of specified Address. Used in translation of
    System Address to DIMM Address.

    * **TranslateTAD**  Determine ChannelAddress and Channel ID; used in
    translation of System Address to DIMM Address.

    * **TranslateSAD**  Determine HA NODE ID, Socket ID and Memory
    controller ID of specified address; used in translation of System
    Address to DIMM Address

    * **GetInterleaveListIndex** Given the Interleave Mode return the
    interleave index.

    * **GetTargetId** Given the SAD\#, socket\#, InterleaveListIndex
    returns the target id

    * **GetLogicalChId** Get the logical channel id for the given target
    id

    * **ExtractOrStuffBankGroupBits** Extract Bank address bits from
    Rank Address or Stuff bank address bits to form Rank Address based
    on flag.

    * **ExtractOrStuffBankBits**  Based on the Opcode, derives Rank
    Address from Bank address bits, or bank Address bits from Rank
    Address bits.

    * **ExtractOrStuffRowBits**  Based on the opcode, derives Rank
    address from Row address bits, or Row Address bits from Rank Address
    bits.

    * **ExtractOrStuffColBits**  Based on the opcode, derives Rank
    Address from Col address bits, or Col Address bits from Rank Address
    bits.

    * **TranslateRankAddress**  Determine DIMM Row, Col and Bank address
    bits from Rank Address.

    * **SystemAddressToDimmAddress**  High level function to translate
    specified System Address to DIMM Address.

    * **MapDimmAddressToRankAddress**  Determines Rank Address from
    specified DIMM Address; used in converting DIMM Address to System
    Address.

    * **MapRankAddressToChannelAddress**  Determines Channel Address from
    specified Rank Address; used in converting DIMM Address to System
    Address.

    * **MapChannelAddressToSystemAddress**  Determines System Address
    from specified Channel Address; used in converting DIMM Address to
    System Address.

    * **DimmAddressToSystemAddress**  High level routine to function to
    translate specified DIMM Address to System Address.

    * **IsMemNodeEnabled** Checks if current memory node is enabled.

    * **ClearUnhandledCorrError** Clears unhandled corrected errors.

    * **GetLogicalToPhysicalRankId** Get Logical to Physical rank ID.

    * **GetSupportedMemRasModes** Get Memory RAS MODES supported such as
    SDDC, ADDDC and Rank Spare.

* **MemRasS3Save.c**

    * Memory RAS S3 related routines.

### Patrol Scrub

* **PatrolScrub.c**

    * **CheckPatrolScrubEnable** Check if patrol scrub is enabled or not

    * **ReEnablePatrolScrubEngine** Enable Patrol scrub engine to
    start from the last Address it had scrubbed

    * **DisablePatrolScrubEngine** Disable Patrol scrub engine and
    save patrol address registers

    * **UpdateAMAP** Map out the non-spare ranks for the Scrub
    engine

    * **ReEnableSSR** Re-enable the scrub engine after the rank
    sparing is complete 

    * **GetNextAvailableNonSprRankForPatrol** Returns the first
    available non spared rank

    * **DoRankScrubbing** Does the rank scrubbing

    * **ScrubNextRank** Start scrub for each rank

    * **HandlePatrolCompleteEventForNode** Handles the patrol scrub
    event for the node

    * **HandlePatrolUCEventForNode** Check to make sure the rank has
    uncorrected error

    * **CheckAndHandlePatrolEvent** Checks whether the node has
    completed all the patrol scrub event 

    * **EnablePatrolScrubForPoisonDetection** Sets up patrol scrub
    engine to do detect poisoned lines

    * **HasPatrolStoppedOnError** Checks if Patrol engine has stopped on
    an error

* **PatrolScrub.h**

    * Declarations for functions defined in PatrolScrub.c

### Rank Sparing

* **RankSparing.c**

    * **ParseRankState** Parses Rank state
    
    * **GetRankState** Gets state of a specific Rank
    
    * **IsSpareRankAvailable** Checks if spare rank is available
    
    * **SetRankState** Sets rank new status
    
    * **CheckRankState** Checks if current tank is in specific state
    
    * **CheckRankStatePerCh** Checks if current channel has specific
    state rank
    
    * **CheckRankStatePerMC** Checks if current MC has specific state
    rank

    * **GetLowestPrioritySpareRank** Get the lowest priority spare
    rank

    * **UpdateScratchPadReg** Update scratch pad register after spare
    rank is mark dirty.

    * **MarkRankDirtyFlag** Mark the rank as dirty flag. Set
    mSpareRankStatus as RankSpareNotAvailable if it is a spare rank. Add it
    to mFailedRank

    * **DoSpareCopy** Do spare copy operation for patrol scrub disabled
    case

    * **InitializeRankSparing** Initialize all rank sparing related
    data structures

    * **CheckAndHandleRankSparingDone** Check and handle ranks
    sparing done event

    * **CheckAndHandleNewRankSparingEvent** Check for new rank
    sparing events and handle

    * **CheckAndHandleSparing** High level function called from SMI
    handler to handle new rank sparing / rank sparing done events

    * **DoSpareCopy** does the spare copy for patrol scrub disabled
    case

    * **MarkRankDirtyFlag** make the rank not available for rank
    sparing

    * **GetLowestPrioritySpareRank** find and mark the lowest
    priority rank

    * **CheckRankStatePerMC** check the per rank status on SCRUB
    engine

* **RankSparing.h**

    * Declarations for functions defined in RankSparing.c

* **SparingEngine.c**

    * **AcquireSparingEngine** Checks the internal status and takes
    ownership of the sparing engine

    * **ReleaseSparingEngine** Checks the internal status and
    releases ownership of the sparing engine. It is expected that only
    caller who has successfully acquired ownership using
    AcquireSparingEngine calls this function

    * **StartSparing** Programs the hardware registers for DDDC or
    Rank Sparing and starts spare copy operation

    * **IsCopyComplete** Checks the hardware registers for spare
    copy completion and returns status

* **SparingEngine.h**

    * Definitions and declarations used in SparingEngine.c

- - -

## Error Log Driver

* **CommonErrorHandlerIncludes.h**

    * Common error handler definitions used

### Main Error Handler

* **\\Ras\\Smm\\Errhandling\\MainErrorHandler.c**

    * **InitializeMainErrHandler** Main error handler entry for error
    enabling and error handling. Contains Entry point for the RAS module
    main handler, which installs mErrorHandlingProtocol used by silicon and
    platform modules

    * **MainErrorHandler** This is the main RAS Error handler

    * **RegisterSiliconErrorHandler** Register a silicon error
    Handler

* **MainErrorHandler.h**

    * Common definitions used by MainErrorHanlder.c error handling code

### Processor Error Handler

* **\\Ras\\Smm\\ErrHandling\\ProcessorErrorHandler\\ProcessorErrorHandler.c**
Processor core/uncore error enabling and error handling routines.
Includes BIOS machine check handlers.

    * **ProcessorNotifyOs**  This procedure notifies the OS via
    processor mechanisms

    * **ProcessorCatastrophicErrorHandler**  Reference implementation
    only has a `CpuDeadLoop()`

    * **MapErrorSeverity** It gets error severity from Error registers

    * **FillProcessorErrorSection** This procedure populates the
    Processor Error record section

    * **GetHighestSeverity** Returns the more severe of the two inputs

    * **McBankErrorHandler** Handles an uncorrected machine check bank
    error

    * **ProcessCoreMcBankError** Process Core MC Bank errors

    * **ProcessUncoreMcBankError** Process Uncore MC Bank errors

    * **ProcessSocketMcBankError** Process Socket MC Bank errors

    * **DisableCmci** Disables the CMCI signaling

    * **InitMpSyncGlobals** Initialize mMpSyncData for runtime error
    handling

    * **EnableUncorrectableErrRep** This function enables the SMI
    generation of uncorrected errors.

    * **EnableCsmi** Enable CSMI generation

    * **GetProcessorLocation** Gets the physical location of the
    processor with the given APICID.

    * Interfaces for reporter protocol:

        * **McaIsSupported**
    
        * **McaEnableReporting**
    
        * **McaDisableReporting**
    
        * **McaCheckStatus**
    
        * **McaClearStatus**
    
        * **InitializeProcessorErrHandler**

* **ProcessorErrorHandler.h**

    * Processor core/uncore error related definitions

#### Memory Error Handler

* **MemoryErrorHandler.c**

    * **GetMemoryFruInf** This function gets the Memory FRU
    information

    * **FillPlatformMemorySection** This function fills the Error
    record for detected Memory error

    * **MCAddress** Calculates and returns physical address from Addr
    and Misc

    * **LogCorrectedMemError** Creates the memory error record based on
    the inputs

    * **EfiSmmGetTime** This function provides the time stamp
    information

    * **EfiSmmGetNoofHoursPast** Gets the number of hours passed

    * **CorrectedErrorSMIThrottling** Throttle SMI if the rate is
    greater than the threshold

    * **ElogMcExecuteCorrectableErrorHandlingPolicies** Executes based
    on error handling policies

    * **EnableElogMemory** Enables or disables the memory and HA
    specific error reporting based on the SYSTEM\_INFO structure

* **MemoryErrorHandler.h**

    * Memory error related definitions

#### FNV Error Handler

* **FnvErrorHandler.c**

    * **EnableElogFnv** Enables error logging for FNV errors

    * **CreateMemUncorrErrRecord** Creates the memory error record
    based on the inputs

    * **FnvErrorHandler** Handles FNV specific errors

* **FnvErrorHandler.h**

    * Handles FNV Error handler related definitions

### Platform Error Handler

* **PlatformErrorHandler.c** Platform level error enabling and
error handling routines

    * **PopulateRasCapabilityStructure**  This function allocates memory
    for the RAS Capability Structure and populates it

    * **PopulateRasTopologyStructure** Populate RAS Topology
    Structure

    * **LoadDefaultSetupVariables** Loads Default Setup Variables into
    RAS Setup Structure

    * **PopulateRasSetupStructure** Populate RAS Setup Structure

    * **InitMcaSyncGlobals** Initialize MP Sync Data Structure

    * **RegisterErrorListener** Registers an error listener

    * **RegisterNotifier** Registers an OS notifier

    * **LogReportedError** Logs a reported error

    * **NotifyOs** Notify the OS based on the Error Severity

    * **InitializePlatformErrHandler** This is the main entry point of
    the Platform Error Handler. It performs general platform error handling
    initialization and installs the Platform Error Handling Protocol

* **PlatformErrorHandler.h**

    * Contains the Platform Error Handler related function definitions

* **{CRB}PlatPkg\\Library\\OemRasLib\\OemRasLib.c**

    * Options for OEM hooks

* **{CRB}PlatPkg\\Library\\OemRasLib\\OemRasLib.h**

    * OEM hooks related definitions

### Chipset Error Handler

* **IIOErrorHandler.c** IIO error enabling and error handling
	routines

	*  **ClearIohExtPcieDeviceStatus** This function clears Extended IIO
    	status registers
	*  **CheckForDeviceFound** This function check if a specific device
	    is found
	*  **IioIsErrOnPcieRootPort** This function checks the XP Global
    	Error Status register to see if one of the enabled errors has
    	occurred
	*  **IohPcieErrorHandler** This function handles IIO PCIe errors
	*  **IohPcieMaskErrors** This function masks PCIe error reporting
	*  **IIOErrorEnableCallBack** This function enables IIO Error
    	reporting
	*  **InitErrDevInfo** This function initialize the error device
    	info
	*  **EnableElogIoHub** This function is used to register the function
    	that enable the IIO error reporting

* **IIOErrorHandler.h**

    * IIO error related definitions

* **PcieErrorHandler.c** PCI express error enabling and error handling
routines

    * **ProcessErrors** This function is the entry for processing PCH
    and IIO errors
    
    * **PciErrLibPciDeviceErrorHandler** This function checks for, and
    handles, errors on standard PCI devices

    * **PciErrLibIsPciDeviceError** This function checks for errors on
    standard PCI devices

    * **PciErrLibIsCorrectablePcieDeviceError** This function checks
    the PCIe device status and the correctable error registers to see if a
    PCIe Correctable error has occurred

    * **PciErrLibHandleUncorrectablePcieDeviceError** This function
    handles PCIe Uncorrectable (Fatal and Non-Fatal) errors

    * **PciErrLibHandleCorrectablePcieDeviceError** This function
    handles PCIe Correctable errors

    * **PciErrLibIsSubordinateDeviceError** This function determines if
    there is a device reporting an error on the specified P2P bridge's
    secondary bus or on a subordinate bus

    * **PciErrLibIsNonfatalPcieDeviceError** This function checks the
    SERR and PCIe device status to see if an Uncorrectable Non-Fatal PCIe
    error has occurred

    * **PciErrLibIsFatalPcieDeviceError** This function checks the SERR
    and PCIe device status to see if a PCIe Uncorrectable Fatal error has
    occurred

    * **PciErrLibPcieDeviceErrorHandler** This function determines if
    the specified device is reporting an error and handles the error if one
    is detected

    * **PciErrLibIsPcieDevice** This function checks if a specified
    device is a PCI Express device

    * **PciErrLibIsRootPortErrReportingEnabled** This function
    checks to see if PCIe error reporting is enabled in the specified PCIe
    root port device

    * **PcieGetCapabilities** his function gets the capability
    structure

    * **PcieGetAer** This function gets the AER capability

    * **GetSlotNum** This function gets the slot number

    * **GetPortType** This value is only valid for Functions that
    implement a Type 01h PCI Configuration Space header

    *  **PcieGetAerErrors** This function gets the AER errors from the
    PCIE Extended Capabilities (Uncorrectable, correctable and RP errors)

    *  **ErrorSeverity** This function finds the highest severity

    *  **PcieLogReportError** This function fills the PCIE error record
    field logs the appropriate type of error by calling
    mPlatformErrorHandlingProtocol

    *  **PciErrLibEnableBridgeControl** This function enables SERR and
    Parity error reporting in the P2P bridge control register

    *  **PciErrLibEnablePcieELogCapabilities** This function programs
    the Uncorrectable Error Mask register if applicable, programs the
    Uncorrectable Error Severity register if applicable, programs the
    Correctable Error Mask register if applicable, clears the standard PCIe
    status registers, disables legacy SERR reporting if appropriate (no AER
    registers), enables error reporting for root ports and regular devices

    *  **PciErrLibEnableElogDeviceAndSubDevices** This function
    initializes and enables error reporting in the specified PCI or PCIe
    device and all subordinate PCI or PCIe devices

    *  **PciErrLibEnableElogDevice** This function initializes and
    enables error reporting in the specified PCI or PCIe device

    *  **PciErrLibEnablePciSerrPerrGeneration** This function enables
    SERR and Parity error reporting in the specified device's legacy PCI
    command register

    *  **ClearGpiSmiStatus** Clear any pending GPI SMI status to avoid
    EOS

    *  **PciErrIsSupported** Interfaces between silicon and common
    module

    *  **PciErrEnableReporting / PciErrDisableReporting** Enable/Disable
    Pcie Error reporting

    *  **PciErrCheckStatus** Checks for pending processor errors

    *  **PciErrClearStatus ** Clear Error Status in various devices

    *  **PciErrDetectAndHandle** Process Pcie errors

    *  **InitializePcieErrHandler** Consists of Entry point for the PCI
    Express Error Handler. This function initializes the error handling and
    enables PCI Express error sources

    *  **PciErrLibGetCapabilitiesOffset** This function obtains the PCI
    configuration space register offset of the specified offset of the
    specified Capabilities register set

    *  **PciErrLibClearLegacyPciDeviceStatus** This function clears the
    specified device's primary and secondary legacy PCI status registers

    *  **PciErrLibIsPciBridgeDevice** This function checks if a
    PCI-to-PCI bridge exists at the specified PCI address

    *  **PciErrLibGetExtendedCapabilitiesOffset** This function obtains
    the extended PCI configuration space register offset for a specified
    Extended Capability for the specified PCI device

    *  **PciErrLibClearPcieDeviceStatus** This function clears the legacy
    and standard PCI Express status registers for the specified PCI Express
    device

    *  **PCIeIsRootPort** This function will verify whether a port is
    PCIe root port

    *  **PciErrLibIsDevicePresent** This function checks if a PCI or
    PCIe device exists at the specified PCI address

* **PcieErrorHandler.h**

    * PCI express error related definitions

* **SbErrorHandler.c**

    * Pch error enabling and error handling routines

* **{CRB}SktPkg\\Library\\ChipsetErrReporting\\ChipsetErrReporting.c**

	*  **CheckForIioErrors** Check For IIIO Errors

	* **InitIOHInfo** This function initialize the IOH info
		structure

	* **MaskIIOErrorReporting** This function masks IIO errors
		reporting

	* **ClearErrStsRegisters** Clear IIO Error Status Registers which
		require write "1" to clear (RW1C)

	* **ClearIioErrorRegisterStatus** Clears local and global IIO
		Errors

	* **ClearIioErrors** Clears IIO Errors

	* **RppIoErrEnable** This function enables all the RP PIO
		errors

	* **IohPcieErrorEnable** This function will enable IIO PCIE error
		reporting

	* **IIOErrorEnable** This function enables IIO error reporting

	* **IIOLerEnable** This Function will enable all the Pcie errors
		that need to be reported as LER

	* **ProcessLocalIioErrors** IIO Local error handler routine

	* **ProcessIioErrors**  This is the main IIO error main handler
		routine

	* **InitPcieSiliconCap** Initialize PCIE\_IIO specific RAS
		capabilities

	* **ChipsetErrReportingConstructor** Allocates protocols and initialize
  internal structure required

- - -

## WHEA Implementation

* **IsPlatformSupportWhea**

	* IsPlatformSupportWhea.c

* **WheaElog**

	* WheaElog.c
	* WheaElog.h

* **WheaErrorInj**

	* Einj.asm
	* WheaErrorInj.c
	* WheaErrorInj.h

* **WheaPlatform**

	* WheaPlatform.c

* **WheaSupport**

	* WheaSupport.c
	* WheaSupport.h

* **WheaErrorLib**

	* WheaErrorLib.c

* **WheaPlatformHooksLib**

	* WheaPlatformHooksLib.c

