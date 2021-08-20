/** @file
  PCH ADR setup library

@copyright
 Copyright (c) 2012-2018 Intel Corporation. All rights reserved
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

#include <Uefi/UefiBaseType.h>
#include <Library/DebugLib.h>
#include <Library/MmPciBaseLib.h>
#include <Library/IoLib.h>
#include <Library/PchCycleDecodingLib.h>
#include <PchAccess.h>
#include <Library/PchAdrLib.h>
#include "PchAdrConfig.h"


/**
  Configures ADR in the PMC

  @param PchAdrConfig  PCH ADR configuration structure

**/
VOID
PchAdrConfigurePmc (
  PCH_ADR_CONFIG *PchAdrConfig
  )
{
  UINT32 PchPwrmBase;
  EFI_STATUS Status;

  Status = PchPwrmBaseGet (&PchPwrmBase);
  if (EFI_ERROR (Status)) {
    ASSERT (FALSE);
    return;
  }

  /// Configure PM_SYNC in serial mode 0/31/2 A8h bit 9 = 1
  MmioOr32 (
    PchPwrmBase + R_PCH_PMC_BM_CX_CNF,
    B_PCH_PMC_BM_CX_CNF_PM_SYNC_MSG_MODE
  );

  if (PchAdrConfig->AdrGpioSel == PM_SYNC_GPIO_B) {
    /// Program PWRMBASE C8h [10:9] = 1b, to map CPU_GP_0 to GPIO_B
    MmioAndThenOr32 (
      PchPwrmBase + R_PCH_PWRM_PMSYNC_MISC_CFG,
      ~(UINT32)(B_PCH_PWRM_PMSYNC_GPIO_C_SEL),
      B_PCH_PWRM_PMSYNC_GPIO_B_SEL
    );

    /// Program PWRMBASE F0h [31:30] to 01b to select GPIO_B as ADR GPIO (Disable timer if requested by SV)
    MmioAndThenOr32 (
      PchPwrmBase + R_PCH_PWRM_ADR_EN,
      ~(B_PCH_PWRM_ADR_EN_ADR_GPIO_SEL_MASK),
      B_PCH_PWRM_ADR_EN_ADR_GPIO_SEL_GPIO_B
    );

  } else if (PchAdrConfig->AdrGpioSel == PM_SYNC_GPIO_C) {
    /// Program PWRMBASE C8h [10:9] = 0b, to map CPU_GP_0 to GPIO_C
    MmioAnd32 (
      PchPwrmBase + R_PCH_PWRM_PMSYNC_MISC_CFG,
      ~(UINT32)(B_PCH_PWRM_PMSYNC_GPIO_C_SEL | B_PCH_PWRM_PMSYNC_GPIO_B_SEL)
    );

    /// Program PWRMBASE F0h [31:30] to 10b to select GPIO_C as ADR GPIO (Disable timer if requested by SV)
    MmioAndThenOr32 (
      PchPwrmBase + R_PCH_PWRM_ADR_EN,
      ~(B_PCH_PWRM_ADR_EN_ADR_GPIO_SEL_MASK),
      B_PCH_PWRM_ADR_EN_ADR_GPIO_SEL_GPIO_C
    );
  }
}

/**
  Configures ADR sources in the PMC

  @param PchAdrConfig  PCH ADR configuration structure
  @param SrcConfigMask PCH ADR sources configuration type mask

**/
VOID
PchAdrConfigureSources (
  PCH_ADR_CONFIG *PchAdrConfig,
  UINT32 SrcConfigMask
  )
{
  UINT32 PchPwrmBase;
  EFI_STATUS Status;
  UINT32 PwrmAdrEnOrMask = 0;
  UINT32 PwrmAdrEnAndMask = ~0U;
  BOOLEAN HprAdrEnable;

  Status = PchPwrmBaseGet (&PchPwrmBase);
  if (EFI_ERROR (Status)) {
    ASSERT (FALSE);
    return;
  }

  if((SrcConfigMask & PCH_ADR_SOURCE_DDR) && !(SrcConfigMask & PCH_ADR_SOURCE_DDRT)) {
    // Program sources (DDR only)

    if (PchAdrConfig->AdrHostPartitionReset != PLATFORM_POR) {
      HprAdrEnable = PchAdrConfig->AdrHostPartitionReset == FORCE_ENABLE;
    } else {
      // POR value for DDR only
      HprAdrEnable = FALSE;
    }

    if (HprAdrEnable) {
      PwrmAdrEnOrMask |= B_PCH_PWRM_ADR_EN_HPR_ADR_EN;
    } else {
      PwrmAdrEnAndMask &= ~(UINT32)(B_PCH_PWRM_ADR_EN_HPR_ADR_EN);
    }

  } else if ((SrcConfigMask & PCH_ADR_SOURCE_DDR) && (SrcConfigMask & PCH_ADR_SOURCE_DDRT)) {
    // Program sources (DDR + DDRT)

    if (PchAdrConfig->AdrHostPartitionReset != PLATFORM_POR) {
      HprAdrEnable = PchAdrConfig->AdrHostPartitionReset == FORCE_ENABLE;
    } else {
      // POR value for DDR + DDRT only
      HprAdrEnable = TRUE;
    }

    if (HprAdrEnable) {
      PwrmAdrEnOrMask |= B_PCH_PWRM_ADR_EN_HPR_ADR_EN;
    } else {
      PwrmAdrEnAndMask &= ~(UINT32)(B_PCH_PWRM_ADR_EN_HPR_ADR_EN);
    }
    if (PchAdrConfig->AdrSysPwrOk == FORCE_ENABLE) {
      PwrmAdrEnOrMask |= B_PCH_PWRM_ADR_EN_SYSPWR_ADR_EN;
    } else {
      PwrmAdrEnAndMask &= ~(UINT32)(B_PCH_PWRM_ADR_EN_SYSPWR_ADR_EN);
    }
    if (PchAdrConfig->AdrOverClockingWdt == FORCE_ENABLE) {
      PwrmAdrEnOrMask |= B_PCH_PWRM_ADR_EN_OCWDT_ADR_EN;
    } else {
      PwrmAdrEnAndMask &= ~(UINT32)(B_PCH_PWRM_ADR_EN_OCWDT_ADR_EN);
    }
    if (PchAdrConfig->AdrCpuThermalWdt == FORCE_ENABLE) {
      PwrmAdrEnOrMask |= B_PCH_PWRM_ADR_EN_CTWDT_ADR_EN;
    } else {
      PwrmAdrEnAndMask &= ~(UINT32)(B_PCH_PWRM_ADR_EN_CTWDT_ADR_EN);
    }
    if (PchAdrConfig->AdrPmcParityError == FORCE_ENABLE) {
      PwrmAdrEnOrMask |= B_PCH_PWRM_ADR_EN_PMC_PARERR_ADR_EN;
    } else {
      PwrmAdrEnAndMask &= ~(UINT32)(B_PCH_PWRM_ADR_EN_PMC_PARERR_ADR_EN);
    }
  }

  MmioAndThenOr32 (
    PchPwrmBase + R_PCH_PWRM_ADR_EN,
    PwrmAdrEnAndMask,
    PwrmAdrEnOrMask
  );
}

/**
  Configures ADR timer in EVA/MROM0.

  @param PchAdrConfig  PCH ADR configuration structure
  @param TimerVal      Timer value in microseconds (us).
                       A finite set of values is supported (see PCH_ADR_TIMER_ macros).
**/
VOID
PchAdrConfigureTimer (
  PCH_ADR_CONFIG *PchAdrConfig,
  UINT32 TimerVal
  )
{
  UINTN EvaMmIo;
  UINT32 AdrTimer;
  UINT8 AdrTimerVal;
  UINT8 AdrMultiplierVal;

  ASSERT (PchAdrConfig->AdrTimerEn != PLATFORM_POR);

  switch (TimerVal) {
    case PCH_ADR_TIMER_0US:
      AdrTimerVal = V_PCH_LBG_MROM_ADRTIMERCTRL_ADR_TMR_0US;
      AdrMultiplierVal = V_PCH_LBG_MROM_ADRTIMERCTRL_ADR_MULT_1;
      break;
    case PCH_ADR_TIMER_25US:
      AdrTimerVal = V_PCH_LBG_MROM_ADRTIMERCTRL_ADR_TMR_25US;
      AdrMultiplierVal = V_PCH_LBG_MROM_ADRTIMERCTRL_ADR_MULT_1;
      break;
    case PCH_ADR_TIMER_50US:
      AdrTimerVal = V_PCH_LBG_MROM_ADRTIMERCTRL_ADR_TMR_50US;
      AdrMultiplierVal = V_PCH_LBG_MROM_ADRTIMERCTRL_ADR_MULT_1;
      break;
    case PCH_ADR_TIMER_100US:
      AdrTimerVal = V_PCH_LBG_MROM_ADRTIMERCTRL_ADR_TMR_100US;
      AdrMultiplierVal = V_PCH_LBG_MROM_ADRTIMERCTRL_ADR_MULT_1;
      break;
    case PCH_ADR_TIMER_400US:
      AdrTimerVal = V_PCH_LBG_MROM_ADRTIMERCTRL_ADR_TMR_50US;
      AdrMultiplierVal = V_PCH_LBG_MROM_ADRTIMERCTRL_ADR_MULT_8;
      break;
    case PCH_ADR_TIMER_600US:
      AdrTimerVal = V_PCH_LBG_MROM_ADRTIMERCTRL_ADR_TMR_25US;
      AdrMultiplierVal = V_PCH_LBG_MROM_ADRTIMERCTRL_ADR_MULT_24;
      break;
    case PCH_ADR_TIMER_1200US:
      AdrTimerVal = V_PCH_LBG_MROM_ADRTIMERCTRL_ADR_TMR_50US;
      AdrMultiplierVal = V_PCH_LBG_MROM_ADRTIMERCTRL_ADR_MULT_24;
      break;
    case PCH_ADR_TIMER_2400US:
      AdrTimerVal = V_PCH_LBG_MROM_ADRTIMERCTRL_ADR_TMR_100US;
      AdrMultiplierVal = V_PCH_LBG_MROM_ADRTIMERCTRL_ADR_MULT_24;
      break;
    case PCH_ADR_TIMER_9600US:
      AdrTimerVal = V_PCH_LBG_MROM_ADRTIMERCTRL_ADR_TMR_100US;
      AdrMultiplierVal = V_PCH_LBG_MROM_ADRTIMERCTRL_ADR_MULT_96;
      break;
    default:
      DEBUG ((DEBUG_ERROR, "PCH ADR config: ERROR: unsupported ADR timer value (%d)\n", TimerVal));
      AdrTimerVal = V_PCH_LBG_MROM_ADRTIMERCTRL_ADR_TMR_100US;
      AdrMultiplierVal = V_PCH_LBG_MROM_ADRTIMERCTRL_ADR_MULT_1;
      ASSERT (FALSE);
  }

  if (!(PchAdrConfig->AdrTimerVal == ADR_TMR_SETUP_DEFAULT_POR || PchAdrConfig->AdrMultiplierVal == ADR_MULT_SETUP_DEFAULT_POR)) {
    AdrTimerVal = PchAdrConfig->AdrTimerVal;
    AdrMultiplierVal = PchAdrConfig->AdrMultiplierVal;
  }

  EvaMmIo = MmPciBase( DEFAULT_PCI_BUS_NUMBER_PCH,
                       PCI_DEVICE_NUMBER_EVA,
                       PCI_FUNCTION_NUMBER_EVA_MROM0);
  AdrTimer = MmioRead32 (EvaMmIo + R_PCH_LBG_MROM_ADRTIMERCTRL);

  //
  // Only disable timer if ADR_DBG_DIS is 1
  //
  if ((AdrTimer & B_PCH_LBG_MROM_ADRTIMERCTRL_ADR_DBG_DIS) == 0 && PchAdrConfig->AdrTimerEn == ADR_TMR_HELD_OFF_SETUP_OPTION) {
    DEBUG ((DEBUG_WARN, "PCH ADR config: WARNING! ADR timer HELD OFF INDEFINITELY! FOR DEBUG PURPOSES ONLY!\n" ));
    MmioOr32 (
      EvaMmIo + R_PCH_LBG_MROM_ADRTIMERCTRL,
      B_PCH_LBG_MROM_ADRTIMERCTRL_ADR_TMR_DIS
    );
  } else {
    // Set the selected timer value
    MmioAndThenOr32 (
      EvaMmIo + R_PCH_LBG_MROM_ADRTIMERCTRL,
      ~B_PCH_LBG_MROM_ADRTIMERCTRL_ADR_TMR_MASK,
      AdrTimerVal << N_PCH_LBG_MROM_ADRTIMERCTRL_ADR_TMR
    );
    // Set the selected timer multiplier
    MmioAndThenOr32 (
      EvaMmIo + R_PCH_LBG_MROM_ADRTIMERCTRL,
      ~(UINT32)B_PCH_LBG_MROM_ADRTIMERCTRL_ADR_MULT_MASK,
      (AdrMultiplierVal << N_PCH_LBG_MROM_ADRTIMERCTRL_ADR_MULT)
    );
  }
}

/**
  PCH ADR Enable master switch.

  Enables/disables PCH-side ADR.
  This function must always be called by MRC even if ADR is not being enabled (with Enable parameter equal to FALSE).
  This is to allow PCH DFX overrides to work.

  @param Enable TRUE:  Enables PCH-side ADR
                FALSE: Disables PCH-side ADR
**/
VOID
PchAdrEnableAdr (
  BOOLEAN Enable
  )
{
  PCH_ADR_CONFIG *PchAdrConfig;
  EFI_STATUS     Status;
  UINT32         PchPwrmBase;
  BOOLEAN        AdrEnable;

  PchAdrGetConfig (&PchAdrConfig);

  AdrEnable = Enable;
  if (PchAdrConfig->PchAdrEn != PLATFORM_POR) {
    AdrEnable = PchAdrConfig->PchAdrEn == FORCE_ENABLE;
  }

  Status = PchPwrmBaseGet (&PchPwrmBase);
  if (EFI_ERROR (Status)) {
    ASSERT (FALSE);
    return;
  }

  if (AdrEnable) {
    MmioOr32 (
      PchPwrmBase + R_PCH_PWRM_ADR_EN,
      B_PCH_PWRM_ADR_EN_ADR_FEAT_EN
    );
    if (PchAdrConfig->AdrGpioSel == PM_SYNC_GPIO_B) {
      /// Program PWRMBASE D4h [13] = 1b for GPIO_B to PM_SYNC Enable
      MmioOr32 (
        PchPwrmBase + R_PCH_PWRM_PM_SYNC_MODE,
        B_PCH_PWRM_PM_SYNC_MODE_GPIO_B
      );

      /// Program PWRMBASE F4h [13] = 1b for GPIO_B to PM_SYNC Enable in C0
      MmioOr32 (
        PchPwrmBase + R_PCH_PWRM_PM_SYNC_MODE_C0,
        B_PCH_PWRM_PM_SYNC_MODE_GPIO_B
      );
    } else if (PchAdrConfig->AdrGpioSel == PM_SYNC_GPIO_C) {
      /// Program PWRMBASE D4h [14] = 1b for GPIO_C to PM_SYNC Enable
      MmioOr32 (
        PchPwrmBase + R_PCH_PWRM_PM_SYNC_MODE,
        B_PCH_PWRM_PM_SYNC_MODE_GPIO_C
      );

      /// Program PWRMBASE F4h [14] = 1b for GPIO_C to PM_SYNC Enable in C0
      MmioOr32 (
        PchPwrmBase + R_PCH_PWRM_PM_SYNC_MODE_C0,
        B_PCH_PWRM_PM_SYNC_MODE_GPIO_C
      );
    }
  } else {
    MmioAnd32 (
      PchPwrmBase + R_PCH_PWRM_ADR_EN,
      ~(UINT32)(B_PCH_PWRM_ADR_EN_ADR_FEAT_EN)
    );
    if (PchAdrConfig->AdrGpioSel == PM_SYNC_GPIO_B) {
      /// Program PWRMBASE D4h [13] = 0b for GPIO_B to PM_SYNC Disable
      MmioAnd32 (
        PchPwrmBase + R_PCH_PWRM_PM_SYNC_MODE,
        ~(UINT32)(B_PCH_PWRM_PM_SYNC_MODE_GPIO_B)
      );

      /// Program PWRMBASE F4h [13] = 0b for GPIO_B to PM_SYNC Disable in C0
      MmioAnd32 (
        PchPwrmBase + R_PCH_PWRM_PM_SYNC_MODE_C0,
        ~(UINT32)(B_PCH_PWRM_PM_SYNC_MODE_GPIO_B)
      );
    } else if (PchAdrConfig->AdrGpioSel == PM_SYNC_GPIO_C) {
      /// Program PWRMBASE D4h [14] = 0b for GPIO_C to PM_SYNC Disable
      MmioAnd32 (
        PchPwrmBase + R_PCH_PWRM_PM_SYNC_MODE,
        ~(UINT32)(B_PCH_PWRM_PM_SYNC_MODE_GPIO_C)
      );

      /// Program PWRMBASE F4h [14] = 0b for GPIO_C to PM_SYNC Disable in C0
      MmioAnd32 (
        PchPwrmBase + R_PCH_PWRM_PM_SYNC_MODE_C0,
        ~(UINT32)(B_PCH_PWRM_PM_SYNC_MODE_GPIO_C)
      );
    }
  }
}

/**
  Configures PCH-side ADR.

  This function must always be called by MRC even if ADR is not being enabled.
  This is to allow PCH DFX overrides to work.

  @param ConfigMask    PCH ADR configuration mask - indicates what needs to be configured (PMC/Timer/Sources)
  @param TimerVal      Timer value in microseconds (us).
                       A finite set of values is supported (see PCH_ADR_TIMER_ macros).
  @param SrcConfigMask PCH ADR sources configuration type mask

**/
VOID
PchAdrConfigure (
  UINT32 ConfigMask,
  UINT32 TimerVal,
  UINT32 SrcConfigMask
  )
{
  PCH_ADR_CONFIG *PchAdrConfig;

  PchAdrGetConfig (&PchAdrConfig);

  if (ConfigMask & PCH_ADR_CFG_PMC) {
    PchAdrConfigurePmc (PchAdrConfig);
  }
  if (ConfigMask & PCH_ADR_CFG_SRC) {
    PchAdrConfigureSources (PchAdrConfig, SrcConfigMask);
  }
  if (ConfigMask & PCH_ADR_CFG_TIMER) {
    PchAdrConfigureTimer (PchAdrConfig, TimerVal);
  }
}

/**
  Clears PCH ADR Reset Status
**/
VOID
PchAdrClearAdrResetStatus (
  VOID
  )
{
  EFI_STATUS Status;
  UINT32     PchPwrmBase;

  Status = PchPwrmBaseGet (&PchPwrmBase);
  if (EFI_ERROR (Status)) {
    ASSERT (FALSE);
    return;
  }

  MmioWrite32 (PchPwrmBase + R_PCH_PWRM_PCH_PM_STS, B_PCH_PWRM_PCH_PM_STS_ADR_RST_STS);
}

/**
  Gets PCH ADR Reset Status

  @retval TRUE  ADR flow was executed during a prior global reset entry.
  @retval FALSE ADR flow was *not* executed during a prior global reset entry.

**/
BOOLEAN
PchAdrGetAdrResetStatus (
  VOID
  )
{
  EFI_STATUS Status;
  UINT32     PchPwrmBase;

  Status = PchPwrmBaseGet (&PchPwrmBase);
  if (EFI_ERROR (Status)) {
    ASSERT (FALSE);
    return FALSE;
  }
  return !!(MmioRead32 (PchPwrmBase + R_PCH_PWRM_PCH_PM_STS) & B_PCH_PWRM_PCH_PM_STS_ADR_RST_STS);
}
