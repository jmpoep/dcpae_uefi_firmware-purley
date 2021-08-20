;
; This file contains an 'Intel Peripheral Driver' and is
; licensed for Intel CPUs and chipsets under the terms of your
; license agreement with Intel or your vendor.  This file may
; be modified by the user, subject to additional terms of the
; license agreement
;
;------------------------------------------------------------------------------
;
; Copyright (c) 2016, Intel Corporation. All rights reserved.<BR>
; This software and associated documentation (if any) is furnished
; under a license and may only be used or copied in accordance
; with the terms of the license. Except as permitted by such
; license, no part of this software or documentation may be
; reproduced, stored in a retrieval system, or transmitted in any
; form or by any means without the express written consent of
; Intel Corporation.
;
; Module Name:
;
;   SmmInit.nasm
;
; Abstract:
;
;   Functions for relocating SMBASE's for all processors
;
; Notes:
;
;------------------------------------------------------------------------------

extern ASM_PFX(SmmInitHandler)
extern ASM_PFX(mRebasedFlag)
extern ASM_PFX(mSmmRelocationOriginalAddress)

global ASM_PFX(gSmmCr3)
global ASM_PFX(gSmmCr4)
global ASM_PFX(gSmmCr0)
global ASM_PFX(gSmmJmpAddr)
global ASM_PFX(gSmmInitStack)
global ASM_PFX(gcSmiInitGdtr)
global ASM_PFX(gcSmmInitSize)
global ASM_PFX(gcSmmInitTemplate)

%define PROTECT_MODE_CS 0x8
%define PROTECT_MODE_DS 0x20

    SECTION .text

ASM_PFX(gcSmiInitGdtr):
            DW      0
            DQ      0

global ASM_PFX(SmmStartup)
ASM_PFX(SmmStartup):
    DB      0x66, 0xb8
ASM_PFX(gSmmCr3): DD 0
    mov     cr3, eax
    DB      0x67, 0x66
    lgdt    [cs:ebp + (ASM_PFX(gcSmiInitGdtr) - ASM_PFX(SmmStartup))]
    DB      0x66, 0xb8
ASM_PFX(gSmmCr4): DD 0
    mov     cr4, eax
    DB      0x66, 0xb8
ASM_PFX(gSmmCr0): DD 0
    DB      0xbf, PROTECT_MODE_DS, 0    ; mov di, PROTECT_MODE_DS
    mov     cr0, eax
    DB      0x66, 0xea                   ; jmp far [ptr48]
ASM_PFX(gSmmJmpAddr):
    DD      @32bit
    DW      PROTECT_MODE_CS
@32bit:
    mov     ds, edi
    mov     es, edi
    mov     fs, edi
    mov     gs, edi
    mov     ss, edi
    DB      0xbc                        ; mov esp, imm32
ASM_PFX(gSmmInitStack): DD 0
    call    ASM_PFX(SmmInitHandler)
    rsm

BITS 16
ASM_PFX(gcSmmInitTemplate):
    mov ebp, ASM_PFX(SmmStartup)
    sub ebp, 0x30000
    jmp ebp

ASM_PFX(gcSmmInitSize): DW $ - ASM_PFX(gcSmmInitTemplate)

BITS 32
global ASM_PFX(SmmRelocationSemaphoreComplete)
ASM_PFX(SmmRelocationSemaphoreComplete):
    push    eax
    mov     eax, [ASM_PFX(mRebasedFlag)]
    mov     byte [eax], 1
    pop     eax
    jmp     [ASM_PFX(mSmmRelocationOriginalAddress)]
