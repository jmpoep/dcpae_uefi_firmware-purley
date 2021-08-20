@echo off
@REM
@REM This file contains 'Framework Code' and is licensed as such
@REM under the terms of your license agreement with Intel or your
@REM vendor.  This file may not be modified, except as allowed by
@REM additional terms of your license agreement.
@REM
@REM #/*++
@REM #
@REM #  Copyright (c) 2018 - 2019, Intel Corporation. All rights reserved.
@REM #  This software and associated documentation (if any) is furnished
@REM #  under a license and may only be used or copied in accordance
@REM #  with the terms of the license. Except as permitted by such
@REM #  license, no part of this software or documentation may be
@REM #  reproduced, stored in a retrieval system, or transmitted in any
@REM #  form or by any means without the express written consent of
@REM #  Intel Corporation.
@REM #
@REM #  Module Name:
@REM #
@REM #    biosbuild.bat
@REM #
@REM #  Abstract:
@REM #
@REM #    Top level build script.
@REM #
@REM #--*/

REM Run setlocal to take a snapshot of the environment variables.  endlocal is called to restore the environment.
setlocal
set SCRIPT_ERROR=0

:ParseArgs
if /I "%1"=="-h" goto Usage
if /I "%1"=="/h" goto Usage
if /I "%1"=="-?" goto Usage
if /I "%1"=="/?" goto Usage
goto setPkgVars


:Usage
echo.
echo Defaults: Build Type=BUILD ONLY  Target=RELEASE Proc=SKX PCH=LBG
echo Available Targets: DEBUG, RELEASE, SILENT
echo Available Proc:    SKX
echo Available PCH:     LBG, SPT
echo.
echo Usage: [-h ^| /h ^| -? ^| /?]              - Display help text.
echo.
echo    BIOS Build Switches
echo    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
echo        [Target] [Proc] [PCH] [report] [knobs] - Build the BIOS.
echo        [-m ^'Path to .inf^']               - Build an .inf without rebuilding the hole project.
echo        [cleantree]                       - Delete built files not in Build
echo        [cleanall] [Target]               - Delete Build or target folder
echo        [rebuildall] [Target] [report]    - Delete files and rebuild BIOS.
echo.
echo        [halo]                            - Build Halo Creek platform.
echo.
echo    MiniBIOS Build Switches
echo    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
echo        [minibios] [I ^| E ^| IE]         - Copy and build MiniBIOS
echo        [minibioscopy]                    - Copy MiniBIOS files
echo        [minibiosclean]                   - Delete MiniBIOS files
echo        [raspkg    ^| raspkgclean]        - Create or Clean RAS Sample package
echo        [cpupmpkg  ^| cpupmpkgclean]      - Create or Clean CPUPM Sample package
echo        [iiopkg    ^| iiopkgclean]        - Create or Clean IIO Sample package
echo        [pchpkg    ^| pchpkgclean]        - Create or Clean PCH Sample package
echo        [crpkg     ^| crPkgClean]         - Create or Clean CR Sample package
echo        [samplepkg ^| samplepkgclean]     - Create or Clean all Sample packages
echo.
echo    Other Switches
echo    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
echo        [doxygen] [DEBUG ^| RELEASE]
echo          *** Generate doxygen data.
echo              Specify build type for desired output, as its build log is parsed for source files.
echo        [doxygen] [PCHRC]                 - Generate doxygen for PCH Reference Code
echo        [doxygen] [REFCODE]               - Generate doxygen for Reference Code
echo        [doxygen] [RASRC ]                - Generate doxygen for RAS Reference Code
echo        [doxygen] [CPUPMRC]               - Generate doxygen for CPU PM Reference Code
echo        [doxygen] [IIORC]                 - Generate doxygen for IIO Reference Code
echo        [doxygen] [CRRC]                  - Generate doxygen for CR Reference Code
echo        CLV                               - Build component level validation configuration
echo    Compiler Selection Switches
echo    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
echo        VS2008                            - Selects VS2008 compiler tools
echo        VS2010                            - Selects VS2010 compiler tools
echo        VS2013                            - Selects VS2013 compiler tools [default]

goto done


:setPkgVars
time /t
REM
REM Capture the full command line
REM
set FULL_COMMAND_LINE=%0 %*

REM
REM Set environment variables for all packages referenced by DSC and FDF here.
REM They are passed to DSC and FDF via build.exe -D switch.
REM
set PLAT_PKG=PurleyPlatPkg
set RP_PKG=PurleyRpPkg
set SKT_PKG=PurleySktPkg
set PCH_PKG=%SKT_PKG%\SouthClusterLbg
set PLATFORM_RESTRICTED_PKG=PurleyRestrictedPkg
set CP_PLAT_PKG=CpPlatPkg
set CL_SEL=VS2013
REM #
REM #

:setPkgPathVars
REM
REM Discover the root workspace path based on the path to this batch file, and change to that directory so all paths that want to be relative to the root, can be.
REM
for %%A in ("%~dp0..") do set ROOTPATH=%%~fA
IF %ROOTPATH:~-1% NEQ \ (Set ROOTPATH=%ROOTPATH%\)
cd /d %ROOTPATH%

REM Now set package path env vars.
set PLAT_PKG_PATH=%ROOTPATH%%PLAT_PKG%
set RP_PKG_PATH=%ROOTPATH%%RP_PKG%
set SKT_PKG_PATH=%ROOTPATH%%SKT_PKG%
set PCH_PKG_PATH=%ROOTPATH%%PCH_PKG%
set PLATFORM_RESTRICTED_PKG_PATH=%ROOTPATH%%PLATFORM_RESTRICTED_PKG%

set BUILDALL_BIN_PATH=%ROOTPATH%BuildAll.Binaries
echo The BUILD_BIN_PATH is %BUILDALL_BIN_PATH%
set BUILD_SUMMARY_LOG=%ROOTPATH%BuildSummary.log
REM #
REM #
REM edksetup.bat sets WORKSPACE.  A subst drive letter for local drive path, e.g. 'subst v: .' and running BIOS builds from the subst
REM drive results in WORKSPACE ending with 2 backslashes '\\'.  You can't remove a backslash without breaking build tools.  We want
REM RP_PKG_PATH to be the platform package path without a double backslash because when deleting files or checking for file
REM existence, you get "The network path was not found." harmless error message on a subst drive.  Solution is to create an env var
REM RP_PKG_PATH which has the appropriate number of backslashes.  Do same for SKT_PKG_PATH.  You also get error when at
REM command prompt in subst drive and doing "cd %workspace%\%RP_PKG%".  Instead you do "cd %RP_PKG_path%".
REM
:setPkgPathVars
REM Set WORKSPACE_PATH to path of build batch file via %~dp0.  prebuild.bat calls edksetup.bat, which sets WORKSPACE to %CD%.
set WORKSPACE_PATH=%ROOTPATH%
set WORKSPACE=%ROOTPATH%
echo WORKSPACE_PATH=%WORKSPACE_PATH%
REM
REM
REM Execute the following labels with "call :label" and exit the label with "goto :EOF" to allow returning to caller:
REM     cleantree
REM     minibios
REM     minibioscopy
REM     raspkg
REM     cpupmpkg
REM     iiopkg
REM     crpkg
REM     pchpkg
REM     samplepkg
REM     minibiosclean
REM     raspkgclean
REM     cpupmpkgclean
REM     iiopkgclean
REM     crpkgclean
REM     pchpkgclean
REM     samplepkgclean
REM This allows returning to the caller so steps can be taken after label execution with "goto done".
REM
REM All other labels are executed with "goto label" and finish with "goto done".
REM

:checkCmdLine
REM Create MiniBIOS.
if /I "%1"=="minibios" (
  call :minibios %1 %2
  goto done
)
REM Copy MiniBIOS files.
if /I "%1"=="minibioscopy" (
  call :minibioscopy
  goto done
)
REM Clean MiniBIOS.
if /I "%1"=="minibiosclean" (
  call :minibiosclean
  goto done
)
REM Create RAS, CPUPM, IIO, PCH Sample Code packages.
if /I "%1"=="raspkg" (
  call :raspkg
  goto done
)
if /I "%1"=="cpupmpkg" (
  call :cpupmpkg
  goto done
)
if /I "%1"=="iiopkg" (
  call :iiopkg
  goto done
)
if /I "%1"=="crpkg" (
  call :crpkg
  goto done
)
if /I "%1"=="pchpkg" (
  call :pchpkg
  goto done
)
REM Create all Sample Code packages.
if /I "%1"=="samplepkg" (
  call :raspkg
  call :cpupmpkg
  call :iiopkg
  call :pchpkg
  call :crpkg
  goto done
)
REM Clean RAS, CPUPM, IIO, PCH Sample Code packages.
if /I "%1"=="raspkgclean" (
  call :raspkgclean
  goto done
)
if /I "%1"=="cpupmpkgclean" (
  call :cpupmpkgclean
  goto done
)
if /I "%1"=="iiopkgclean" (
  call :iiopkgclean
  goto done
)
if /I "%1"=="crpkgclean" (
  call :crpkgclean
  goto done
)
if /I "%1"=="pchpkgclean" (
  call :pchpkgclean
  goto done
)
REM Clean all Sample Code packages.
if /I "%1"=="samplepkgclean" (
  call :raspkgclean
  call :cpupmpkgclean
  call :iiopkgclean
  call :crpkgclean
  call :pchpkgclean
  if exist SampleCodePkg rd /s /q SampleCodePkg
  goto done
)

:setDefaultBuildValues
set BUILD_TYPE=buildonly
set BUILD_TARGET=RELEASE
set CPUTARGET=SKX
set SSAFD=FALSE
set SSALFD=FALSE
set PCHTARGET=LBG
set BUILD_REPORT_FLAGS=
set PLATFORM=NONE
set SECURITY=NONE
set MAXSOCKET=4
set COMPONENT_LEVEL_VALIDATION_BUILD=DISABLE
set ACPI_COMPILE=ENABLE
set XMLCLI_COMPILE=ENABLE
set SPS_ONLY=DISABLE
set NVDIMM_FLAG=ENABLE
set BUILD_INF=
set BUILD_FPGA=FALSE
set HALO_FLAG=DISABLE
:parseCmdLineParamsLoop
set IS_VALID_SWITCH=FALSE
if "%1"=="" goto endParseCmdLineParamsLoop
if /I "%1"=="cleantree" (
  set BUILD_TYPE=cleantree
  call :cleantree
  goto done
)
if /I "%1"=="cleanall" set BUILD_TYPE=cleanall& goto parseCmdLineParamsLoopNext
if /I "%1"=="rebuildall" set BUILD_TYPE=rebuildall& goto parseCmdLineParamsLoopNext
if /I "%1"=="buildall" set BUILD_TYPE=buildall&     goto parseCmdLineParamsLoopNext
if /I "%1"=="all" if /I "%BUILD_TYPE%"=="cleanall" set BUILD_TARGET=ALL& goto parseCmdLineParamsLoopNext
if /I "%1"=="debug" set BUILD_TARGET=DEBUG& goto parseCmdLineParamsLoopNext
if /I "%1"=="release" set BUILD_TARGET=RELEASE& goto parseCmdLineParamsLoopNext
if /I "%1"=="silent" set BUILD_TARGET=SILENT& goto parseCmdLineParamsLoopNext
if /I "%1"=="hybrid" set BUILD_TARGET=HYBRID& goto parseCmdLineParamsLoopNext
if /I "%1"=="LBG" set PCHTARGET=LBG& goto parseCmdLineParamsLoopNext
if /I "%1"=="SKX" set CPUTARGET=SKX& goto parseCmdLineParamsLoopNext
if /I "%1"=="SSAFD" set SSAFD=TRUE& goto parseCmdLineParamsLoopNext
if /I "%1"=="SSALFD" set SSALFD=TRUE& goto parseCmdLineParamsLoopNext
if /I "%1"=="report" set BUILD_REPORT_FLAGS=-Y EXECUTION_ORDER -Y PCD -Y LIBRARY -Y FLASH -Y DEPEX -Y BUILD_FLAGS -Y FIXED_ADDRESS& goto parseCmdLineParamsLoopNext
if /I "%1"=="shellpkg" set BUILD_TYPE=shellpkg& goto parseCmdLineParamsLoopNext

if /I "%1"=="NEON"     set PLATFORM=NEON& goto parseCmdLineParamsLoopNext
if /I "%1"=="NEONECB"  set PLATFORM=NEONECB& goto parseCmdLineParamsLoopNext
if /I "%1"=="OPAL"     set PLATFORM=OPAL& goto parseCmdLineParamsLoopNext
if /I "%1"=="LIGHTNINGRIDGE" set PLATFORM=%1& goto parseCmdLineParamsLoopNext
if /I "%1"=="PURLEYEPDVP"     set PLATFORM=%1& goto parseCmdLineParamsLoopNext
if /I "%1"=="MONO"     set PLATFORM=%1& goto parseCmdLineParamsLoopNext
if /I "%1"=="NONVDIMM"  set NVDIMM_FLAG=DISABLE& goto parseCmdLineParamsLoopNext
if /I "%1"=="NVDIMM"  set NVDIMM_FLAG=ENABLE& goto parseCmdLineParamsLoopNext

REM Config the MAX_SOCKET variable.
if /I "%1" == "1SKT" set MAXSOCKET=1& goto parseCmdLineParamsLoopNext
if /I "%1" == "2SKT" set MAXSOCKET=2& goto parseCmdLineParamsLoopNext
if /I "%1" == "4SKT" set MAXSOCKET=4& goto parseCmdLineParamsLoopNext
if /I "%1" == "8SKT" set MAXSOCKET=8& goto parseCmdLineParamsLoopNext

REM Generate doxygen data.
if /I "%1" == "DOXYGEN" set DOXYGEN=TRUE& goto parseCmdLineParamsLoopNext
if /I "%1" == "PCHRC" set PCHRC=TRUE& goto parseCmdLineParamsLoopNext
if /I "%1" == "REFCODE" set REFCODE=TRUE& goto parseCmdLineParamsLoopNext
if /I "%1" == "RASRC" set RASRC=TRUE& goto parseCmdLineParamsLoopNext
if /I "%1" == "CPUPMRC" set CPUPMRC=TRUE& goto parseCmdLineParamsLoopNext
if /I "%1" == "IIORC" set IIORC=TRUE& goto parseCmdLineParamsLoopNext
if /I "%1" == "CRRC" set CRRC=TRUE& goto parseCmdLineParamsLoopNext

REM Build Component Level Validation Drivers.
if /I "%1" == "CLV" set COMPONENT_LEVEL_VALIDATION_BUILD=ENABLE& goto parseCmdLineParamsLoopNext

REM Build Fast without ACPI and Stitching
if /I "%1" == "NOACPI" set ACPI_COMPILE=DISABLE& goto parseCmdLineParamsLoopNext
if /I "%1" == "NOXMLCLI" set XMLCLI_COMPILE=DISABLE& goto parseCmdLineParamsLoopNext
if /I "%1" == "NOIFWI" set IFWI_EN=FALSE& goto parseCmdLineParamsLoopNext
if /I "%1" == "SPSONLY" set SPS_ONLY=ENABLE& goto parseCmdLineParamsLoopNext

REM Compiler Selection
if /I "%1" == "VS2008" set CL_SEL=VS2008& goto parseCmdLineParamsLoopNext
if /I "%1" == "VS2010" set CL_SEL=VS2010& goto parseCmdLineParamsLoopNext
if /I "%1" == "VS2013" set CL_SEL=VS2013& goto parseCmdLineParamsLoopNext
if /I "%1" == "VS2015" set CL_SEL=VS2015& goto parseCmdLineParamsLoopNext

REM Build only one .inf without rebuilding the hole project.
if /I "%1" == "-m" (
  set BUILD_INF=-m %2
  shift
  goto parseCmdLineParamsLoopNext
)

REM SKX-P FPGA Patch Begin
if /I "%1" == "FPGA" set BUILD_FPGA=TRUE& goto parseCmdLineParamsLoopNext
REM SKX-P FPGA Patch End

REM HALOCREEK Patch Begin
if /I "%1" == "HALO" (
  set HALO_FLAG=ENABLE
  set PCHTARGET=LBG
  set NVDIMM_FLAG=DISABLE
  goto parseCmdLineParamsLoopNext
)
REM HALOCREEK Patch End
REM Valid switches have been checked, so abort build if invalid switch is specified.
echo ERROR: invalid switch %1
goto done

:parseCmdLineParamsLoopNext
shift
goto parseCmdLineParamsLoop

if /I "%BUILD_TYPE%"=="cleanall" set ACPI_COMPILE=ENABLE&
if /I "%BUILD_TYPE%"=="cleanall" set IFWI_EN=TRUE&
if /I "%BUILD_TYPE%"=="cleanall" set XMLCLI_COMPILE=ENABLE&
if /I "%BUILD_TYPE%"=="rebuildall" set ACPI_COMPILE=ENABLE&
if /I "%BUILD_TYPE%"=="rebuildall" set IFWI_EN=TRUE&
if /I "%BUILD_TYPE%"=="rebuildall" set XMlCLI_COMPILE=ENABLE&
if /I "%BUILD_TYPE%"=="rebuildall" set SPS_ONLY=DISABLE&

:endParseCmdLineParamsLoop

REM #
REM # 4S/8S build parameter <START>
REM #
del /f %RP_PKG_PATH%\Tool\ACE\PlatformLbg.ini 2>nul
REM ##if MAXSOCKET <= 4 (
REM #  copy %RP_PKG_PATH%\Tool\ACE\Platform4SLbg.ini %RP_PKG_PATH%\Tool\ACE\PlatformLbg.ini /Y
REM #)

if /I "%MAXSOCKET%" == "8" (
  copy %RP_PKG_PATH%\Tool\ACE\Platform8SLbg.ini %RP_PKG_PATH%\Tool\ACE\PlatformLbg.ini /Y
) else (
  copy %RP_PKG_PATH%\Tool\ACE\Platform4SLbg.ini %RP_PKG_PATH%\Tool\ACE\PlatformLbg.ini /Y
)
REM #
REM # 4S/8S build parameter <END>
REM #

set BUILD_SUMMARY=TRUE
if /I not "%BUILD_TYPE%" == "buildall" goto checkBuildVariables

:BuildAll.BIOS
set BUILDALL_COPYFILES=1
rd /s /q %BUILDALL_BIN_PATH% 2>nul
md %BUILDALL_BIN_PATH%
if exist *.log del *.log
if exist *.bin del *.bin
if exist *.rom del *.rom
if exist *.efi del *.efi

set PCHTARGET=LBG& set BUILD_TARGET=DEBUG
call :BuildAll "%PCHTARGET% %BUILD_TARGET%"
if %ERRORLEVEL% EQU 1 goto BuildAll.fail

set PCHTARGET=SPT& set BUILD_TARGET=DEBUG
call :BuildAll "%PCHTARGET% %BUILD_TARGET%"
if %ERRORLEVEL% EQU 1 goto BuildAll.fail

set PCHTARGET=LBG& set BUILD_TARGET=RELEASE
call :BuildAll "%PCHTARGET% %BUILD_TARGET%"
if %ERRORLEVEL% EQU 1 goto BuildAll.fail

set PCHTARGET=LBG& set BUILD_TARGET=SILENT
call :BuildAll "%PCHTARGET% %BUILD_TARGET%"
if %ERRORLEVEL% EQU 1 goto BuildAll.fail

:BuildAll.MiniBIOS
call :BuildAll "MiniBIOS"
if %ERRORLEVEL% EQU 1 goto BuildAll.fail

goto BuildAll.pass

:BuildAll
pushd %cd%
if /I not %1=="MiniBIOS" call :checkBuildVariables
if %ERRORLEVEL% NEQ 0 set SCRIPT_ERROR=%ERRORLEVEL%
if /I %1 == "MiniBIOS"   call :minibios
if %ERRORLEVEL% NEQ 0 set SCRIPT_ERROR=%ERRORLEVEL%
popd

if %SCRIPT_ERROR% == 0 if %BUILDALL_COPYFILES% == 1 (
  copy %BUILD_LOG% %BUILDALL_BIN_PATH%
  if /I not %1 == "MiniBIOS" copy *.bin %BUILDALL_BIN_PATH%
  if /I %1 == "MiniBIOS" (
    md %BUILDALL_BIN_PATH%\EXT_RC_RELEASE
    md %BUILDALL_BIN_PATH%\INT_RC_RELEASE
    md %BUILDALL_BIN_PATH%\INT_EMU_RELEASE
    md %BUILDALL_BIN_PATH%\INT_HY_RELEASE
    xcopy /y /e %SKT_PKG_PATH%\MiniBIOS\EXT_RC_RELEASE %BUILDALL_BIN_PATH%\EXT_RC_RELEASE
    xcopy /y /e %SKT_PKG_PATH%\MiniBIOS\INT_RC_RELEASE %BUILDALL_BIN_PATH%\INT_RC_RELEASE
    xcopy /y /e %SKT_PKG_PATH%\MiniBIOS\INT_EMU_RELEASE %BUILDALL_BIN_PATH%\INT_EMU_RELEASE
    xcopy /y /e %SKT_PKG_PATH%\MiniBIOS\INT_HY_RELEASE %BUILDALL_BIN_PATH%\INT_HY_RELEASE

  )
)
REM return to caller
goto :EOF

:BuildAll.fail
set CMDTITLE=%RP_PKG:~0,-3% BuildAll %1
echo.
echo.
echo ERROR: BuildAll failed
EXIT /b %ERRORLEVEL%

:BuildAll.pass
call :cleantree
set CMDTITLE=%RP_PKG:~0,-3% BuildAll
echo.
echo.
echo **********************************************************************************************************
echo Build bins are in %BUILDALL_BIN_PATH% directory. Expecting 6 binaries, 6 log files, and 4 miniBIOS folders
echo **********************************************************************************************************
goto done
REM ##############


:checkBuildVariables
set BUILD_CMD_LINE=
set BUILD_CMD_LINE=%BUILD_CMD_LINE% -D MAX_SOCKET=%MAXSOCKET%
REM set build log filenames after parameters are parsed because they are used in the filename
set BUILD_LOG_PREFIX=build_
set BUILD_SV_LOG_SUFFIX=_sv
set BUILD_REPORT_LOG_SUFFIX=_report
set BUILD_LOG=%ROOTPATH%%BUILD_LOG_PREFIX%%BUILD_TARGET%_%CPUTARGET%_%PCHTARGET%.log
set BUILD_SV_LOG=%ROOTPATH%%BUILD_LOG_PREFIX%%BUILD_TARGET%%BUILD_SV_LOG_SUFFIX%.log
set BUILD_REPORT_LOG=%ROOTPATH%%BUILD_LOG_PREFIX%%BUILD_TARGET%_%BUILD_REPORT_LOG_SUFFIX%.log
set BUILD_ENVIROMENT_LOG=%ROOTPATH%%BUILD_LOG_PREFIX%%BUILD_TARGET%_Enviroment.log

REM now that report filename has been established, append report filename to flags
if /I NOT "%BUILD_REPORT_FLAGS%"=="" set BUILD_REPORT_FLAGS=%BUILD_REPORT_FLAGS% -y %BUILD_REPORT_LOG%

REM Generate doxygen data.
if /I "%DOXYGEN%" == "TRUE" (
  if /I "%PCHRC%" == "TRUE" call :pchpkg
  if /I "%REFCODE%" == "TRUE" call :minibios
  if /I "%RASRC%" == "TRUE" call :raspkg
  if /I "%CPUPMRC%" == "TRUE" call :cpupmpkg
  if /I "%IIORC%" == "TRUE" call :iiopkg
  if /I "%CRRC%" == "TRUE" call :crpkg
  call %PLAT_PKG_PATH%\Tool\doxygen\dox.bat
  @if %ERRORLEVEL% NEQ 0 set SCRIPT_ERROR=%ERRORLEVEL% && call :outputError "doxygen failed, examine screen output for failure source"
  @if %SCRIPT_ERROR% NEQ 0 goto :EOF

  goto done
)

if /I not "%BUILD_TYPE%" == "buildall" (
  if exist %BUILD_LOG% del %BUILD_LOG%
  if exist %BUILD_SV_LOG% del %BUILD_SV_LOG%
  if exist %BUILD_REPORT_LOG% del %BUILD_REPORT_LOG%
  if exist %BUILD_ENVIROMENT_LOG% del %BUILD_ENVIROMENT_LOG%
  if exist %BUILD_ENVIROMENT_LOG% del %BUILD_ENVIROMENT_LOG%
  if exist %BUILDALL_BIN_PATH% rd /s /q %BUILDALL_BIN_PATH%
  del *.bin
  del *.rom
  del *.efi
  del *.log
)

REM #
REM #

REM SKX-P FPGA Patch Begin
if not "%tc.build.type%"=="" (
  set BUILD_FPGA=TRUE
)
REM SKX-P FPGA Patch End

REM can't use BUILD_LOG as EDKII build nukes it, use the BUILD_ENVIROMENT_LOG
REM  To report the build enviroment.
echo ==============================================>> %BUILD_ENVIROMENT_LOG%
echo Build Command:         =%FULL_COMMAND_LINE%   >> %BUILD_ENVIROMENT_LOG%
echo final build arguments are:                    >> %BUILD_ENVIROMENT_LOG%
echo  BUILD_TYPE            =%BUILD_TYPE%          >> %BUILD_ENVIROMENT_LOG%
echo  BUILD_TARGET          =%BUILD_TARGET%        >> %BUILD_ENVIROMENT_LOG%
echo  CPUTARGET             =%CPUTARGET%           >> %BUILD_ENVIROMENT_LOG%
echo  SSAFD                 =%SSAFD%               >> %BUILD_ENVIROMENT_LOG%
echo  SSALFD                =%SSALFD%              >> %BUILD_ENVIROMENT_LOG%
echo  PCHTARGET             =%PCHTARGET%           >> %BUILD_ENVIROMENT_LOG%
echo  BUILD_REPORT_FLAGS    =%BUILD_REPORT_FLAGS%  >> %BUILD_ENVIROMENT_LOG%
echo  PLATFORM              =%PLATFORM%            >> %BUILD_ENVIROMENT_LOG%
echo  SECURITY              =%SECURITY%            >> %BUILD_ENVIROMENT_LOG%
echo  MAX_SOCKET            =%MAXSOCKET%           >> %BUILD_ENVIROMENT_LOG%
echo  NVDIMM_FLAG           =%NVDIMM_FLAG%         >> %BUILD_ENVIROMENT_LOG%
echo  HALO_FLAG             =%HALO_FLAG%           >> %BUILD_ENVIROMENT_LOG%
echo  BUILD_INF             =%BUILD_INF%           >> %BUILD_ENVIROMENT_LOG%
echo  BUILD_FPGA            =%BUILD_FPGA%          >> %BUILD_ENVIROMENT_LOG%
echo ==============================================>> %BUILD_ENVIROMENT_LOG%


:doPreBuild
REM Call prebuild.bat so GenBiosId and HintTable run because the FDF expects X64\BiosId.bin to be present, otherwise build exits with error.
REM Must use RP_PKG because prebuild.bat sets PLAT_PKG_PATH.
REM If we are doing a rebuildall, make sure the first call to prebuild.bat is for "cleanall" to skip most of the prebuild path on the first pass.
if /I "%BUILD_TYPE%"=="rebuildall" (
  if not defined REPEAT_PREBUILD (
    call %RP_PKG%\prebuild.bat  %BUILD_TARGET% %CPUTARGET% %PLATFORM% cleanall
    goto afterPreBuild
  )
)

call %RP_PKG%\prebuild.bat  %BUILD_TARGET% %CPUTARGET% %PLATFORM% %BUILD_TYPE% %BUILD_INF%

:afterPreBuild
if %ERRORLEVEL% NEQ 0 EXIT /b %ERRORLEVEL%
REM rebuildall requires prebuild.bat to be repeated before building BIOS.  rebuildall defines REPEAT_PREBUILD.
REM If REPEAT_PREBUILD is defined, then goto build_after_prebuild label because rebuildall was already run.
if defined REPEAT_PREBUILD goto build_after_prebuild

if /I "%SSAFD%"=="TRUE" (
  set BUILD_CMD_LINE=%BUILD_CMD_LINE% -D SSAFD=TRUE
) else if /I "%SSALFD%"=="TRUE" (
  set BUILD_CMD_LINE=%BUILD_CMD_LINE% -D SSALFD=TRUE
) else (
  set BUILD_CMD_LINE=%BUILD_CMD_LINE%
)

if /I "%NVDIMM_FLAG%" == "ENABLE" (
  set BUILD_CMD_LINE=%BUILD_CMD_LINE% -D NVDIMM_ENABLE=TRUE
)

if /I "%HALO_FLAG%" == "ENABLE" (
  set BUILD_CMD_LINE=%BUILD_CMD_LINE% -D HALO_ENABLE=TRUE
)

if /I "%COMPONENT_LEVEL_VALIDATION_BUILD%" == "ENABLE" (
  set BUILD_CMD_LINE=%BUILD_CMD_LINE% -D COMPONENT_LEVEL_VALIDATION_BUILD_ENABLE=TRUE
)

REM Do all PCHTARGET configuration in PlatformPkgConfig.dsc
set BUILD_CMD_LINE=%BUILD_CMD_LINE% -D PCHTARGET=%PCHTARGET%


if /I "%BUILD_TARGET%" == "SILENT" (
  set BUILD_TARGET=RELEASE
  set BUILD_CMD_LINE=%BUILD_CMD_LINE% -D SUB_TARGET=SILENT
) else (
  set BUILD_CMD_LINE=%BUILD_CMD_LINE% -D SUB_TARGET=NONE
)
REM prebuild.bat sets TOOL_CHAIN_TAG, so all build command line references must be after it.
REM #
REM #
  set BUILD_CMD_LINE=%BUILD_CMD_LINE% -D BUILD_TARGET=%BUILD_TARGET% -D PLAT_PKG=%PLAT_PKG% -D SKT_PKG=%SKT_PKG% -D PLATFORM_RESTRICTED_PKG=%PLATFORM_RESTRICTED_PKG% -D PCH_PKG=%PCH_PKG% -D RP_PKG=%RP_PKG% -D BIOS_ID="%BIOS_ID%" -t %TOOL_CHAIN_TAG% -a IA32 -a X64 -p %RP_PKG%\PlatformPkg.dsc
REM #
REM #

REM #
REM #

echo BUILD_CMD_LINE=%BUILD_CMD_LINE% --log=%BUILD_LOG%


if /I "%BUILD_TYPE%"=="cleanall" goto cleanall
if /I "%BUILD_TYPE%"=="rebuildall" goto rebuildall
goto build_after_prebuild

:cleantree
choice /t 3 /d y /m "Confirm: clean tree of intermediate files created in tree during build"
if %ERRORLEVEL% EQU 2 goto :EOF

REM *.offset.h are generated by iASL
REM
REM file descriptors for stdin, stdout, and stderr are 0, 1, and 2, respectively
REM suppresses "file not found" messages by redirecting stderr output to nul device.
del /f %PLAT_PKG_PATH%\Acpi\Dxe\AcpiPlatform\CpuPm.offset.h 2>nul
del /f %PLAT_PKG_PATH%\Acpi\Dxe\AcpiPlatform\EPRPPlatform.offset.h 2>nul
del /f %PLAT_PKG_PATH%\Acpi\Dxe\AcpiPlatform\EXRPPlatform.offset.h 2>nul

REM #
REM #
goto :EOF

:cleanall
REM don't redirect these echo statements to %BUILD_LOG% because build.exe overwrites %BUILD_LOG%
if /I "%BUILD_TARGET%"=="all" (
  REM Must use 'setlocal enabledelayedexpansion' and !ERRORLEVEL! in if () clause, otherwise
  REM %ERRORLEVEL% will always return 0.  See 'setlocal /?' from command prompt.
  setlocal enabledelayedexpansion
  choice /t 3 /d y /m "Confirm: clean all targets *** deletes Build folder"
  if !ERRORLEVEL! EQU 2 goto done
  echo *******************************************************************************
  echo Build Type [cleanall] - clean all targets
  echo command: rd /s /q Build
  echo *******************************************************************************
  if exist Build rd /s /q Build
  REM Use 'dir' to determine if files exist; redirect STDOUT (1) and STDERR(2) to nul device.
  REM ERRORLEVEL will be 0 if files are found, else it will be 1.
  dir %BUILD_SV_LOG_PREFIX%*.log 1>nul 2>nul
  if !ERRORLEVEL! EQU 0 del %BUILD_SV_LOG_PREFIX%*.log
  dir %BUILD_REPORT_LOG_PREFIX%*.log 1>nul 2>nul
  if !ERRORLEVEL! EQU 0 del %BUILD_REPORT_LOG_PREFIX%*.log
  dir %BUILD_LOG_PREFIX%*.log 1>nul 2>nul
  if !ERRORLEVEL! EQU 0 del %BUILD_LOG_PREFIX%*.log
  endlocal
) else (
  echo *******************************************************************************
  echo Build Type [cleanall] - clean %BUILD_TARGET% target
  echo command: build %BUILD_CMD_LINE% cleanall --log=%BUILD_LOG%
  echo *******************************************************************************
  build %BUILD_CMD_LINE% cleanall --log=%BUILD_LOG%
  if exist %BUILD_LOG% del %BUILD_LOG%
  if exist %BUILD_SV_LOG% del %BUILD_SV_LOG%
  if exist %BUILD_REPORT_LOG% del %BUILD_REPORT_LOG%
  if exist Build rd /s /q Build
  if exist Conf\.cache rd /s /q Conf\.cache
)
call :cleantree
goto done

:rebuildall
REM don't redirect these echo statements to %BUILD_LOG% because build.exe overwrites %BUILD_LOG%
echo *********************************************************************************
echo Build Type [rebuildall]
echo Step   : cleanall
echo command: build %BUILD_CMD_LINE% cleanall --log=%BUILD_LOG%
echo *********************************************************************************
build %BUILD_CMD_LINE% cleanall --log=%BUILD_LOG%
if %ERRORLEVEL% NEQ 0 EXIT /b %ERRORLEVEL%

call :cleantree
REM rebuildall requires prebuild.bat to be repeated before building because cleanall deletes X64\BiosId.bin.
REM Set REPEAT_PREBUILD environment variable to TRUE so doPreBuild knows rebuildall was run and can skip it to build BIOS.
set REPEAT_PREBUILD=TRUE
goto doPreBuild

:build_after_prebuild
REM don't redirect these echo statements to %BUILD_LOG% because build.exe overwrites %BUILD_LOG%
echo *********************************************************************************
echo Build Type [build only]
echo Step   : build BIOS
echo *********************************************************************************
REM Remove rebuildall environment variable because it's no longer needed.
set REPEAT_PREBUILD=


REM :build_custom_ShellPkg
REM build custom ShellPkg with fixed persistant memory SMBIOS dump
if /I not "%BUILD_TYPE%" == "shellpkg"  goto build_BIOS

REM choice /t 3 /d n /m "Confirm: rebuild custom ShellPkg with fixed persistant memory SMBIOS dump"
REM if %ERRORLEVEL% EQU 2 goto build_BIOS

:build_custom_ShellPkg2
echo Step   : build custom ShellPkg fixed persistant memory SMBIOS dump
echo build two versions due to SPT needing overiden Timer lib.
echo command: build -b DEBUG -t %TOOL_CHAIN_TAG% -a X64 -p ServerCommonPkg\BpHotFix\ShellPkg\ShellPkg.dsc -D INCLUDE_DP --log=%BUILD_LOG%
echo **************************************************************************************************
echo build normal shell
echo
build -b %TARGET% -t %TOOL_CHAIN_TAG% -a X64 -p ServerCommonPkg\BpHotFix\ShellPkg\ShellPkg.dsc -D INCLUDE_DP --log=%BUILD_LOG%
if %ERRORLEVEL% NEQ 0 EXIT /b %ERRORLEVEL%
REM copy Shell.efi to location specified in PlatformPkg.fdf
if exist %PLAT_PKG%\Override\ShellBinPkg\UefiShell\X64\Shell.efi del /f ServerCommonPkg\BpHotFix\ShellBinPkg\UefiShell\X64\Shell.efi
copy /y Build\Shell\%TARGET%_%TOOL_CHAIN_TAG%\X64\Shell.efi ServerCommonPkg\BpHotFix\ShellBinPkg\UefiShell\X64

REM delete intermediate Shell build folder so user isn't confused by its presence
rd /s /q Build\Shell


:build_BIOS

REM #
REM #

REM build BIOS
echo *********************************************************************************
echo Step   : build
echo command: build %BUILD_CMD_LINE% --log=%BUILD_LOG% %BUILD_REPORT_FLAGS%
echo *********************************************************************************
REM Force single threaded build with -n 1 build parameter
call build %BUILD_CMD_LINE% --log=%BUILD_LOG% %BUILD_REPORT_FLAGS%
if defined BUILD_INF goto :EOF
if %ERRORLEVEL% NEQ 0 EXIT /b %ERRORLEVEL%

:doPostBuild
if /I "%PLATFORM%"=="MONO" (
    for %%A in (NEON, LIGHTNINGRIDGE, PURLEYEPDVP, HEDT) do (
      call :POST_BUILD_ROUTINE %%A
    )
)else (
  call :POST_BUILD_ROUTINE %PLATFORM%
)
goto :END_PB_ROUTINE

:POST_BUILD_ROUTINE
set BIN_FILENAME=spi_image.bin
REM set CAPSULE_NAME=CAPSULE_BOOT
REM Delete PLATFORM env var so BuildImage.bat (called from postbuild.bat) forces a default case.
echo at biosbuild
REM set PLATFORM=%1 //Commenting this out as PLATFORM argument is already parsed in parseCmdLineParamsLoop
call %RP_PKG%\postbuild.bat %BUILD_TARGET% %CPUTARGET% %PLATFORM% %PCHTARGET%
if %ERRORLEVEL% NEQ 0 EXIT /b %ERRORLEVEL%

:doCopyBIOSFiles
REM #
REM #
:END_PB_ROUTINE

REM #
REM #
  copy Build\%RP_PKG%\%TARGET%_%TOOL_CHAIN_TAG%\ROM\spi_image*.bin /Y
REM #
REM #
goto done

REM
REM MiniBIOS labels below here ...
REM MiniBIOS items don't call prebuild.bat, so don't use environment variables set in prebuild.bat.
REM

REM ===================================================
REM Create MiniBIOS Reference Code package
REM ===================================================
:minibios
call :minibioscopy
if %ERRORLEVEL% NEQ 0 EXIT /b %ERRORLEVEL%

echo *****************************************************
echo       Create MiniBIOS Reference Code package
echo *****************************************************
cd %SKT_PKG_PATH%\MiniBIOS
call BuildMiniBIOS %2
if %ERRORLEVEL% NEQ 0 set SCRIPT_ERROR=%ERRORLEVEL%
if %SCRIPT_ERROR% NEQ 0 EXIT /b %SCRIPT_ERROR%
goto :EOF

REM ===================================================
REM Create MiniBIOS Reference Code package
REM ===================================================
:minibioscopy
set RC_PKG_DIR=%SKT_PKG%\Minibios\Internal_Source\RefCode
set EV_PKG_DIR=%SKT_PKG%\Minibios\Internal_Source\MemoryEvContent
echo *****************************************************
echo       Copy MiniBIOS Reference Code package
echo *****************************************************
attrib -r %RC_PKG_DIR%\*.* /s /d
attrib -r %EV_PKG_DIR%\*.* /s /d
echo   Deleting Files ...
rd /s /q %RC_PKG_DIR%
rd /s /q %EV_PKG_DIR%
echo   Copying Files ...

xcopy /y /e %SKT_PKG%\Library\ProcMemInit %RC_PKG_DIR%\
xcopy /y /e %ROOTPATH%CpRcPkg\Library\BaseMemoryCoreLib\Core %RC_PKG_DIR%\Core\
xcopy /y /e %ROOTPATH%CpRcPkg\Library\AdvMemTestLib %RC_PKG_DIR%\Core\Mem
copy /y %ROOTPATH%CpRcPkg\Include\MaxSocket.h %RC_PKG_DIR%\Platform\Include\
copy /y %ROOTPATH%CpRcPkg\Library\BaseMemoryCoreLib\Chip\Skx\Include\*.h %RC_PKG_DIR%\Chip\Include\
copy /y %ROOTPATH%CpRcPkg\Library\BaseMemoryCoreLib\Platform\Purley\Include\*.h %RC_PKG_DIR%\Platform\Include\
copy /y %PLAT_PKG%\Library\OemProcMemInitLib\OemProcMemInit.c %RC_PKG_DIR%\Platform\Hooks\
copy /y %PLAT_PKG%\Library\OemProcMemInitLib\CommonHooks.c %RC_PKG_DIR%\Platform\Hooks\
copy /y %PLAT_PKG%\Library\OemProcMemInitLib\KtiHooks.c %RC_PKG_DIR%\Platform\Hooks\
copy /y %PLAT_PKG%\Library\UbaKtiLib\UbaKtiLib.c %RC_PKG_DIR%\Platform\Hooks\
copy /y %PLAT_PKG%\Library\OemProcMemInitLib\MemHooks.c %RC_PKG_DIR%\Platform\Hooks\
copy /y %SKT_PKG%\Include\Library\OemIioInit.h %RC_PKG_DIR%\Chip\Include\
copy /y %ROOTPATH%CpRcPkg\Library\BaseMemoryCoreLib\Chip\Skx\Include\Library\IioPlatformData.h %RC_PKG_DIR%\Chip\Include\
copy /y %SKT_PKG%\Include\Library\IioPciePortInfo.h %RC_PKG_DIR%\Chip\Include\
copy /y %ROOTPATH%CpRcPkg\Library\BaseMemoryCoreLib\Chip\Skx\Include\Setup\IioConfig.h %RC_PKG_DIR%\Chip\Include\
copy /y %ROOTPATH%CpRcPkg\Library\BaseMemoryCoreLib\Chip\Skx\Include\Iio\*.h %RC_PKG_DIR%\Chip\Include\
copy /y %SKT_PKG%\Iio\Include\*.h %RC_PKG_DIR%\Chip\Include\
copy /y %SKT_PKG%\Iio\Include\Library\*.h %RC_PKG_DIR%\Chip\Include\
copy /y %SKT_PKG%\Iio\Library\PeiIioInitLib\IioEarlyInit.h %RC_PKG_DIR%\Chip\Include\
copy /y %SKT_PKG%\Iio\Library\PeiDxeCommonIioInitLib\*.h %RC_PKG_DIR%\Chip\Include\
xcopy /y /e %SKT_PKG%\Iio\Library\PeiDxeCommonIioInitLib\*.c %RC_PKG_DIR%\Iio\
copy /y %SKT_PKG%\Iio\Library\PeiIioInitLib\IioEarlyInit.c %RC_PKG_DIR%\Iio\
copy /y %SKT_PKG%\Iio\Library\PeiIioInitLib\IioDmiEarlyInit.c %RC_PKG_DIR%\Iio\
copy /y %PLAT_PKG%\Library\OemIioInit\OemIioInit.c %RC_PKG_DIR%\Iio\
copy /y %PLAT_PKG%\Library\OemIioInit\IioPciePortInfo.c %RC_PKG_DIR%\Iio\
copy /y %SKT_PKG%\Iio\Library\PeiDxeCommonIioInitLib\*.mak %RC_PKG_DIR%\Iio\
copy /y %ROOTPATH%CpRcPkg\Include\Library\PcieAddress.h %RC_PKG_DIR%\Core\Include\
copy /y %ROOTPATH%CpRcPkg\Include\Library\UsraAccessApi.h %RC_PKG_DIR%\Core\Include\
copy /y %ROOTPATH%CpRcPkg\Include\UsraAccessType.h %RC_PKG_DIR%\Core\Include\
copy /y %ROOTPATH%CpRcPkg\Include\Library\CsrToPcieAddress.h %RC_PKG_DIR%\Core\Include\
copy /y %SKT_PKG%\\Library\CsrToPcieLib\CsrToPcieLib.c %RC_PKG_DIR%\Chip\Common\
copy /y %SKT_PKG%\\Library\PcieAddressLib\PcieAddressLib.c %RC_PKG_DIR%\Chip\Common\
copy /y %SKT_PKG%\\Library\UsraRegisterFilterLib\UsraRegisterFilterLib.c %RC_PKG_DIR%\Chip\Common\
copy /y %ROOTPATH%CpRcPkg\Library\BaseMemoryCoreLib\Core\Include\DataTypes.h %RC_PKG_DIR%\Core\Include\Base.h
xcopy /y /e %SKT_PKG%\Library\ProcessorStartup\IA32 %RC_PKG_DIR%\ProcessorStartup\
xcopy /y %PLAT_PKG%\Library\OemProcessorStartupLib\*.asm %RC_PKG_DIR%\ProcessorStartup\
xcopy /y /e %SKT_PKG%\Library\MemoryEvContent %EV_PKG_DIR%\
xcopy /y /e %PCH_PKG%\Include\PchReservedResources.h %RC_PKG_DIR%\Pch\Include\
xcopy /y /e %PCH_PKG%\Include\internal.mak %RC_PKG_DIR%\Pch\Include\
xcopy /y /e %PCH_PKG%\Include\Register\PchRegsPcr.h %RC_PKG_DIR%\Pch\Include\Register\
xcopy /y /e %PCH_PKG%\Include\Register\PchRegsPmc.h %RC_PKG_DIR%\Pch\Include\Register\
xcopy /y /e %PCH_PKG%\Include\Register\PchRegsDmi.h %RC_PKG_DIR%\Pch\Include\Register\
xcopy /y /e %PCH_PKG%\Include\Register\PchRegsPcie.h %RC_PKG_DIR%\Pch\Include\Register\
del %RC_PKG_DIR%\ProcessorStartup\Flat32.asm
del %RC_PKG_DIR%\ProcessorStartup\Flat32.inc
echo.
echo Copied MiniBIOS RC package to %RC_PKG_DIR%.
cd %SKT_PKG%\MiniBIOS
goto :EOF

REM ===================================================
REM Clean MiniBIOS Reference Code package
REM ===================================================
:minibiosclean
set RC_PKG_DIR=%SKT_PKG%\Minibios\Internal_Source\RefCode
set EV_PKG_DIR=%SKT_PKG%\Minibios\Internal_Source\MemoryEvContent
set RC_INT_PKG_DIR=%SKT_PKG%\Minibios\INT_RC_RELEASE
set RC_EXT_PKG_DIR=%SKT_PKG%\Minibios\EXT_RC_RELEASE
set RC_EMU_PKG_DIR=%SKT_PKG%\Minibios\INT_EMU_RELEASE
set RC_HY_PKG_DIR=%SKT_PKG%\Minibios\INT_HY_RELEASE
set RC_SIM_DIR=%SKT_PKG%\Minibios\Internal_Source\Build\Winapp
echo *****************************************************
echo       Cleaning Reference Code generation package
echo *****************************************************
echo   Deleting Files ...
if exist %RC_PKG_DIR% attrib -r %RC_PKG_DIR%\*.* /s /d
if exist %RC_PKG_DIR% (
  echo   %RC_PKG_DIR%
  rd /s /q %RC_PKG_DIR%
)
if exist %EV_PKG_DIR% attrib -r %EV_PKG_DIR%\*.* /s /d
if exist %EV_PKG_DIR% (
  echo   %EV_PKG_DIR%
  rd /s /q %EV_PKG_DIR%
)
if exist %RC_INT_PKG_DIR% (
  echo   %RC_INT_PKG_DIR%
  rd /s /q %RC_INT_PKG_DIR%
)
if exist %RC_EXT_PKG_DIR% (
  echo   %RC_EXT_PKG_DIR%
  rd /s /q %RC_EXT_PKG_DIR%
)
if exist %RC_EMU_PKG_DIR% (
  echo   %RC_EMU_PKG_DIR%
  rd /s /q %RC_EMU_PKG_DIR%
)
if exist %RC_HY_PKG_DIR% (
  echo   %RC_HY_PKG_DIR%
  rd /s /q %RC_HY_PKG_DIR%
)
if exist %RC_SIM_DIR%\Release (
  echo   %RC_SIM_DIR%\Release
  rd /s /q %RC_SIM_DIR%\Release
)
if exist %RC_SIM_DIR%\Debug (
  echo   %RC_SIM_DIR%\Debug
  rd /s /q %RC_SIM_DIR%\Debug
)
if exist %RC_SIM_DIR%\Silent (
  echo   %RC_SIM_DIR%\Silent
  rd /s /q %RC_SIM_DIR%\Silent
)
if exist %SKT_PKG%\Minibios\*.log (
  del %SKT_PKG%\Minibios\*.log
)
goto :EOF

REM ===================================================
REM Create RAS Sample Code package
REM ===================================================
:raspkg
set RAS_SAMPLE_PKG_DIR=SampleCodePkg\RAS
if NOT exist %RAS_SAMPLE_PKG_DIR% md SampleCodePkg
echo *****************************************************
echo       Create RAS Sample Codepackage
echo *****************************************************
attrib -r %RAS_SAMPLE_PKG_DIR%\*.* /s /d
echo   Deleting Files ...
rd /s /q %RAS_SAMPLE_PKG_DIR%
echo   Copying Platform Files ...
xcopy /y /e /I %PLAT_PKG%\Ras %RAS_SAMPLE_PKG_DIR%\PlatformPkg\
xcopy /y /e /I %SKT_PKG%\Ras %RAS_SAMPLE_PKG_DIR%\SocketPkg\
xcopy /y /e /I %PLAT_PKG%\Library\OemRasLib %RAS_SAMPLE_PKG_DIR%\PlatformPkg\Library\OemRasLib\
copy /y %PLAT_PKG%\PlatPkg.dec %RAS_SAMPLE_PKG_DIR%\PlatformPkg
copy /y %RP_PKG%\PlatformPkg.fdf %RAS_SAMPLE_PKG_DIR%\PlatformPkg
copy /y %RP_PKG%\PlatformPkg.dsc %RAS_SAMPLE_PKG_DIR%\PlatformPkg
echo   Copying Acpi Files ...
xcopy /y /e /I %PLAT_PKG%\Acpi %RAS_SAMPLE_PKG_DIR%\PlatformPkg\Acpi
echo   Copying Whea Files ...
md %RAS_SAMPLE_PKG_DIR%\ServerCommonPkg\Include
md %RAS_SAMPLE_PKG_DIR%\ServerCommonPkg\Include\Library
xcopy /y /e /I ServerCommonPkg\Whea %RAS_SAMPLE_PKG_DIR%\ServerCommonPkg\Whea
xcopy /y /e /I ServerCommonPkg\Library\WheaErrorLib %RAS_SAMPLE_PKG_DIR%\ServerCommonPkg\Library\WheaErroLib
xcopy /y /e /I ServerCommonPkg\Include\IndustryStandard %RAS_SAMPLE_PKG_DIR%\ServerCommonPkg\Include\IndustryStandard
copy /y ServerCommonPkg\Include\WheaPlatformDefs.h %RAS_SAMPLE_PKG_DIR%\ServerCommonPkg\Include\.
copy /y ServerCommonPkg\Include\Library\WheaErrorLib.h %RAS_SAMPLE_PKG_DIR%\ServerCommonPkg\Include\Library\.
echo   Copying Include Files ...
md %RAS_SAMPLE_PKG_DIR%\PlatformPkg\Include\Guid
md %RAS_SAMPLE_PKG_DIR%\SocketPkg\Include\Guid
md %RAS_SAMPLE_PKG_DIR%\SocketPkg\Include\Setup
md %RAS_SAMPLE_PKG_DIR%\PlatformPkg\Include\Protocol
md %RAS_SAMPLE_PKG_DIR%\PlatformPkg\Include\Acpi
md %RAS_SAMPLE_PKG_DIR%\PlatformPkg\Include\EfiServerManagement
copy /y %PLAT_PKG%\Include\Guid\AcpiVariable.h %RAS_SAMPLE_PKG_DIR%\PlatformPkg\Include\Guid\.
copy /y %SKT_PKG%\Include\Guid\MemoryMapData.h %RAS_SAMPLE_PKG_DIR%\SocketPkg\Include\Guid\.
copy /y %SKT_PKG%\Include\Setup\IioUniversalData.h %RAS_SAMPLE_PKG_DIR%\SocketPkg\Include\Setup\.
copy /y %PLAT_PKG%\Include\Acpi %RAS_SAMPLE_PKG_DIR%\PlatformPkg\Include\Acpi\.
copy /y %PLAT_PKG%\Include\Protocol\CpuHotAddData.h %RAS_SAMPLE_PKG_DIR%\PlatformPkg\Include\Protocol\.
copy /y %PLAT_PKG%\Include\Protocol\DmaRemap.h %RAS_SAMPLE_PKG_DIR%\PlatformPkg\Include\Protocol\.
copy /y %PLAT_PKG%\Include\Protocol\GlobalNvsArea.h %RAS_SAMPLE_PKG_DIR%\PlatformPkg\Include\Protocol\.
copy /y %PLAT_PKG%\Include\Protocol\HpIoxAccess.h %RAS_SAMPLE_PKG_DIR%\PlatformPkg\Include\Protocol\.
copy /y %PLAT_PKG%\Include\Protocol\IioUds.h %RAS_SAMPLE_PKG_DIR%\PlatformPkg\Include\Protocol\.
copy /y %PLAT_PKG%\Include\Protocol\MemRasProtocol.h %RAS_SAMPLE_PKG_DIR%\PlatformPkg\Include\Protocol\.
copy /y %PLAT_PKG%\Include\Protocol\RasMpLinkProtocol.h %RAS_SAMPLE_PKG_DIR%\PlatformPkg\Include\Protocol\.
copy /y %PLAT_PKG%\Include\Acpi\AcpiVtd.h %RAS_SAMPLE_PKG_DIR%\PlatformPkg\Include\Acpi\.
copy /y %PLAT_PKG%\Include\EfiServerManagement.h %RAS_SAMPLE_PKG_DIR%\PlatformPkg\Include\EfiServerManagement\.
copy /y %PLAT_PKG%\Include\Acpi\Fadt.h %RAS_SAMPLE_PKG_DIR%\PlatformPkg\Include\Acpi\.
copy /y %PLAT_PKG%\Include\Acpi\Hpet.h %RAS_SAMPLE_PKG_DIR%\PlatformPkg\Include\Acpi\.
copy /y %PLAT_PKG%\Include\Acpi\Mcfg.h %RAS_SAMPLE_PKG_DIR%\PlatformPkg\Include\Acpi\.
copy /y %PLAT_PKG%\Include\Platform.h %RAS_SAMPLE_PKG_DIR%\PlatformPkg\Include\Acpi\.
copy /y %PLAT_PKG%\Include\Acpi\Srat.h %RAS_SAMPLE_PKG_DIR%\PlatformPkg\Include\Acpi\.
echo   Copying EDKII Core Files ...
xcopy /y /e /I IA32FamilyCpuPkg\PiSmmCommunication %RAS_SAMPLE_PKG_DIR%\IA32FamilyCpuPkg\PiSmmCommunication
xcopy /y /e /I IA32FamilyCpuPkg\PiSmmCpuDxeSmm %RAS_SAMPLE_PKG_DIR%\IA32FamilyCpuPkg\PiSmmCpuDxeSmm
xcopy /y /e /I IntelFrameworkPkg\Library\DxeSmmDriverEntryPoint %RAS_SAMPLE_PKG_DIR%\IA32FamilyCpuPkg\DxeSmmDriverEntryPoint
xcopy /y /e /I MdePkg\Library\BaseLib %RAS_SAMPLE_PKG_DIR%\MdePkg\Library\BaseLib
xcopy /y /e /I MdePkg\Library\BaseMemoryLib %RAS_SAMPLE_PKG_DIR%\MdePkg\Library\BaseMemoryLib
xcopy /y /e /I MdePkg\Library\UefiBootServicesTableLib %RAS_SAMPLE_PKG_DIR%\MdePkg\Library\UefiBootServicesTableLib
xcopy /y /e /I MdePkg\Library\UefiRuntimeServicesTableLib %RAS_SAMPLE_PKG_DIR%\MdePkg\Library\UefiRuntimeServicesTableLib
xcopy /y /e /I MdePkg\Library\UefiLib %RAS_SAMPLE_PKG_DIR%\MdePkg\Library\UefiLib
xcopy /y /e /I MdePkg\Library\UefiRuntimeLib %RAS_SAMPLE_PKG_DIR%\MdePkg\Library\UefiRuntimeLib
xcopy /y /e /I MdePkg\Library\SmmPeriodicSmiLib %RAS_SAMPLE_PKG_DIR%\MdePkg\Library\SmmPeriodicSmiLib
xcopy /y /e /I MdePkg\Library\UefiDriverEntryPoint %RAS_SAMPLE_PKG_DIR%\MdePkg\Library\UefiDriverEntryPoint
xcopy /y /e /I MdePkg\Library\DxeHobLib %RAS_SAMPLE_PKG_DIR%\MdePkg\Library\DxeHobLib
xcopy /y /e /I MdePkg\Library\BasePcdLibNull %RAS_SAMPLE_PKG_DIR%\MdePkg\Library\BasePcdLibNull
xcopy /y /e /I IntelFrameworkPkg\Include\Protocol\Smm*.h %RAS_SAMPLE_PKG_DIR%\IntelFramworkPkg\Include\Protocol\.
xcopy /y /e /I MdePkg\Include\Library\Smm*.h %RAS_SAMPLE_PKG_DIR%\MdePkg\Include\Library\.
xcopy /y /e /I MdePkg\Include\Library\Uefi*.h %RAS_SAMPLE_PKG_DIR%\MdePkg\Include\Library\.
xcopy /y /e /I MdePkg\Include\Library\Base*.h %RAS_SAMPLE_PKG_DIR%\MdePkg\Include\Library\.
xcopy /y /e /I MdePkg\Include\Library\Dxe*.h %RAS_SAMPLE_PKG_DIR%\MdePkg\Include\Library\.
xcopy /y /e /I MdePkg\Include\Protocol\Smm*.h %RAS_SAMPLE_PKG_DIR%\MdePkg\Include\Protocol\.
xcopy /y /e /I MdeModulePkg\Include\Protocol\Smm*.h %RAS_SAMPLE_PKG_DIR%\MdeModulePkg\Include\Protocol\.
xcopy /y /e /I MdeModulePkg\Include\Library\Smm*.h %RAS_SAMPLE_PKG_DIR%\MdePkg\Include\Library\.
xcopy /y /e /I MdePkg\Include\IndustryStandard %RAS_SAMPLE_PKG_DIR%\MdePkg\Include\IndustryStandard
goto :EOF

REM ===================================================
REM Clean RAS Sample Code package
REM ===================================================
:raspkgclean
set RAS_SAMPLE_PKG_DIR=SampleCodePkg\RAS
echo *****************************************************
echo       Clean RAS Sample Code package
echo *****************************************************
attrib -r %RAS_SAMPLE_PKG_DIR%\*.* /s /d
echo   Deleting Files ...
rd /s /q %RAS_SAMPLE_PKG_DIR%
goto :EOF

REM ===================================================
REM Create CPU PM Sample Code package
REM ===================================================
:cpupmpkg
set CPUPM_SAMPLE_PKG_DIR=SampleCodePkg\CPUPM
if NOT exist %CPUPM_SAMPLE_PKG_DIR% md SampleCodePkg
echo *****************************************************
echo       Create CPU PM Sample Code package
echo *****************************************************
attrib -r %CPUPM_SAMPLE_PKG_DIR%\*.* /s /d
echo   Deleting Files ...
rd /s /q %CPUPM_SAMPLE_PKG_DIR%
echo   Copying Files ...
xcopy /y /e /I %PLAT_PKG%\Cpu\Dxe\PlatformCpuPolicy %CPUPM_SAMPLE_PKG_DIR%\PlatformPkg\Dxe\PlatformCpuPolicy
xcopy /y /e /I %PLAT_PKG%\Cpu\Dxe\GetCpuInfo %CPUPM_SAMPLE_PKG_DIR%\PlatformPkg\Dxe\GetCpuInfo
xcopy /y /e /I %SKT_PKG%\Override\IA32FamilyCpuPkg\CpuMpDxe %CPUPM_SAMPLE_PKG_DIR%\SocketPkg\CpuMpDxe
xcopy /y /e /I IA32FamilyCpuPkg\CpuArchDxe %CPUPM_SAMPLE_PKG_DIR%\IA32FamilyCpuPkg\CpuArchDxe
xcopy /y /e /I %SKT_PKG%\Dxe\PowerManagement %CPUPM_SAMPLE_PKG_DIR%\SocketPkg\Dxe\PowerManagement
xcopy /y /e /I %SKT_PKG%\Library\ProcessorPowerManagement %CPUPM_SAMPLE_PKG_DIR%\SocketPkg\Library\ProcessorPowerManagement
md %CPUPM_SAMPLE_PKG_DIR%\PlatformPkg\Include
md %CPUPM_SAMPLE_PKG_DIR%\SocketPkg\Include
md %CPUPM_SAMPLE_PKG_DIR%\SocketPkg\Include\Setup
md %CPUPM_SAMPLE_PKG_DIR%\SocketPkg\Include\Library
md %CPUPM_SAMPLE_PKG_DIR%\SocketPkg\Include\Protocol
md %CPUPM_SAMPLE_PKG_DIR%\PlatformPkg\Include\Guid
copy /y %SKT_PKG%\Include\Setup\IioUniversalData.h %CPUPM_SAMPLE_PKG_DIR%\SocketPkg\Include\Setup\.
copy /y %SKT_PKG%\Override\IA32FamilyCpuPkg\Include\Library\SocketLga1156Lib.h %CPUPM_SAMPLE_PKG_DIR%\SocketPkg\Include\Library\.
xcopy /y /e /I %SKT_PKG%\Include\Cpu %CPUPM_SAMPLE_PKG_DIR%\SocketPkg\Include\Cpu\.
copy /y %PLAT_PKG%\Include\Platform.h %CPUPM_SAMPLE_PKG_DIR%\PlatformPkg\Include\.
copy /y %SKT_PKG%\Include\Protocol\PlatformCpu.h %CPUPM_SAMPLE_PKG_DIR%\SocketPkg\Include\Protocol\.
copy /y %PLAT_PKG%\Include\Guid\SetupVariable.h %CPUPM_SAMPLE_PKG_DIR%\PlatformPkg\Include\Guid\.
echo   Copying Acpi Files ...
md %CPUPM_SAMPLE_PKG_DIR%\SocketPkg\Include\Acpi
xcopy /y /e /I %PLAT_PKG%\Acpi\AcpiTables\Dsdt %CPUPM_SAMPLE_PKG_DIR%\PlatformPkg\Acpi\AcpiTables\Dsdt
xcopy /y /e /I %PLAT_PKG%\Acpi\AcpiTables\Madt %CPUPM_SAMPLE_PKG_DIR%\PlatformPkg\Acpi\AcpiTables\Madt
xcopy /y /e /I %PLAT_PKG%\Acpi\AcpiTables\Ssdt %CPUPM_SAMPLE_PKG_DIR%\PlatformPkg\Acpi\AcpiTables\Ssdt
xcopy /y /e /I %PLAT_PKG_PATH%\Acpi\Dxe\AcpiPlatform %CPUPM_SAMPLE_PKG_DIR%\PlatformPkg\Acpi\AcpiPlatform
copy /y /e /I %PLAT_PKG%\Include\Acpi\Madt.h %CPUPM_SAMPLE_PKG_DIR%\PlatformPkg\Include\Acpi.
echo   Copying Core Files ...
xcopy /y /e /I MdePkg\Library\BaseLib %CPUPM_SAMPLE_PKG_DIR%\\MdePkg\LibraryBaseLib
xcopy /y /e /I MdePkg\Library\BasePciExpressLib %CPUPM_SAMPLE_PKG_DIR%\MdePkg\Library\BasePciExpressLib
xcopy /y /e /I MdePkg\Library\BaseS3BootScriptLibNull %CPUPM_SAMPLE_PKG_DIR%\MdePkg\Library\BaseS3BootScriptLibNull
xcopy /y /e /I MdePkg\Library\BaseMemoryLib %CPUPM_SAMPLE_PKG_DIR%\MdePkg\Library\BaseMemoryLib
xcopy /y /e /I MdePkg\Library\UefiBootServicesTableLib %CPUPM_SAMPLE_PKG_DIR%\MdePkg\Library\UefiBootServicesTableLib
xcopy /y /e /I MdePkg\Library\UefiRuntimeServicesTableLib %CPUPM_SAMPLE_PKG_DIR%\MdePkg\Library\UefiRuntimeServicesTableLib
xcopy /y /e /I MdePkg\Library\DxeHobLib %CPUPM_SAMPLE_PKG_DIR%\MdePkg\Library\DxeHobLib
xcopy /y /e /I MdePkg\Library\BasePcdLibNull %CPUPM_SAMPLE_PKG_DIR%\MdePkg\Library\BasePcdLibNull
xcopy /y /e /I MdePkg\Library\BaseSynchronizationLib %CPUPM_SAMPLE_PKG_DIR%\MdePkg\Library\BaseSynchronizationLib
xcopy /y /e /I MdePkg\Library\BaseTimerLibNullTemplate %CPUPM_SAMPLE_PKG_DIR%\MdePkg\Librarye\BaseTimerLibNullTemplate
xcopy /y /e /I MdePkg\Library\UefiLib %CPUPM_SAMPLE_PKG_DIR%\MdePkg\Library\UefiLib
xcopy /y /e /I MdePkg\Library\BaseIoLibIntrinsic %CPUPM_SAMPLE_PKG_DIR%\MdePkg\Library\BaseIoLibIntrinsic
xcopy /y /e /I MdePkg\Library\PeiDxePostCodeLibReportStatusCode %CPUPM_SAMPLE_PKG_DIR%\MdePkg\Library\PeiDxePostCodeLibReportStatusCode
xcopy /y /e /I MdePkg\Library\BasePrintLib %CPUPM_SAMPLE_PKG_DIR%\MdePkg\Library\BasePrintLib
xcopy /y /e /I MdeModulePkg\Library\UefiHiiLib %CPUPM_SAMPLE_PKG_DIR%\MdeModulePkg\Library\UefiHiiLib
xcopy /y /e /I MdeModulePkg\Library\DebugAgentLibNull %CPUPM_SAMPLE_PKG_DIR%\MdeModulePkg\Library\DebugAgentLibNull
xcopy /y /e /I UefiCpuPkg\Library\MtrrLib %CPUPM_SAMPLE_PKG_DIR%\UefiCpuPkg\Library\MtrrLib
xcopy /y /e /I MdePkg\Include\Library\Uefi*.h %CPUPM_SAMPLE_PKG_DIR%\MdePkg\Include\Library\.
xcopy /y /e /I MdePkg\Include\Library\Base*.h %CPUPM_SAMPLE_PKG_DIR%\MdePkg\Include\Library\.
xcopy /y /e /I MdePkg\Include\Library\Dxe*.h %CPUPM_SAMPLE_PKG_DIR%\MdePkg\Include\Library\.
md %CPUPM_SAMPLE_PKG_DIR%\MdeModulePkg\Include\Library
md %CPUPM_SAMPLE_PKG_DIR%\UefiCpuPkg\Include\Library
copy /y MdePkg\Include\Library\PciExpressLib.h %CPUPM_SAMPLE_PKG_DIR%\MdePkg\Include\Library\.
copy /y MdePkg\Include\Library\IoLib.h %CPUPM_SAMPLE_PKG_DIR%\MdePkg\Include\Library\.
copy /y MdePkg\Include\Library\S3BootScriptLib.h %CPUPM_SAMPLE_PKG_DIR%\MdePkg\Include\Library\.
copy /y MdePkg\Include\Library\TimerLib.h %CPUPM_SAMPLE_PKG_DIR%\MdePkg\Include\Library\.
copy /y MdePkg\Include\Library\ReportStatusCodeLib.h %CPUPM_SAMPLE_PKG_DIR%\MdePkg\Include\Library\.
copy /y MdePkg\Include\Library\PrintLib.h %CPUPM_SAMPLE_PKG_DIR%\MdePkg\Include\Library\.
copy /y MdeModulePkg\Include\Library\HiiLib.h %CPUPM_SAMPLE_PKG_DIR%\MdeModulePkg\Include\Library\.
copy /y MdeModulePkg\Include\Library\DebugAgentLib.h %CPUPM_SAMPLE_PKG_DIR%\MdeModulePkg\Include\Library\.
copy /y UefiCpuPkg\Include\Library\LocalApicLib.h %CPUPM_SAMPLE_PKG_DIR%\UefiCpuPkg\Include\Library\.
copy /y UefiCpuPkg\Include\Library\MtrrLib.h %CPUPM_SAMPLE_PKG_DIR%\UefiCpuPkg\Include\Library\.
goto :EOF

REM ===================================================
REM Clean CPU PM Sample Code
REM ===================================================
:cpupmpkgclean
set CPUPM_SAMPLE_PKG_DIR=SampleCodePkg\CPUPM
echo *****************************************************
echo       Clean CPU PM Sample Code package
echo *****************************************************
attrib -r %CPUPM_SAMPLE_PKG_DIR%\*.* /s /d
echo   Deleting Files ...
rd /s /q %CPUPM_SAMPLE_PKG_DIR%
goto :EOF

REM ===================================================
REM Create IIO Sample Code package
REM ===================================================
:iiopkg
set IIO_SAMPLE_PKG_DIR=SampleCodePkg\IIO
if NOT exist %IIO_SAMPLE_PKG_DIR% md SampleCodePkg
echo *****************************************************
echo       Create IIO Sample Code package
echo *****************************************************
attrib -r %IIO_SAMPLE_PKG_DIR%\*.* /s /d
echo   Deleting Files ...
rd /s /q %IIO_SAMPLE_PKG_DIR%
echo   Copying Files ...
xcopy /y /e /I %PLAT_PKG%\Library\IioInitHooksLib %IIO_SAMPLE_PKG_DIR%\PlatformPkg\Library\IioInitHooksLib
xcopy /y /e /I %PLAT_PKG%\Pci\Dxe\PciPlatform %IIO_SAMPLE_PKG_DIR%\PlatformPkg\Pci\Dxe\PciPlatform
xcopy /y /e /I %SKT_PKG%\Dxe\PciHotPlug %IIO_SAMPLE_PKG_DIR%\SocketPkg\Dxe\PciHotPlug
xcopy /y /e /I %PLAT_PKG%\Library\PcieSlotOpromLib %IIO_SAMPLE_PKG_DIR%\PlatformPkg\Library\PcieSlotOpromLib
xcopy /y /e /I %SKT_PKG%\Dxe\IioInit %IIO_SAMPLE_PKG_DIR%\SocketPkg\Dxe\IioInit
xcopy /y /e /I %SKT_PKG%\Dxe\PciHostBridge %IIO_SAMPLE_PKG_DIR%\SocketPkg\Dxe\PciHostBridge
xcopy /y /e /I %SKT_PKG%\Library\IioEarlyInitialize %IIO_SAMPLE_PKG_DIR%\SocketPkg\Library\IioEarlyInitialize
xcopy /y /e /I %SKT_PKG%\Library\IioLateInitialize %IIO_SAMPLE_PKG_DIR%\SocketPkg\Library\IioLateInitialize
xcopy /y /e /I %SKT_PKG%\Dxe\CpuCsrAccess %IIO_SAMPLE_PKG_DIR%\SocketPkg\Dxe\CpuCsrAccess
xcopy /y /e /I %SKT_PKG%\Smm\Iio %IIO_SAMPLE_PKG_DIR%\SocketPkg\Smm\IioSmm
xcopy /y /e /I %SKT_PKG%\Pei\IioInit %IIO_SAMPLE_PKG_DIR%\SocketPkg\Pei\IioInit
md %IIO_SAMPLE_PKG_DIR%\PlatformPkg\Include
md %IIO_SAMPLE_PKG_DIR%\SocketPkg\Include
md %IIO_SAMPLE_PKG_DIR%\SocketPkg\Include\Setup
md %IIO_SAMPLE_PKG_DIR%\SocketPkg\Include\Library
md %IIO_SAMPLE_PKG_DIR%\SocketPkg\Include\Protocol
md %IIO_SAMPLE_PKG_DIR%\SocketPkg\Include\Guid
copy /y %SKT_PKG%\Include\Setup\IioUniversalData.h %IIO_SAMPLE_PKG_DIR%\SocketPkg\Include\Setup\.
copy /y %PLAT_PKG%\Include\Platform.h %IIO_SAMPLE_PKG_DIR%\PlatformPkg\Include\.
copy /y %SKT_PKG%\Include\Library\OemIioInit.h %IIO_SAMPLE_PKG_DIR%\SocketPkg\Include\Library\.
copy /y %SKT_PKG%\Include\Protocol\IioSystem.h %IIO_SAMPLE_PKG_DIR%\SocketPkg\Include\Protocol\.
copy /y %ROOTPATH%CpRcPkg\Library\BaseMemoryCoreLib\Chip\Skx\Include\Library\IioPlatformData.h %IIO_SAMPLE_PKG_DIR%\SocketPkg\Include\Library\.
copy /y %SKT_PKG%\Include\Guid\SocketIioVariable.h %IIO_SAMPLE_PKG_DIR%\SocketPkg\Include\Guid\.
xcopy /y /e /I %SKT_PKG%\Include\Iio %IIO_SAMPLE_PKG_DIR%\SocketPkg\Include\Iio\.
echo   Copying Acpi Files ...
xcopy /y /e /I %PLAT_PKG%\Acpi\AcpiTables\Dsdt %IIO_SAMPLE_PKG_DIR%\PlatformPkg\Acpi\AcpiTables\Dsdt\.
xcopy /y /e /I %PLAT_PKG_PATH%\Acpi\Dxe\AcpiPlatform %IIO_SAMPLE_PKG_DIR%\PlatformPkg\Acpi\Dxe\AcpiPlatform\.
echo   Copying Core Files ...
xcopy /y /e /I MdeModulePkg\Bus\Pci\PciBusDxe %IIO_SAMPLE_PKG_DIR%\MdeModulePkg\Pci\PciBusDxe
xcopy /y /e /I MdePkg\Library\BaseLib %IIO_SAMPLE_PKG_DIR%\MdePkg\Library\BaseLib
xcopy /y /e /I MdePkg\Library\BasePciExpressLib %IIO_SAMPLE_PKG_DIR%\MdePkg\Library\BasePciExpressLib
xcopy /y /e /I MdePkg\Library\DxeServicesTableLib %IIO_SAMPLE_PKG_DIR%\MdePkg\Library\DxeServicesTableLib
xcopy /y /e /I MdePkg\Library\BaseIoLibIntrinsic %IIO_SAMPLE_PKG_DIR%\MdePkg\Library\BaseIoLibIntrinsic
xcopy /y /e /I MdePkg\Library\BaseMemoryLib %IIO_SAMPLE_PKG_DIR%\MdePkg\Library\BaseMemoryLib
xcopy /y /e /I MdePkg\Library\UefiBootServicesTableLib %IIO_SAMPLE_PKG_DIR%\MdePkg\Library\UefiBootServicesTableLib
xcopy /y /e /I MdePkg\Library\UefiDriverEntryPoint %IIO_SAMPLE_PKG_DIR%\MdePkg\Library\UefiDriverEntryPoint
xcopy /y /e /I MdePkg\Library\DxeHobLib %IIO_SAMPLE_PKG_DIR%\MdePkg\Library\DxeHobLib
xcopy /y /e /I MdePkg\Library\BasePcdLibNull %IIO_SAMPLE_PKG_DIR%\MdePkg\Library\BasePcdLibNull
xcopy /y /e /I MdePkg\Library\BaseS3BootScriptLibNull %IIO_SAMPLE_PKG_DIR%\MdePkg\Library\BaseS3BootScriptLibNull
xcopy /y /e /I MdePkg\Library\UefiDevicePathLib %IIO_SAMPLE_PKG_DIR%\MdePkg\Library\UefiDevicePathLib
xcopy /y /e /I MdePkg\Library\SmmServicesTableLib %IIO_SAMPLE_PKG_DIR%\MdePkg\Library\SmmServicesTableLib
xcopy /y /e /I MdePkg\Library\PeimEntryPoint %IIO_SAMPLE_PKG_DIR%\MdePkg\Library\PeimEntryPoint
xcopy /y /e /I MdePkg\Library\DxeHobLib %IIO_SAMPLE_PKG_DIR%\MdePkg\Library\DxeHobLib
xcopy /y /e /I MdePkg\Library\UefiLib %IIO_SAMPLE_PKG_DIR%\MdePkg\Library\UefiLib
md %IIO_SAMPLE_PKG_DIR%\MdePkg\Include
md %IIO_SAMPLE_PKG_DIR%\MdePkg\Include\Library
xcopy /y /e /I MdePkg\Include\Library\Uefi*.h %IIO_SAMPLE_PKG_DIR%\MdePkg\Library\Include\Library\.
xcopy /y /e /I MdePkg\Include\Library\Base*.h %IIO_SAMPLE_PKG_DIR%\MdePkg\Include\Library\.
xcopy /y /e /I MdePkg\Include\Library\Dxe*.h %IIO_SAMPLE_PKG_DIR%\MdePkg\Include\Library\.
copy /y MdePkg\Include\Library\PciExpressLib.h %IIO_SAMPLE_PKG_DIR%\MdePkg\Include\Library\.
copy /y MdePkg\Include\Library\IoLib.h %IIO_SAMPLE_PKG_DIR%\MdePkg\Include\Library\.
copy /y MdePkg\Include\Library\S3BootScriptLib.h %IIO_SAMPLE_PKG_DIR%\MdePkg\Include\Library\.
copy /y MdePkg\Include\Library\DevicePathLib.h %IIO_SAMPLE_PKG_DIR%\MdePkg\Include\Library\.
copy /y MdePkg\Include\Library\SmmServicesTableLib.h %IIO_SAMPLE_PKG_DIR%\MdePkg\Include\Library\.
copy /y MdePkg\Include\Library\PeimEntryPoint.h %IIO_SAMPLE_PKG_DIR%\MdePkg\Include\Library\.
copy /y MdePkg\Include\Library\HobLib.h %IIO_SAMPLE_PKG_DIR%\MdePkg\Include\Library\.
goto :EOF

REM ===================================================
REM Clean IIO Sample Code package
REM ===================================================
:iiopkgclean
set IIO_SAMPLE_PKG_DIR=SampleCodePkg\IIO
echo *****************************************************
echo       Clean IIO Sample Code package
echo *****************************************************
attrib -r %IIO_SAMPLE_PKG_DIR%\*.* /s /d
echo   Deleting Files ...
rd /s /q %IIO_SAMPLE_PKG_DIR%
goto :EOF

REM ===================================================
REM Create CR Sample Code package
REM ===================================================
:crpkg
set CR_SAMPLE_PKG_DIR=SampleCodePkg\CR
if NOT exist %CR_SAMPLE_PKG_DIR% md SampleCodePkg
echo *****************************************************
echo       Create CR Sample Code package
echo *****************************************************
attrib -r %CR_SAMPLE_PKG_DIR%\*.* /s /d
echo   Deleting Files ...
rd /s /q %CR_SAMPLE_PKG_DIR%
echo   Copying Files ...
xcopy /y /e /I %SKT_PKG%\Dxe\CrystalRidge %CR_SAMPLE_PKG_DIR%\SocketPkg\Dxe\CrystalRidge
xcopy /y /e /I %SKT_PKG%\Pei\CrystalRidge %CR_SAMPLE_PKG_DIR%\SocketPkg\Pei\CrystalRidge
xcopy /y /e /I %SKT_PKG%\Dxe\CpuCsrAccess %CR_SAMPLE_PKG_DIR%\SocketPkg\Dxe\CpuCsrAccess
xcopy /y /e /i %PLAT_PKG%\Library\OemCrystalRidgeLib %CR_SAMPLE_PKG_DIR%\PlatformPkg\Library\OemCrystalRidgeLib
xcopy /y /e /i %PLAT_PKG%\Library\OemProcMemInitLib %CR_SAMPLE_PKG_DIR%\PlatformPkg\Library\OemProcMemInitLib
md %CR_SAMPLE_PKG_DIR%\PlatformPkg\Include
md %CR_SAMPLE_PKG_DIR%\PlatformPkg\Include\Acpi
md %CR_SAMPLE_PKG_DIR%\PlatformPkg\Acpi\AcpiTables\NFIT
md %CR_SAMPLE_PKG_DIR%\PlatformPkg\Acpi\AcpiTables\PCAT
md %CR_SAMPLE_PKG_DIR%\SocketPkg\Include
md %CR_SAMPLE_PKG_DIR%\SocketPkg\Include\Setup
md %CR_SAMPLE_PKG_DIR%\SocketPkg\Include\Library
md %CR_SAMPLE_PKG_DIR%\SocketPkg\Include\Protocol
md %CR_SAMPLE_PKG_DIR%\SocketPkg\Include\Guid
copy /y %PLATFORM_RESTRICTED_PKG%\OemRoms\IntelDCPersistentMemoryDriver.efi %CR_SAMPLE_PKG_DIR%\PlatformPkg\.
copy /y %PLATFORM_RESTRICTED_PKG%\OemRoms\IntelDCPersistentMemoryHii.efi %CR_SAMPLE_PKG_DIR%\PlatformPkg\.
copy /y %SKT_PKG%\Include\Setup\IioUniversalData.h %CR_SAMPLE_PKG_DIR%\SocketPkg\Include\Setup\.
copy /y %PLAT_PKG%\Include\Platform.h %CR_SAMPLE_PKG_DIR%\PlatformPkg\Include\.
copy /y %PLAT_PKG%\Include\Acpi\Nfit.h %CR_SAMPLE_PKG_DIR%\PlatformPkg\Include\Acpi\.
copy /y %PLAT_PKG%\Include\Acpi\Pcat.h %CR_SAMPLE_PKG_DIR%\PlatformPkg\Include\Acpi\.
copy /y %PLAT_PKG%\Acpi\AcpiTables\NFIT\Nfit.aslc %CR_SAMPLE_PKG_DIR%\PlatformPkg\Acpi\AcpiTables\NFIT\.
copy /y %PLAT_PKG%\Acpi\AcpiTables\PCAT\Pcat.aslc %CR_SAMPLE_PKG_DIR%\PlatformPkg\Acpi\AcpiTables\PCAT\.
copy /y %SKT_PKG%\Include\Protocol\CrystalRidge.h %CR_SAMPLE_PKG_DIR%\SocketPkg\Include\Protocol\.
echo   Copying Acpi Files ...
xcopy /y /e /I %PLAT_PKG%\Acpi\AcpiTables\Dsdt %CR_SAMPLE_PKG_DIR%\PlatformPkg\Acpi\AcpiTables\Dsdt\.
xcopy /y /e /I %PLAT_PKG_PATH%\Acpi\Dxe\AcpiPlatform %CR_SAMPLE_PKG_DIR%\PlatformPkg\Acpi\Dxe\AcpiPlatform\.
echo   Copying Core Files ...
xcopy /y /e /I MdePkg\Library\BaseLib %CR_SAMPLE_PKG_DIR%\MdePkg\Library\BaseLib
xcopy /y /e /I MdePkg\Library\BasePciExpressLib %CR_SAMPLE_PKG_DIR%\MdePkg\Library\BasePciExpressLib
xcopy /y /e /I MdePkg\Library\DxeServicesTableLib %CR_SAMPLE_PKG_DIR%\MdePkg\Library\DxeServicesTableLib
xcopy /y /e /I MdePkg\Library\BaseIoLibIntrinsic %CR_SAMPLE_PKG_DIR%\MdePkg\Library\BaseIoLibIntrinsic
xcopy /y /e /I MdePkg\Library\BaseMemoryLib %CR_SAMPLE_PKG_DIR%\MdePkg\Library\BaseMemoryLib
xcopy /y /e /I MdePkg\Library\UefiBootServicesTableLib %CR_SAMPLE_PKG_DIR%\MdePkg\Library\UefiBootServicesTableLib
xcopy /y /e /I MdePkg\Library\UefiDriverEntryPoint %CR_SAMPLE_PKG_DIR%\MdePkg\Library\UefiDriverEntryPoint
xcopy /y /e /I MdePkg\Library\DxeHobLib %CR_SAMPLE_PKG_DIR%\MdePkg\Library\DxeHobLib
xcopy /y /e /I MdePkg\Library\BasePcdLibNull %CR_SAMPLE_PKG_DIR%\MdePkg\Library\BasePcdLibNull
xcopy /y /e /I MdePkg\Library\BaseS3BootScriptLibNull %CR_SAMPLE_PKG_DIR%\MdePkg\Library\BaseS3BootScriptLibNull
xcopy /y /e /I MdePkg\Library\UefiDevicePathLib %CR_SAMPLE_PKG_DIR%\MdePkg\Library\UefiDevicePathLib
xcopy /y /e /I MdePkg\Library\SmmServicesTableLib %CR_SAMPLE_PKG_DIR%\MdePkg\Library\SmmServicesTableLib
xcopy /y /e /I MdePkg\Library\PeimEntryPoint %CR_SAMPLE_PKG_DIR%\MdePkg\Library\PeimEntryPoint
xcopy /y /e /I MdePkg\Library\DxeHobLib %CR_SAMPLE_PKG_DIR%\MdePkg\Library\DxeHobLib
xcopy /y /e /I MdePkg\Library\UefiLib %CR_SAMPLE_PKG_DIR%\MdePkg\Library\UefiLib
md %CR_SAMPLE_PKG_DIR%\MdePkg\Include
md %CR_SAMPLE_PKG_DIR%\MdePkg\Include\Library
xcopy /y /e /I MdePkg\Include\Library\Uefi*.h %CR_SAMPLE_PKG_DIR%\MdePkg\Library\Include\Library\.
xcopy /y /e /I MdePkg\Include\Library\Base*.h %CR_SAMPLE_PKG_DIR%\MdePkg\Include\Library\.
xcopy /y /e /I MdePkg\Include\Library\Dxe*.h %CR_SAMPLE_PKG_DIR%\MdePkg\Include\Library\.
copy /y MdePkg\Include\Library\PciExpressLib.h %CR_SAMPLE_PKG_DIR%\MdePkg\Include\Library\.
copy /y MdePkg\Include\Library\IoLib.h %CR_SAMPLE_PKG_DIR%\MdePkg\Include\Library\.
copy /y MdePkg\Include\Library\S3BootScriptLib.h %CR_SAMPLE_PKG_DIR%\MdePkg\Include\Library\.
copy /y MdePkg\Include\Library\DevicePathLib.h %CR_SAMPLE_PKG_DIR%\MdePkg\Include\Library\.
copy /y MdePkg\Include\Library\SmmServicesTableLib.h %CR_SAMPLE_PKG_DIR%\MdePkg\Include\Library\.
copy /y MdePkg\Include\Library\PeimEntryPoint.h %CR_SAMPLE_PKG_DIR%\MdePkg\Include\Library\.
copy /y MdePkg\Include\Library\HobLib.h %CR_SAMPLE_PKG_DIR%\MdePkg\Include\Library\.
copy /y MdePkg\Include\PiDxe.h %CR_SAMPLE_PKG_DIR%\MdePkg\Include\.
goto :EOF

REM ===================================================
REM Clean CR Sample Code package
REM ===================================================
:crpkgclean
set CR_SAMPLE_PKG_DIR=SampleCodePkg\CR
echo *****************************************************
echo       Clean CR Sample Code package
echo *****************************************************
attrib -r %CR_SAMPLE_PKG_DIR%\*.* /s /d
echo   Deleting Files ...
rd /s /q %CR_SAMPLE_PKG_DIR%
goto :EOF

REM ===================================================
REM Create PCH Sample Code package
REM ===================================================
:pchpkg
set PCH_SAMPLE_PKG_DIR=SampleCodePkg\PCH
if NOT exist %PCH_SAMPLE_PKG_DIR% md SampleCodePkg
echo *****************************************************
echo       Create PCH Sample Code package
echo *****************************************************
attrib -r %PCH_SAMPLE_PKG_DIR%\*.* /s /d
echo   Deleting Files ...
rd /s /q %PCH_SAMPLE_PKG_DIR%
echo   Copying Files ...
xcopy /y /e /I %PCH_PKG%\*.* %PCH_SAMPLE_PKG_DIR%\LewisburgPkg
echo   Copying Platform Files ...
md %PCH_SAMPLE_PKG_DIR%\PlatformPkg
md %PCH_SAMPLE_PKG_DIR%\PlatformPkg\Library\Include
md %PCH_SAMPLE_PKG_DIR%\PlatformPkg\Guid\Include
copy /y %PLAT_PKG%\Include\Library\PchPlatformPolicyInitCommonLib.h %PCH_SAMPLE_PKG_DIR%\PlatformPkg\Library\Include\.
copy /y %PLAT_PKG%\Include\Library\PchPlatformPolicyInitDxeLib.h %PCH_SAMPLE_PKG_DIR%\PlatformPkg\Library\Include\.
copy /y %PLAT_PKG%\Include\Library\PchPlatformPolicyInitPeiLib.h %PCH_SAMPLE_PKG_DIR%\PlatformPkg\Library\Include\.
copy /y %PLAT_PKG%\Include\Guid\SetupVariable.h %PCH_SAMPLE_PKG_DIR%\PlatformPkg\Guid\Include.
echo   Copying Acpi Files ...
xcopy /y /e /I %PLAT_PKG%\Acpi\AcpiTables\Dsdt %CPUPM_SAMPLE_PKG_DIR%\PlatformPkg\Acpi\AcpiTables\Dsdt
xcopy /y /e /I %PLAT_PKG%\Acpi\Dxe\AcpiPlatform %CPUPM_SAMPLE_PKG_DIR%\PlatformPkg\Acpi\AcpiPlatform
echo   Copying Core Files ...
xcopy /y /e /I MdePkg\Library\BaseLib %PCH_SAMPLE_PKG_DIR%\MdePkg\Library\BaseLib
xcopy /y /e /I MdePkg\Library\BasePciExpressLib %PCH_SAMPLE_PKG_DIR%\MdePkg\Library\BasePciExpressLib
xcopy /y /e /I MdePkg\Library\UefiMemoryAllocationLib %PCH_SAMPLE_PKG_DIR%\MdePkg\Library\UefiMemoryAllocationLib
xcopy /y /e /I MdePkg\Library\DxeServicesTableLib %PCH_SAMPLE_PKG_DIR%\MdePkg\Library\DxeServicesTableLib
xcopy /y /e /I MdePkg\Library\BaseIoLibIntrinsic %PCH_SAMPLE_PKG_DIR%\MdePkg\Library\BaseIoLibIntrinsic
xcopy /y /e /I MdePkg\Library\BaseMemoryLib %PCH_SAMPLE_PKG_DIR%\MdePkg\Library\BaseMemoryLib
xcopy /y /e /I MdePkg\Library\UefiBootServicesTableLib %PCH_SAMPLE_PKG_DIR%\MdePkg\Library\UefiBootServicesTableLib
xcopy /y /e /I MdePkg\Library\UefiDriverEntryPoint %PCH_SAMPLE_PKG_DIR%\MdePkg\Library\UefiDriverEntryPoint
xcopy /y /e /I MdePkg\Library\UefiRuntimeServicesTableLib %PCH_SAMPLE_PKG_DIR%\MdePkg\Library\UefiRuntimeServicesTableLib
xcopy /y /e /I MdePkg\Library\UefiRuntimeLib %PCH_SAMPLE_PKG_DIR%\MdePkg\Library\UefiRuntimeLib
xcopy /y /e /I MdePkg\Library\DxeHobLib %PCH_SAMPLE_PKG_DIR%\MdePkg\Library\DxeHobLib
xcopy /y /e /I MdePkg\Library\BasePcdLibNull %PCH_SAMPLE_PKG_DIR%\MdePkg\Library\BasePcdLibNull
xcopy /y /e /I MdePkg\Library\BaseS3BootScriptLibNull %PCH_SAMPLE_PKG_DIR%\MdePkg\Library\BaseS3BootScriptLibNull
xcopy /y /e /I MdePkg\Library\UefiDevicePathLib %PCH_SAMPLE_PKG_DIR%\MdePkg\Library\UefiDevicePathLib
xcopy /y /e /I MdePkg\Library\SmmServicesTableLib %PCH_SAMPLE_PKG_DIR%\MdePkg\Library\SmmServicesTableLib
xcopy /y /e /I MdePkg\Library\PeimEntryPoint %PCH_SAMPLE_PKG_DIR%\MdePkg\Library\PeimEntryPoint
xcopy /y /e /I MdePkg\Library\PeiServicesLib %PCH_SAMPLE_PKG_DIR%\MdePkg\Library\PeiServicesLib
xcopy /y /e /I MdePkg\Library\DxeHobLib %PCH_SAMPLE_PKG_DIR%\MdePkg\Library\DxeHobLib
xcopy /y /e /I MdePkg\Library\UefiLib %PCH_SAMPLE_PKG_DIR%\MdePkg\Library\UefiLib
xcopy /y /e /I MdeModulePkg\Library\DebugAgentLibNull %PCH_SAMPLE_PKG_DIR%\MdeModulePkg\Library\DebugAgentLibNull
xcopy /y /e /I MdePkg\Include\Library\Uefi*.h %PCH_SAMPLE_PKG_DIR%\MdePkg\Library\Include\.
xcopy /y /e /I MdePkg\Include\Library\Base*.h %PCH_SAMPLE_PKG_DIR%\MdePkg\Library\Include\.
xcopy /y /e /I MdePkg\Include\Library\Dxe*.h %PCH_SAMPLE_PKG_DIR%\MdePkg\Library\Include\.
md %PCH_SAMPLE_PKG_DIR%\MdePkg\Include\Library
md %PCH_SAMPLE_PKG_DIR%\MdeModulePkg\Include\Library
copy /y MdePkg\Include\Library\PciExpressLib.h %PCH_SAMPLE_PKG_DIR%\MdePkg\Include\Library\.
copy /y MdePkg\Include\Library\IoLib.h %PCH_SAMPLE_PKG_DIR%\MdePkg\Include\Library\.
copy /y MdePkg\Include\Library\S3BootScriptLib.h %PCH_SAMPLE_PKG_DIR%\MdePkg\Include\Library\.
copy /y MdePkg\Include\Library\DevicePathLib.h %PCH_SAMPLE_PKG_DIR%\MdePkg\Include\Library\.
copy /y MdePkg\Include\Library\SmmServicesTableLib.h %PCH_SAMPLE_PKG_DIR%\MdePkg\Include\Library\.
copy /y MdePkg\Include\Library\PeimEntryPoint.h %PCH_SAMPLE_PKG_DIR%\MdePkg\Include\Library\.
copy /y MdePkg\Include\Library\HobLib.h %PCH_SAMPLE_PKG_DIR%\MdePkg\Include\Library\.
copy /y MdeModulePkg\Include\Library\DebugAgentLib.h %PCH_SAMPLE_PKG_DIR%\MdeModulePkg\Include\Library\.
goto :EOF

REM ===================================================
REM Clean PCH Sample Code package
REM ===================================================
:pchpkgclean
set PCH_SAMPLE_PKG_DIR=SampleCodePkg\PCH
echo *****************************************************
echo       Clean PCH Sample Code package
echo *****************************************************
attrib -r %PCH_SAMPLE_PKG_DIR%\*.* /s /d
echo   Deleting Files ...
rd /s /q %PCH_SAMPLE_PKG_DIR%
goto :EOF

REM ===================================================
REM Clean PCH Sample Code package
REM ===================================================
:pchpkgclean
set PCH_SAMPLE_PKG_DIR=SampleCodePkg\PCH
echo *****************************************************
echo       Clean PCH Sample Code package
echo *****************************************************
attrib -r %PCH_SAMPLE_PKG_DIR%\*.* /s /d
echo   Deleting Files ...
rd /s /q %PCH_SAMPLE_PKG_DIR%
goto :EOF

:outputError
echo %1
goto :EOF

:done
:skipBuildSummary
if exist %BUILD_SUMMARY_LOG% del %BUILD_SUMMARY_LOG%

endlocal & EXIT /b %SCRIPT_ERROR%
