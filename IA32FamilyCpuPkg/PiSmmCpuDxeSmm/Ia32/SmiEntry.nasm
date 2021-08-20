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
;   SmiEntry.nasm
;
; Abstract:
;
;   Code template of the SMI handler for a particular processor
;
;------------------------------------------------------------------------------

%define DSC_OFFSET 0xfb00
%define DSC_GDTPTR 0x30
%define DSC_GDTSIZ 0x38
%define DSC_CS 14
%define DSC_DS 16
%define DSC_SS 18
%define DSC_OTHERSEG 20
%define MSR_DR6 0xc05
%define MSR_DR7 0xc06

%define PROTECT_MODE_CS 0x8
%define PROTECT_MODE_DS 0x20
%define TSS_SEGMENT 0x40

extern ASM_PFX(SmiRendezvous)
extern ASM_PFX(FeaturePcdGet (PcdCpuSmmDebug))
extern ASM_PFX(FeaturePcdGet (PcdCpuSmmStackGuard))
extern ASM_PFX(gcSmiIdtr)
extern ASM_PFX(mSaveStateInMsr)

global ASM_PFX(gcSmiHandlerTemplate)
global ASM_PFX(gcSmiHandlerSize)
global ASM_PFX(gSmiCr3)
global ASM_PFX(gSmiStack)
global ASM_PFX(gSmbase)
global ASM_PFX(gcSmiPMHandlerTemplate)
global ASM_PFX(gcSmiPMHandlerSize)
global ASM_PFX(gProtModeSmbase)

    SECTION .text

BITS 16
ASM_PFX(gcSmiHandlerTemplate):
_SmiEntryPoint:
    mov     bx, _GdtDesc - _SmiEntryPoint + 0x8000
    mov     ax,[cs:DSC_OFFSET + DSC_GDTSIZ]
    dec     ax
    mov     [cs:bx], ax
    mov     eax, [cs:DSC_OFFSET + DSC_GDTPTR]
    mov     [cs:bx + 2], eax
    mov     ebp, eax                      ; ebp = GDT base
o32 lgdt    [cs:bx]                       ; lgdt fword ptr cs:[bx]
    mov     ax, PROTECT_MODE_CS
    mov     [cs:bx-0x2],ax    
    DB      0x66, 0xbf                   ; mov edi, SMBASE
ASM_PFX(gSmbase): DD 0
    lea     eax, [edi + (@32bit - _SmiEntryPoint) + 0x8000]
    mov     [cs:bx-0x6],eax
    mov     ebx, cr0
    and     ebx, 0x9ffafff3
    or      ebx, 0x23
    mov     cr0, ebx
    jmp     dword 0x0:0x0
_GdtDesc:   
    DW 0
    DD 0

BITS 32
@32bit:
    mov     ax, PROTECT_MODE_DS
o16 mov     ds, ax
o16 mov     es, ax
o16 mov     fs, ax
o16 mov     gs, ax
o16 mov     ss, ax
    DB      0xbc                   ; mov esp, imm32
ASM_PFX(gSmiStack): DD 0
    mov     eax, ASM_PFX(gcSmiIdtr)
    lidt    [eax]
    jmp     ProtFlatMode

ASM_PFX(gcSmiPMHandlerTemplate):
_SmiPMEntryPoint:
; if SMM PROT MODE feature is ok, processor will break here with 32bit protected mode
    DB      0xbf                        ; mov edi, SMBASE
ASM_PFX(gProtModeSmbase): DD 0
    mov     eax, PROTECT_MODE_DS
    push    eax
    mov     eax, esp
    add     eax, edi
    add     eax, 4
    push    eax
    sub     eax, 8
    lss     esp, [eax]

    mov     eax, edi
    add     eax, ProtFlatMode - _SmiPMEntryPoint + 0x8000
    mov     dx, PROTECT_MODE_CS
    mov     [eax - 2], dx               ; mov cs:[bx - 6], eax
    mov     [eax - 6], eax              ; mov cs:[bx - 6], eax

    sgdt    [esp - 6]
    mov     ebp, [esp - 4]              ; save GDT base into ebp 

    ; set cr0 value that is the same with real mode
    mov     ebx, cr0
    and     ebx, 0x9ffafff3
    or      ebx, 0x00000023
    mov     cr0, ebx

    DB      0xea                        ; jmp @ProtFlatMode
    DD      0
    DW      0

ProtFlatMode:
    DB      0xb8                        ; mov eax, imm32
ASM_PFX(gSmiCr3): DD 0
    mov     cr3, eax
    mov     eax, 0x668                   ; as cr4.PGE is not set here, refresh cr3
    mov     cr4, eax                    ; in PreModifyMtrrs() to flush TLB.
    mov     ebx, cr0
    or      ebx, 0x80000000             ; enable paging
    mov     cr0, ebx
    lea     ebx, [edi + DSC_OFFSET]
    mov     ax, [ebx + DSC_DS]
    mov     ds, eax
    mov     ax, [ebx + DSC_OTHERSEG]
    mov     es, eax
    mov     fs, eax
    mov     gs, eax
    mov     ax, [ebx + DSC_SS]
    mov     ss, eax

    cmp     byte [dword ASM_PFX(FeaturePcdGet (PcdCpuSmmStackGuard))], 0
    jz      .0

; Load TSS
    mov     byte [ebp + TSS_SEGMENT + 5], 0x89 ; clear busy flag
    mov     eax, TSS_SEGMENT
    ltr     ax
.0:
;   jmp     _SmiHandler                 ; instruction is not needed

global ASM_PFX(SmiHandler)
ASM_PFX(SmiHandler):
    cmp     byte [dword ASM_PFX(FeaturePcdGet (PcdCpuSmmDebug))], 0
    jz      @3
    cmp     byte [dword ASM_PFX(mSaveStateInMsr)], 0
    jz      .1
    mov     ecx, MSR_DR6
    rdmsr
    push    eax
    mov     ecx, MSR_DR7
    rdmsr
    mov     edx, eax
    pop     ecx
    jmp     @5

.1:
    call    @1
@1:
    pop     ebp
    mov     eax, 0x80000001
    cpuid
    bt      edx, 29                     ; check cpuid to identify X64 or IA32
    lea     edi, [ebp - (@1 - _SmiEntryPoint) + 0x7fc8]
    lea     esi, [edi + 4]
    jnc     @2
    add     esi, 4
@2:
    mov     ecx, [esi]
    mov     edx, [edi]
@5:
    mov     dr6, ecx
    mov     dr7, edx                    ; restore DR6 & DR7 before running C code
@3:
    mov     ecx, [esp]                  ; CPU Index

    push    ecx
    mov     eax, ASM_PFX(SmiRendezvous)
    call    eax
    pop     ecx

    cmp     byte [dword ASM_PFX(FeaturePcdGet (PcdCpuSmmDebug))], 0
    jz      @4

    cmp     byte [dword ASM_PFX(mSaveStateInMsr)], 0
    jnz     @4                          ; DR6/DR7 MSR is RO

    mov     ecx, dr6
    mov     edx, dr7
    mov     [esi], ecx
    mov     [edi], edx
@4:
    rsm

ASM_PFX(gcSmiHandlerSize): DW $ - _SmiEntryPoint
ASM_PFX(gcSmiPMHandlerSize): DW $ - _SmiPMEntryPoint

