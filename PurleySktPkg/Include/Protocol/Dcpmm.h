/** @file
  Header file for DCPMM NVDIMM Firmware Interface protocol.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2019 Intel Corporation. <BR>

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary    and
  confidential information of Intel Corporation and its suppliers and licensors,
  and is protected by worldwide copyright and trade secret laws and treaty
  provisions. No part of the Material may be used, copied, reproduced, modified,
  published, uploaded, posted, transmitted, distributed, or disclosed in any way
  without Intel's prior express written permission.

  No license under any patent, copyright, trade secret or other intellectual
  property right is granted to or conferred upon you by disclosure or delivery
  of the Materials, either expressly, by implication, inducement, estoppel or
  otherwise. Any license under such intellectual property rights must be
  express and approved by Intel in writing.

  Unless otherwise agreed by Intel in writing, you may not remove or alter
  this notice or any other notice embedded in Materials by Intel or
  Intel's suppliers or licensors in any way.
**/

#ifndef _PROTOCOL_DCPMM_H_
#define _PROTOCOL_DCPMM_H_

#define EFI_DCPMM_GUID {0x25E78B4B, 0xEEF6, 0x4FDD, 0x9D, 0xFA, 0x07, 0x88, 0x33, 0x7F, 0x77, 0xC2}

//
// DCPMM Protocol versions
//
#define DCPMM_PROTOCOL_VER_1   1
#define DCPMM_PROTOCOL_VER_2   2
#define DCPMM_PROTOCOL_VER_CUR DCPMM_PROTOCOL_VER_2

//
// NFIT NVDIMM handle has the following layout:
// Bit [3:0]   - DIMM
// Bit [7:4]   - Channel in Memory Controller
// Bit [11:8]  - Memory Controller
// Bit [15:12] - Socket
// Bit [27:16] - Node ID
// Bit [31:28] - reserved
// The macros below operate on NFIT handle.
//
#define NFIT_HANDLE_BUILD(Skt, Mc, Ch, Dimm) ((UINT32)(Skt) << 12 | (UINT32)(Mc) << 8 | (Ch) << 4 | (Dimm))
#define NFIT_HANDLE_TO_DIMM(Handle)          ((UINT8)(Handle) & 0x0F)
#define NFIT_HANDLE_TO_CH(Handle)            ((UINT8)((Handle) >> 4) & 0x0F)
#define NFIT_HANDLE_TO_MC(Handle)            ((UINT8)((Handle) >> 8) & 0x0F)
#define NFIT_HANDLE_TO_SKT(Handle)           ((UINT8)((Handle) >>12) & 0x0F)

#define NFIT_HANDLE_NONE ((UINT32)-1)

//
// The state of DCPMM NVDIMM in the current boot.
// This state determines what interface can be used for communication with
// the NVDIMM firmware.
//
typedef enum {
  NvdimmStateFunctional,    ///< Media and DDRT mailbox registers mapped.
  NvdimmStateMediaDisabled, ///< Media not mapped, but DDRT mailbox available.
  NvdimmStateNotMapped,     ///< Media and DDRT mailbox not available, try SMBUS.
  NvdimmStateMax
} DCPMM_NVDIMM_STATE;

//
// Interface to use for communication with NVDIMM firmware.
// It can be SMBUS or DDRT mailbox. There are two mailboxes on DDRT,
// OS and SMM, but this protocol always use OS mailbox. The user must be
// aware that commands restricted to SMM mailbox are not available here.
// The user must also be aware of limitations of SMBUS interface when using it.
//
typedef enum {
  FisOverDdrt,   ///< Default communication channel
  FisOverSmbus,  ///< Use if NVDIMM is not mapped to DDRT
  FisIfaceMax
} DCPMM_FIS_INTERFACE;

//
// The below listed commands are emulated, are not sent to the NVDIMM,
// but provide access to large payload for the user of the FIS API.
// These commands are not defined in the FIS specification and work for DDRT only.
//
#define FIS_CMD_EMULATED_MAJOR      0xFD
#define FIS_CMD_GET_LP_MB_INFO    0x00FD  ///< Get large payload info
#define FIS_CMD_WRITE_LP_INPUT_MB 0x01FD  ///< Write large payload input buffer
#define FIS_CMD_READ_LP_OUTPUT_MB 0x02FD  ///< Read large payload output buffer
#define FIS_CMD_GET_BOOT_STATUS   0x03FD  ///< Read Boot Status Register
#define FIS_CMD_RESERVED          0X04FD  ///< Reserved
#define FIS_CMD_EXTENDED_VEN_SPEC 0X05FD  ///< Extended Vendor-Specific Command

//
// Parameters for _DSM function Pass-Through Command DSM_FN_VENDOR_COMMAND.
// This DSM can send commands to the NVDIMM, but there are also emulated commands
// to read or write large payload.
//
#pragma pack(1)
typedef struct {
  struct {
    UINT16   FisCmd;           ///< FisCmd[15:8] -- Mailbox Sub-opcode
                               ///< FisCmd[7:0]  -- Mailbox Opcode
    UINT16   Reserved;
    UINT32   DataSize;         ///< Size of command specific data following Head structure
  } Head;
  union {
    struct {                   ///< This structure is used with FW commands
      UINT32 Payload[0];       ///< FIS request payload to copy to Small Payload input registers
    } Fis;
    struct {                   ///< This structure is used with FIS_CMD_READ_LP_OUTPUT_MB
      UINT32 Size;             ///< Number of bytes to read from Large Payload output
      UINT32 Offset;           ///< Offset in Large Payload where to start reading
    } LpRead;
    struct {                   ///< This structure is used with FIS_CMD_WRITE_LP_INPUT_MB
      UINT32 Size;             ///< Number of bytes to write to Large Payload input
      UINT32 Offset;           ///< Offset in Large Payload input where to start writing
      UINT32 Payload[0];       ///< Data to write follow the LpWrite structure
    } LpWrite;
    struct {
      UINT16 FisCmd;
      UINT16 TransportInterface : 2,  ///< 0: DDRT, 1: SMBUS, 2: Reserved, 3: Reserved
             Reserved1          : 14;
      UINT32 Reserved2;
      UINT32 Timeout;                 ///< Timeout in milisecond, 0: use default timeout
      UINT32 Payload[0];              ///< Data buffer to be transferred to input payload registers
    } ExtVenSpec;
  } Data;
} DCPMM_FIS_INPUT;

typedef struct {
  struct {
    UINT32   DataSize;         ///< On input the size of buffer following Head, on exit size of data stored
  } Head;
  union {
    struct {                   ///< This structure is used with FW commands
      UINT32 Payload[0];       ///< Small Payload output from FIS request
    } Fis;
    struct {                   ///< This structure is used with FIS_CMD_GET_LP_MB_INFO
      UINT32 InpPayloadSize;
      UINT32 OutPayloadSize;
      UINT32 DataChunkSize;
    } LpInfo;
    struct {                   ///< This structure is used with FIS_CMD_READ_LP_OUTPUT_MB
      UINT32 Payload[0];       ///< Large Payload output from FIS requst
    } LpData;
    struct {
      UINT32 Payload[0];       ///< NVMDIMM Command Output Data
    } ExtVenSpecData;
    UINT64   Bsr;              ///< Boot Status Register value if it was FIS_CMD_GET_BOOT_STATUS
  } Data;
} DCPMM_FIS_OUTPUT;
#pragma pack()

//
// Error record that can be reported by DCPMM_ARS_STATUS() function.
//
typedef struct {
  UINT32    NfitHandle;       // NFIT handle of the NVDIMM that is part of the error record
  UINT32    Reserved;
  UINT64    SpaOfErrLoc;      // Start SPA of the error location
  UINT64    Length;           // Length of the error location region
} DCPMM_ARS_ERROR_RECORD;

/**
 * Platform Interpreted Last Shutdown Status
 * Takes in to account whether LSS was latched on previous shutdown, alters final LSS state based on system configuration.
 * See Intel Optain DC Persistent Memory Module Software RAS and Error Recovery
 * Specifiction for details. In particular:
 * Table 6-5 Power Down Signals and LSS Clean Shutdown Determination
 * Table 6-6 Power Down Signals and LSS Dirty Shutdown Determination
 **/
typedef enum {
  DcpmmLssClean = 0,       // Last platform adjusted shutdown was clean – LSS Reason enum is valid
  DcpmmLssDirty,           // Last platform adjusted shutdown was dirty – LSS Reason enum is valid
  DcpmmLssCleanNotLatched, // Current LSS is reporting a clean shutdown but the latch was not enabled – LSS Reason enum is valid
                           // The LSS state returned is reporting a clean shutdown for the first shutdown that occurred after the latch was enabled.
                           // You cannot imply ANYTHING about the last shutdown that just occurred.
  DcpmmLssDirtyNotLatched, // DIRTY_NOT_LATCHED – Current LSS is reporting a dirty shutdown but the latch was not enabled – LSS Reason enum is valid
                           // A previous system shutdown was detected by the Intel Optane DC Module as a dirty shutdown and the OS/sw/apps have not
                           // yet acknowledged or handled the dirty shutdown case.  Previous write data sent to the Intel Optane DCM… may or may not
                           // be lost and it is left to other SW components or applications to determine if their data is affected by the dirty shutdown.
                           // NOTE: that the returned enumeration values are valid in this case and represent the specific state of those bits at the time latch
                           // was enabled.  To be clear, LSS, DSC, Vendor Specific LSS Details, Vendor Specific LSS Extended Details update on shutdown after
                           // latch is enabled and remain frozen in previous state while latch is not enabled.
  DcpmmLssMax
} DCPMM_INTERPRETED_LSS;

typedef enum {
  //
  // Clean LSS reasons – matches row decode in Table 6-5
  //
  DcpmmLssReasonCleanAdrPwrFlow = 0,
  DcpmmLssReasonCleanS3PwrFlow,
  DcpmmLssReasonCleanS4PwrFlow,
  DcpmmLssReasonCleanS5PwrFlow,
  DcpmmLssReasonCleanIdlePwrFlow,
  DcpmmLssReasonCleanWarmResetPwrFlow,
  DcpmmLssReasonCleanIgnoredDueToCfg,
  DcpmmLssReasonCleanOther,            // Used if none of above clean shutdown reasons matched
  //
  // Dirty LSS reasons – matches row decode in Table 6-6
  //
  DcpmmLssReasonDirtyAdrStartedNoDdrtPwrFailCmdRecv,
  DcpmmLssReasonDirtyDdrtPwrFailCmdRecvAdrNotStarted,
  DcpmmLssReasonDirtySuprisePwrLossAtNvdimm,
  DcpmmLssReasonDirtyNvdimmThermalTrip,
  DcpmmLssReasonDirtyViralTriggeredBeforPwrDown,
  DcpmmLssReasonDirtyDdrtSupriseClkStop,
  DcpmmLssReasonDirtyWriteDataFlushNotCompleted,
  DcpmmLssReasonDirtyInjectedDirtyShutdown,
  DcpmmLssReasonDirtyNvdimmInternalCtxFlushFailed,
  DcpmmLssReasonDirtyPersistentCachingFailure,
  DcpmmLssReasonDirtyDimmFatal,        // The DCPMM has gone FATAL. The media error log or BIOS EWL may contain further details on why. 
  DcpmmLssReasonDirtyDimmFatalFwHang,  // The DCPMM detected a DIMM Fatal, and the WTS FW watch dog interrupt fired. No additional Media Error Log information will be reported
  DcpmmLssReasonDirtyTransactionFatal, // Restarting should allow normal operations to continue. The media error log or BIOS EWL may contain further details on why. 
  DcpmmLssReasonDirtyTempFatal,        // The DCPMM Controller detected a shutdown media temperature has been reached. No additional Media Error Log information will be reported

  DcpmmLssReasonDirtyOther,            // Used if none of above dirty shutdown reasons matched
  DcpmmLssReasonMax
} DCPMM_INTERPRETED_LSS_REASON;



/**
  @brief Get NFIT handle and state of next DCPMM NVDIMM.

  This function facilities iterating over NVDIMMs present in the system.

  @param[in,out] NfitHandlePtr  On input handle from former call, or NFIT_HANDLE_NONE for first call.
                                On output NFIT handle of next NVDIMM found.
  @param[out]    NvdimmStatePtr Optional pointer to a buffer for the state of NVDIMM.

  @retval EFI_SUCCESS           Command completed successfully.
  @retval EFI_INVALID_PARAMETER NfitHandlePtr doesn't correspond to a DCPMM NVDIMM.
  @retval EFI_NOT_FOUND         No more DCPMM NVDIMMs found.
**/
typedef
EFI_STATUS
(EFIAPI *DCPMM_NEXT_NVDIMM) (
  IN OUT UINT32             *NfitHandlePtr,
     OUT DCPMM_NVDIMM_STATE *NvdimmStatePtr
  );

/**
  @brief Performs commands defined in DCPMM Firmware Interface Specification.

  This method handles also BIOS emulated commands, with command major code FIS_CMD_EMULATED_MAJOR.
  If FIS command is an emulated BIOS command, it is handled by BIOS, not sent to NVDIMM FW.
  Otherwise the command is handled by the NVDIMM FW (FW command).

  @param[in]     Reserved     Must be 0.
  @param[in]     NfitHandle   NFIT handle of the target DCPMM NVDIMM.
  @param[in]     InputPtr     Pointer to a structure with FIS command opcode and input data.
                              [FW command]   InputPtr->Payload is copied to Small Payload Input.
                              [BIOS command] Consumption is command-specific.
  @param[in,out] OutputPtr    Pointer to a structure prepared for output data.
                              May be NULL if not expecting output payload.
                              [FW command]   Small Payload Output is stored in output data
                                             if mailbox command has executed successfully.
                              [BIOS command] Use of Output is command-specific.
                              In OutputPtr->Head.DataSize the size of output buffer is provided on entry
                              and on exit the size of returned data is provided.
  @param[in]     Timeout      [FW command]   Optional timeout in us to use for FIS command
                              [BIOS command] Always ignored.
                              If set to zero then command specific default timeout is calculated.
  @param[out]    MboxStsPtr   [FW command]   Optional pointer to store FW Mailbox Status Code under.
                              [BIOS command] Always ignored.
                              Ignored if NULL.

  @retval  EFI_SUCCESS            Command completed successfully.
                                  [FW command]   FW MB Status Code is zero.
  @retval  EFI_INVALID_PARAMETER  NfitHandle doesn't correspond to a DCPMM NVDIMM.
                                  [FW command]   InputSize is larger than controller's Small Payload Input size.
                                  [FW command]   OutputSize is larger than controller's Small Payload Output size.
                                  [BIOS command] InputSize not valid for specified FisCmd.
                                  [BIOS command] OutputSize is not valid for specified CommandOpCode.
  @retval  EFI_NOT_SUPPORTED      [BIOS command] Unknown Mailbox Sub-Opcode
  @retval  EFI_NOT_READY          [FW command]   Mailbox not ready for command.
                                                 Mailbox status not updated.
  @retval  EFI_DEVICE_ERROR       [FW command]   Mailbox command did not complete. Device error.
                                                 MailboxStatus not updated.
  @retval  EFI_TIMEOUT            [FW command]   Mailbox command did not complete in time allotted.
                                                 Mailbox status not updated.
  @retval  EFI_PROTOCOL_ERROR     [FW command]   Mailbox command completed with non-zero FW MB Status Code
**/
typedef
EFI_STATUS
(EFIAPI *DCPMM_FIS_REQUEST) (
  IN     UINT32              Reserved,
  IN     UINT32              NfitHandle,
  IN     DCPMM_FIS_INPUT    *InputPtr,
  IN OUT DCPMM_FIS_OUTPUT   *OutputPtr,
  IN     UINT64              Timeout,
     OUT UINT8              *MboxStsPtr
  );

/**
  @brief Get status of Address Range Scrub done on DCPMM NVDIMM.

  This function allows reading list of errors in the medium that could cause boot failure.
  When called with NULL in ArsErrTablePtr it will simply return length of ARS errors table.

  @param[in,out] ArsErrTableLenPtr On input number of entries in the ArsErrTablePtr buffer for ARS errors.
                                   On output number of all known error records is returned, can be larger than the input value.
  @param[out]    ArsErrTablePtr    Pointer to a buffer for ARS errors, or
                                   NULL if user just needs to retrieve the number of error records.

  @retval EFI_SUCCESS           Command completed successfully, ArsErrTableLenPtr and ArsErrTablePtr updated.
  @retval EFI_INVALID_PARAMETER ArsErrTableLenPtr is NULL, or contains value greater than zero while ArsErrTablePtr is NULL.
  @retval EFI_NOT_STARTED       There is no ARS data in the system, ARS was not started.
  @retval EFI_NOT_READY         ARS is running, data returned may be incomplete.
  @retval EFI_BUFFER_TOO_SMALL  Buffer at ArsErrTablePtr is too small for all error entries,
                                at ArsErrTableLenPtr retuning number of all entries.
**/
typedef
EFI_STATUS
(EFIAPI *DCPMM_ARS_STATUS) (
  IN OUT UINT32                 *ArsErrTableLenPtr,
     OUT DCPMM_ARS_ERROR_RECORD *ArsErrTablePtr
  );

/**
  @brief Return maximum amount of time allowed to use the SMBus

  This function returns maximum amount of time allowed for UEFI MGMT SW to use the SMBus
  and can be overwritten by OEM hook. Note that zero is special value and means no limit.

  @param[out] TimeMs            Maximum amount of time allowed to use the SMBus

  @retval EFI_SUCCESS           Command completed successfully.
  @retval EFI_INVALID_PARAMETER TimeMs is NULL.
**/
typedef
EFI_STATUS
(EFIAPI *DCPMM_SMBUS_GET_MAX_ACCESS_TIME) (
  OUT UINT32 *TimeMsPtr
  );

/**
  @brief Provides interface to acquire SMBus access

  @param[in]  Socket            Socket index at system level
  @param[in]  Imc               iMC index at socket level

  @retval EFI_SUCCESS           Command completed successfully.
  @retval EFI_NOT_READY         TSOD control acquisition or TSOD state change failed.
  @retval EFI_INVALID_PARAMETER Invalid Socket or Imc value.
**/
typedef
EFI_STATUS
(EFIAPI *DCPMM_SMBUS_ACCESS_ACQUIRE) (
  IN UINT8 Socket,
  IN UINT8 Imc
  );

/**
  @brief Provides interface to release SMBus access

  @param[in]  Socket            Socket index at system level
  @param[in]  Imc               iMC index at socket level

  @retval EFI_SUCCESS           Command completed successfully.
  @retval EFI_NOT_READY         TSOD control release failed.
  @retval EFI_INVALID_PARAMETER Invalid Socket or Imc value.
**/
typedef
EFI_STATUS
(EFIAPI *DCPMM_SMBUS_ACCESS_RELEASE) (
  IN UINT8 Socket,
  IN UINT8 Imc
  );

/**
  @brief Provides interpreted last shutdown state and reason using latched data.

  @param[in]  NfitHandle              NFIT handle of the target DCPMM NVDIMM.
  @param[out] InterpretedLssPtr       Buffer for interpreted last shutdown state.
  @param[out] InterpretedLssReasonPtr Buffer for interpreted last shutdown state reason.

  @return Standard EFI_STATUS is returned.
**/
typedef
EFI_STATUS
(EFIAPI *DCPMM_GET_PLATFORM_INTERPRETED_LSS) (
  IN     UINT32                        NfitHandle,
     OUT DCPMM_INTERPRETED_LSS        *InterpretedLssPtr,
     OUT DCPMM_INTERPRETED_LSS_REASON *InterpretedLssReasonPtr
  );

typedef struct _EFI_DCPMM_PROTOCOL_ {
  UINT16                          ProtocolVersion;
  UINT8                           Reserved[6];
  DCPMM_NEXT_NVDIMM               DcpmmNextNvdimm;
  DCPMM_FIS_REQUEST               DcpmmFisRequest;
  DCPMM_ARS_STATUS                DcpmmArsStatus;
  DCPMM_SMBUS_GET_MAX_ACCESS_TIME DcpmmSmbusGetMaxAccessTime;
  DCPMM_SMBUS_ACCESS_ACQUIRE      DcpmmSmbusAccessAcquire;
  DCPMM_SMBUS_ACCESS_RELEASE      DcpmmSmbusAccessRelease;
  //
  // Functions added in DCPMM_PROTOCOL_VER_2 start here.
  //
  DCPMM_GET_PLATFORM_INTERPRETED_LSS           DcpmmGetPlatformInterpretedLss;
} EFI_DCPMM_PROTOCOL;

extern EFI_GUID gEfiDcpmmProtocolGuid;

#endif // _PROTOCOL_DCPMM_H_

