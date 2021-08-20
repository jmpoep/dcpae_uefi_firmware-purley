/**++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// This file contains an 'Intel Peripheral Driver' and is
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may
// be modified by the user, subject to additional terms of the
// license agreement
//

Copyright (c) 2009 - 2018 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


  @file DdrtErrorHandler.h

  This file will contain all definitions related to Memory Error Handler

----------------------------------------------------------------**/

#ifndef _DDRT_ERROR_HANDLER_H_
#define _DDRT_ERROR_HANDLER_H_

#define RAS_INFO_STR(module)  "[RAS] (" module ") "
#define RAS_ERROR_STR(module) "[RAS] (" module ") ERROR: "

#ifndef NELEMENTS
#define NELEMENTS(Array) (sizeof(Array)/sizeof((Array)[0]))
#endif

#define NUM_PAYLOAD_REG            32
#define ONE_SECOND_TIME_IN_NANOSEC 1000000000

typedef enum _AP_DIMM_STATE {
  AP_STOP = 0,
  AP_ERROR,
  AP_IN_PROGRESS,
  AP_FINISHED
} AP_DIMM_STATE;

typedef struct _MEDIA_ERR_LOG_DATA_RSP {
  MEDIA_ERR_LOG_DATA MediaErrLog;
  EFI_STATUS         ErrorMediaLogStatus;
  UINT16             NewSeqNumber;
}MEDIA_ERR_LOG_DATA_RSP;

typedef struct _THERMAL_ERR_LOG_DATA_RSP {
  THERMAL_ERR_LOG_DATA ThermalErrLog;
  EFI_STATUS           ErrorThermalLogStatus;
  UINT16               NewSeqNumber;
}THERMAL_ERR_LOG_DATA_RSP;

typedef struct _DIMM_ERROR_LOG_DATA {
  THERMAL_ERR_LOG_DATA_RSP ThermalErrLogDataRsp[MAX_THERMAL_PAYLOADS];
  MEDIA_ERR_LOG_DATA_RSP MediaErrLogDataRsp[MAX_MEDIA_PAYLOADS];
  UINT32     LongOpRsp[NUM_PAYLOAD_REG];
  EFI_STATUS LongOpStatus;
  EFI_STATUS DetectErrorStatus;
  UINT32     DetectErrorData;
  UINT16     MediaErrSeqFirst;
  UINT16     ThermalErrSeqFirst;
  BOOLEAN    ProcessLongOp;
  BOOLEAN    ProcessMediaErrLogData;
  BOOLEAN    ProcessThermalErrLogData;
  BOOLEAN    ProcessFwHang;
  BOOLEAN    ProcessFwFatalErr;
} DIMM_ERROR_LOG_DATA;

VOID *
EFIAPI
AsmWriteFlushCacheLine (
  IN      VOID                      *LinearAddress
  );

#endif // _DDRT_ERROR_HANDLER_H_

