/** @file
  Header file for DxePchHdaLib - NHLT structure definitions.
  
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

#ifndef _DXE_PCH_HDA_NHLT_H_
#define _DXE_PCH_HDA_NHLT_H_

#include <IndustryStandard/Acpi.h>

//
// ACPI support protocol instance signature definition.
//
#define NHLT_ACPI_TABLE_SIGNATURE  SIGNATURE_32 ('N', 'H', 'L', 'T')

// MSFT defined structures
#define SPEAKER_FRONT_LEFT      0x1
#define SPEAKER_FRONT_RIGHT     0x2
#define SPEAKER_FRONT_CENTER    0x4
#define SPEAKER_BACK_LEFT       0x10
#define SPEAKER_BACK_RIGHT      0x20

#define KSAUDIO_SPEAKER_MONO   (SPEAKER_FRONT_CENTER)
#define KSAUDIO_SPEAKER_STEREO (SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT)
#define KSAUDIO_SPEAKER_QUAD   (SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT | SPEAKER_BACK_LEFT | SPEAKER_BACK_RIGHT)

#define WAVE_FORMAT_EXTENSIBLE    0xFFFE /* Microsoft */
#define KSDATAFORMAT_SUBTYPE_PCM \
        {0x00000001, 0x0000, 0x0010, {0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71}}

#pragma pack (push, 1)

typedef struct {
  UINT16  wFormatTag;
  UINT16  nChannels;
  UINT32  nSamplesPerSec;
  UINT32  nAvgBytesPerSec;
  UINT16  nBlockAlign;
  UINT16  wBitsPerSample;
  UINT16  cbSize;
} WAVEFORMATEX;

typedef struct {
  WAVEFORMATEX Format;
  union {
    UINT16 wValidBitsPerSample;
    UINT16 wSamplesPerBlock;
    UINT16 wReserved;
  } Samples;
  UINT32       dwChannelMask;
  GUID         SubFormat;
} WAVEFORMATEXTENSIBLE;

//
// List of supported link type.
//
enum NHLT_LINK_TYPE
{
  HdaNhltLinkHd   = 0,
  HdaNhltLinkDsp  = 1,
  HdaNhltLinkDmic = 2,
  HdaNhltLinkSsp  = 3,
  HdaNhltLinkInvalid
};

//
// List of supported device type.
//
enum NHLT_DEVICE_TYPE
{
  HdaNhltDeviceBt   = 0,
  HdaNhltDeviceDmic = 1,
  HdaNhltDeviceI2s  = 4,
  HdaNhltDeviceInvalid
};

typedef struct {
  UINT32    CapabilitiesSize;
  UINT8     Capabilities[1];
} SPECIFIC_CONFIG;

typedef struct {
  WAVEFORMATEXTENSIBLE Format;
  SPECIFIC_CONFIG      FormatConfiguration;
} FORMAT_CONFIG;

typedef struct {
  UINT8           FormatsCount;
  FORMAT_CONFIG   FormatsConfiguration[1];
} FORMATS_CONFIG;

typedef struct {
  UINT32          EndpointDescriptorLength;
  UINT8           LinkType;
  UINT8           InstanceId;
  UINT16          HwVendorId;
  UINT16          HwDeviceId;
  UINT16          HwRevisionId;
  UINT32          HwSubsystemId;
  UINT8           DeviceType;
  UINT8           Direction;
  UINT8           VirtualBusId;
  SPECIFIC_CONFIG EndpointConfig;
  FORMATS_CONFIG  FormatsConfig;
} ENDPOINT_DESCRIPTOR;

//
// High Level Table structure
//
typedef struct {
  EFI_ACPI_DESCRIPTION_HEADER Header; //{'N', 'H', 'L', 'T'}
  UINT8                       EndpointCount;   // Actual number of endpoints
  ENDPOINT_DESCRIPTOR         EndpointDescriptors[1];
  SPECIFIC_CONFIG             OedConfiguration;
} NHLT_ACPI_TABLE;

#pragma pack (pop)

#endif // _DXE_PCH_HDA_NHLT_H_
