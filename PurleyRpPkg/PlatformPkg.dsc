#
# This file contains 'Framework Code' and is licensed as such
# under the terms of your license agreement with Intel or your
# vendor.  This file may not be modified, except as allowed by
# additional terms of your license agreement.
#
## @file
# X64 Platform with 64-bit DXE.
#
# Copyright (c) 2008 - 2019, Intel Corporation. All rights reserved.
# This software and associated documentation (if any) is furnished
# under a license and may only be used or copied in accordance
# with the terms of the license. Except as permitted by such
# license, no part of this software or documentation may be
# reproduced, stored in a retrieval system, or transmitted in any
# form or by any means without the express written consent of
# Intel Corporation.
#
##

################################################################################
#
# Defines Section - statements that will be processed to create a Makefile.
#
################################################################################
[Defines]
  PLATFORM_NAME                       = $(RP_PKG)
  PLATFORM_GUID                       = D7EAF54D-C9B9-4075-89F0-71943DBCFA61
  PLATFORM_VERSION                    = 0.1
  DSC_SPECIFICATION                   = 0x00010005
  OUTPUT_DIRECTORY                    = Build/$(RP_PKG)
  SUPPORTED_ARCHITECTURES             = IA32|X64
  BUILD_TARGETS                       = DEBUG|RELEASE
  SKUID_IDENTIFIER                    = DEFAULT

  #
  # Set the global variables
  #
  EDK_GLOBAL PLAT_PKG                 = $(PLAT_PKG)
  EDK_GLOBAL RP_PKG                   = $(RP_PKG)
  EDK_GLOBAL SV_RESTRICTED_PKG        = $(PLATFORM_SV_RESTRICTED_PKG)
  EDK_GLOBAL RESTRICTED_PKG           = $(PLATFORM_RESTRICTED_PKG)
  EDK_GLOBAL SKT_PKG                  = $(SKT_PKG)
  EDK_GLOBAL EFI_ACPI_TABLE_STORAGE_GUID = 7E374E25-8E01-4FEE-87F2-390C23C606CD
  EDK_GLOBAL UEFI_PREFIX =
  EDK_GLOBAL PI_PERFIX   =
  EDK_GLOBAL INTERNAL_PKG              = PurleyInternalPkg

  #
  # Platform On/Off features are defined here
  #
  !include $(RP_PKG)/PlatformPkgConfig.dsc
  EDK_GLOBAL PCH_PKG                  = $(PCH_PKG)
  FLASH_DEFINITION                    = $(RP_PKG)/PlatformPkg.fdf
  FIX_LOAD_TOP_MEMORY_ADDRESS         = 0


################################################################################
#
# SKU Identification section - list of all SKU IDs supported by this
#                              Platform.
#
################################################################################
[SkuIds]
  0|DEFAULT              # The entry: 0|DEFAULT is reserved and always required.

################################################################################
#
# Library Class section - list of all Library Classes needed by this Platform.
#
################################################################################
[LibraryClasses.common]
  #
  # Entry point
  #
  PeiCoreEntryPoint|MdePkg/Library/PeiCoreEntryPoint/PeiCoreEntryPoint.inf
  PeimEntryPoint|MdePkg/Library/PeimEntryPoint/PeimEntryPoint.inf
  DxeCoreEntryPoint|MdePkg/Library/DxeCoreEntryPoint/DxeCoreEntryPoint.inf
  UefiDriverEntryPoint|MdePkg/Library/UefiDriverEntryPoint/UefiDriverEntryPoint.inf
  UefiApplicationEntryPoint|MdePkg/Library/UefiApplicationEntryPoint/UefiApplicationEntryPoint.inf
  DxeSmmDriverEntryPoint|IntelFrameworkPkg/Library/DxeSmmDriverEntryPoint/DxeSmmDriverEntryPoint.inf
  PeCoffExtraActionLib|MdePkg/Library/BasePeCoffExtraActionLibNull/BasePeCoffExtraActionLibNull.inf
  #
  # Basic
  #
  BaseLib|MdePkg/Library/BaseLib/BaseLib.inf
  BaseMemoryLib|MdePkg/Library/BaseMemoryLibRepStr/BaseMemoryLibRepStr.inf
  PrintLib|MdePkg/Library/BasePrintLib/BasePrintLib.inf
  CpuLib|MdePkg/Library/BaseCpuLib/BaseCpuLib.inf
  IoLib|MdePkg/Library/BaseIoLibIntrinsic/BaseIoLibIntrinsic.inf
  PciLib|MdePkg/Library/BasePciLibCf8/BasePciLibCf8.inf
  PciCf8Lib|MdePkg/Library/BasePciCf8Lib/BasePciCf8Lib.inf
  PciExpressLib|MdePkg/Library/BasePciExpressLib/BasePciExpressLib.inf
  PciSegmentLib|MdePkg/Library/BasePciSegmentLibPci/BasePciSegmentLibPci.inf
  CsrToPcieLib|CpRcPkg/Library/CsrToPcieLibNull/BaseCsrToPcieLibNull.inf
  USRATraceLib|CpRcPkg/Library/USRATraceLibNull/USRATraceLibNull.inf
  MicrocodeLib|$(SKT_PKG)/Library/MicrocodeLib/MicrocodeLib.inf
  PcieAddrLib|CpRcPkg/Library/PcieAddressLib/PcieAddressLib.inf
  CacheMaintenanceLib|MdePkg/Library/BaseCacheMaintenanceLib/BaseCacheMaintenanceLib.inf
  PeCoffLib|MdePkg/Library/BasePeCoffLib/BasePeCoffLib.inf
  PeCoffGetEntryPointLib|MdePkg/Library/BasePeCoffGetEntryPointLib/BasePeCoffGetEntryPointLib.inf
  SortLib|MdeModulePkg/Library/BaseSortLib/BaseSortLib.inf
  #
  # UEFI & PI
  #
  UefiBootServicesTableLib|MdePkg/Library/UefiBootServicesTableLib/UefiBootServicesTableLib.inf
  UefiRuntimeServicesTableLib|MdePkg/Library/UefiRuntimeServicesTableLib/UefiRuntimeServicesTableLib.inf
  UefiRuntimeLib|MdePkg/Library/UefiRuntimeLib/UefiRuntimeLib.inf
  UefiLib|MdePkg/Library/UefiLib/UefiLib.inf
  HiiLib|MdeModulePkg/Library/UefiHiiLib/UefiHiiLib.inf
  UefiHiiServicesLib|MdeModulePkg/Library/UefiHiiServicesLib/UefiHiiServicesLib.inf
  DevicePathLib|MdePkg/Library/UefiDevicePathLib/UefiDevicePathLib.inf
  UefiDecompressLib|IntelFrameworkModulePkg/Library/BaseUefiTianoCustomDecompressLib/BaseUefiTianoCustomDecompressLib.inf
  PeiServicesTablePointerLib|MdePkg/Library/PeiServicesTablePointerLibIdt/PeiServicesTablePointerLibIdt.inf
  PeiServicesLib|MdePkg/Library/PeiServicesLib/PeiServicesLib.inf
  DxeServicesLib|MdePkg/Library/DxeServicesLib/DxeServicesLib.inf
  DxeServicesTableLib|MdePkg/Library/DxeServicesTableLib/DxeServicesTableLib.inf
  UefiCpuLib|UefiCpuPkg/Library/BaseUefiCpuLib/BaseUefiCpuLib.inf
  #
  # Framework
  #
  #S3BootScriptLib|MdeModulePkg/Library/PiDxeS3BootScriptLib/DxeS3BootScriptLib.inf
  S3BootScriptLib|$(PLAT_PKG)/Override/MdeModulePkg/Library/PiDxeS3BootScriptLib/DxeS3BootScriptLib.inf
  S3IoLib|MdePkg/Library/BaseS3IoLib/BaseS3IoLib.inf
  S3PciLib|MdePkg/Library/BaseS3PciLib/BaseS3PciLib.inf

  #
  # Generic Modules
  #
  UefiUsbLib|MdePkg/Library/UefiUsbLib/UefiUsbLib.inf
  UefiScsiLib|MdePkg/Library/UefiScsiLib/UefiScsiLib.inf
  NetLib|MdeModulePkg/Library/DxeNetLib/DxeNetLib.inf
  IpIoLib|MdeModulePkg/Library/DxeIpIoLib/DxeIpIoLib.inf
  UdpIoLib|MdeModulePkg/Library/DxeUdpIoLib/DxeUdpIoLib.inf
  TcpIoLib|MdeModulePkg/Library/DxeTcpIoLib/DxeTcpIoLib.inf
  DpcLib|MdeModulePkg/Library/DxeDpcLib/DxeDpcLib.inf
  OemHookStatusCodeLib|MdeModulePkg/Library/OemHookStatusCodeLibNull/OemHookStatusCodeLibNull.inf
  CapsuleLib|$(PLAT_PKG)/Library/PlatformCapsuleLib/PlatformCapsuleLib.inf

  LogoLib|ServerCommonPkg/Library/LogoLib/LogoLib.inf
  UefiBootManagerLib|MdeModulePkg/Library/UefiBootManagerLib/UefiBootManagerLib.inf
  SynchronizationLib|MdePkg/Library/BaseSynchronizationLib/BaseSynchronizationLib.inf
  SecurityManagementLib|MdeModulePkg/Library/DxeSecurityManagementLib/DxeSecurityManagementLib.inf
  SmmCorePlatformHookLib|$(PLAT_PKG)/Override/MdeModulePkg/Library/SmmCorePlatformHookLib/SmmCorePlatformHookLib.inf
  CustomizedDisplayLib|$(PLAT_PKG)/Library/CustomizedDisplayLib/CustomizedDisplayLib.inf

  #
  # CPU
  #
  #
  # OVERIDE_HSD_5372585_BEGIN
  #
  MtrrLib|$(SKT_PKG)/Override/UefiCpuPkg/Library/MtrrLib/MtrrLib.inf
  #
  # OVERIDE_HSD_5372585_END
  #

  SmmCpuPlatformHookLib|$(SKT_PKG)/Library/SmmCpuPlatformHookLib/SmmCpuPlatformHookLib.inf
  PlatformCpuLib|$(PLAT_PKG)/Library/PlatformCpuLib/PlatformCpuLib.inf
  CpuS3MsrLib|$(SKT_PKG)/Library/CpuS3MsrLib/CpuS3MsrLib.inf

!if $(AP_ENABLE) == TRUE
  LocalApicLib|$(SKT_PKG)/Override/UefiCpuPkg/Library/BaseXApicX2ApicLib/BaseXApicX2ApicLib.inf
!else
  LocalApicLib|UefiCpuPkg/Library/BaseXApicX2ApicLib/BaseXApicX2ApicLib.inf
!endif
  CpuConfigLib|$(SKT_PKG)/Override/IA32FamilyCpuPkg/Library/CpuConfigLib/CpuConfigLib.inf
  #
  # RcCommon
  #
  PcieAddrLib|CpRcPkg/Library/PcieAddressLib/PcieAddressLib.inf
  SiliconAccessLib|CpRcPkg/Library/UsraAccessLib/UsraAccessLib.inf
  UsraLib|CpRcPkg/Library/DxeUsraLib/DxeUsraLib.inf
  UsraRegisterFilterLib|CpRcPkg/Library/UsraRegisterFilterLibNull/UsraRegisterFilterLibNull.inf

  #
  # PCH
  #
  SmbusLib|CpPlatPkg/Library/BaseSmbusLibNull/BaseSmbusLibNull.inf
  SmmLib|$(PCH_PKG)/Library/PchSmmLib/PchSmmLib.inf
  MmPciLib|CpRcPkg/Library/MmPciBaseLib/MmPciBaseLib.inf
  ResetSystemLib|$(PCH_PKG)/Library/PchResetSystemLib/ResetSystemLib.inf
  PchChipsetInitHobLib|$(PCH_PKG)/Library/PchChipsetInitHobLib/PchChipsetInitHobLib.inf
  PchOemSmmLib|$(PCH_PKG)/Library/PchOemSmmLib/PchOemSmmLib.inf

  PchPlatformLib|$(PCH_PKG)/Library/PchLibNull/PchLibNull.inf
  PeiDxeSmmPchPciExpressHelpersLib|$(PCH_PKG)/LibraryPrivate/PeiDxeSmmPchPciExpressHelpersLib/PeiDxeSmmPchPciExpressHelpersLib.inf
  PchPciExpressHelpersLib|$(PCH_PKG)/LibraryPrivate/PeiDxeSmmPchPciExpressHelpersLib/PeiDxeSmmPchPciExpressHelpersLib.inf
  PchInitLib|$(PCH_PKG)/LibraryPrivate/PeiPchInitLib/PeiPchInitLib.inf
  PchAdrLib|$(PCH_PKG)/Library/PeiDxeSmmPchAdrLib/PeiDxeSmmPchAdrLib.inf
  GpioLib|$(PCH_PKG)/Library/PeiDxeSmmGpioLib/PeiDxeSmmGpioLib.inf
  GpioPrivateLib|$(PCH_PKG)/LibraryPrivate/PeiDxeSmmGpioPrivateLib/PeiDxeSmmGpioPrivateLib.inf
  PchPolicyLib|$(PCH_PKG)/Library/PeiPchPolicyLib/PeiPchPolicyLib.inf
  RcFviLib|$(PCH_PKG)/Library/DxeSiFviLib/DxeSiFviLib.inf
  PlatformPostCodeMapLib|CpPlatPkg/Library/PlatformPostCodeMapLib/PlatformPostCodeMapLib.inf
  PeiPolicyInit|$(PLAT_PKG)/Platform/Pei/PolicyInit/PeiPolicyInitLib.inf
  PchSmmControlLib|$(PCH_PKG)/LibraryPrivate/PeiPchSmmControlLib/PeiPchSmmControlLib.inf
  PchHdaLib|$(PCH_PKG)/LibraryPrivate/DxePchHdaLib/DxePchHdaLib.inf
  PeiPchRcLib|$(PCH_PKG)/LibraryPrivate/PeiPchRcLib/PeiPchRcLib.inf
  PchInitCommonLib|$(PCH_PKG)/LibraryPrivate/PeiDxeSmmPchInitCommonLib/PeiDxeSmmPchInitCommonLib.inf
  PchResetCommonLib|$(PCH_PKG)/LibraryPrivate/BasePchResetCommonLib/BasePchResetCommonLib.inf
  PchSmbusCommonLib|$(PCH_PKG)/LibraryPrivate/PeiDxeSmmPchSmbusCommonLib/PeiDxeSmmPchSmbusCommonLib.inf
  PchSpiCommonLib|$(PCH_PKG)/LibraryPrivate/BasePchSpiCommonLib/BasePchSpiCommonLib.inf
  PchWdtCommonLib|$(PCH_PKG)/LibraryPrivate/PeiDxeSmmPchWdtCommonLib/PeiDxeSmmPchWdtCommonLib.inf
  PchCycleDecodingLib|$(PCH_PKG)/Library/PeiDxeSmmPchCycleDecodingLib/PeiDxeSmmPchCycleDecodingLib.inf
  PchGbeLib|$(PCH_PKG)/Library/PeiDxeSmmPchGbeLib/PeiDxeSmmPchGbeLib.inf
  PchInfoLib|$(PCH_PKG)/Library/PeiDxeSmmPchInfoLib/PeiDxeSmmPchInfoLib.inf
  PchP2sbLib|$(PCH_PKG)/Library/PeiDxeSmmPchP2sbLib/PeiDxeSmmPchP2sbLib.inf
  PchPcieRpLib|$(PCH_PKG)/Library/PeiDxeSmmPchPcieRpLib/PeiDxeSmmPchPcieRpLib.inf
  PchPcrLib|$(PCH_PKG)/Library/PeiDxeSmmPchPcrLib/PeiDxeSmmPchPcrLib.inf
  PchPmcLib|$(PCH_PKG)/Library/PeiDxeSmmPchPmcLib/PeiDxeSmmPchPmcLib.inf
  PchSbiAccessLib|$(PCH_PKG)/Library/PeiDxeSmmPchSbiAccessLib/PeiDxeSmmPchSbiAccessLib.inf
  PchAlternateAccessModeLib|$(PCH_PKG)/LibraryPrivate/PeiDxeSmmPchAlternateAccessModeLib/PeiDxeSmmPchAlternateAccessModeLib.inf
  PchHsioLib|$(PCH_PKG)/Library/PeiDxeSmmPchHsioLib/PeiDxeSmmPchHsioLib.inf
  PchXhciLib|$(PCH_PKG)/LibraryPrivate/PeiDxeSmmPchXhciLib/PeiDxeSmmPchXhciLib.inf
  PchEspiLib|$(PCH_PKG)/Library/PeiDxeSmmPchEspiLib/PeiDxeSmmPchEspiLib.inf
  !if $(TRACE_HUB_ENABLE) == TRUE
    TraceHubInitLib|$(PCH_PKG)/Library/PeiDxeSmmTraceHubInitLib/PeiDxeSmmTraceHubInitLib.inf
    TraceHubLib|$(PCH_PKG)/Library/PeiDxeSmmTraceHubLib/PeiDxeSmmTraceHubLib.inf
    TraceHubDebugLib|$(PLAT_PKG)/Library/BaseTraceHubDebugLib/BaseTraceHubDebugLib.inf
  !else
    TraceHubInitLib|$(PCH_PKG)/Library/BaseTraceHubInitLibNull/BaseTraceHubInitLibNull.inf
    TraceHubLib|$(PLAT_PKG)/Library/BaseTraceHubLibNull/BaseTraceHubLibNull.inf
    TraceHubDebugLib|$(PLAT_PKG)/Library/BaseTraceHubDebugLibNull/BaseTraceHubDebugLibNull.inf
  !endif

  SetupLib|$(RP_PKG)/Library/SetupLib/DxeSetupLib.inf
  TimerLib|PurleyPlatPkg/Override/PcAtChipsetPkg/Library/AcpiTimerLib/DxeAcpiTimerLib.inf

  MultiPlatSupportLib|CpPcPlatPkg/Library/MultiPlatSupportLib/MultiPlatSupportLib.inf
  ReadFfsLib|CpPcPlatPkg/Library/ReadFfsLib/ReadFfsLib.inf
  PlatformSetupVariableSyncLib|CpPcPlatPkg/Library/PlatformSetupVariableSyncLibNull/PlatformSetupVariableSyncLibNull.inf
  PlatformVariableHookLib |CpPcPlatPkg/Library/PlatformVariableHookLibNull/PlatformVariableHookLibNull.inf

  PlatformBootManagerLib|$(PLAT_PKG)/Library/PlatformBootManagerLib/PlatformBootManagerLib.inf
!if $(AP_ENABLE) == TRUE
    PlatformSecLib|$(SKT_PKG)/Library/ProcessorStartup/ProcessorStartupLBG_AP.inf
!else
  !if $(BTG_ENABLE) == TRUE
    PlatformSecLib|$(SKT_PKG)/Library/ProcessorStartup/ProcessorStartupLBGBTG.inf
  !else
    PlatformSecLib|$(SKT_PKG)/Library/ProcessorStartup/ProcessorStartupLBG.inf
  !endif
!endif
  CpuPpmLib|$(SKT_PKG)/Library/ProcessorPowerManagement/CpuPpmLib.inf
  OemProcessorStartupLib|$(PLAT_PKG)/Library/OemProcessorStartupLib/OemProcessorStartupLib.inf
  SerialPortLib|$(PLAT_PKG)/Library/SerialPortLib/SerialPortLib.inf
  PlatformHooksLib|$(PLAT_PKG)/Library/PlatformHooksLib/PlatformHooksLib.inf
  SmmScriptLib|$(SKT_PKG)/Library/SmmScriptLib/SmmScriptLib.inf
  BiosGuardInitLib|$(PLAT_PKG)/Library/BiosGuardInitLib/BiosGuardInit.inf
  FpgaPlatformHooksLib|$(PLAT_PKG)/Library/FpgaPlatformHooksLib/FpgaPlatformHooksLib.inf

!if $(RAS_SUPPORT) == TRUE
  ProcMemErrReporting | $(SKT_PKG)/Library/ProcMemErrReporting/$(CLV)ProcMemErrReporting.inf
  OemRasLib|$(PLAT_PKG)/Library/OemRasLib/OemRasLib.inf
  WheaPlatformHooksLib|$(PLAT_PKG)/Ras/Whea/WheaPlatformHooksLib/WheaPlatformHooksLib$(SPT).inf
  WheaErrorLib|CpPlatPkg/Library/WheaErrorLib/WheaErrorLib.inf
  EmcaPlatformHooksLib | $(SKT_PKG)/Library/emcaplatformhookslib/emcaplatformhookslib.inf
  EmcaLib | $(PLAT_PKG)/Ras/Library/emcalib/emcalib.inf
  MpSyncDataLib | $(PLAT_PKG)/Ras/Library/mpsyncdatalib/mpsyncdatalib.inf
  ChipsetErrReporting | $(SKT_PKG)/Library/ChipsetErrReporting/ChipsetErrReporting.inf
  WheaSiliconHooksLib|$(SKT_PKG)/Library/WheaSiliconHooksLib/WheaSiliconHooksLib.inf
!endif

!if $(IPMI_SUPPORT) == TRUE
  IpmiMgmtModeRuntimeLib|CpPlatPkg/Library/IpmiMgmtModeRuntimeLib/IpmiMgmtModeRuntimeLib.inf
!endif
  PlatformSmbiosIFWILib|$(PLAT_PKG)/Library/PlatformSmbiosIFWILib/PlatformSmbiosIFWILib.inf

  #
  # CmosLib
  #
  CmosAccessLib|BpCommonPkg/Library/CmosAccessLib/CmosAccessLib.inf
  PlatformCmosAccessLib|$(PLAT_PKG)/Library/PlatformCmosAccessLib/PlatformCmosAccessLib.inf

  #
  # iflash32 related Lib
  #
  PlatformSmiFlashLib|$(RP_PKG)/Library/PlatformSmiFlashLib/PlatformSmiFlashLib.inf
  SecurityRevisonLib|CpPcPlatPkg/Library/PeiDxeSecurityRevisionLib/PeiDxeSecurityRevisionLib.inf
  FlashUpdateLib|CpPcPlatPkg/Library/FlashUpdateLibNull/FlashUpdateLibNull.inf
  PlatformRecoveryLib|CpPcPlatPkg/Library/PlatformRecoveryLibNull/PlatformRecoveryLibNull.inf
  ReadFfsLib|CpPcPlatPkg/Library/ReadFfsLib/ReadFfsLib.inf
  #PlatformFlashLib|$(RP_PKG)/Library/PlatformFlashLib/PlatformFlashLib.inf
  FrontPanelLockLib|CpPlatPkg/Library/DxeFrontPanelLockLib/DxeFrontPanelLockLib.inf
  SmmMemLib|$(PLAT_PKG)/Override/MdePkg/Library/SmmMemLib/SmmMemLib.inf
#BPE_1310.400_TODO:which one?
  SmmPeriodicTimerLib|PurleyPlatPkg/Library/SmmPchPeriodicTimerLib/SmmPchPeriodicTimerLib.inf
#  SmmPeriodicTimerLib|CpPcPlatPkg/Library/SmmPeriodicTimerLib/SmmPeriodicTimerLib.inf

  LegacyInterruptSupportLib|BpCommonPkg/Csm/Library/LegacyInterruptSupportLib/LegacyInterruptSupportLib.inf
  PasswordEncodeLib|CpPcPlatPkg/Library/BasePasswordEncodeLibSha2/BasePasswordEncodeLibSha2.inf
  PlatformSaltLib|$(PLAT_PKG)/Library/PlatformSaltLib/PlatformSaltLib.inf

  #
  # Misc
  #
#BPE_1310.400_TODO: switch to new one?
#  DebugLib|MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf
  DebugLib|IntelFrameworkModulePkg/Library/PeiDxeDebugLibReportStatusCode/PeiDxeDebugLibReportStatusCode.inf
  DebugPrintErrorLevelLib|$(PLAT_PKG)/Library/DebugPrintErrorLevelLibCmos/DebugPrintErrorLevelLibCmos.inf
  PerformanceLib|MdePkg/Library/BasePerformanceLibNull/BasePerformanceLibNull.inf
  TpmCommLib|SecurityPkg/Library/TpmCommLib/TpmCommLib.inf
  PostCodeLib|MdePkg/Library/BasePostCodeLibDebug/BasePostCodeLibDebug.inf
  Tpm12CommandLib|SecurityPkg/Library/Tpm12CommandLib/Tpm12CommandLib.inf
#  TcgPhysicalPresenceLib|$(PLAT_PKG)/Override/SecurityPkg/Library/DxeTcgPhysicalPresenceLib/DxeTcgPhysicalPresenceLib.inf

#
# BPE_1310.400_TODO: add a real version of this to PurleyPlatPkg from old code.
#

!if $(TPM2_ENABLE) == TRUE
  Tpm2CommandLib|SecurityPkg/Library/Tpm2CommandLib/Tpm2CommandLib.inf
#  FTpmHciCommandLib|...Pkg/Library/FTpmHciCommandLib/FTpmHciCommandLib.inf
!endif

  DebugAgentLib|MdeModulePkg/Library/DebugAgentLibNull/DebugAgentLibNull.inf

  IntrinsicLib|CryptoPkg/Library/IntrinsicLib/IntrinsicLib.inf
  OpensslLib|CryptoPkg/Library/OpensslLib/OpensslLib.inf


  ProcMemInitLib|$(SKT_PKG)/Library/ProcMemInit/ProcMemInit.inf
  SoftSkuLib|$(SKT_PKG)/Library/SoftSkuLib/SoftSku.inf


  BaseMemoryCoreLib|CpRcPkg/Library/BaseMemoryCoreLib/BaseMemoryCoreLibPurley.inf
  AdvMemTestLib|CpRcPkg/Library/AdvMemTestLib/AdvMemTestLib.inf

#
# If we are building the MRC OEM Hooks PPI, we build a library for optimizing OEM
# hook PEIM size.
#
# If we are not building the MRC OEM Hooks PPI, we use the original OEM hooks
# library class implementations for all modules.
# If we use the PPI solution, we use the original library class instances for
# DXE and SMM, but not for all PEIM.
#
# See the PlatformPkgConfig.dsc for the basic overall design comment.
#
!if $(MRC_OEM_HOOKS_PPI_SUPPORT) == TRUE
  MrcHooksServicesLib|$(PLAT_PKG)/Library/MrcHooksServicesLib/MrcHooksServicesLib.inf
!else
  OemProcMemInitLib|$(PLAT_PKG)/Library/OemProcMemInitLib/OemProcMemInitLib.inf
  OemIioInitLib|$(PLAT_PKG)/Library/OemIioInit/OemIioInitHooks.inf
!endif

  OemCrystalRidgeLib|$(PLAT_PKG)/Library/OemCrystalRidgeLib/OemCrystalRidgeHooks.inf
  IioInitLib|$(SKT_PKG)/Iio/Library/PeiDxeCommonIioInitLib/IioInitLib.inf
  MemoryEvContentLib|$(SKT_PKG)/Library/MemoryEvContent/MemoryEvContent.inf
  DxeCrystalRidgeFlushNearMemoryLib|$(SKT_PKG)/Library/DxeCrystalRidgeFlushNearMemoryLib/DxeCrystalRidgeFlushNearMemoryLib.inf
  SmmCrystalRidgeFlushNearMemoryLib|$(SKT_PKG)/Library/SmmCrystalRidgeFlushNearMemoryLib/SmmCrystalRidgeFlushNearMemoryLib.inf

!if $(IPMI_SUPPORT) == TRUE
  IpmiBaseLib|CpPlatPkg/Library/IpmiBaseLib/IpmiBaseLib.inf
  EfiServerManagementLib|CpPlatPkg/Library/ServerManagementLib/ServerManagementLib.inf
!endif

  PchAslUpdateLib | $(PCH_PKG)/Library/DxeAslUpdateLib/DxeAslUpdateLib.inf
# ME Stack
!if $(ME_ENABLE) == TRUE
  MeChipsetLib | $(SKT_PKG)/Me/Heci/Library/MeChipsetLib/MeChipsetLib.inf
!endif

  BeepLib|BpCommonPkg/Library/BeepLibNull/BeepLibNull.inf
  SmmMemLib|$(PLAT_PKG)/Override/MdePkg/Library/SmmMemLib/SmmMemLib.inf
  TcgPpVendorLib|SecurityPkg/Library/TcgPpVendorLibNull/TcgPpVendorLibNull.inf
  Tcg2PpVendorLib|SecurityPkg/Library/Tcg2PpVendorLibNull/Tcg2PpVendorLibNull.inf

[LibraryClasses.common.SEC]
  #
  # SEC phase
  #
  PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf

[LibraryClasses.IA32]
  #
  # PEI phase common
  #
  PcdLib|MdePkg/Library/PeiPcdLib/PeiPcdLib.inf
  HobLib|MdePkg/Library/PeiHobLib/PeiHobLib.inf
  CsrToPcieLib|$(SKT_PKG)/Library/CsrToPcieLib/CsrToPciePeiLib.inf
  UsraLib|CpRcPkg/Library/PeiUsraLib/PeiUsraLib.inf
  USRATraceLib|$(SKT_PKG)/Library/USRATraceLib/USRATraceLib.inf
  MicrocodeLib|$(SKT_PKG)/Library/MicrocodeLib/MicrocodeLib.inf
  PcieAddrLib|$(SKT_PKG)/Library/PcieAddressLib/PcieAddressLib.inf
  S3BootScriptLib|MdePkg/Library/BaseS3BootScriptLibNull/BaseS3BootScriptLibNull.inf
  MemoryAllocationLib|MdePkg/Library/PeiMemoryAllocationLib/PeiMemoryAllocationLib.inf
  ReportStatusCodeLib|MdeModulePkg/Library/PeiReportStatusCodeLib/PeiReportStatusCodeLib.inf
  ExtractGuidedSectionLib|MdePkg/Library/PeiExtractGuidedSectionLib/PeiExtractGuidedSectionLib.inf
  LockBoxLib|MdeModulePkg/Library/SmmLockBoxLib/SmmLockBoxPeiLib.inf
  CpuExceptionHandlerLib|UefiCpuPkg/Library/CpuExceptionHandlerLib/SecPeiCpuExceptionHandlerLib.inf
  MpInitLib|$(SKT_PKG)/Override/UefiCpuPkg/Library/MpInitLib/PeiMpInitLib.inf
  PeiPolicyUpdateLib|$(PLAT_PKG)/Library/PeiPolicyUpdateLib/PeiPolicyUpdateLib.inf

  SetupLib|$(RP_PKG)/Library/SetupLib/PeiSetupLib.inf
  PerformanceLib|$(PLAT_PKG)/Override/MdeModulePkg/Library/PeiPerformanceLib/PeiPerformanceLib.inf
  TimerLib|PurleyPlatPkg/Override/PcAtChipsetPkg/Library/AcpiTimerLib/PeiAcpiTimerLib.inf

  PeiPdrAccessLib|$(PLAT_PKG)/Library/PeiPdrAccessLib/PeiPdrAccessLib.inf

  BaseCryptLib|CryptoPkg/Library/BaseCryptLib/PeiCryptLib.inf

!if $(TPM2_ENABLE) == TRUE
  Tpm2DeviceLib|SecurityPkg/Library/Tpm2DeviceLibRouter/Tpm2DeviceLibRouterPei.inf
  Tpm12DeviceLib|SecurityPkg/Library/Tpm12DeviceLibDTpm/Tpm12DeviceLibDTpm.inf
  HashLib|SecurityPkg/Library/HashLibBaseCryptoRouter/HashLibBaseCryptoRouterPei.inf
!endif

  PeiPlatformHookLib|$(RP_PKG)/Library/PeiPlatformHookLib/PeiPlatformHooklib.inf
  PlatformClocksLib | $(RP_PKG)/Library/PlatformClocksLib/Pei/PlatformClocksLib.inf

# ME Stack
!if $(ME_ENABLE) == TRUE
  MeTypeLib | $(SKT_PKG)/Me/Heci/Library/MeTypeLib/MeTypeLib.inf
  HeciMsgPeiLib   | $(SKT_PKG)/Me/Heci/Library/HeciMsgPeiLib/HeciMsgPeiLib.inf
  MeSpsPolicyPeiLib | $(PLAT_PKG)/Me/Policy/Library/PeiSpsPolicy/MeSpsPolicyPeiLib.inf
  MeSpsPolicyAccessPeiLib | $(SKT_PKG)/Me/Policy/Library/MeSpsPolicyAccessPeiLib/MeSpsPolicyAccessPeiLib.inf
  HeciCoreLib  | $(SKT_PKG)/Me/Heci/Library/HeciCoreLib/HeciCoreLib.inf
  MeLibPei | $(PLAT_PKG)/Me/Library/MeLibPei/MeLibPei.inf
  PeiMePolicyLib | $(PLAT_PKG)/Me/Policy/AMT/Library/PeiMePolicyLib/PeiMePolicyLib.inf
  MeConfigLib | $(PLAT_PKG)/Me/Policy/Library/MeConfigLib/MeConfigLib.inf

  SpsPeiLib  | $(SKT_PKG)/Me/Sps/Library/SpsPeiLib/SpsPeiLib.inf

  PeiMePolicyUpdateLib | $(PLAT_PKG)/Me/Policy/AMT/Library/PeiMePolicyUpdateLib/PeiMePolicyUpdateLib.inf
  AmtLibPei | $(SKT_PKG)/Me/AMT/Library/AmtLibPei/AmtLibPei.inf
  MeShowBufferLib | $(SKT_PKG)/Me/Heci/Library/PeiDxeMeShowBufferLib/PeiDxeMeShowBufferLib.inf
  !if $(ME_WS_ENABLE) == TRUE
    !if $(AMT_ENABLE) == TRUE
      AlertStandardFormatLib | $(SKT_PKG)/Me/AMT/LibraryPrivate/PeiDxeAlertStandardFormatLib/PeiDxeAlertStandardFormatLib.inf
      ActiveManagementLib    | $(SKT_PKG)/Me/AMT/LibraryPrivate/PeiActiveManagementLib/PeiActiveManagementLib.inf
      PeiAmtPolicyLib        | $(PLAT_PKG)/Me/Policy/AMT/Library/PeiAmtPolicyLib/PeiAmtPolicyLib.inf
    !else
      AlertStandardFormatLib | $(SKT_PKG)/Me/Heci/Library/MeDummyLib/MeDummyLib.inf
      ActiveManagementLib    | $(SKT_PKG)/Me/Heci/Library/MeDummyLib/MeDummyLib.inf
      PeiAmtPolicyLib        | $(SKT_PKG)/Me/Heci/Library/MeDummyLib/MeDummyLib.inf
    !endif
    PeiAmtLib              | $(SKT_PKG)/Me/AMT/Library/PeiAmtLib/PeiAmtLib.inf
  !else
    AlertStandardFormatLib | $(SKT_PKG)/Me/Heci/Library/MeDummyLib/MeDummyLib.inf
    ActiveManagementLib    | $(SKT_PKG)/Me/Heci/Library/MeDummyLib/MeDummyLib.inf
    PeiAmtPolicyLib        | $(SKT_PKG)/Me/Heci/Library/MeDummyLib/MeDummyLib.inf
    PeiAmtLib              | $(SKT_PKG)/Me/Heci/Library/MeDummyLib/MeDummyLib.inf
  !endif
!else
  HeciCoreLib          | $(SKT_PKG)/Me/Heci/Library/MeDummyLib/MeDummyLib.inf
  MeTypeLib            | $(SKT_PKG)/Me/Heci/Library/MeDummyLib/MeDummyLib.inf
  MeLibPei             | $(SKT_PKG)/Me/Heci/Library/MeDummyLib/MeDummyLib.inf
  HeciMsgPeiLib        | $(SKT_PKG)/Me/Heci/Library/MeDummyLib/MeDummyLib.inf
  AmtLibPei            | $(SKT_PKG)/Me/Heci/Library/MeDummyLib/MeDummyLib.inf
  SpsPeiLib            | $(SKT_PKG)/Me/Heci/Library/MeDummyLib/MeDummyLib.inf
  AlertStandardFormatLib | $(SKT_PKG)/Me/Heci/Library/MeDummyLib/MeDummyLib.inf
  ActiveManagementLib    | $(SKT_PKG)/Me/Heci/Library/MeDummyLib/MeDummyLib.inf
  PeiAmtPolicyLib        | $(SKT_PKG)/Me/Heci/Library/MeDummyLib/MeDummyLib.inf
  PeiAmtLib              | $(SKT_PKG)/Me/Heci/Library/MeDummyLib/MeDummyLib.inf
  PeiMePolicyLib         | $(SKT_PKG)/Me/Heci/Library/MeDummyLib/MeDummyLib.inf
  MeConfigLib            | $(SKT_PKG)/Me/Heci/Library/MeDummyLib/MeDummyLib.inf
  PeiMePolicyUpdateLib   | $(SKT_PKG)/Me/Heci/Library/MeDummyLib/MeDummyLib.inf
!endif

  #
  # NULL library for when platform package has no $(PLAT_PKG)/Library/LtPeiLib
  #
  LtPeiLib|ServerCommonPkg/Library/LtPeiLibNull/LtPeiLibNull.inf

  UbaPlatLib|$(PLAT_PKG)/Library/PeiUbaPlatLib/PeiUbaPlatLib.inf
  UbaGpioInitLib|$(RP_PKG)/Library/UbaGpioInitLib/UbaGpioInitLib.inf

  IePeiPolicyAccessLib  | $(SKT_PKG)/Ie/Policy/Library/IePeiPolicyAccessLib/IePeiPolicyAccessLib.inf
!if $(IE_ENABLE) == TRUE
  IeHeciCoreLib         | $(SKT_PKG)/Ie/Heci/Library/IeHeciCoreLib/IeHeciCoreLib.inf
  IePeiPolicyUpdateLib  | $(PLAT_PKG)/Ie/Policy/Library/Pei/IePeiPolicyUpdateLib.inf
!endif

   FpgaConfigurationLib | $(SKT_PKG)/Library/FpgaConfigurationLib/FpgaConfigurationPeiLib.inf
   FpgaFvDataLib|$(SKT_PKG)/Library/FpgaFvDataLib/FpgaFvDataPeiLib.inf

  IioInitPeiLib|$(SKT_PKG)/Iio/Library/PeiIioInitLib/IioInitPeiLib.inf

#
# As we are using the PPI OEM hooks solution, PEIM consuming OEM hooks
# should use the new OEM hooks library class instances that call the PPI
#
# See the PlatformPkgConfig.dsc for the basic overall design comment.
#
!if $(MRC_OEM_HOOKS_PPI_SUPPORT) == TRUE
  OemProcMemInitLib|$(SKT_PKG)/Library/OemProcMemInitLibPpi/OemProcMemInitLibPpi.inf
  OemIioInitLib|$(SKT_PKG)/Library/OemIioInitLibPpi/OemIioInitLibPpi.inf
!endif

  UbaKtiLib|$(PLAT_PKG)/Library/UbaKtiLib/UbaKtiLib.inf

  PeiVariableAddressLib|$(PLAT_PKG)/Library/PeiVariableAddress/PeiVariableAddress.inf

[LibraryClasses.common.DXE_CORE]
  PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf

[LibraryClasses.common.DXE_DRIVER]
  PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  MmPciLib|CpRcPkg/Library/DxeMmPciBaseLib/DxeMmPciBaseLib.inf
  FpgaConfigurationLib | $(SKT_PKG)/Library/FpgaConfigurationLib/FpgaConfigurationDxeLib.inf
  FpgaFvDataLib|$(SKT_PKG)/Library/FpgaFvDataLib/FpgaFvDataDxeLib.inf
  Tpm12DeviceLib|SecurityPkg/Library/Tpm12DeviceLibDTpm/Tpm12DeviceLibDTpm.inf
  Tpm12CommandLib|SecurityPkg/Library/Tpm12CommandLib/Tpm12CommandLib.inf

[LibraryClasses.common.DXE_RUNTIME_DRIVER]
  PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  MmPciLib|CpRcPkg/Library/DxeMmPciBaseLib/DxeMmPciBaseLib.inf

[LibraryClasses.common.UEFI_DRIVER,LibraryClasses.common.UEFI_APPLICATION]
  PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf

[LibraryClasses.common.SMM_CORE]
  PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  HobLib|MdePkg/Library/DxeHobLib/DxeHobLib.inf

[LibraryClasses.common.DXE_SMM_DRIVER]
  PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  MmPciLib|CpRcPkg/Library/DxeMmPciBaseLib/SmmMmPciBaseLib.inf
  FpgaConfigurationLib | $(SKT_PKG)/Library/FpgaConfigurationLib/FpgaConfigurationSmmLib.inf

[LibraryClasses.X64]
  #
  # DXE phase common
  #
  HobLib|MdePkg/Library/DxeHobLib/DxeHobLib.inf
  MemoryAllocationLib|MdePkg/Library/UefiMemoryAllocationLib/UefiMemoryAllocationLib.inf
  ReportStatusCodeLib|MdeModulePkg/Library/DxeReportStatusCodeLib/DxeReportStatusCodeLib.inf
  ExtractGuidedSectionLib|MdePkg/Library/DxeExtractGuidedSectionLib/DxeExtractGuidedSectionLib.inf
  #DxeS3BootScriptLib|MdeModulePkg/Library/PiDxeS3BootScriptLib/DxeS3BootScriptLib.inf
  DxeS3BootScriptLib|$(PLAT_PKG)/Override/MdeModulePkg/Library/PiDxeS3BootScriptLib/DxeS3BootScriptLib.inf

  FileExplorerLib|MdeModulePkg/Library/FileExplorerLib/FileExplorerLib.inf
  Tpm2DeviceLib|SecurityPkg/Library/Tpm2DeviceLibRouter/Tpm2DeviceLibRouterDxe.inf
  HashLib|SecurityPkg/Library/HashLibBaseCryptoRouter/HashLibBaseCryptoRouterDxe.inf
  Tcg2PhysicalPresenceLib|$(PLAT_PKG)/Override/SecurityPkg/Library/DxeTcg2PhysicalPresenceLib/DxeTcg2PhysicalPresenceLib.inf
  TpmMeasurementLib|SecurityPkg//Library/DxeTpmMeasurementLib/DxeTpmMeasurementLib.inf

  MeStorageLib|$(SKT_PKG)/Me/Library/MeStorageLib/MeStorageLib.inf

  TcgPhysicalPresenceLib|$(PLAT_PKG)/Override/SecurityPkg/Library/DxeTcgPhysicalPresenceLib/DxeTcgPhysicalPresenceLib.inf
  Tcg2PpVendorLib|$(PLAT_PKG)/Override/BpCommonPkg/Library/TxtOneTouchPpVendorLibSample/DxeTcg2TxtOneTouchPpVendorLibSample.inf
  LockBoxLib|MdeModulePkg/Library/SmmLockBoxLib/SmmLockBoxDxeLib.inf
  BiosIdLib|BpCommonPkg/Library/BiosIdLib/DxeBiosIdLib.inf
  IfwiIdLib|PurleyPlatPkg/Library/IfwiIdLib/DxeIfwiIdLib.inf
  CpuExceptionHandlerLib|UefiCpuPkg/Library/CpuExceptionHandlerLib/DxeCpuExceptionHandlerLib.inf
  MpInitLib|$(SKT_PKG)/Override/UefiCpuPkg/Library/MpInitLib/DxeMpInitLib.inf

!if $(PERFORMANCE_ENABLE) == TRUE
  PerformanceLib|MdeModulePkg/Library/DxePerformanceLib/DxePerformanceLib.inf
  TimerLib|PurleyPlatPkg/Override/PcAtChipsetPkg/Library/AcpiTimerLib/DxeAcpiTimerLib.inf
!endif
  TpmMeasurementLib|SecurityPkg/Library/DxeTpmMeasurementLib/DxeTpmMeasurementLib.inf
  TcgPpVendorLib|BpCommonPkg/Library/TxtOneTouchPpVendorLibSample/DxeTcgTxtOneTouchPpVendorLibSample.inf

  BaseCryptLib|CryptoPkg/Library/BaseCryptLib/BaseCryptLib.inf

#
# ME Stack
#
!if $(ME_ENABLE) == TRUE
  DxeMeLib       | $(PLAT_PKG)/Me/Heci/Library/DxeMeLib/DxeMeLib.inf
  MeTypeLib      | $(SKT_PKG)/Me/Heci/Library/MeTypeLib/MeTypeLib.inf
  PttHeciLib     |$(SKT_PKG)/Me/Library/DxePttHeciLib/DxePttHeciLib.inf
  MeStorageLib   | $(SKT_PKG)/Me/Library/MeStorageLib/MeStorageLib.inf
  HeciCoreLib  | $(SKT_PKG)/Me/Heci/Library/HeciCoreLib/HeciCoreLib.inf
  DxeSpsPolicyLib  | $(PLAT_PKG)/Me/Policy/Library/DxeSpsPolicy/MeSpsPolicyDxeLib.inf
  DxeMePolicyLib   | $(PLAT_PKG)/Me/Policy/AMT/Library/DxeMePolicyLib/DxeMePolicyLib.inf
  DxePolicyUpdateLib | $(PLAT_PKG)/Me/Policy/AMT/Library/DxePolicyUpdateLib/DxePolicyUpdateLib.inf
  MeConfigLib | $(PLAT_PKG)/Me/Policy/Library/MeConfigLib/MeConfigLib.inf
  MeSpsPolicyAccessDxeLib | $(SKT_PKG)/Me/Policy/Library/MeSpsPolicyAccessDxeLib/MeSpsPolicyAccessDxeLib.inf
  !if $(ME_WS_ENABLE) == TRUE
    !if $(AMT_ENABLE) == TRUE
      AmtPlatformLib | $(PLAT_PKG)/Me/AMT/Library/AmtPlatformLib/AmtPlatformLib.inf
      AmtWrapperLib  | $(SKT_PKG)/Me/AMT/Library/AmtWrapperLib/AmtWrapperLib.inf
      AlertStandardFormatLib | $(SKT_PKG)/Me/AMT/LibraryPrivate/PeiDxeAlertStandardFormatLib/PeiDxeAlertStandardFormatLib.inf
      DxeAmtPolicyLib        | $(PLAT_PKG)/Me/Policy/AMT/Library/DxeAmtPolicyLib/DxeAmtPolicyLib.inf
    !else
      AmtPlatformLib | $(SKT_PKG)/Me/Heci/Library/MeDummyLib/MeDummyLib.inf
      AmtWrapperLib  | $(SKT_PKG)/Me/Heci/Library/MeDummyLib/MeDummyLib.inf
      AlertStandardFormatLib | $(SKT_PKG)/Me/Heci/Library/MeDummyLib/MeDummyLib.inf
      DxeAmtPolicyLib        | $(SKT_PKG)/Me/Heci/Library/MeDummyLib/MeDummyLib.inf
    !endif
    MeShowBufferLib        | $(SKT_PKG)/Me/Heci/Library/PeiDxeMeShowBufferLib/PeiDxeMeShowBufferLib.inf
    DxeAmtLib              | $(PLAT_PKG)/Me/AMT/Library/DxeAmtLib/DxeAmtLib.inf
    DxeMbpDataLib          | $(SKT_PKG)/Me/AMT/Library/DxeMbpDataLib/DxeMbpDataLib.inf
  !else
    AmtPlatformLib  | $(SKT_PKG)/Me/Heci/Library/MeDummyLib/MeDummyLib.inf
    MeShowBufferLib | $(SKT_PKG)/Me/Heci/Library/MeDummyLib/MeDummyLib.inf
    DxeAmtPolicyLib | $(SKT_PKG)/Me/Heci/Library/MeDummyLib/MeDummyLib.inf
    DxeAmtLib       | $(SKT_PKG)/Me/Heci/Library/MeDummyLib/MeDummyLib.inf
    DxeMbpDataLib   | $(SKT_PKG)/Me/Heci/Library/MeDummyLib/MeDummyLib.inf
  !endif
  !if $(SPS_ENABLE) == TRUE
    MeUmaEinjLib | $(SKT_PKG)/Me/Heci/Library/MeUmaEinjLib/MeUmaEinjLib.inf
  !else
    MeUmaEinjLib | $(SKT_PKG)/Me/Heci/Library/MeDummyLib/MeDummyLib.inf
  !endif
!else
  HeciCoreLib    | $(SKT_PKG)/Me/Heci/Library/MeDummyLib/MeDummyLib.inf
  AmtPlatformLib | $(SKT_PKG)/Me/Heci/Library/MeDummyLib/MeDummyLib.inf
  DxeMeLib       | $(SKT_PKG)/Me/Heci/Library/MeDummyLib/MeDummyLib.inf
  MeTypeLib      | $(SKT_PKG)/Me/Heci/Library/MeDummyLib/MeDummyLib.inf
  DxeSpsPolicyLib| $(SKT_PKG)/Me/Heci/Library/MeDummyLib/MeDummyLib.inf
  PttHeciLib     | $(SKT_PKG)/Me/Heci/Library/MeDummyLib/MeDummyLib.inf
  DxeMePolicyLib     | $(SKT_PKG)/Me/Heci/Library/MeDummyLib/MeDummyLib.inf
  DxePolicyUpdateLib | $(SKT_PKG)/Me/Heci/Library/MeDummyLib/MeDummyLib.inf
  MeConfigLib        | $(SKT_PKG)/Me/Heci/Library/MeDummyLib/MeDummyLib.inf
  MeStorageLib       | $(SKT_PKG)/Me/Heci/Library/MeDummyLib/MeDummyLib.inf
  DxeAmtPolicyLib | $(SKT_PKG)/Me/Heci/Library/MeDummyLib/MeDummyLib.inf
  DxeAmtLib       | $(SKT_PKG)/Me/Heci/Library/MeDummyLib/MeDummyLib.inf
  DxeMbpDataLib   | $(SKT_PKG)/Me/Heci/Library/MeDummyLib/MeDummyLib.inf
  MeUmaEinjLib    | $(SKT_PKG)/Me/Heci/Library/MeDummyLib/MeDummyLib.inf
  MeSpsPolicyAccessDxeLib | $(SKT_PKG)/Me/Heci/Library/MeDummyLib/MeDummyLib.inf
!endif

!if $(IE_ENABLE) == TRUE
  IeHeciCoreLib  | $(SKT_PKG)/Ie/Heci/Library/IeHeciCoreLib/IeHeciCoreLib.inf
!endif

  PcieCommonInitLib |CpRcPkg/Library/PcieCommonInitLib/PcieCommonInitLib.inf

  UbaPlatLib|$(PLAT_PKG)/Library/UbaPlatLib/UbaPlatLib.inf
  PlatformOpromPolicyLib|$(PLAT_PKG)/Library/PlatformOpromPolicyLibNull/PlatformOpromPolicyLibNull.inf

  IioInitDxeLib|$(SKT_PKG)/Iio/Library/DxeIioInitLib/IioInitDxeLib.inf

#
# As we are using the PPI version of OEM hooks for PEI, we still need to use
# the original library class instances in DXE and SMM components.
# Note we may separate these via protocols in the future.
#
# See the PlatformPkgConfig.dsc for the basic overall design comment.
#
!if $(MRC_OEM_HOOKS_PPI_SUPPORT) == TRUE
  OemProcMemInitLib|$(PLAT_PKG)/Library/OemProcMemInitLib/OemProcMemInitLib.inf
  OemIioInitLib|$(PLAT_PKG)/Library/OemIioInit/OemIioInitHooks.inf
!endif

  PlatformOobOneTouchLib|$(PLAT_PKG)/Library/PlatformOobOneTouchLibNull/PlatformOobOneTouchLibNull.inf

[LibraryClasses.X64.DXE_CORE]
  HobLib|MdePkg/Library/DxeCoreHobLib/DxeCoreHobLib.inf
  MemoryAllocationLib|MdeModulePkg/Library/DxeCoreMemoryAllocationLib/DxeCoreMemoryAllocationLib.inf
  PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
!if $(PERFORMANCE_ENABLE) == TRUE
  PerformanceLib|$(PLAT_PKG)/Override/MdeModulePkg/Library/DxeCorePerformanceLib/DxeCorePerformanceLib.inf
  TimerLib|PurleyPlatPkg/Override/PcAtChipsetPkg/Library/AcpiTimerLib/DxeAcpiTimerLib.inf
!endif

[LibraryClasses.X64.DXE_SMM_DRIVER]
  PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  TcgPpVendorLib|BpCommonPkg/Library/TxtOneTouchPpVendorLibSample/SmmTcgTxtOneTouchPpVendorLibSample.inf
  Tcg2PpVendorLib|$(PLAT_PKG)/Override/BpCommonPkg/Library/TxtOneTouchPpVendorLibSample/SmmTcg2TxtOneTouchPpVendorLibSample.inf

  SmmMemLib|$(PLAT_PKG)/Override/MdePkg/Library/SmmMemLib/SmmMemLib.inf
  UsraLib|CpRcPkg/Library/DxeUsraLib/DxeSmmUsraLib.inf
  MmPciLib|CpRcPkg/Library/DxeMmPciBaseLib/SmmMmPciBaseLib.inf
  CsrToPcieLib|$(SKT_PKG)/Library/CsrToPcieLib/CsrToPcieDxeLib.inf
  USRATraceLib|$(SKT_PKG)/Library/USRATraceLib/USRATraceLib.inf
  MicrocodeLib|$(SKT_PKG)/Library/MicrocodeLib/MicrocodeLib.inf

  SmmServicesTableLib|MdePkg/Library/SmmServicesTableLib/SmmServicesTableLib.inf
  ReportStatusCodeLib|MdeModulePkg/Library/SmmReportStatusCodeLib/SmmReportStatusCodeLib.inf
  MemoryAllocationLib|MdePkg/Library/SmmMemoryAllocationLib/SmmMemoryAllocationLib.inf
  LockBoxLib|MdeModulePkg/Library/SmmLockBoxLib/SmmLockBoxSmmLib.inf
  SmmPeriodicSmiLib|MdePkg/Library/SmmPeriodicSmiLib/SmmPeriodicSmiLib.inf
!if $(PERFORMANCE_ENABLE) == TRUE
  PerformanceLib|MdeModulePkg/Library/SmmPerformanceLib/SmmPerformanceLib.inf
  TimerLib|PurleyPlatPkg/Override/PcAtChipsetPkg/Library/AcpiTimerLib/DxeAcpiTimerLib.inf
!endif
  CpuExceptionHandlerLib|UefiCpuPkg/Library/CpuExceptionHandlerLib/SmmCpuExceptionHandlerLib.inf

!if $(IPMI_SUPPORT) == TRUE
  IpmiBaseLib|CpPlatPkg/Library/SmmIpmiBaseLib/SmmIpmiBaseLib.inf
!endif

  BaseCryptLib|CryptoPkg/Library/BaseCryptLib/SmmCryptLib.inf

  UsraQuiesceLib|PurleySktPkg/Library/UsraQuiesceLib/UsraQuiesceLib.inf
#
# This Library instance is intended to support CLX soft sku.
#
  UsraRegisterFilterLib|PurleySktPkg/Library/UsraRegisterFilterLibSmm/UsraRegisterFilterLibSmm.inf

[LibraryClasses.X64.SMM_CORE]
  MemoryAllocationLib|MdeModulePkg/Library/PiSmmCoreMemoryAllocationLib/PiSmmCoreMemoryAllocationLib.inf
  SmmServicesTableLib|MdeModulePkg/Library/PiSmmCoreSmmServicesTableLib/PiSmmCoreSmmServicesTableLib.inf
  ReportStatusCodeLib|MdeModulePkg/Library/SmmReportStatusCodeLib/SmmReportStatusCodeLib.inf
!if $(PERFORMANCE_ENABLE) == TRUE
  PerformanceLib|$(PLAT_PKG)/Override/MdeModulePkg/Library/SmmCorePerformanceLib/SmmCorePerformanceLib.inf
  TimerLib|PurleyPlatPkg/Override/PcAtChipsetPkg/Library/AcpiTimerLib/DxeAcpiTimerLib.inf
!endif

  Tcg2PpVendorLib|$(PLAT_PKG)/Override/BpCommonPkg/Library/TxtOneTouchPpVendorLibSample/SmmTcg2TxtOneTouchPpVendorLibSample.inf

  BaseCryptLib|CryptoPkg/Library/BaseCryptLib/SmmCryptLib.inf

 SmmMemLib|$(PLAT_PKG)/Override/MdePkg/Library/SmmMemLib/SmmMemLib.inf

[LibraryClasses.X64.DXE_RUNTIME_DRIVER]
  ReportStatusCodeLib|MdeModulePkg/Library/RuntimeDxeReportStatusCodeLib/RuntimeDxeReportStatusCodeLib.inf
!if $(PERFORMANCE_ENABLE) == TRUE
  PerformanceLib|MdeModulePkg/Library/DxePerformanceLib/DxePerformanceLib.inf
  TimerLib|PurleyPlatPkg/Override/PcAtChipsetPkg/Library/AcpiTimerLib/DxeAcpiTimerLib.inf
!endif

  BaseCryptLib|CryptoPkg/Library/BaseCryptLib/RuntimeCryptLib.inf

[LibraryClasses.X64.UEFI_APPLICATION]
  PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
!if $(PERFORMANCE_ENABLE) == TRUE
#  PerformanceLib|MdeModulePkg/Library/DxeSmmPerformanceLib/DxeSmmPerformanceLib.inf
  PerformanceLib|MdeModulePkg/Library/DxePerformanceLib/DxePerformanceLib.inf
  TimerLib|PurleyPlatPkg/Override/PcAtChipsetPkg/Library/AcpiTimerLib/DxeAcpiTimerLib.inf
  FileHandleLib|MdePkg/Library/UefiFileHandleLib/UefiFileHandleLib.inf
!endif
  UsraLib|CpRcPkg/Library/DxeUsraLib/DxeUsraLib.inf

#!if $(PERFORMANCE_ENABLE) == TRUE
#  PerformanceLib|MdeModulePkg/Library/DxePerformanceLib/DxePerformanceLib.inf
#  PerformanceLib|MdeModulePkg/Library/DxeSmmPerformanceLib/DxeSmmPerformanceLib.inf
#!endif

################################################################################
#
# Library Section - list of EDK/Framework libraries for LegacyUsb modules
#
################################################################################
[Libraries.X64]

################################################################################
#
# Pcd Section - list of all EDK II PCD Entries defined by this Platform
#
################################################################################
[PcdsFeatureFlag.common]
  # Server doesn't support capsle update on Reset.
  gEfiMdeModulePkgTokenSpaceGuid.PcdSupportUpdateCapsuleReset|FALSE
  gEfiMdeModulePkgTokenSpaceGuid.PcdFrameworkCompatibilitySupport|FALSE
#  gEfiCpuTokenSpaceGuid.PcdCpuIEDEnabled|TRUE
  gEfiCpuTokenSpaceGuid.PcdCpuSmmEnableBspElection|TRUE
  gEfiCpuTokenSpaceGuid.PcdCpuHotPlugSupport|TRUE
  gEfiMdeModulePkgTokenSpaceGuid.PcdPciBusHotplugDeviceSupport|FALSE

#S3 add
  gEfiMdeModulePkgTokenSpaceGuid.PcdDxeIplSwitchToLongMode|TRUE
#S3 add

  gEfiCpuTokenSpaceGuid.PcdCpuConroeFamilyFlag|FALSE
  gEfiCpuTokenSpaceGuid.PcdCpuCedarMillFamilyFlag|FALSE
  gEfiCpuTokenSpaceGuid.PcdCpuPrescottFamilyFlag|FALSE
  gEfiCpuTokenSpaceGuid.PcdCpuNehalemFamilyFlag|FALSE
  gEfiCpuTokenSpaceGuid.PcdCpuIvyBridgeFamilyFlag|FALSE
  gEfiCpuTokenSpaceGuid.PcdCpuSandyBridgeFamilyFlag|FALSE
  gEfiCpuTokenSpaceGuid.PcdCpuHaswellFamilyFlag|TRUE
  gEfiCpuTokenSpaceGuid.PcdCpuSkylakeFamilyFlag|TRUE

  gEfiCpuTokenSpaceGuid.PcdCpuGateA20MDisableFlag|FALSE
  gEfiCpuTokenSpaceGuid.PcdCpuSmmDebug|TRUE
  gEfiCpuTokenSpaceGuid.PcdCpuSelectLfpAsBspFlag|TRUE
  gEfiCpuTokenSpaceGuid.PcdCpuSocketIdReassignmentFlag|TRUE
  gEfiCpuTokenSpaceGuid.PcdCpuSmmCodeAccessCheckSupport|FALSE
  gEfiCpuTokenSpaceGuid.PcdCpuSmmMsrSaveStateSupport|FALSE

  ## Uncomment for better boot performance
#  gPerfOptTokenSpaceGuid.PcdPreUefiLegacyEnable|FALSE
#  gPerfOptTokenSpaceGuid.PcdLocalVideoEnable|FALSE

  gPlatformTokenSpaceGuid.PcdSupportUnsignedCapsuleImage|TRUE

!if $(DEBUG_FLAGS_ENABLE) == FALSE
  ## This PCD specifies whether or not StatusCode should use serial
  gEfiMdeModulePkgTokenSpaceGuid.PcdStatusCodeUseSerial|FALSE
!endif

  ## This PCD specified whether ACPI SDT protocol is installed.
  gEfiMdeModulePkgTokenSpaceGuid.PcdInstallAcpiSdtProtocol|TRUE

  ## This PCD specifies whether capsule update routine will force update NVRAM region
  gEfiCpPcPlatTokenSpaceGuid.PcdForceUpdateNVram|TRUE

  ## This PCD specifies whether FPGA routine will be active
  gSocketPkgFpgaGuid.PcdSktFpgaActive|TRUE

[PcdsFeatureFlag.X64]

[PcdsFeatureFlag]
  gEfiMdeModulePkgTokenSpaceGuid.PcdHiiOsRuntimeSupport|FALSE
!if $(PERFORMANCE_ENABLE) == TRUE
  gEfiMdeModulePkgTokenSpaceGuid.PcdFirmwarePerformanceDataTableS3Support|TRUE
!endif
[PcdsFixedAtBuild.common]
  gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask|0x2F
!if $(PERFORMANCE_ENABLE) == TRUE
  gEfiMdePkgTokenSpaceGuid.PcdReportStatusCodePropertyMask|0x07
!else
  gEfiMdePkgTokenSpaceGuid.PcdReportStatusCodePropertyMask|0x06
!endif
  gEfiMdeModulePkgTokenSpaceGuid.PcdLoadModuleAtFixAddressEnable|0
  gEfiMdeModulePkgTokenSpaceGuid.PcdHwErrStorageSize|0x0
#S3 modified
  gEfiMdeModulePkgTokenSpaceGuid.PcdMaxVariableSize|0x10000
  gEfiMdeModulePkgTokenSpaceGuid.PcdShadowPeimOnS3Boot|TRUE
#S3 modified

  gEfiMdeModulePkgTokenSpaceGuid.PcdHwErrStorageSize|0x0
  gEfiMdePkgTokenSpaceGuid.PcdPerformanceLibraryPropertyMask|0x0
  gEfiMdePkgTokenSpaceGuid.PcdFSBClock|100000000
  gEfiMdeModulePkgTokenSpaceGuid.PcdPeiCoreMaxPeiStackSize|0x100000
  gEfiMdeModulePkgTokenSpaceGuid.PcdPeiCoreMaxFvSupported|8
  gEfiMdeModulePkgTokenSpaceGuid.PcdPeiCoreMaxPpiSupported|128
  gEfiMdeModulePkgTokenSpaceGuid.PcdMaxSizeNonPopulateCapsule|0x2100000
  gEfiMdeModulePkgTokenSpaceGuid.PcdSmbiosVersion|0x0208
  gEfiMdeModulePkgTokenSpaceGuid.PcdPropertiesTableEnable|FALSE

  gEfiCpuTokenSpaceGuid.PcdCpuIEDRamSize|0x400000
  gEfiCpuTokenSpaceGuid.PcdCpuSmmApSyncTimeout|10000
  gEfiCpuTokenSpaceGuid.PcdCpuMaxLogicalProcessorNumber|512
  gEfiCpuTokenSpaceGuid.PcdPlatformType|2
  gEfiCpuTokenSpaceGuid.PcdPlatformCpuMaxCoreFrequency|4000
  gEfiCpuTokenSpaceGuid.PcdPlatformCpuMaxFsbFrequency|1066
  gEfiCpuTokenSpaceGuid.PcdCpuSmmStackSize|0x10000
  #bypass Microcode FV FV Header + FFS file header
  #SKX_TODO: fix this hard coded value and make a calculated value from .fdf value
  #PcdCpuMicrocodePatchAddress = PcdFlashNvStorageMicrocodeBase + (EFI_FIRMWARE_VOLUME_HEADER. HeaderLength + sizeof (EFI_FFS_FILE_HEADER))
  gEfiCpuTokenSpaceGuid.PcdCpuMicrocodePatchAddress|0xFF8C0060
  gUefiCpuPkgTokenSpaceGuid.PcdCpuMicrocodePatchAddress|0xFF8C0060

  #PcdCpuMicrocodePatchRegionSize = PcdFlashNvStorageMicrocodeSize - (EFI_FIRMWARE_VOLUME_HEADER. HeaderLength + sizeof (EFI_FFS_FILE_HEADER))
  gEfiCpuTokenSpaceGuid.PcdCpuMicrocodePatchRegionSize|0x3FFA0
  gUefiCpuPkgTokenSpaceGuid.PcdCpuMicrocodePatchRegionSize|0x3FFA0

  gCpuUncoreTokenSpaceGuid.PcdFlashSecCacheRegionBase|0x00FFD00000
  gCpuUncoreTokenSpaceGuid.PcdFlashSecCacheRegionSize|0x0000300000

!if $(PERFORMANCE_ENABLE) == TRUE
  gEfiMdeModulePkgTokenSpaceGuid.PcdEdkiiFpdtStringRecordEnableOnly|TRUE
  gEfiMdeModulePkgTokenSpaceGuid.PcdMaxPeiPerformanceLogEntries|100
  gEfiMdeModulePkgTokenSpaceGuid.PcdMaxPeiPerformanceLogEntries16|1000
  gEfiMdeModulePkgTokenSpaceGuid.PcdExtFpdtBootRecordPadSize|0x1000
!endif

  gEfiIntelFrameworkModulePkgTokenSpaceGuid.PcdOpromReservedMemorySize|0x27000

  ## Specifies delay value in microseconds after sending out an INIT IPI.
  # @Prompt Configure delay value after send an INIT IPI
  gUefiCpuPkgTokenSpaceGuid.PcdCpuInitIpiDelayInMicroSeconds|10
  ## Specifies max supported number of Logical Processors.
  # @Prompt Configure max supported number of Logical Processorss
  gUefiCpuPkgTokenSpaceGuid.PcdCpuMaxLogicalProcessorNumber|512
  gUefiCpuPkgTokenSpaceGuid.PcdCpuApStackSize|0x1000
!if $(PERFORMANCE_ENABLE) == TRUE
  gEfiMdePkgTokenSpaceGuid.PcdPerformanceLibraryPropertyMask|0x1

  gServerCommonTokenSpaceGuid.PcdPerfPkgPchPmBaseFunctionNumber|0x2

!endif

  gPlatformModuleTokenSpaceGuid.PcdWsmtProtectionFlags|0x07

!if $(BUILD_TARGET) == "HYBRID"
  gPlatformTokenSpaceGuid.PcdBusStack|0x03
!else
  gPlatformTokenSpaceGuid.PcdBusStack|0x06
!endif
  gPlatformTokenSpaceGuid.PcdUboDev|0x08
  gPlatformTokenSpaceGuid.PcdUboFunc|0x02
  gPlatformTokenSpaceGuid.PcdUboCpuBusNo0|0xCC

!if $(AP_ENABLE) == TRUE
  gPlatformTokenSpaceGuid.PcdDragonRockCLXAPBuild|0x01
!endif

  gEfiCpuTokenSpaceGuid.PcdCpuIEDEnabled|TRUE

!if $(BUILD_WITH_LEGACY_STACK) == FALSE
  gPlatformTokenSpaceGuid.PcdSupportLegacyStack|FALSE
!else
  gPlatformTokenSpaceGuid.PcdSupportLegacyStack|TRUE
!endif

  ## Defines the ACPI register set base address.
  #  The invalid 0xFFFF is as its default value. It must be configured to the real value.
  # @Prompt ACPI Timer IO Port Address
  gPcAtChipsetPkgTokenSpaceGuid.PcdAcpiIoPortBaseAddress         |  0x0500

  ## Defines the PCI Bus Number of the PCI device that contains the BAR and Enable for ACPI hardware registers.
  # @Prompt ACPI Hardware PCI Bus Number
  gPcAtChipsetPkgTokenSpaceGuid.PcdAcpiIoPciBusNumber            |  0x00

  ## Defines the PCI Device Number of the PCI device that contains the BAR and Enable for ACPI hardware registers.
  #  The invalid 0xFF is as its default value. It must be configured to the real value.
  # @Prompt ACPI Hardware PCI Device Number
  gPcAtChipsetPkgTokenSpaceGuid.PcdAcpiIoPciDeviceNumber         |  0x1F

  ## Defines the PCI Function Number of the PCI device that contains the BAR and Enable for ACPI hardware registers.
  #  The invalid 0xFF is as its default value. It must be configured to the real value.
  # @Prompt ACPI Hardware PCI Function Number
  gPcAtChipsetPkgTokenSpaceGuid.PcdAcpiIoPciFunctionNumber       |  0x02

  ## Defines the PCI Register Offset of the PCI device that contains the Enable for ACPI hardware registers.
  #  The invalid 0xFFFF is as its default value. It must be configured to the real value.
  # @Prompt ACPI Hardware PCI Register Offset
  gPcAtChipsetPkgTokenSpaceGuid.PcdAcpiIoPciEnableRegisterOffset |0x0044

  ## Defines the bit mask that must be set to enable the APIC hardware register BAR.
  # @Prompt ACPI Hardware PCI Bar Enable BitMask
  gPcAtChipsetPkgTokenSpaceGuid.PcdAcpiIoBarEnableMask           |  0x80

  ## Defines the PCI Register Offset of the PCI device that contains the BAR for ACPI hardware registers.
  #  The invalid 0xFFFF is as its default value. It must be configured to the real value.
  # @Prompt ACPI Hardware PCI Bar Register Offset
  gPcAtChipsetPkgTokenSpaceGuid.PcdAcpiIoPciBarRegisterOffset    |0x0040

  ## Defines the offset to the 32-bit Timer Value register that resides within the ACPI BAR.
  # @Prompt Offset to 32-bit Timer register in ACPI BAR
  gPcAtChipsetPkgTokenSpaceGuid.PcdAcpiPm1TmrOffset              |0x0008


  # Indicates the max nested level
  gEfiCpRcPkgTokenSpaceGuid.PcdMaxNestedLevel|0x00000010

  ## Socket count used to indicate maximum number of CPU sockets supported by the platform.
  gEfiCpRcPkgTokenSpaceGuid.PcdMaxCpuSocketCount|$(MAX_SOCKET)

  ## Indicates whether it needs to clear temp bus assignment in PCIE common init library
  gEfiCpRcPkgTokenSpaceGuid.PcdCleanTempBusAssignment|TRUE

[PcdsFixedAtBuild.X64]
!if $(PERFORMANCE_ENABLE) == TRUE
  gEfiMdePkgTokenSpaceGuid.PcdReportStatusCodePropertyMask|0x07
!endif
  gPcAtChipsetPkgTokenSpaceGuid.Pcd8259LegacyModeMask|0x0eB8
# Change PcdBootManagerMenuFile to UiApp
##

  gEfiMdeModulePkgTokenSpaceGuid.PcdBootManagerMenuFile|{ 0x21, 0xaa, 0x2c, 0x46, 0x14, 0x76, 0x03, 0x45, 0x83, 0x6e, 0x8a, 0xb6, 0xf4, 0x66, 0x23, 0x31 }

#S3 add
  gEfiIntelFrameworkModulePkgTokenSpaceGuid.PcdS3AcpiReservedMemorySize|0x600000
#S3 add


  gEfiCpuTokenSpaceGuid.PcdCpuSmmCodeAccessCheckEnable|TRUE

[PcdsPatchableInModule.common]
  gEfiMdePkgTokenSpaceGuid.PcdDebugPrintErrorLevel|0x80000042
  gEfiMdePkgTokenSpaceGuid.PcdPciExpressBaseAddress|0x80000000
  gEfiIntelFrameworkModulePkgTokenSpaceGuid.PcdLegacyBiosCacheLegacyRegion|FALSE
  gEfiIntelFrameworkModulePkgTokenSpaceGuid.PcdEbdaReservedMemorySize|0x19000
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoHorizontalResolution|0
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoVerticalResolution|0
  gUefiCpuPkgTokenSpaceGuid.PcdCpuNumberOfReservedVariableMtrrs|0

[PcdsDynamicDefault.common.DEFAULT]
  gEfiCpuTokenSpaceGuid.PcdCpuProcessorFeatureUserConfiguration|0x002CF6CF
  gEfiCpuTokenSpaceGuid.PcdCpuProcessorFeatureUserConfigurationEx1|0
  gUefiCpuPkgTokenSpaceGuid.PcdCpuApInitTimeOutInMicroSeconds|30000
  gEfiCpuTokenSpaceGuid.PcdCpuApInitTimeOutInMicroSeconds|200000
  gEfiSecurityPkgTokenSpaceGuid.PcdTpmPhysicalPresence |TRUE
  gEfiSecurityPkgTokenSpaceGuid.PcdTpmAutoDetection|TRUE
  gServerCommonTokenSpaceGuid.PcdLtConfigLockEnable|TRUE
  gServerCommonTokenSpaceGuid.PcdProcessorLtsxEnable|FALSE
  gEfiMdeModulePkgTokenSpaceGuid.PcdAriSupport|TRUE
  gEfiMdeModulePkgTokenSpaceGuid.PcdResetOnMemoryTypeInformationChange|FALSE
  gEfiCpuTokenSpaceGuid.PcdCpuSmmMsrSaveStateEnable|FALSE
  gEfiCpuTokenSpaceGuid.PcdCpuSmmProtectedModeEnable|FALSE
  gEfiCpuTokenSpaceGuid.PcdCpuSmmRuntimeCtlHooks|FALSE

  gEfiPchTokenSpaceGuid.PcdWakeOnRTCS5|FALSE
  gEfiPchTokenSpaceGuid.PcdRtcWakeupTimeHour|0
  gEfiPchTokenSpaceGuid.PcdRtcWakeupTimeMinute|0
  gEfiPchTokenSpaceGuid.PcdRtcWakeupTimeSecond|0

  #Platform should change it to by code
  gEfiPchTokenSpaceGuid.PcdPchSataInitReg78Data|0xAAAA0000
  gEfiPchTokenSpaceGuid.PcdPchSataInitReg88Data|0xAA33AA22


[PcdsDynamicHii.common.DEFAULT]
  gEfiCpuTokenSpaceGuid.PcdCpuSmmUseDelayIndication|L"CpuSmmUseDelayIndication"|gServerCommonTokenSpaceGuid|0x0|FALSE
  gEfiCpuTokenSpaceGuid.PcdCpuSmmUseBlockIndication|L"CpuSmmUseBlockIndication"|gServerCommonTokenSpaceGuid|0x0|FALSE
  gEfiCpuTokenSpaceGuid.PcdCpuSmmUseSmmEnableIndication|L"CpuSmmUseSmmEnableIndication"|gServerCommonTokenSpaceGuid|0x0|FALSE
  gEfiMdeModulePkgTokenSpaceGuid.PcdUse1GPageTable|L"1GPageTable"|gServerCommonTokenSpaceGuid|0x0|TRUE
!if $(DEBUG_FLAGS_ENABLE) == TRUE
  gEfiMdePkgTokenSpaceGuid.PcdPlatformBootTimeOut|L"Timeout"|gEfiGlobalVariableGuid|0x0|2 # Variable: L"Timeout"
!else
  gEfiMdePkgTokenSpaceGuid.PcdPlatformBootTimeOut|L"Timeout"|gEfiGlobalVariableGuid|0x0|0 # Variable: L"Timeout"
!endif
  gServerCommonTokenSpaceGuid.PcdPlatformMemoryCheck|L"MemoryCheck"|gServerCommonTokenSpaceGuid|0x0|0
  gServerCommonTokenSpaceGuid.PcdUefiOptimizedBoot|L"UefiOptimizedBoot"|gServerCommonTokenSpaceGuid|0x0|TRUE
  gEfiIntelFrameworkModulePkgTokenSpaceGuid.PcdBootState|L"BootState"|gServerCommonTokenSpaceGuid|0x0|TRUE
  gEfiMdePkgTokenSpaceGuid.PcdHardwareErrorRecordLevel|L"HwErrRecSupport"|gEfiGlobalVariableGuid|0x0|1 # Variable: L"HwErrRecSupport"
  gPlatformTokenSpaceGuid.PcdBootDeviceScratchPad5Changed|L"BootDeviceScratchPad"|gServerCommonTokenSpaceGuid|0x0|FALSE


[PcdsDynamicDefault]
## *** PURLEY_PPO *** - Added in 8th segment in PcdPcieMmcfgTablePtr to fix size assert in PcieAddressLib.c
##                                                 |                           MMCFG Table Header                                 |                               Segment 0                                        |                              Segment 1                                       |                               Segment 2                                        |                               Segment 3                                       |                               Segment 4                                        |                               Segment 5                                       |                               Segment 6                                       |                               Segment 7                                      |                                     Segment 8
  gEfiCpRcPkgTokenSpaceGuid.PcdPcieMmcfgTablePtr|{0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,  0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}

  gEfiCpuTokenSpaceGuid.PcdCpuEnergyPolicy|0
  gEfiCpuTokenSpaceGuid.PcdCpuAcpiLvl2Addr|0
  gEfiCpuTokenSpaceGuid.PcdCpuPackageCStateLimit|0
  gEfiCpuTokenSpaceGuid.PcdCpuCoreCStateValue|0
  gEfiCpuTokenSpaceGuid.PcdCpuClockModulationDutyCycle|0
  gEfiCpuTokenSpaceGuid.PcdCpuHwCoordination|TRUE
  gEfiCpuTokenSpaceGuid.PcdCpuDcuMode|0
  gEfiCpuTokenSpaceGuid.PcdCpuTurboOverride|0x0
  gEfiCpuTokenSpaceGuid.PcdCpuProcessorMsrLockCtrl|0
  gEfiCpuTokenSpaceGuid.PcdCpuIioLlcWaysBitMask|0x0
  gEfiCpuTokenSpaceGuid.PcdCpuExpandedIioLlcWaysBitMask|0x0
  gEfiCpuTokenSpaceGuid.PcdCpuRemoteWaysBitMask|0x0
  gEfiCpuTokenSpaceGuid.PcdPchTraceHubEn|0x0
  gEfiCpuTokenSpaceGuid.PcdCpuQlruCfgBitMask|0x0
  gEfiCpuTokenSpaceGuid.PcdSbspSelection|0xFF
#  gEfiCpuTokenSpaceGuid.PcdCpuSocketId|{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1,0x0,0x0,0x0,0x1,0x0,0x0,0x0,0x2,0x0,0x0,0x0,0x2,0x0,0x0,0x0,0x3,0x0,0x0,0x0,0x3,0x0,0x0,0x0}
  gEfiCpuTokenSpaceGuid.PcdCpuPmStructAddr|0x0
  gEfiCpuTokenSpaceGuid.PcdCpuRRQCountThreshold|0x0

  gEfiSecurityPkgTokenSpaceGuid.PcdUserPhysicalPresence |TRUE

  gEfiMdeModulePkgTokenSpaceGuid.PcdSrIovSupport|TRUE
  gEfiMdeModulePkgTokenSpaceGuid.PcdAriSupport|TRUE
  gEfiMdeModulePkgTokenSpaceGuid.PcdMrIovSupport|FALSE
  gEfiMdeModulePkgTokenSpaceGuid.PcdSrIovSystemPageSize|0x01

  ## Put fTPM guid here: e.g. { 0xf9c6a62f, 0xc60f, 0x4b44, { 0xa6, 0x29, 0xed, 0x3d, 0x40, 0xae, 0xfa, 0x5f } }
  ## TPM1.2 { 0x8b01e5b6, 0x4f19, 0x46e8, { 0xab, 0x93, 0x1c, 0x53, 0x67, 0x1b, 0x90, 0xcc } }
  ## TPM2.0Dtpm { 0x286bf25a, 0xc2c3, 0x408c, { 0xb3, 0xb4, 0x25, 0xe6, 0x75, 0x8b, 0x73, 0x17 } }

  #TPM2.0#
  gEfiSecurityPkgTokenSpaceGuid.PcdTpmInstanceGuid|{0x5a, 0xf2, 0x6b, 0x28, 0xc3, 0xc2, 0x8c, 0x40, 0xb3, 0xb4, 0x25, 0xe6, 0x75, 0x8b, 0x73, 0x17}

  #TPM1.2#
  #gEfiSecurityPkgTokenSpaceGuid.PcdTpmInstanceGuid|{0xb6, 0xe5, 0x01, 0x8b, 0x19, 0x4f, 0xe8, 0x46, 0xab, 0x93, 0x1c, 0x53, 0x67, 0x1b, 0x90, 0xcc}

  #gEfiSecurityPkgTokenSpaceGuid.PcdTpmInstanceGuid |L"TREE_CONFIGURATION"|gTrEEConfigFormSetGuid|0x0|{0x2f, 0xa6, 0xc6, 0xf9, 0x0f, 0xc6, 0x44, 0x4b, 0xa6, 0x29, 0xed, 0x3d, 0x40, 0xae, 0xfa, 0x5f}
  gEfiSecurityPkgTokenSpaceGuid.PcdTpmInitializationPolicy|0



!if $(AP_ENABLE) == TRUE
  gUefiCpuPkgTokenSpaceGuid.PcdMcpSupport|FALSE
!endif

[PcdsDynamicDefault.X64]

#
# LegacySol_Common_start
#
  gEfiMdePkgTokenSpaceGuid.PcdUartDefaultBaudRate|115200
  gEfiMdePkgTokenSpaceGuid.PcdUartDefaultDataBits|8
  gEfiMdePkgTokenSpaceGuid.PcdUartDefaultParity|1
  gEfiMdePkgTokenSpaceGuid.PcdUartDefaultStopBits|1
  gEfiMdePkgTokenSpaceGuid.PcdDefaultTerminalType|0
#from here
#
# LegacySol_Common_end

[PcdsDynamicExDefault.X64]
#

[Components.IA32]
  # EDKII Driver
  UefiCpuPkg/SecCore/SecCore.inf {
    <LibraryClasses>
      TimerLib|MdePkg/Library/BaseTimerLibNullTemplate/BaseTimerLibNullTemplate.inf
      ReportStatusCodeLib|MdePkg/Library/BaseReportStatusCodeLibNull/BaseReportStatusCodeLibNull.inf
  }
  MdeModulePkg/Core/Pei/PeiMain.inf {
    <LibraryClasses>
      DebugLib|MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf
#    DebugLib|MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf
  }
  MdeModulePkg/Universal/PCD/Pei/Pcd.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
  }
  CpRcPkg/Universal/RegAccess/Pei/RegAccess.inf {
    <LibraryClasses>
      S3BootScriptLib|MdePkg/Library/BaseS3BootScriptLibNull/BaseS3BootScriptLibNull.inf
      PcdLib|MdePkg/Library/PeiPcdLib/PeiPcdLib.inf
  }
  MdeModulePkg/Universal/ReportStatusCodeRouter/Pei/ReportStatusCodeRouterPei.inf
  MdeModulePkg/Universal/StatusCodeHandler/Pei/StatusCodeHandlerPei.inf {
    <LibraryClasses>
      SerialPortLib|$(PLAT_PKG)/Library/SerialPortLib/SerialPortLib.inf
  }
  $(PLAT_PKG)/StatusCodeHandlerUsb/Pei/StatusCodeHandlerUsbPei.inf
!if $(TRACE_HUB_DEBUG_ENABLE) == TRUE
  $(PLAT_PKG)/Universal/TraceHubStatusCodeHandler/Pei/TraceHubStatusCodeHandlerPei.inf {
    <PcdsFixedAtBuild>
      gTraceHubTokenSpaceGuid.PcdTraceHubDebugLibChannel|0xD
  }
!endif

  CpPlatPkg/Uba/CfgDb/Pei/CfgDbPei.inf
!include $(RP_PKG)/Uba/UbaPei.dsc

  UefiCpuPkg/CpuIoPei/CpuIoPei.inf

!if $(PERFORMANCE_ENABLE) == TRUE
  $(PLAT_PKG)/Override/MdeModulePkg/Universal/Acpi/FirmwarePerformanceDataTablePei/FirmwarePerformancePei.inf
!endif

  CpPcPlatPkg/Variable/PlatformVariable/Pei/PlatformVariableInitPei.inf

#S3 delete
#  UefiCpuPkg/Universal/Acpi/S3Resume2Pei/S3Resume2Pei.inf
#S3 delete

    MdeModulePkg/Universal/Variable/Pei/VariablePei.inf {
    <LibraryClasses>
      DebugLib|MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf
    }

  MdeModulePkg/Universal/PcatSingleSegmentPciCfg2Pei/PcatSingleSegmentPciCfg2Pei.inf

  MdeModulePkg/Core/DxeIplPeim/DxeIpl.inf {
    <LibraryClasses>
    NULL|IntelFrameworkModulePkg/Library/LzmaCustomDecompressLib/LzmaCustomDecompressLib.inf
    DebugLib|MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf
  }

  #EDK platform driver

  $(SKT_PKG)/Override/IA32FamilyCpuPkg/CpuPei/CpuPei.inf
  $(RP_PKG)/Platform/Pei/PlatformInfo/PlatformInfo.inf
  $(PLAT_PKG)/Platform/Pei/PlatformInit/PlatformEarlyInit.inf

  $(PLAT_PKG)/Platform/Pei/MemCacheInit/MemCacheInit.inf {
    <LibraryClasses>
      DebugPrintErrorLevelLib|MdePkg/Library/BaseDebugPrintErrorLevelLib/BaseDebugPrintErrorLevelLib.inf
  }

#
# This module is special in that if we are building with the MRC OEM hooks
# separated from the MRC via a PPI, this modules produces the PPI and needs
# to use the original OEM hooks functions to minimize changes to customer
# implementations.
# In essence, this module wraps the original OEM hook libraries.
#
# See the PlatformPkgConfig.dsc for the basic overall design comment.
#
!if $(MRC_OEM_HOOKS_PPI_SUPPORT) == TRUE
  $(PLAT_PKG)/Platform/Pei/MrcOemHooksPeim/MrcOemHooksPeim.inf {
    <LibraryClasses>
      OemProcMemInitLib|$(PLAT_PKG)/Library/OemProcMemInitLib/OemProcMemInitLib.inf
      OemIioInitLib|$(PLAT_PKG)/Library/OemIioInit/OemIioInitHooks.inf
  }
!endif

  $(PLAT_PKG)/Platform/Pei/BootMode/BootMode.inf

  $(PLAT_PKG)/Platform/Pei/FpgaPlatformInit/FpgaPlatformEarlyInit.inf

  $(SKT_PKG)/Pei/FpgaLoader/FpgaLoader.inf

#S3 add
  UefiCpuPkg/PiSmmCommunication/PiSmmCommunicationPei.inf
#S3 add

  $(SKT_PKG)/Pei/UncoreInit/UncoreInit.inf {
    <LibraryClasses>
      S3BootScriptLib|MdePkg/Library/BaseS3BootScriptLibNull/BaseS3BootScriptLibNull.inf
      UsraRegisterFilterLib| $(SKT_PKG)/Library/UsraRegisterFilterLib/UsraRegisterFilterLib.inf
  }

  UefiCpuPkg/CpuMpPei/CpuMpPei.inf

  $(PCH_PKG)/SiInit/Pei/SiInitPreMem.inf
  $(PCH_PKG)/SiInit/Pei/SiInit.inf

  $(SKT_PKG)/Pei/CrystalRidge/CrystalRidgePeim.inf

  $(PLAT_PKG)/Override/ServerCommonPkg/Universal/GetSec/Pei/TxtPei.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/PeiPcdLib/PeiPcdLib.inf
      DebugLib|MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf
      LtPeiLib|$(PLAT_PKG)/Library/LtPeiLib/LtPeiLib.inf
  }

  $(PLAT_PKG)/Library/LtPeiLib/LtPeiLib.inf {
    <LibraryClasses>
      DebugLib|MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf
  }


  #
  #IFLASH32_START
  #
  $(RP_PKG)/Platform/Pei/SecurityRevision/SecurityRevision.inf
  #
  #IFLASH32_END
  #

  #TPM
  SecurityPkg/Tcg/PhysicalPresencePei/PhysicalPresencePei.inf {
    <LibraryClasses>
#      DebugLib|MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf

  }

!if $(TPM2_ENABLE) == TRUE
  $(PLAT_PKG)/Override/SecurityPkg/Tcg/Tcg2Config/Tcg2ConfigPei.inf {
    <LibraryClasses>
      Tpm12DeviceLib|SecurityPkg/Library/Tpm12DeviceLibDTpm/Tpm12DeviceLibDTpm.inf
      PcdLib|MdePkg/Library/PeiPcdLib/PeiPcdLib.inf
      DebugLib|MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf

  }
  SecurityPkg/Tcg/Tcg2Pei/Tcg2Pei.inf {
    <LibraryClasses>
      NULL|$(PLAT_PKG)/Override/SecurityPkg/Library/Tpm2DeviceLibDTpm/Tpm2InstanceLibDTpm.inf
#      NULL|...Pkg/Library/Tpm2DeviceLibFTpm/Tpm2InstanceLibFTpm.inf
      NULL|SecurityPkg/Library/HashInstanceLibSha1/HashInstanceLibSha1.inf
      NULL|SecurityPkg/Library/HashInstanceLibSha256/HashInstanceLibSha256.inf
      PcdLib|MdePkg/Library/PeiPcdLib/PeiPcdLib.inf
#  DebugLib|MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf
      DebugLib|MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf
      Tcg2PhysicalPresenceLib|SecurityPkg/Library/PeiTcg2PhysicalPresenceLib/PeiTcg2PhysicalPresenceLib.inf
  }

  $(PLAT_PKG)/Override/SecurityPkg/Tcg/TcgPei/TcgPei.inf {
    <LibraryClasses>
     DebugLib|MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf
  }

!else

  $(PLAT_PKG)/Override/SecurityPkg/Tcg/TcgPei/TcgPei.inf {
    <LibraryClasses>
      DebugLib|MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf
  }
!endif

#UUID
  $(PLAT_PKG)/Platform/Pei/UuidPeiInit/UuidPeiInit.inf

#Ipmi
!if $(IPMI_SUPPORT) == TRUE
  CpPlatPkg/Ipmi/Frb/FrbPei/FrbPei.inf
!endif


  $(RP_PKG)/SystemBoard/Pei/SystemBoardPpi.inf
  #
  # S3 driver
  #

#S3 delete
# $(PLAT_PKG)/Cpu/Pei/MpS3/MpS3.inf
# $(PLAT_PKG)/Acpi/Pei/S3Resume/S3Resume.inf
#S3 delete

#S3 add
  $(SKT_PKG)/Smm/SmmAccessPei/SmmAccessPei.inf
  UefiCpuPkg/Universal/Acpi/S3Resume2Pei/S3Resume2Pei.inf {
    <LibraryClasses>
    !if $(PERFORMANCE_ENABLE) == TRUE
      PerformanceLib|$(PLAT_PKG)/Override/MdeModulePkg/Library/PeiPerformanceLib/PeiPerformanceLib.inf
      TimerLib|UefiCpuPkg/Library/SecPeiDxeTimerLibUefiCpu/SecPeiDxeTimerLibUefiCpu.inf
    !endif
    DebugLib|MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf
  }
#S3 add

#
# ME Stack
#
!if $(ME_ENABLE) == TRUE
  $(SKT_PKG)/Me/Heci/Pei/HeciInit.inf
  $(PLAT_PKG)/Me/Policy/Pei/MePolicyInitPei.inf
  $(SKT_PKG)/Me/Heci/MeUma/MeUma.inf
  !if $(SPS_ENABLE) == TRUE
    $(SKT_PKG)/Me/Sps/Pei/SpsPei.inf
  !endif
  !if $(AMT_ENABLE) == TRUE
    $(SKT_PKG)/Me/AMT/Platform/Pei/AmtStatusCode/AmtStatusCodePei.inf
  !endif
!endif

!if $(IE_ENABLE) == TRUE
  $(PLAT_PKG)/Ie/Policy/Pei/IePolicyInitPei.inf
  $(SKT_PKG)/Ie/Heci/Pei/IeHeciInit.inf
!endif

  #
  # Recovery
  #
  MdeModulePkg/Bus/Pci/EhciPei/EhciPei.inf
  MdeModulePkg/Bus/Usb/UsbBotPei/UsbBotPei.inf
  MdeModulePkg/Bus/Usb/UsbBusPei/UsbBusPei.inf
  FatPkg/FatPei/FatPei.inf
  MdeModulePkg/Bus/Pci/IdeBusPei/IdeBusPei.inf
  MdeModulePkg/Universal/Disk/CdExpressPei/CdExpressPei.inf
  MdeModulePkg/Universal/FaultTolerantWritePei/FaultTolerantWritePei.inf

[Components.X64]
  #
  # Generic EDKII Driver
  #

  $(PLAT_PKG)/Override/MdeModulePkg/Core/Dxe/DxeMain.inf {
    <LibraryClasses>
      DevicePathLib|MdePkg/Library/UefiDevicePathLib/UefiDevicePathLib.inf
      DebugLib|MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf
      PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
      NULL|IntelFrameworkModulePkg/Library/LzmaCustomDecompressLib/LzmaCustomDecompressLib.inf
      NULL|MdeModulePkg/Library/DxeCrc32GuidedSectionExtractLib/DxeCrc32GuidedSectionExtractLib.inf
      DebugLib|MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf
    <PcdsFixedAtBuild>
      gEfiMdePkgTokenSpaceGuid.PcdMaximumLinkedListLength|0
  }

  CpRcPkg/Universal/RegAccess/Dxe/RegAccess.inf {
   <LibraryClasses>
     CsrToPcieLib|$(SKT_PKG)/Library/CsrToPcieLib/CsrToPcieDxeLib.inf
     PcieAddrLib|CpRcPkg/Library/PcieAddressLib/PcieAddressLib.inf
     PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
     USRATraceLib|$(SKT_PKG)/Library/USRATraceLib/USRATraceLib.inf
  }
  CpRcPkg/Universal/RegAccess/Smm/RegAccessSMM.inf {
   <LibraryClasses>
     CsrToPcieLib|$(SKT_PKG)/Library/CsrToPcieLib/CsrToPcieDxeLib.inf
     PcieAddrLib|CpRcPkg/Library/PcieAddressLib/PcieAddressLib.inf
     PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
     USRATraceLib|$(SKT_PKG)/Library/USRATraceLib/USRATraceLib.inf
  }

#  !if $(IPMI_SUPPORT) == TRUE
##    CpPlatPkg/Ipmi/GenericIpmi/Dxe/GenericIpmi.inf {
#    <LibraryClasses>
#      PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
#      DebugLib|MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf
#      ReportStatusCodeLib|MdeModulePkg/Library/DxeReportStatusCodeLib/DxeReportStatusCodeLib.inf
#    }
#    CpPlatPkg/Ipmi/GenericIpmi/Smm/SmmGenericIpmi.inf {
#    <LibraryClasses>
#      PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
#      DebugLib|MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf
#      ReportStatusCodeLib|MdeModulePkg/Library/DxeReportStatusCodeLib/DxeReportStatusCodeLib.inf
#    }
#  !endif
  CpPlatPkg/Uba/CfgDb/Dxe/CfgDbDxe.inf
!include $(RP_PKG)/Uba/UbaDxe.dsc
!if $(PERFORMANCE_ENABLE) == TRUE
  $(PLAT_PKG)/Override/MdeModulePkg/Universal/Acpi/FirmwarePerformanceDataTableDxe/FirmwarePerformanceDxe.inf
  $(PLAT_PKG)/Override/MdeModulePkg/Universal/Acpi/FirmwarePerformanceDataTableSmm/FirmwarePerformanceSmm.inf
!endif

  $(PLAT_PKG)/Platform/Dxe/PlatformType/PlatformType.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  }
  $(PLAT_PKG)/Platform/Dxe/UuidDxe/UuidDxe.inf
  $(PLAT_PKG)/Platform/Dxe/BreakpointCallbackDxe/BreakpointCallbackDxe.inf
  MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf
# OVERRIDE_HSD_5330788_BEGIN
  $(PLAT_PKG)/Override/MdeModulePkg/Bus/Pci/XhciDxe/XhciDxe.inf {
# OVERRIDE_HSD_5330788_END
    <PcdsFixedAtBuild>
      gEfiMdePkgTokenSpaceGuid.PcdFixedDebugPrintErrorLevel|0
  }
  MdeModulePkg/Bus/Pci/EhciDxe/EhciDxe.inf
  MdeModulePkg/Bus/Pci/UhciDxe/UhciDxe.inf
  MdeModulePkg/Bus/Scsi/ScsiBusDxe/ScsiBusDxe.inf
# 4168296: SDP: Legacy DVD boot device disappears from boot device list
  MdeModulePkg/Bus/Scsi/ScsiDiskDxe/ScsiDiskDxe.inf
  MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
  MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
  MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf
  MdeModulePkg/Bus/Usb/UsbMouseDxe/UsbMouseDxe.inf

# Include NvmExpress only for LBG now
  MdeModulePkg/Bus/Pci/NvmExpressDxe/NvmExpressDxe.inf

  MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf {
    <LibraryClasses>
    !if $(SECURE_BOOT_ENABLE) == TRUE
       NULL|SecurityPkg/Library/DxeImageVerificationLib/DxeImageVerificationLib.inf
    !endif
#      NULL|SecurityPkg/Library/DxeImageVerificationLib/DxeImageVerificationLib.inf
#      NULL|SecurityPkg/Library/DxeDeferImageLoadLib/DxeDeferImageLoadLib.inf
       NULL|SecurityPkg/Library/DxeTpmMeasureBootLib/DxeTpmMeasureBootLib.inf

!if $(TPM2_ENABLE) == TRUE
       NULL|SecurityPkg/Library/DxeTpm2MeasureBootLib/DxeTpm2MeasureBootLib.inf
!endif

  }

  $(RP_PKG)/SystemBoard/Dxe/SystemBoardDxe.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  }


  MdeModulePkg/Application/BootManagerMenuApp/BootManagerMenuApp.inf {
    <LibraryClasses>
      NULL|IntelFrameworkModulePkg/Library/LegacyBootManagerLib/LegacyBootManagerLib.inf
      PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf

  }
# PURLEY_OVERRIDE_BEGIN_5387253
  $(PLAT_PKG)/Override/ServerCommonPkg/Applications/UiApp/UiApp.inf {
# PURLEY_OVERRIDE_END_5387253
    <LibraryClasses>
      PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
      NULL|IntelFrameworkModulePkg/Library/LegacyBootManagerLib/LegacyBootManagerLib.inf
  }
  $(PLAT_PKG)/Override/MdeModulePkg/Universal/BdsDxe/BdsDxe.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
      NULL|IntelFrameworkModulePkg/Library/LegacyBootManagerLib/LegacyBootManagerLib.inf
      Tpm2DeviceLib|SecurityPkg/Library/Tpm2DeviceLibTcg2/Tpm2DeviceLibTcg2.inf
  }


  $(PLAT_PKG)/Override/ServerCommonPkg/Universal/GetSec/Dxe/TxtDxe.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
!if $(AP_ENABLE) == TRUE
      LocalApicLib|$(SKT_PKG)/Override/UefiCpuPkg/Library/BaseXApicX2ApicLib/BaseXApicX2ApicLib.inf
!else
      LocalApicLib|UefiCpuPkg/Library/BaseXApicX2ApicLib/BaseXApicX2ApicLib.inf
!endif
      LtDxeLib|$(PLAT_PKG)/Library/LtDxeLib/LtDxeLib.inf
      PlatformLtDxeLib|$(PLAT_PKG)/Library/PlatformLtDxeLibNull/PlatformLtDxeLibNull.inf
  }

  $(PLAT_PKG)/Library/LtDxeLib/LtDxeLib.inf {
    <LibraryClasses>
      #S3BootScriptLib|MdeModulePkg/Library/PiDxeS3BootScriptLib/DxeS3BootScriptLib.in
      S3BootScriptLib|$(PLAT_PKG)/Override/MdeModulePkg/Library/PiDxeS3BootScriptLib/DxeS3BootScriptLib.inf
  }

  $(PLAT_PKG)/Platform/Dxe/Hsti/HstiIbvPlatformDxe/HstiIbvPlatformDxe.inf {
    <LibraryClasses>
      HstiLib|MdePkg/Library/DxeHstiLib/DxeHstiLib.inf
  }
  $(PLAT_PKG)/Platform/Dxe/Hsti/HstiIhvProviderDxe/HstiIhvProviderDxe.inf {
    <LibraryClasses>
      HstiLib | MdePkg/Library/DxeHstiLib/DxeHstiLib.inf
      PciSegmentLib | MdePkg/Library/BasePciSegmentLibPci/BasePciSegmentLibPci.inf
      Tpm2DeviceLib|SecurityPkg/Library/Tpm2DeviceLibTcg2/Tpm2DeviceLibTcg2.inf
  }


  MdeModulePkg/Universal/PlatformDriOverrideDxe/PlatformDriOverrideDxe.inf
  MdeModulePkg/Universal/EbcDxe/EbcDxe.inf

#S3 delete
#  MdeModulePkg/Universal/LockBox/SmmLockBox/SmmLockBox.inf
#S3 delete

  BpCommonPkg/Csm/LegacyInterruptHookDxe/LegacyInterruptHook.inf

  BpCommonPkg/Csm/LegacyBiosReverseThunkDxe/LegacyBiosReverseThunk.inf
  BpCommonPkg/Csm/TcgLegacy/TcgLegacy.inf

  BpCommonPkg/Csm/TcgLegacy/TcgLegacyInstallInt1A.inf
  MdeModulePkg/Universal/FaultTolerantWriteDxe/FaultTolerantWriteSmm.inf
  $(PLAT_PKG)/Override/MdeModulePkg/Universal/Variable/RuntimeDxe/VariableSmmRuntimeDxe.inf {
    <LibraryClasses>
!if $(SECURE_BOOT_ENABLE) == FALSE
      TpmMeasurementLib|MdeModulePkg/Library/TpmMeasurementLibNull/TpmMeasurementLibNull.inf
!endif
  }

  $(PLAT_PKG)/Override/MdeModulePkg/Universal/Variable/RuntimeDxe/VariableSmm.inf {
    <LibraryClasses>
      VarCheckLib|MdeModulePkg/Library/VarCheckLib/VarCheckLib.inf
      NULL|MdeModulePkg/Library/VarCheckUefiLib/VarCheckUefiLib.inf
      NULL|MdeModulePkg/Library/VarCheckHiiLib/VarCheckHiiLib.inf
!if $(SECURE_BOOT_ENABLE) == TRUE
      PlatformSecureLib|SecurityPkg/Library/PlatformSecureLibNull/PlatformSecureLibNull.inf
      AuthVariableLib|SecurityPkg/Library/AuthVariableLib/AuthVariableLib.inf
!else
      AuthVariableLib|MdeModulePkg/Library/AuthVariableLibNull/AuthVariableLibNull.inf
!endif
    }

!if $(SECURE_BOOT_ENABLE) == TRUE
  SecurityPkg/VariableAuthenticated/SecureBootConfigDxe/SecureBootConfigDxe.inf {
    <LibraryClasses>
      PlatformSecureLib|SecurityPkg/Library/PlatformSecureLibNull/PlatformSecureLibNull.inf
  }
!endif


  $(PLAT_PKG)/Platform/Dxe/ResetTesting/ResetTesting.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
   }
  $(PLAT_PKG)/Platform/Dxe/SocketSetup/SocketSetup.inf {
      <LibraryClasses>
        PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  }
  $(PLAT_PKG)/Platform/Dxe/FpgaSocketSetup/FpgaSktSetup.inf

  MdeModulePkg/Universal/WatchdogTimerDxe/WatchdogTimer.inf
  MdeModulePkg/Universal/MonotonicCounterRuntimeDxe/MonotonicCounterRuntimeDxe.inf
  $(PLAT_PKG)/Override/MdeModulePkg/Universal/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  }
  MdeModulePkg/Universal/Console/ConPlatformDxe/ConPlatformDxe.inf
  MdeModulePkg/Universal/Console/ConSplitterDxe/ConSplitterDxe.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  }
  MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf {
    <LibraryClasses>
      DevicePathLib|MdePkg/Library/UefiDevicePathLib/UefiDevicePathLib.inf
      PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
  }
  MdeModulePkg/Universal/Console/GraphicsConsoleDxe/GraphicsConsoleDxe.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  }
#
# LegacySol_Common_start
#
  MdeModulePkg/Universal/Console/TerminalDxe/TerminalDxe.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
}
#
# LegacySol_Common_end
#
  MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
  MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf
  $(PLAT_PKG)/Override/PcAtChipsetPkg/PcatRealTimeClockRuntimeDxe/PcatRealTimeClockRuntimeDxe.inf
  MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf
  MdeModulePkg/Universal/SetupBrowserDxe/SetupBrowserDxe.inf
  MdeModulePkg/Universal/DisplayEngineDxe/DisplayEngineDxe.inf
  $(PLAT_PKG)/Platform/Dxe/FrameworkHiiAlias/FrameworkHiiAlias.inf
  MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf
  MdeModulePkg/Universal/PCD/Dxe/Pcd.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
  }

  IntelFrameworkModulePkg/Bus/Isa/IsaBusDxe/IsaBusDxe.inf
  IntelFrameworkModulePkg/Bus/Isa/IsaFloppyDxe/IsaFloppyDxe.inf
#
# LegacySol_Common_start
#
  IntelFrameworkModulePkg/Bus/Isa/IsaSerialDxe/IsaSerialDxe.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  }
#
# LegacySol_Common_end
#
  IntelFrameworkModulePkg/Bus/Isa/Ps2KeyboardDxe/Ps2keyboardDxe.inf
  IntelFrameworkModulePkg/Bus/Isa/Ps2MouseDxe/Ps2MouseDxe.inf
  MdeModulePkg/Bus/Ata/AtaBusDxe/AtaBusDxe.inf {
    <PcdsPatchableInModule>
      gEfiMdePkgTokenSpaceGuid.PcdDebugPrintErrorLevel|0x80000000
  }
  MdeModulePkg/Bus/Ata/AtaAtapiPassThru/AtaAtapiPassThru.inf
  ServerCommonPkg/Csm/IntelRaidAtaAtapiPassThru/IntelRaidAtaAtapiPassThru.inf
  ServerCommonPkg/Csm/IntelRaidBiosThunk/IntelRaidBiosThunk.inf
# To support MROM Hook
  $(PLAT_PKG)/Override/MdeModulePkg/Bus/Pci/PciBusDxe/PciBusDxe.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  }
  IntelFrameworkModulePkg/Universal/Console/VgaClassDxe/VgaClassDxe.inf
  IntelFrameworkModulePkg/Universal/DataHubDxe/DataHubDxe.inf {
    <PcdsFixedAtBuild>
      gEfiMdePkgTokenSpaceGuid.PcdMaximumLinkedListLength|0
  }
  IntelFrameworkModulePkg/Universal/DataHubStdErrDxe/DataHubStdErrDxe.inf
  IntelFrameworkModulePkg/Universal/SectionExtractionDxe/SectionExtractionDxe.inf
  # IntelFrameworkModulePkg/Universal/StatusCode/RuntimeDxe/StatusCodeRuntimeDxe.inf
  MdeModulePkg/Universal/ReportStatusCodeRouter/RuntimeDxe/ReportStatusCodeRouterRuntimeDxe.inf
  MdeModulePkg/Universal/ReportStatusCodeRouter/Smm/ReportStatusCodeRouterSmm.inf
  MdeModulePkg/Universal/StatusCodeHandler/Smm/StatusCodeHandlerSmm.inf {
    <LibraryClasses>
      SerialPortLib| $(PLAT_PKG)/Library/SerialPortLib/SerialPortLib.inf
  }
  MdeModulePkg/Universal/StatusCodeHandler/RuntimeDxe/StatusCodeHandlerRuntimeDxe.inf {
    <LibraryClasses>
      SerialPortLib|$(PLAT_PKG)/Library/SerialPortLib/SerialPortLib.inf
  }
  $(PLAT_PKG)/StatusCodeHandlerUsb/RuntimeDxe/StatusCodeHandlerRuntimeDxeUsb.inf {
    <LibraryClasses>
      SerialPortLib| $(PLAT_PKG)/Library/SerialPortLibUsb/SerialPortLibUsb.inf
      PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  }
  !if $(TRACE_HUB_DEBUG_ENABLE) == TRUE
    $(PLAT_PKG)/Universal/TraceHubStatusCodeHandler/RuntimeDxe/TraceHubStatusCodeHandlerRuntimeDxe.inf
    $(PLAT_PKG)/Universal/TraceHubStatusCodeHandler/Smm/TraceHubStatusCodeHandlerSmm.inf
  !endif

  $(RESTRICTED_PKG)/Roms/OnboardVideoEfi.inf
  ServerCommonPkg/Csm/BiosThunk/VideoDxe/VideoDxe.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  }

  $(PLAT_PKG)/Override/IntelFrameworkModulePkg/Csm/LegacyBiosDxe/LegacyBiosDxe.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  }


  PcAtChipsetPkg/8259InterruptControllerDxe/8259.inf

#  CHECK THIS WITH Platform TEAM
#  TianoModulePkg/Universal/Acpi/BootScriptSaveDxe/BootScriptSaveDxe.inf {
#    <LibraryClasses>
#      PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
#  }

  MdeModulePkg/Universal/Acpi/AcpiTableDxe/AcpiTableDxe.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  }

  MdeModulePkg/Universal/Acpi/S3SaveStateDxe/S3SaveStateDxe.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  }
  IntelFrameworkModulePkg/Universal/Acpi/AcpiS3SaveDxe/AcpiS3SaveDxe.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  }


  MdeModulePkg/Universal/Acpi/BootScriptExecutorDxe/BootScriptExecutorDxe.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
      DebugLib|MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf
  }

#S3 add
  MdeModulePkg/Universal/LockBox/SmmLockBox/SmmLockBox.inf
  UefiCpuPkg/PiSmmCommunication/PiSmmCommunicationSmm.inf
#S3 add

  #(Temp WA for Simics for skipping memory test) MdeModulePkg/Universal/MemoryTest/GenericMemoryTestDxe/GenericMemoryTestDxe.inf
  MdeModulePkg/Universal/MemoryTest/NullMemoryTestDxe/NullMemoryTestDxe.inf
  MdeModulePkg/Universal/Metronome/Metronome.inf
  MdeModulePkg/Universal/SmbiosDxe/SmbiosDxe.inf
  MdeModulePkg/Universal/SmbiosMeasurementDxe/SmbiosMeasurementDxe.inf
  ServerCommonPkg/Csm/BiosThunk/BlockIoDxe/BlockIoDxe.inf
  ServerCommonPkg/Csm/BiosThunk/Snp16Dxe/Snp16Dxe.inf
  IntelFrameworkModulePkg/Universal/CpuIoDxe/CpuIoDxe.inf
  UefiCpuPkg/CpuIo2Dxe/CpuIo2Dxe.inf

  #
  # UEFI network modules
  #
  MdeModulePkg/Universal/Network/DpcDxe/DpcDxe.inf
  $(PLAT_PKG)/Override/MdeModulePkg/Universal/Network/SnpDxe/SnpDxe.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  }
  MdeModulePkg/Universal/Network/MnpDxe/MnpDxe.inf
  MdeModulePkg/Universal/Network/VlanConfigDxe/VlanConfigDxe.inf
  MdeModulePkg/Universal/Network/ArpDxe/ArpDxe.inf
  MdeModulePkg/Universal/Network/Dhcp4Dxe/Dhcp4Dxe.inf
  MdeModulePkg/Universal/Network/Ip4Dxe/Ip4Dxe.inf
  MdeModulePkg/Universal/Network/Mtftp4Dxe/Mtftp4Dxe.inf
  MdeModulePkg/Universal/Network/Tcp4Dxe/Tcp4Dxe.inf {
    <PcdsPatchableInModule>
      gEfiMdePkgTokenSpaceGuid.PcdDebugPrintErrorLevel|0x80000000
  }
  MdeModulePkg/Universal/Network/Udp4Dxe/Udp4Dxe.inf
  NetworkPkg/UefiPxeBcDxe/UefiPxeBcDxe.inf
  $(PLAT_PKG)/Override/MdeModulePkg/Universal/Network/IScsiDxe/IScsiDxe.inf
  NetworkPkg/Ip6Dxe/Ip6Dxe.inf
  NetworkPkg/TcpDxe/TcpDxe.inf
  NetworkPkg/Udp6Dxe/Udp6Dxe.inf
  NetworkPkg/Dhcp6Dxe/Dhcp6Dxe.inf
  NetworkPkg/Mtftp6Dxe/Mtftp6Dxe.inf

  # EDK Dxe Drivers
  $(SKT_PKG)/Override/IA32FamilyCpuPkg/CpuArchDxe/CpuArchDxe.inf {
    <LibraryClasses>
     PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
    <BuildOptions>
      MSFT:*_*_X64_CC_FLAGS    = -Od
      GCC:*_*_X64_CC_FLAGS    = -Og
  }
  $(PLAT_PKG)/Cpu/Dxe/PlatformCpuPolicy/PlatformCpuPolicy.inf {
    <LibraryClasses>
     PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
     DebugLib|MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf
  }
  #BPE_1310.400_TODO: still need Override?
  $(SKT_PKG)/Override/IA32FamilyCpuPkg/CpuMpDxe/CpuMpDxe.inf {
    <LibraryClasses>
     PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
     DebugLib|MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf
  }
  $(SKT_PKG)/Dxe/IioInit/IioInit.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  }
  $(PLAT_PKG)/Pci/Dxe/PciHostBridge/PciHostBridge.inf
  $(SKT_PKG)/Dxe/PowerManagement/PpmInitialize.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  }
  $(SKT_PKG)/Smm/Iio/IioSmm.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  }

  $(SKT_PKG)/Smm/SoftSkuSmm/SoftSkuSmm.inf
  $(SKT_PKG)/Dxe/SoftSkuDxe/SoftSkuDxe.inf

#S3 add
  $(PLAT_PKG)/Universal/LpcPlatform/LpcPlatform.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  }
  $(PLAT_PKG)/Universal/PilotIVPc8374/PilotIVPc8374.inf {
  #ServerCommonPkg/Universal/PilotIIIPc8374/PilotIIIPc8374.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  }
#S3 add

  $(PLAT_PKG)/Platform/Dxe/ReserveMemory/ReserveMem.inf

  $(PLAT_PKG)/Platform/SpiFvbServices/PlatformSpi.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  }
  $(PLAT_PKG)/Platform/SpiFvbServices/PlatformSmmSpi.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  }
  #EDKII Variable
  ServerCommonPkg/Universal/OemBadgingDxe/OemBadgingDxe.inf
  $(PLAT_PKG)/Legacy/Dxe/LegacyRegion/LegacyRegion2.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  }
  $(PCH_PKG)/PchInit/Dxe/PchInitDxe.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  }
  $(PCH_PKG)/Smbus/Dxe/PchSmbusDxe.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  }
  $(PCH_PKG)/Smbus/Smm/PchSmbusSmm.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  }
  $(PCH_PKG)/LegacyInterrupt/Dxe/LegacyInterrupt.inf {
    <LibraryClasses>
  }

  $(PCH_PKG)/PchSmiDispatcher/Smm/PchSmiDispatcher.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  }

  $(PCH_PKG)/PchInit/Smm/PchInitSmm.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  }
  $(PCH_PKG)/SmmControl/RuntimeDxe/SmmControl.inf {
    <PcdsPatchableInModule>
      gEfiMdePkgTokenSpaceGuid.PcdDebugPrintErrorLevel|0x80000000
    <LibraryClasses>
      DebugPrintErrorLevelLib|MdePkg/Library/BaseDebugPrintErrorLevelLib/BaseDebugPrintErrorLevelLib.inf
      PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  }
  $(PCH_PKG)/SataController/Dxe/SataController.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  }
  #$(PCH_PKG)/ActiveBiosDxe/ActiveBios.inf
  $(PCH_PKG)/Spi/RuntimeDxe/PchSpiRuntime.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
      PciExpressLib|MdePkg/Library/DxeRuntimePciExpressLib/DxeRuntimePciExpressLib.inf
  }
  $(PCH_PKG)/Spi/Smm/PchSpiSmm.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  }

  $(PCH_PKG)/SerialGpio/Dxe/PchSerialGpio.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  }
  $(PCH_PKG)/SmartTimer/Dxe/SmartTimer.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  }
  $(PCH_PKG)/Reset/RuntimeDxe/PchResetRuntime.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  }
  $(PCH_PKG)/Wdt/Dxe/WdtDxe.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  }

  $(PLAT_PKG)/Platform/RuntimeDxe/PlatformReset/PlatformReset.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  }
  MdeModulePkg/Bus/Pci/IncompatiblePciDeviceSupportDxe/IncompatiblePciDeviceSupportDxe.inf

  $(PLAT_PKG)/Platform/Dxe/SyscfgSyncDxe/SyscfgSyncDxe.inf {
    <LibraryClasses>
    PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  }

  $(PLAT_PKG)/Pci/Dxe/PciPlatform/PciPlatform.inf {

#S3 add
    <LibraryClasses>
      PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  }
#S3 add

  $(PLAT_PKG)/Legacy/Dxe/LegacyBiosPlatform/LegacyBiosPlatform.inf {
   <LibraryClasses>
      PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  }

  $(PLAT_PKG)/Rsa/BmcSmbiosDxe/BmcSmbiosDxe.inf
  $(PLAT_PKG)/Rsa/RsaBmcDxe/RsaBmcCommands.inf
  $(PLAT_PKG)/Rsa/RsaInit/RsaInit.inf
  $(PLAT_PKG)/Rsa/RsaSmm/RsaSmm.inf

!if $(ME_ENABLE) == TRUE
# ME Stack
  $(SKT_PKG)/Me/Heci/Dxe/HeciInitDxe.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  }
  $(PLAT_PKG)/Me/Policy/Dxe/MePolicyInitDxe.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  }
  !if $(ME_WS_ENABLE) == TRUE
    $(PLAT_PKG)/Me/AMT/FwUpdate/MeFwDowngrade/Dxe/MeFwDowngrade.inf {
        <LibraryClasses>
          PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
    }
    !if $(AMT_ENABLE) == TRUE
      $(PLAT_PKG)/Me/AMT/ActiveManagement/AlertStandardFormat/Dxe/AlertStandardFormatDxe.inf {
          <LibraryClasses>
            PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
      }
      $(PLAT_PKG)/Me/AMT/ActiveManagement/Sol/Dxe/SerialOverLan.inf {
          <LibraryClasses>
           PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
      }
      $(PLAT_PKG)/Me/AMT/ActiveManagement/AmtBootOptions/Dxe/ActiveManagement.inf
      $(PLAT_PKG)/Me/AMT/BiosExtension/Efi/BiosExtensionLoader/Dxe/BiosExtensionLoader.inf {
        <LibraryClasses>
         PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
      }

      $(PLAT_PKG)/Me/AMT/Platform/Dxe/AmtWrapperDxe/AmtWrapperDxe.inf
      $(SKT_PKG)/Me/AMT/Platform/Dxe/AsfTable/AsfTable.inf {
        <LibraryClasses>
        PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
      }
      $(PLAT_PKG)/Me/AMT/Platform/Dxe/AmtPetAlert/AmtPetAlert.inf {
          <LibraryClasses>
            PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
      }

      $(SKT_PKG)/Me/AMT/Platform/Dxe/AmtInt16/AmtInt16.inf
      $(SKT_PKG)/Me/AMT/Platform/Dxe/AmtInt16/AmtInt16Csm.inf
    !endif

    $(PLAT_PKG)/Me/AMT/Platform/Dxe/MeSmbios/MeSmbios.inf {
      <LibraryClasses>
      PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
    }

    !if $(ICC_ENABLE) == TRUE
        $(SKT_PKG)/Me/AMT/Icc/Dxe/IccPlatform.inf {
            <LibraryClasses>
            PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
        }
    !endif
  !endif
  # !if $(SPS_ENABLE) == TRUE
  # As long as .offset.h generation does not handle conditional compilation have to load SPS ASL unconditionaly
    $(PLAT_PKG)/Me/Sps/Acpi/SpsAcpiHooks.inf
    !if $(AP_ENABLE) == TRUE
      $(PLAT_PKG)/Me/Sps/Acpi/SpsAcpiSsdt_AP.inf
    !else
      $(PLAT_PKG)/Me/Sps/Acpi/SpsAcpiSsdt.inf
    !endif
  !if $(SPS_ENABLE) == TRUE
      $(SKT_PKG)/Me/Sps/Dxe/SpsDxe.inf {
        <LibraryClasses>
        PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
      }
      $(SKT_PKG)/Me/Sps/Smm/SpsSmm.inf
      $(SKT_PKG)/Me/Heci/Smm/HeciSmm.inf {
      <LibraryClasses>
        PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
      }
      $(SKT_PKG)/Me/Heci/SmmHeci3/MeHeci3Smm.inf
      !if $(PCHTARGET) == "SPT"
        $(SKT_PKG)/Me/PtuLoader/PtuLoader.inf
      !endif
  !endif
!endif
!if $(IE_ENABLE) == TRUE
  $(PLAT_PKG)/Ie/Policy/Dxe/IePolicyInitDxe.inf
!endif
  $(RP_PKG)/Platform/Dxe/MeSmmProtocolThunk/MeSmmProtocolThunk.inf
  $(RP_PKG)/Platform/Dxe/SmbiosRpTable/SmbiosRpTable.inf
  #TPM
  SecurityPkg/Tcg/MemoryOverwriteControl/TcgMor.inf

  SecurityPkg/Tcg/TcgConfigDxe/TcgConfigDxe.inf {
  <LibraryClasses>
   PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  }

!if $(TPM2_ENABLE) == TRUE

  $(PLAT_PKG)/Override/SecurityPkg/Tcg/Tcg2Config/Tcg2ConfigDxe.inf {
  <LibraryClasses>
   DebugLib|MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf
   PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
   Tpm2DeviceLib|SecurityPkg/Library/Tpm2DeviceLibTcg2/Tpm2DeviceLibTcg2.inf
  }

  $(PLAT_PKG)/Override/SecurityPkg/Tcg/Tcg2Dxe/Tcg2Dxe.inf {
    <LibraryClasses>
      NULL|$(PLAT_PKG)/Override/SecurityPkg/Library/Tpm2DeviceLibDTpm/Tpm2InstanceLibDTpm.inf
#      NULL|...Pkg/Library/Tpm2DeviceLibFTpm/Tpm2InstanceLibFTpm.inf
      NULL|SecurityPkg/Library/HashInstanceLibSha1/HashInstanceLibSha1.inf
      NULL|SecurityPkg/Library/HashInstanceLibSha256/HashInstanceLibSha256.inf
      PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  DebugLib|MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf
  }

  SecurityPkg/Tcg/TcgDxe/TcgDxe.inf {
    <LibraryClasses>
      DebugLib|MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf
      PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  }

  $(PLAT_PKG)/Override/SecurityPkg/Tcg/TcgSmm/TcgSmm.inf {
      <LibraryClasses>
      TpmMeasurementLib|SecurityPkg//Library/DxeTpmMeasurementLib/DxeTpmMeasurementLib.inf
  }

  $(PLAT_PKG)/Override/SecurityPkg/Tcg/Tcg2Smm/Tcg2Smm.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
      Tcg2PhysicalPresenceLib|$(PLAT_PKG)/Override/SecurityPkg/Library/SmmTcg2PhysicalPresenceLib/SmmTcg2PhysicalPresenceLib.inf
      Tpm2DeviceLib|SecurityPkg/Library/Tpm2DeviceLibTcg2/Tpm2DeviceLibTcg2.inf
  }
!else
  SecurityPkg/Tcg/TcgDxe/TcgDxe.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
  }
  SecurityPkg/Tcg/TcgSmm/TcgSmm.inf {
    <LibraryClasses>
      TpmMeasurementLib|SecurityPkg//Library/DxeTpmMeasurementLib/DxeTpmMeasurementLib.inf
  }
!endif

  $(PLAT_PKG)/Platform/Dxe/MemorySubClass/MemorySubClass.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
}
!if $(AP_ENABLE) == TRUE
  $(PLAT_PKG)/Acpi/Dxe/AcpiPlatform/AcpiPlatform_AP.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
      AcpiPlatformTableLib|$(RP_PKG)/Library/AcpiPlatformTableLib/AcpiPlatformLib_AP.inf
  }
!else
  $(PLAT_PKG)/Acpi/Dxe/AcpiPlatform/AcpiPlatform.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
      AcpiPlatformTableLib|$(RP_PKG)/Library/AcpiPlatformTableLib/AcpiPlatformLib.inf
  }
!endif   # AP_ENABLE

#S3 delete
#  $(PLAT_PKG)/Universal/LpcPlatform/LpcPlatform.inf {
#    <LibraryClasses>
#    DebugLib|MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf
#  }
#S3 delete

  $(SKT_PKG)/Smm/Access/SmmAccess.inf {
  <LibraryClasses>
    PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  }
  MdeModulePkg/Core/PiSmmCore/PiSmmIpl.inf
  $(PLAT_PKG)/Override/MdeModulePkg/Core/PiSmmCore/PiSmmCore.inf {
    <LibraryClasses>
      DebugLib|MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf
  }
   MdeModulePkg/Universal/SmmCommunicationBufferDxe/SmmCommunicationBufferDxe.inf

  $(SKT_PKG)/Override/IA32FamilyCpuPkg/PiSmmCpuDxeSmm/PiSmmCpuDxeSmm.inf {
    <LibraryClasses>
      DebugLib|MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf
  }

  UefiCpuPkg/CpuIo2Smm/CpuIo2Smm.inf

#TODO S3:  $(PLAT_PKG)/Platform/Dxe/S3NvramSave/S3NvramSave.inf

 $(PLAT_PKG)/Platform/Dxe/S3NvramSave/S3NvramSave.inf {
    <LibraryClasses>
      CompressDxeLib|CpPlatPkg/Library/Compress/CompressLib.inf
      UefiDecompressLib|MdePkg/Library/BaseUefiDecompressLib/BaseUefiDecompressLib.inf
  }

  $(PLAT_PKG)/Platform/Dxe/PlatformEarlyDxe/PlatformEarlyDxe.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  }

  $(PLAT_PKG)/Acpi/DxeSmm/AcpiSmm/AcpiSmmPlatform.inf {

#S3 add
    <LibraryClasses>
      PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  }
#S3 add

  $(PLAT_PKG)/Pci/Dxe/PciHotPlug/PciHotPlug.inf

  $(RP_PKG)/Platform/Dxe/Setup/DxePlatform.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
      DebugLib|MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf
  }
  $(PLAT_PKG)/Platform/Dxe/SmbiosMiscDxe/SmbiosMiscDxe.inf
!if $(AP_ENABLE) == TRUE
  $(PLAT_PKG)/Acpi/AcpiTables/AcpiTables_AP.inf {
    <BuildOptions>
        *_*_*_ASLCC_FLAGS    = -D PURLEY_FLAG
  }
!else
  $(PLAT_PKG)/Acpi/AcpiTables/AcpiTables.inf {
    <BuildOptions>
        *_*_*_ASLCC_FLAGS    = -D PURLEY_FLAG
  }
!endif

  $(PLAT_PKG)/Acpi/BmcAcpiTables/BmcAcpiTables.inf
  $(PLAT_PKG)/Acpi/BmcAcpiDxe/BmcAcpiDxe.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  }

  $(PLAT_PKG)/Acpi/Dxe/AcpiVtd/AcpiVTD.inf {
  <LibraryClasses>
    PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  }
  $(PLAT_PKG)/Cpu/Dxe/GetCpuInfo/GetCpuInfo.inf
  $(PLAT_PKG)/Platform/Dxe/SmbiosIFWIDxe/SmbiosIFWIDxe.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  }

  $(SKT_PKG)/Dxe/FpgaInit/FpgaDxe/FpgaDxe.inf
  $(SKT_PKG)/Smm/FpgaInit/FpgaSmm/FpgaSmm.inf

  $(RESTRICTED_PKG)/OemMicrocode/Microcode.inf

#Ipmi related drivers
!if $(IPMI_SUPPORT) == TRUE
  CpPlatPkg/Ipmi/BmcAcpi/Smm/BmcAcpiSwChild.inf
  CpPlatPkg/Ipmi/BmcElog/Smm/BmcElog.inf
  CpPlatPkg/Ipmi/GenericElog/Smm/GenericElog.inf
  CpPlatPkg/Ipmi/IpmiRedirFru/Dxe/IpmiRedirFru.inf
  CpPlatPkg/Ipmi/OsWdt/OsWdt.inf
  CpPlatPkg/Ipmi/ServerBmcVariable/Dxe/BmcVariableDriver.inf
  CpPlatPkg/Ipmi/SolStatus/Dxe/SolStatus.inf
  CpPlatPkg/Ipmi/Frb/FrbDxe/FrbDriver.inf
  CpPlatPkg/Ipmi/GenericFru/GenericFru.inf
!endif

  #
  # RAS modules
  #
!if $(RAS_SUPPORT) == TRUE



!if $(RAS_CPU_ONLINE_OFFLINE_ENABLE) == TRUE
  $(SKT_PKG)/Smm/Ras/CpuHotAdd/CpuHotAdd.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
      <BuildOptions>
        *_*_X64_ASM_FLAGS          = -DEFIX64
  }
  $(SKT_PKG)/Smm/Ras/KtiRas/KtiRas.inf
  $(SKT_PKG)/Smm/Ras/CpuRas/CpuRas.inf
  $(SKT_PKG)/Smm/Ras/IioRas/IioRas.inf
  $(PLAT_PKG)/Ras/Smm/HpIOXAccess/HpIOXAccess.inf
  $(PLAT_PKG)/Ras/Smm/RasInit/RasInit.inf {
    <libraryclasses>
      DebugLib|MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf
  }
!endif
  $(SKT_PKG)/Dxe/MemRas/$(CLV)MemRas.inf {
    <LibraryClasses>
      CompressDxeLib|CpPlatPkg/Library/Compress/CompressLib.inf
  }
  $(SKT_PKG)/Smm/Ras/SmbusRecovery/SmbusRecovery.inf
  $(SKT_PKG)/Smm/Ras/QuiesceSupport/QuiesceSupport.inf

  $(PLAT_PKG)/Ras/AddressTranslationDsm/AddressTranslationDsm.inf

  #
  # Error logging
  #
   $(PLAT_PKG)/Ras/Whea/IsPlatformSupportWhea/IsPlatformSupportWhea.inf
   CpPlatPkg/Whea/WheaSupport/WheaSupport.inf
   $(PLAT_PKG)/Ras/Whea/WheaErrorInj/WheaErrorInj.inf
   CpPlatPkg/Whea/WheaElog/WheaElog.inf
   CpPlatPkg/Whea/WheaPlatformBoot/WheaPlatformBoot.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
   }

   $(PLAT_PKG)/Ras/Whea/LastBootErrorLog/LastBootErrorLog.inf
   $(PLAT_PKG)/Ras/Smm/McBankErrorInjection/McBankErrorInjection.inf
   $(PLAT_PKG)/Ras/Smm/ErrHandling/DdrtErrorHandler/DdrtErrorHandler.inf
   $(PLAT_PKG)/Ras/Smm/ErrHandling/ProcessorErrorHandler/ProcessorErrorHandler.inf
   $(PLAT_PKG)/Ras/Smm/ErrHandling/MainErrorHandler/MainErrorHandler.inf
   $(PLAT_PKG)/Ras/Smm/ErrHandling/PlatformErrorHandler/PlatformErrorHandler.inf
   $(PLAT_PKG)/Ras/Smm/PartialMirrorHandler/PartialMirrorHandler.inf
   $(PLAT_PKG)/Ras/Smm/ErrHandling/FpgaErrorHandler/FpgaErrorHandler.inf
   $(PLAT_PKG)/Ras/Smm/ErrHandling/ChipsetErrorHandler/$(CLV)PcieErrorHandler$(SPT).inf {
     !if $(COMPONENT_LEVEL_VALIDATION_BUILD_ENABLE) == TRUE
      <LibraryClasses>
        IoLib|$(SKT_PKG)/Smm/Ras/RasClvHandler/IoOverrideLib/IoOverrideLib.inf
     !endif
   }
   $(PLAT_PKG)/Ras/Whea/WheaErrorLog/WheaErrorLog.inf
   $(PLAT_PKG)/Ras/Emca/EmcaErrorLog/EmcaErrorLog.inf
!endif


  $(SKT_PKG)/Dxe/CpuCsrAccess/CpuCsrAccess.inf
  $(SKT_PKG)/Dxe/CpuCsrAccess/CpuCsrAccessSMM.inf
  $(SKT_PKG)/Dxe/NvdimmCommon/NvdimmCommon.inf
  $(SKT_PKG)/Dxe/NvdimmCommon/NvdimmSmbusCommon.inf
  $(SKT_PKG)/Dxe/CrystalRidge/CrystalRidge.inf
  $(SKT_PKG)/Dxe/CrystalRidge/CrystalRidgeSMM.inf
  $(SKT_PKG)/Dxe/JedecNvDimm/JedecNvDimm.inf
  $(SKT_PKG)/Dxe/JedecNvDimm/JedecNvDimmSMM.inf

  # Capsule update service.
!if $(SIGNED_CAPSULE_ENABLE) == TRUE
   $(PLAT_PKG)/Override/CpPcPlatPkg/SmiFlashSigned/SmiFlashSigned.inf
!endif

$(PLAT_PKG)/Platform/DxeSmm/BiosGuard/BiosGuardServices.inf {
  <LibraryClasses>
    CsrToPcieLib|$(SKT_PKG)/Library/CsrToPcieLib/CsrToPcieDxeLib.inf
    PcieAddrLib|CpRcPkg/Library/PcieAddressLib/PcieAddressLib.inf
    PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
    USRATraceLib|$(SKT_PKG)/Library/USRATraceLib/USRATraceLib.inf
}
# HWP LVT support
  $(SKT_PKG)/Smm/PowerManagement/HwpLvtSmm/HwpLvtSmm.inf

  $(PLAT_PKG)/Platform/Dxe/ForceEfiBoot/ForceEfiBootDxe.inf

#
# All component level validation items are optionally built in order to save time in the average build case
#
!if $(COMPONENT_LEVEL_VALIDATION_BUILD_ENABLE) == TRUE

   $(SKT_PKG)/Dxe/RasClvTester/RasClvTester.inf
   $(SKT_PKG)/Smm/Ras/RasClvHandler/RasClvHandler.inf {
     <LibraryClasses>
       IoLib|$(SKT_PKG)/Smm/Ras/RasClvHandler/IoOverrideLib/IoOverrideLib.inf
       CsrAccessOverrideLib|$(SKT_PKG)/Smm/Ras/RasClvHandler/CsrAccessOverrideLib/CsrAccessOverrideLib.inf
       PciExpressRasSimulationLib|$(SKT_PKG)/Smm/Ras/RasClvHandler/PciExpressRasSimulationLib/PciExpressRasSimulationLib.inf
       PciExpressValidationLib|$(SKT_PKG)/Smm/Ras/RasClvHandler/PciExpressValidationLib/PciExpressValidationLib.inf
       AdddcValidationLib|$(SKT_PKG)/Smm/Ras/RasClvHandler/AdddcValidationLib/AdddcValidationLib.inf
       SystemTopologyLib|$(SKT_PKG)/Smm/Ras/RasClvHandler/SystemTopologyLib/SystemTopologyLib.inf
       SddcValidationLib|$(SKT_PKG)/Smm/Ras/RasClvHandler/SddcValidationLib/SddcValidationLib.inf
       MemorySimulationLib|$(SKT_PKG)/Smm/Ras/RasClvHandler/MemorySimulationLib/MemorySimulationLib.inf
       RankSparingValidationLib|$(SKT_PKG)/Smm/Ras/RasClvHandler/RankSparingValidationLib/RankSparingValidationLib.inf
       PatrolScrubValidationLib|$(SKT_PKG)/Smm/Ras/RasClvHandler/PatrolScrubValidationLib/PatrolScrubValidationLib.inf
       MemoryMirroringValidationLib|$(SKT_PKG)/Smm/Ras/RasClvHandler/MemoryMirroringValidationLib/MemoryMirroringValidationLib.inf
       BiosToPcodeMailboxSimulationLib|$(SKT_PKG)/Smm/Ras/RasClvHandler/BiosToPcodeMailboxSimulationLib/BiosToPcodeMailboxSimulationLib.inf
   }
   $(SKT_PKG)/Smm/Restricted/Mem/MrcClvHandler/MrcClvHandler.inf

!endif #$(COMPONENT_LEVEL_VALIDATION_BUILD_ENABLE)

  $(PLAT_PKG)/Platform/Dxe/OpaPlatCfg/OpaPlatCfg.inf
  $(RP_PKG)/Platform/Dxe/FpkSetup/Dxe/FpkSetup.inf

###################################################################################################
#
# BuildOptions Section - Define the module specific tool chain flags that should be used as
#                        the default flags for a module. These flags are appended to any
#                        standard flags that are defined by the build process. They can be
#                        applied for any modules or only those modules with the specific
#                        module style (EDK or EDKII) specified in [Components] section.
#
###################################################################################################
[BuildOptions.Common.EDK]
  DEFINE CRB_EDK_BUILD_OPTIONS = -D CRB_FLAG

  DEFINE SV_EDK_BUILD_OPTIONS =

!if $(SLE_FLAG_ENABLE) == TRUE
  DEFINE SV_EDK_BUILD_OPTIONS = $(SV_EDK_BUILD_OPTIONS) -D HW_EMULATION -D SLE_FLAG
!endif

!if $(SSAFD) == TRUE
  DEFINE CPU_BUILD_OPTIONS = -D PURLEY_FLAG
!else
!if $(SSALFD) == TRUE
  DEFINE CPU_BUILD_OPTIONS = -D PURLEY_FLAG -D SSA_FLAG
!else
  DEFINE CPU_BUILD_OPTIONS = -D PURLEY_FLAG -D SSA_FLAG -D SSA_LOADER_FLAG
!endif
!endif

!if $(SOFTSIM_BIOS_ENABLE) == TRUE
  DEFINE SOFTSIM_EDK_BUILD_OPTIONS = -D SOFTSIM_FLAG
!else
  DEFINE SOFTSIM_EDK_BUILD_OPTIONS =
!endif

  DEFINE LT_BUILD_OPTIONS = -D LT_FLAG



!if $(SPARING_SCRATCHPAD_ENABLE) == TRUE
  DEFINE SPARING_SCRATCHPAD_OPTION = -D SPARING_SCRATCHPAD_SUPPORT
!else
  DEFINE SPARING_SCRATCHPAD_OPTIONS =
!endif

!if ( $(TRAD_BIOS_ENABLE) == TRUE )
  DEFINE TRAD_BUILD_OPTION = -DTRAD_FLAG=1
!else
  DEFINE TRAD_BUILD_OPTION =
!endif

!if ( $(ULT_BIOS_ENABLE) == TRUE )
  DEFINE ULT_BUILD_OPTION = -DULT_FLAG=1
!else
  DEFINE ULT_BUILD_OPTION =
!endif

!if ( $(SERIAL_IO_ENABLE) == TRUE )
  DEFINE SERIAL_IO_BUILD_OPTION = -DSERIAL_IO_FLAG=1
!else
  DEFINE SERIAL_IO_BUILD_OPTION =
!endif

!if ( $(SUS_WELL_RESTORE_ENABLE) == TRUE )
  DEFINE SUS_WELL_RESTORE_BUILD_OPTION = -DSUS_WELL_RESTORE=1
!else
  DEFINE SUS_WELL_RESTORE_BUILD_OPTION =
!endif

!if ( $(ADSP_ENABLE) == TRUE )
  DEFINE ADSP_BUILD_OPTION = -DADSP_FLAG=1
!else
  DEFINE ADSP_BUILD_OPTION =
!endif

!if ( $(WDT_SUPPORT_ENABLE) == TRUE )
  DEFINE WDT_BUILD_OPTION = -DWDT_SUPPORT_ENABLED=1
!else
  DEFINE WDT_BUILD_OPTION =
!endif

!if ( $(USB_PRECONDITION_ENABLE) == TRUE )
  DEFINE USB_PRECONDITION_BUILD_OPTION = -DUSB_PRECONDITION_ENABLE_FLAG=1
!else
  DEFINE USB_PRECONDITION_BUILD_OPTION =
!endif

!if ( $(PCH_SERVER_BIOS_ENABLE) == TRUE )
  DEFINE PCH_BUILD_OPTION = -DPCH_SERVER_BIOS_FLAG=1
!else
  DEFINE PCH_BUILD_OPTION =
!endif

!if ( $(SERVER_BIOS_ENABLE) == TRUE )
  DEFINE SERVER_BUILD_OPTION = -DSERVER_BIOS_FLAG=1
!else
  DEFINE SERVER_BUILD_OPTION =
!endif

!if ( $(BIOSGUARD_EC_ENABLE) == TRUE )
  DEFINE BIOSGUARD_BUILD_OPTION = -DBIOSGUARD_FLAG=1 -DBIOSGUARD_EC_FLAG=1
!else
  DEFINE BIOSGUARD_BUILD_OPTION = -DBIOSGUARD_FLAG=1
!endif

!if ( $(BTG_ENABLE) == TRUE )
  DEFINE BTG_BUILD_OPTIONS = -DBTG_FLAG
!else
  DEFINE BTG_BUILD_OPTIONS =
!endif

!if $(LIGHTNINGRIDGE_PPO_ENABLE) == TRUE
  DEFINE PPO_PLATFORM_OPTIONS = -D LIGHTNINGRIDGE_PPO_FLAG
!endif
!if $(PURLEYEPDVP_PPO_ENABLE) == TRUE
  DEFINE PPO_PLATFORM_OPTIONS = -D PURLEYEPDVP_PPO_FLAG
!endif
!if $(NEON_PPO_ENABLE) == TRUE
  DEFINE PPO_PLATFORM_OPTIONS = -D NEON_PPO_FLAG
!endif

!if $(HEDT_PPO_ENABLE) == TRUE
  DEFINE PPO_PLATFORM_OPTIONS = -D HEDT_PPO_FLAG
!endif

!if $(HEDTME_PPO_ENABLE) == TRUE
  DEFINE PPO_PLATFORM_OPTIONS = -D HEDT_PPO_FLAG
!endif

!if $(IPMI_SUPPORT) == TRUE
  DEFINE IPMI_BUILD_OPTION = -D IPMI_SUPPORT
!else
  DEFINE IPMI_BUILD_OPTION =
!endif

  DEFINE PCH_PKG_OPTIONS =

!if $(TRACE_HUB_DEBUG_ENABLE) == TRUE
  DEFINE TRACE_HUB_DEBUG_BUILD_OPTIONS = -DTRACE_HUB_DEBUG_FLAG
!else
  DEFINE TRACE_HUB_DEBUG_BUILD_OPTIONS =
!endif

!if $(TRACE_HUB_ENABLE) == TRUE
  DEFINE TRACE_HUB_INIT_BUILD_OPTIONS = -DTRACE_HUB_FLAG
!else
  DEFINE TRACE_HUB_INIT_BUILD_OPTIONS =
!endif

!if $(PPO_FLAG) == TRUE
  DEFINE EDK_PPO_FLAG = -D PPO_FLAG
!else
  DEFINE EDK_PPO_FLAG =
!endif

  DEFINE EDK_PCH_PO_FLAG =

  DEFINE HALO_BUILD_OPTION =


!if $(AP_ENABLE) == TRUE
  DEFINE AP_BUILD_OPTION = -D AP_BUILD
!else
  DEFINE AP_BUILD_OPTION =
!endif

  DEFINE ALL_PPO_OPTIONS = $(CPU_BUILD_OPTIONS) $(PPO_PLATFORM_OPTIONS) $(EDK_PPO_FLAG)

  DEFINE PCH_BIOS_BUILD_OPTIONS = $(TRAD_BUILD_OPTION) $(ULT_BUILD_OPTION) $(SERIAL_IO_BUILD_OPTION) $(PCH_BUILD_OPTION) $(SUS_WELL_RESTORE_BUILD_OPTION) $(ADSP_BUILD_OPTION) $(USB_PRECONDITION_BUILD_OPTION) $(WDT_BUILD_OPTION) $(SERVER_BUILD_OPTION) $(EDK_PCH_PO_FLAG)

!if $(AP_ENABLE) == TRUE
  DEFINE EDK_DSC_FEATURE_BUILD_OPTIONS = $(ALL_PPO_OPTIONS) $(SDP_EDKII_BUILD_OPTIONS) $(CRB_EDK_BUILD_OPTIONS) $(SV_EDK_BUILD_OPTIONS) $(SOFTSIM_EDK_BUILD_OPTIONS) $(BIOSGUARD_BUILD_OPTION) $(TRACE_HUB_DEBUG_BUILD_OPTIONS) $(TRACE_HUB_INIT_BUILD_OPTIONS) $(HALO_BUILD_OPTION) $(AP_BUILD_OPTION)
!else
  DEFINE EDK_DSC_FEATURE_BUILD_OPTIONS = $(ALL_PPO_OPTIONS) $(SDP_EDKII_BUILD_OPTIONS) $(CRB_EDK_BUILD_OPTIONS) $(SV_EDK_BUILD_OPTIONS) $(SOFTSIM_EDK_BUILD_OPTIONS) $(BIOSGUARD_BUILD_OPTION) $(TRACE_HUB_DEBUG_BUILD_OPTIONS) $(TRACE_HUB_INIT_BUILD_OPTIONS) $(HALO_BUILD_OPTION) $(AP_BUILD_OPTION)
!endif

  DEFINE DSC_GLOBAL_BUILD_OPTIONS = $(EDK_DSC_FEATURE_BUILD_OPTIONS) $(PCH_BIOS_BUILD_OPTIONS) $(IPMI_BUILD_OPTION) $(PCH_PKG_OPTIONS) $(LT_BUILD_OPTIONS) $(BTG_BUILD_OPTIONS) -D EFI_SPECIFICATION_VERSION=0x00020000 -D PI_SPECIFICATION_VERSION=0x00000009 -D TIANO_RELEASE_VERSION=0x00080006 -D SUPPORT_DEPRECATED_PCI_CFG_PPI -D MMCFG_BASE_ADDRESS=0x80000000 -D MAX_VARIABLE_SIZE=0x2000 -D EFI_PEI_REPORT_STATUS_CODE_ON -D EFI_PCI_IOV_SUPPORT -D SKX_HOST -D CLX_HOST -D EFI_FIRMWARE_VENDOR="L\"INTEL\"" -D EFI_BUILD_VERSION="L\"EDKII\"" -D EFI_S3_RESUME -D WHEA_SUPPORT -D DISABLE_NEW_DEPRECATED_INTERFACES

!if $(DEBUG_FLAGS_ENABLE) == TRUE
  DEFINE DEBUG_BUILD_OPTIONS      = -D EFI_DEBUG /Od
!else
  DEFINE DEBUG_BUILD_OPTIONS =
!endif

  *_*_IA32_ASM_FLAGS         = -DEFI32 $(TRACE_HUB_DEBUG_BUILD_OPTIONS) $(TRACE_HUB_INIT_BUILD_OPTIONS)
  DEBUG_*_IA32_CC_FLAGS      = -D EFI32 $(DSC_GLOBAL_BUILD_OPTIONS) $(DEBUG_BUILD_OPTIONS)
  RELEASE_*_IA32_CC_FLAGS    = -D EFI32 $(DSC_GLOBAL_BUILD_OPTIONS)
  DEBUG_*_IA32_VFRPP_FLAGS   = -D EFI32 $(DSC_GLOBAL_BUILD_OPTIONS) $(DEBUG_BUILD_OPTIONS)
  RELEASE_*_IA32_VFRPP_FLAGS = -D EFI32 $(DSC_GLOBAL_BUILD_OPTIONS)
  DEBUG_*_IA32_APP_FLAGS     = -D EFI32 $(DSC_GLOBAL_BUILD_OPTIONS) $(DEBUG_BUILD_OPTIONS)
  RELEASE_*_IA32_APP_FLAGS   = -D EFI32 $(DSC_GLOBAL_BUILD_OPTIONS)
  DEBUG_*_IA32_PP_FLAGS      = -D EFI32 $(DSC_GLOBAL_BUILD_OPTIONS) $(DEBUG_BUILD_OPTIONS)
  RELEASE_*_IA32_PP_FLAGS    = -D EFI32 $(DSC_GLOBAL_BUILD_OPTIONS)

  *_*_X64_ASM_FLAGS          = -DEFIX64
  DEBUG_*_X64_CC_FLAGS       = -D EFIX64 $(DSC_GLOBAL_BUILD_OPTIONS) $(DEBUG_BUILD_OPTIONS)
  RELEASE_*_X64_CC_FLAGS     = -D EFIX64 $(DSC_GLOBAL_BUILD_OPTIONS)
  DEBUG_*_X64_VFRPP_FLAGS    = -D EFIX64 $(DSC_GLOBAL_BUILD_OPTIONS) $(DEBUG_BUILD_OPTIONS)
  RELEASE_*_X64_VFRPP_FLAGS  = -D EFIX64 $(DSC_GLOBAL_BUILD_OPTIONS)
  DEBUG_*_X64_APP_FLAGS      = -D EFIX64 $(DSC_GLOBAL_BUILD_OPTIONS) $(DEBUG_BUILD_OPTIONS)
  RELEASE_*_X64_APP_FLAGS    = -D EFIX64 $(DSC_GLOBAL_BUILD_OPTIONS)
  DEBUG_*_X64_PP_FLAGS       = -D EFIX64 $(DSC_GLOBAL_BUILD_OPTIONS) $(DEBUG_BUILD_OPTIONS)
  RELEASE_*_X64_PP_FLAGS     = -D EFIX64 $(DSC_GLOBAL_BUILD_OPTIONS)

  *_*_*_VFR_FLAGS   = -c
  *_*_*_BUILD_FLAGS = -c

[BuildOptions.Common.EDKII]
# Append build options for EDK and EDKII drivers (= is Append, == is Replace)
  DEFINE CRB_EDKII_BUILD_OPTIONS = -D CRB_FLAG

  !if $(DEBUG_FLAGS_ENABLE) == TRUE
    DEFINE EDKII_DEBUG_BUILD_OPTIONS = -D SERIAL_DBG_MSG=1 -D PLATFORM_VARIABLE_ATTRIBUTES=0x3
  !else
    DEFINE EDKII_DEBUG_BUILD_OPTIONS = -D MDEPKG_NDEBUG -D SILENT_MODE -D PLATFORM_VARIABLE_ATTRIBUTES=0x3
  !endif

!if $(SDP_PLATFORM) == TRUE
  DEFINE SDP_EDKII_BUILD_OPTIONS = -D SDP_FLAG -D SV_OR_SDP_FLAG
!else
  DEFINE SDP_EDKII_BUILD_OPTIONS =
!endif

!if $(SSAFD) == TRUE
  DEFINE EDKII_CPU_BUILD_OPTIONS = -D PURLEY_FLAG
!else
!if $(SSALFD) == TRUE
  DEFINE EDKII_CPU_BUILD_OPTIONS = -D PURLEY_FLAG -D SSA_FLAG
!else
  DEFINE EDKII_CPU_BUILD_OPTIONS = -D PURLEY_FLAG -D SSA_FLAG -D SSA_LOADER_FLAG
!endif
!endif

  DEFINE SV_EDKII_BUILD_OPTIONS =

!if $(SLE_FLAG_ENABLE) == TRUE
  DEFINE SV_EDKII_BUILD_OPTIONS = $(SV_EDKII_BUILD_OPTIONS) -D HW_EMULATION -D SLE_FLAG
!endif

!if $(SOFTSIM_BIOS_ENABLE) == TRUE
  DEFINE SOFTSIM_EDKII_BUILD_OPTIONS = -D SOFTSIM_FLAG
!else
  DEFINE SOFTSIM_EDKII_BUILD_OPTIONS =
!endif

  DEFINE LT_BUILD_OPTIONS = -D LT_FLAG


!if $(SPARING_SCRATCHPAD_ENABLE) == TRUE
  DEFINE SPARING_SCRATCHPAD_OPTION = -D SPARING_SCRATCHPAD_SUPPORT
!else
  DEFINE SPARING_SCRATCHPAD_OPTIONS =
!endif

!if ( $(TRAD_BIOS_ENABLE) == TRUE )
  DEFINE TRAD_BUILD_OPTION = -DTRAD_FLAG=1
!else
  DEFINE TRAD_BUILD_OPTION =
!endif

!if ( $(ULT_BIOS_ENABLE) == TRUE )
  DEFINE ULT_BUILD_OPTION = -DULT_FLAG=1
!else
  DEFINE ULT_BUILD_OPTION =
!endif

!if ( $(SERIAL_IO_ENABLE) == TRUE )
  DEFINE SERIAL_IO_BUILD_OPTION = -DSERIAL_IO_FLAG=1
!else
  DEFINE SERIAL_IO_BUILD_OPTION =
!endif

!if ( $(SUS_WELL_RESTORE_ENABLE) == TRUE )
  DEFINE SUS_WELL_RESTORE_BUILD_OPTION = -DSUS_WELL_RESTORE=1
!else
  DEFINE SUS_WELL_RESTORE_BUILD_OPTION =
!endif

!if ( $(ADSP_ENABLE) == TRUE )
  DEFINE ADSP_BUILD_OPTION = -DADSP_FLAG=1
!else
  DEFINE ADSP_BUILD_OPTION =
!endif

!if ( $(WDT_SUPPORT_ENABLE) == TRUE )
  DEFINE WDT_BUILD_OPTION = -DWDT_SUPPORT_ENABLED=1
!else
  DEFINE WDT_BUILD_OPTION =
!endif

!if ( $(USB_PRECONDITION_ENABLE) == TRUE )
  DEFINE USB_PRECONDITION_BUILD_OPTION = -DUSB_PRECONDITION_ENABLE_FLAG=1
!else
  DEFINE USB_PRECONDITION_BUILD_OPTION =
!endif

DEFINE EWL_BUILD_OPTION = -DENHANCED_WARNING_LOG=1

!if ( $(RSTE_TESTING) )
  DEFINE RSTE_BUILD_OPTION = -DRSTE_TEST=1
!else
  DEFINE RSTE_BUILD_OPTION =
!endif

!if ( $(PCH_SERVER_BIOS_ENABLE) == TRUE )
  DEFINE PCH_BUILD_OPTION = -DPCH_SERVER_BIOS_FLAG=1
!else
  DEFINE PCH_BUILD_OPTION =
!endif

!if ( $(BTG_ENABLE) == TRUE )
  DEFINE BTG_BUILD_OPTIONS = -DBTG_FLAG
!else
  DEFINE BTG_BUILD_OPTIONS =
!endif

!if ( $(SERVER_BIOS_ENABLE) == TRUE )
  DEFINE SERVER_BUILD_OPTION = -DSERVER_BIOS_FLAG=1
!else
  DEFINE SERVER_BUILD_OPTION =
!endif

!if $(LIGHTNINGRIDGE_PPO_ENABLE) == TRUE
  DEFINE EDKII_PPO_PLATFORM_OPTIONS = -D LIGHTNINGRIDGE_PPO_FLAG
!endif

!if $(PURLEYEPDVP_PPO_ENABLE) == TRUE
  DEFINE EDKII_PPO_PLATFORM_OPTIONS = -D PURLEYEPDVP_PPO_FLAG
!endif

!if $(NEON_PPO_ENABLE) == TRUE
  DEFINE EDKII_PPO_PLATFORM_OPTIONS = -D NEON_PPO_FLAG
!endif

!if $(HEDT_PPO_ENABLE) == TRUE
  DEFINE EDKII_PPO_PLATFORM_OPTIONS = -D HEDT_PPO_FLAG
!endif

!if $(HEDTME_PPO_ENABLE) == TRUE
  DEFINE EDKII_PPO_PLATFORM_OPTIONS = -D HEDT_PPO_FLAG
!endif

!if $(IPMI_SUPPORT) == TRUE
  DEFINE IPMI_BUILD_OPTION = -D IPMI_SUPPORT
!else
  DEFINE IPMI_BUILD_OPTION =
!endif

  DEFINE PCH_PKG_OPTIONS =

!if $(PPO_FLAG) == TRUE
  DEFINE EDKII_PPO_FLAG = -D PPO_FLAG
!else
  DEFINE EDKII_PPO_FLAG =
!endif

  DEFINE EDKII_PCH_PO_FLAG =

  DEFINE EDKII_HALO_BUILD_OPTION =

!if $(AP_ENABLE) == TRUE
  DEFINE EDKII_AP_BUILD_OPTION = -D AP_BUILD
!else
  DEFINE EDKII_AP_BUILD_OPTION =
!endif

!if $(NVDIMM_ENABLE) == TRUE
 DEFINE NVDIMM_OPTIONS = -D MEM_NVDIMM_EN -D NVMEM_FEATURE_EN
!else
 DEFINE NVDIMM_OPTIONS =
!endif


  DEFINE MAX_SOCKET_OPTIONS = -D MAX_SOCKET=$(MAX_SOCKET)

  DEFINE EDKII_ALL_PPO_OPTIONS = $(EDKII_CPU_BUILD_OPTIONS) $(EDKII_PPO_PLATFORM_OPTIONS) $(EDKII_PPO_FLAG)

  DEFINE PCH_BIOS_BUILD_OPTIONS = $(TRAD_BUILD_OPTION) $(ULT_BUILD_OPTION) $(SERIAL_IO_BUILD_OPTION) $(PCH_BUILD_OPTION) $(SUS_WELL_RESTORE_BUILD_OPTION) $(ADSP_BUILD_OPTION) $(USB_PRECONDITION_BUILD_OPTION) $(WDT_BUILD_OPTION) $(SERVER_BUILD_OPTION) $(EDKII_PCH_PO_FLAG)

!if $(AP_ENABLE) == TRUE
  DEFINE EDKII_DSC_FEATURE_BUILD_OPTIONS = $(SDP_EDKII_BUILD_OPTIONS) $(CRB_EDKII_BUILD_OPTIONS) $(EDKII_DEBUG_BUILD_OPTIONS) $(SV_EDKII_BUILD_OPTIONS) $(IPMI_BUILD_OPTION) $(SOFTSIM_EDKII_BUILD_OPTIONS) $(PCH_BIOS_BUILD_OPTIONS)  $(PCH_PKG_OPTIONS) $(EDKII_ALL_PPO_OPTIONS) $(MAX_SOCKET_OPTIONS) $(LT_BUILD_OPTIONS) $(BTG_BUILD_OPTIONS) $(SPARING_SCRATCHPAD_OPTION) $(TRACE_HUB_DEBUG_BUILD_OPTIONS) $(TRACE_HUB_INIT_BUILD_OPTIONS) $(NVDIMM_OPTIONS) $(EWL_BUILD_OPTION) $(EDKII_HALO_BUILD_OPTION) $(EDKII_AP_BUILD_OPTION) -D EFI_PCI_IOV_SUPPORT -D WHEA_SUPPORT -D SKX_HOST -D CLX_HOST -D MMCFG_BASE_ADDRESS=0x80000000 -D DISABLE_NEW_DEPRECATED_INTERFACES
!else
  DEFINE EDKII_DSC_FEATURE_BUILD_OPTIONS = $(SDP_EDKII_BUILD_OPTIONS) $(CRB_EDKII_BUILD_OPTIONS) $(EDKII_DEBUG_BUILD_OPTIONS) $(SV_EDKII_BUILD_OPTIONS) $(IPMI_BUILD_OPTION) $(SOFTSIM_EDKII_BUILD_OPTIONS) $(PCH_BIOS_BUILD_OPTIONS)  $(PCH_PKG_OPTIONS) $(EDKII_ALL_PPO_OPTIONS) $(MAX_SOCKET_OPTIONS) $(LT_BUILD_OPTIONS) $(BTG_BUILD_OPTIONS) $(SPARING_SCRATCHPAD_OPTION) $(TRACE_HUB_DEBUG_BUILD_OPTIONS) $(TRACE_HUB_INIT_BUILD_OPTIONS) $(NVDIMM_OPTIONS) $(EWL_BUILD_OPTION) $(EDKII_HALO_BUILD_OPTION) $(EDKII_AP_BUILD_OPTION) -D EFI_PCI_IOV_SUPPORT -D WHEA_SUPPORT -D SKX_HOST -D CLX_HOST -D MMCFG_BASE_ADDRESS=0x80000000 -D DISABLE_NEW_DEPRECATED_INTERFACES
!endif

!if $(IE_ENABLE) == TRUE
  DEFINE IE_OPTIONS = -DIE_SUPPORT=1
!else
  DEFINE IE_OPTIONS =
!endif

  DEFINE EDKII_DSC_FEATURE_BUILD_OPTIONS = $(EDKII_DSC_FEATURE_BUILD_OPTIONS) $(IE_OPTIONS)


!if $(ME_ENABLE) == TRUE
  DEFINE ME_OPTIONS = -DME_SUPPORT_FLAG=1
  !if $(SPS_ENABLE) == TRUE
    DEFINE ME_OPTIONS = -DSPS_SUPPORT=1 $(ME_OPTIONS)
  !endif
  !if $(ME_TESTMENU) == TRUE
    DEFINE ME_OPTIONS = -D ME_TESTMENU_FLAG=1 $(ME_OPTIONS)
  !endif
  !if $(ME_WS_ENABLE) == TRUE
    DEFINE ME_OPTIONS = -DME_WS_SUPPORT=1 $(ME_OPTIONS)
    !if $(AMT_ENABLE) == TRUE
      DEFINE ME_OPTIONS = -DAMT_SUPPORT=1 $(ME_OPTIONS)
      !if $(ICC_ENABLE) == TRUE
        DEFINE ME_OPTIONS = -DICC_SUPPORT=1 $(ME_OPTIONS)
      !endif
      !if $(ASF_PEI_ENABLE) == TRUE
        DEFINE ME_OPTIONS = -DASF_PEI=1 $(ME_OPTIONS)
      !endif
    !endif
  !endif
!else
  DEFINE ME_OPTIONS =
!endif



  DEFINE EDKII_DSC_FEATURE_BUILD_OPTIONS = $(EDKII_DSC_FEATURE_BUILD_OPTIONS) $(ME_OPTIONS)

# ASPEED BMC SUPPORT_START
!if $(AP_ENABLE) == TRUE
  DEFINE ASPEED_ENABLE_BUILD_OPTIONS = -D ASPEED_ENABLE
!else
  DEFINE ASPEED_ENABLE_BUILD_OPTIONS =
!endif

  DEFINE EDKII_DSC_FEATURE_BUILD_OPTIONS = $(EDKII_DSC_FEATURE_BUILD_OPTIONS) $(ASPEED_ENABLE_BUILD_OPTIONS)
# ASPEED BMC SUPPORT_END

  MSFT:*_*_*_CC_FLAGS= $(EDKII_DSC_FEATURE_BUILD_OPTIONS) /wd4819
  GCC:*_*_*_CC_FLAGS= $(EDKII_DSC_FEATURE_BUILD_OPTIONS)
  *_*_*_VFRPP_FLAGS = $(EDKII_DSC_FEATURE_BUILD_OPTIONS)
  *_*_*_APP_FLAGS   = $(EDKII_DSC_FEATURE_BUILD_OPTIONS)
  *_*_*_PP_FLAGS    = $(EDKII_DSC_FEATURE_BUILD_OPTIONS)
  *_*_*_ASLPP_FLAGS = $(EDKII_DSC_FEATURE_BUILD_OPTIONS)
  *_*_*_ASLCC_FLAGS = $(EDKII_DSC_FEATURE_BUILD_OPTIONS)


#
# Enable source level debugging for RELEASE build
#
!if $(TARGET) == "RELEASE"
  DEFINE EDKII_RELEASE_SRCDBG_ASM_BUILD_OPTIONS   =
  DEFINE EDKII_RELEASE_SRCDBG_CC_BUILD_OPTIONS    =
  DEFINE EDKII_RELEASE_SRCDBG_DLINK_BUILD_OPTIONS =

  MSFT:*_*_*_ASM_FLAGS   = $(EDKII_RELEASE_SRCDBG_ASM_BUILD_OPTIONS) /Zi
  MSFT:*_*_*_CC_FLAGS    = $(EDKII_RELEASE_SRCDBG_CC_BUILD_OPTIONS) /Zi /Gm
  MSFT:*_*_*_DLINK_FLAGS = $(EDKII_RELEASE_SRCDBG_DLINK_BUILD_OPTIONS) /DEBUG
  GCC:*_*_*_ASM_FLAGS    = $(EDKII_RELEASE_SRCDBG_ASM_BUILD_OPTIONS)
  GCC:*_*_*_CC_FLAGS     = $(EDKII_RELEASE_SRCDBG_CC_BUILD_OPTIONS)
  GCC:*_*_*_DLINK_FLAGS  = $(EDKII_RELEASE_SRCDBG_DLINK_BUILD_OPTIONS)
!endif


#
# Override the existing iasl path in tools_def.template
#
  MSFT:*_*_*_ASL_PATH == $(WORKSPACE)/CpPlatPkg/Tools/iASL/5.1/iasl.exe
  GCC:*_*_*_ASL_PATH == $(WORKSPACE)/CpPlatPkg/Tools/iASL/5.1/iasl
#
# Override the VFR compile flags to speed the build time
#

*_*_*_VFR_FLAGS                     ==  -n


#
# add to the build options for DXE drivers to remove the log message:
# !!!!!!!!  InsertImageRecord - Section Alignment(0x20) is not 4K  !!!!!!!!
#
[BuildOptions.common.EDKII.DXE_RUNTIME_DRIVER]
  MSFT:*_*_*_DLINK_FLAGS = /ALIGN:4096
  GCC:*_*_*_DLINK_FLAGS = -z common-page-size=0x1000

