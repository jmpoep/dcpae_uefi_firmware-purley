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
;   SmmInit.Asm
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
global ASM_PFX(mRebasedFlagAddr32)
global ASM_PFX(mSmmRelocationOriginalAddressPtr32)

    DEFAULT REL
    SECTION .text

ASM_PFX(gcSmiInitGdtr):
            DW      0
            DQ      0

global ASM_PFX(SmmStartup)
ASM_PFX(SmmStartup):
    DB      0x66, 0xb8                   ; mov eax, imm32
ASM_PFX(gSmmCr3): DD 0
    mov     cr3, rax
    DB      0x66, 0x2e
    lgdt    [ebp + (ASM_PFX(gcSmiInitGdtr) - ASM_PFX(SmmStartup))]
    DB      0x66, 0xb8                   ; mov eax, imm32
ASM_PFX(gSmmCr4): DD 0
    or      ah,  2                      ; enable XMM registers access
    mov     cr4, rax
    DB      0x66
    mov     ecx, 0xc0000080             ; IA32_EFER MSR
    rdmsr
    or      ah, 1                       ; set LME bit
    wrmsr
    DB      0x66, 0xb8                   ; mov eax, imm32
ASM_PFX(gSmmCr0): DD 0
    mov     cr0, rax                    ; enable protected mode & paging
    DB      0x66, 0xea                   ; far jmp to long mode
ASM_PFX(gSmmJmpAddr): DQ @LongMode
@LongMode:                              ; long-mode starts here
    DB      0x48, 0xbc                   ; mov rsp, imm64
ASM_PFX(gSmmInitStack): DQ 0
    and     sp, 0xfff0                  ; make sure RSP is 16-byte aligned
    ;
    ; Accoring to X64 calling convention, XMM0~5 are volatile, we need to save
    ; them before calling C-function.
    ;
    sub     rsp, 0x60
    movdqa  [rsp], xmm0
    movdqa  [rsp + 0x10], xmm1
    movdqa  [rsp + 0x20], xmm2
    movdqa  [rsp + 0x30], xmm3
    movdqa  [rsp + 0x40], xmm4
    movdqa  [rsp + 0x50], xmm5

    add     rsp, -0x20
    call    ASM_PFX(SmmInitHandler)
    add     rsp, 0x20

    ;
    ; Restore XMM0~5 after calling C-function.
    ;
    movdqa  xmm0, [rsp]
    movdqa  xmm1, [rsp + 0x10]
    movdqa  xmm2, [rsp + 0x20]
    movdqa  xmm3, [rsp + 0x30]
    movdqa  xmm4, [rsp + 0x40]
    movdqa  xmm5, [rsp + 0x50]

    rsm

BITS 16
ASM_PFX(gcSmmInitTemplate):
    mov ebp, [cs:@L1 - ASM_PFX(gcSmmInitTemplate) + 0x8000]
    sub ebp, 0x30000
    jmp ebp
@L1:
    DQ      ASM_PFX(SmmStartup)

ASM_PFX(gcSmmInitSize): DW $ - ASM_PFX(gcSmmInitTemplate)

BITS 64
global ASM_PFX(SmmRelocationSemaphoreComplete)
ASM_PFX(SmmRelocationSemaphoreComplete):
    push    rax
    mov     rax, [ASM_PFX(mRebasedFlag)]
    mov     byte [rax], 1
    pop     rax
    jmp     [ASM_PFX(mSmmRelocationOriginalAddress)]

;
; Semaphore code running in 32-bit mode
;
global ASM_PFX(SmmRelocationSemaphoreComplete32)
ASM_PFX(SmmRelocationSemaphoreComplete32):
    ;
    ; mov byte ptr [], 1
    ;
    db      0xc6, 0x5
ASM_PFX(mRebasedFlagAddr32): dd 0
    db      1
    ;
    ; jmp dword ptr []
    ;
    db      0xff, 0x25
ASM_PFX(mSmmRelocationOriginalAddressPtr32): dd 0
