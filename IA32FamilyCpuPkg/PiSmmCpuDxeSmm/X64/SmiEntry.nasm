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

;
; Variables referrenced by C code
;

;
; Constants relating to PROCESSOR_SMM_DESCRIPTOR
;
%define DSC_OFFSET 0xfb00
%define DSC_GDTPTR 0x30
%define DSC_GDTSIZ 0x38
%define DSC_CS 14
%define DSC_DS 16
%define DSC_SS 18
%define DSC_OTHERSEG 20
%define MSR_DR6 0xc05
%define MSR_DR7 0xc06
;
; Constants relating to CPU State Save Area
;
%define SSM_DR6 0xffd0
%define SSM_DR7 0xffc8

%define PROTECT_MODE_CS 0x8
%define PROTECT_MODE_DS 0x20
%define LONG_MODE_CS 0x38
%define TSS_SEGMENT 0x40
%define GDT_SIZE 0x50

extern ASM_PFX(SmiRendezvous)
extern ASM_PFX(FeaturePcdGet (PcdCpuSmmDebug))
extern ASM_PFX(gcSmiIdtr)
extern ASM_PFX(mSaveStateInMsr)
extern ASM_PFX(mSmmProtectedModeEnable)
extern ASM_PFX(mSaveStateInMsr)
extern ASM_PFX(mSmmProtectedModeEnable)

global ASM_PFX(gcSmiPMHandlerSize)
global ASM_PFX(gSmbase)
global ASM_PFX(gSmiStack)
global ASM_PFX(gcSmiPMHandlerTemplate)
global ASM_PFX(gProtModeSmbase)
global ASM_PFX(gSmiCr3)
global ASM_PFX(gcSmiHandlerTemplate)
global ASM_PFX(gcSmiHandlerSize)

    DEFAULT REL
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
o32 lgdt    [cs:bx]                       ; lgdt fword ptr cs:[bx]
    mov     ax, PROTECT_MODE_CS
    mov     [cs:bx-0x2],ax    
    DB      0x66, 0xbf                   ; mov edi, SMBASE
ASM_PFX(gSmbase): DD 0
    lea     eax, [edi + (@ProtectedMode - _SmiEntryPoint) + 0x8000]
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
@ProtectedMode:
    mov     ax, PROTECT_MODE_DS
o16 mov     ds, ax
o16 mov     es, ax
o16 mov     fs, ax
o16 mov     gs, ax
o16 mov     ss, ax
    DB      0xbc                   ; mov esp, imm32
ASM_PFX(gSmiStack): DD 0
    jmp     ProtFlatMode

BITS 64
ASM_PFX(gcSmiPMHandlerTemplate):
_SmiPMEntryPoint:
; if SMM PROT MODE feature is ok, processor will break here with 32bit protected mode
    DB      0xbf                        ; mov edi, SMBASE
ASM_PFX(gProtModeSmbase): DD 0
    mov     eax, PROTECT_MODE_DS
    push    rax
    mov     eax, esp
    add     eax, edi
    add     eax, 4
    push    rax
    sub     eax, 8
    lss     esp, [rax]

    mov     eax, edi
    add     eax, ProtFlatMode - _SmiPMEntryPoint + 0x8000
    mov     dx, PROTECT_MODE_CS
    mov     [rax - 2], dx               ; mov cs:[bx - 6], eax
    mov     [rax - 6], eax              ; mov cs:[bx - 6], eax

    ; set cr0 value that is the same with real mode
    mov     rbx, cr0
    and     ebx, 0x9ffafff3
    or      ebx, 0x00000023
    mov     cr0, rbx

    DB      0xea                        ; jmp @ProtFlatMode
    DD      0
    DW      0

ProtFlatMode:
    DB      0xb8                        ; mov eax, offset gSmiCr3
ASM_PFX(gSmiCr3): DD 0
    mov     cr3, rax
    mov     eax, 0x668                   ; as cr4.PGE is not set here, refresh cr3
    mov     cr4, rax                    ; in PreModifyMtrrs() to flush TLB.
; Load TSS
    sub     esp, 8                      ; reserve room in stack
    sgdt    [rsp]
    mov     eax, [rsp + 2]              ; eax = GDT base
    add     esp, 8
    mov     edx, eax
    add     edx, GDT_SIZE
    mov     [rax + TSS_SEGMENT + 2], dl
    mov     [rax + TSS_SEGMENT + 3], dh
    DB      0xc1, 0xea, 0x10             ; shr     edx, 16
    mov     [rax + TSS_SEGMENT + 4], dl
    mov     [rax + TSS_SEGMENT + 7], dh
    mov     edx, eax
    mov     dl, 0x89
    mov     [rax + TSS_SEGMENT + 5], dl ; clear busy flag
    mov     eax, TSS_SEGMENT
    ltr     ax

; Switch into @LongMode
    push    LONG_MODE_CS                ; push cs hardcore here
    call    Base                       ; push reture address for retf later
Base:
    add     dword [rsp], @LongMode - Base; offset for far retf, seg is the 1st arg
    mov     ecx, 0xc0000080
    rdmsr
    or      ah, 1
    wrmsr
    mov     rbx, cr0
    bts     ebx, 31
    mov     cr0, rbx
    retf
@LongMode:                              ; long mode (64-bit code) starts here
    mov     rax, ASM_PFX(gcSmiIdtr)
    lidt    [rax]
    lea     ebx, [rdi + DSC_OFFSET]
    mov     ax, [rbx + DSC_DS]
    mov     ds, eax
    mov     ax, [rbx + DSC_OTHERSEG]
    mov     es, eax
    mov     fs, eax
    mov     gs, eax
    mov     ax, [rbx + DSC_SS]
    mov     ss, eax
;   jmp     _SmiHandler                 ; instruction is not needed

_SmiHandler:
;
; The following lines restore DR6 & DR7 before running C code. They are useful
; when you want to enable hardware breakpoints in SMM without setting
; smmentrybreak=true in ITP.
;
; NOTE: These lines might not be appreciated in runtime since they might
;       conflict with OS debugging facilities. Turn them off in RELEASE.
;
    mov     rax, ASM_PFX(FeaturePcdGet (PcdCpuSmmDebug)) ;Get absolute address. Avoid RIP relative addressing
    cmp     byte [rax], 0
    jz      @1

    mov     rax, ASM_PFX(mSaveStateInMsr)
    cmp     byte [rax], 0
    jz      .0
    mov     ecx, MSR_DR6
    rdmsr
    shl     rax, 32
    shld    rdx, rax, 32
    push    rdx
    mov     ecx, MSR_DR7
    rdmsr
    shl     rax, 32
    shld    rdx, rax, 32
    pop     rcx
    jmp     @3

.0:
    mov     rax, ASM_PFX(mSmmProtectedModeEnable)
    cmp     byte [rax], 1
    jz      @4
    DB      0x48, 0x8b, 0xd               ; mov rcx, [rip + disp32]
    DD      SSM_DR6 - ($ + 4 - _SmiEntryPoint + 0x8000)
    DB      0x48, 0x8b, 0x15               ; mov rdx, [rip + disp32]
    DD      SSM_DR7 - ($ + 4 - _SmiEntryPoint + 0x8000)
    jmp     @3
@4:
    DB      0x48, 0x8b, 0xd               ; mov rcx, [rip + disp32]
    DD      SSM_DR6 - ($ + 4 - _SmiPMEntryPoint + 0x8000)
    DB      0x48, 0x8b, 0x15               ; mov rdx, [rip + disp32]
    DD      SSM_DR7 - ($ + 4 - _SmiPMEntryPoint + 0x8000)
@3:
    mov     dr6, rcx
    mov     dr7, rdx
@1:
    mov     rcx, [rsp]                  ; rcx <- CpuIndex
    mov     rax, ASM_PFX(SmiRendezvous)          ; rax <- absolute addr of SmiRedezvous

    ;
    ; Save FP registers
    ;
    sub     rsp, 0x208
    DB      0x48                         ; FXSAVE64
    fxsave  [rsp]

    add     rsp, -0x20
    call    rax
    add     rsp, 0x20

    ;
    ; Restore FP registers
    ;
    DB      0x48                         ; FXRSTOR64
    fxrstor [rsp]

    mov     rax, ASM_PFX(FeaturePcdGet (PcdCpuSmmDebug)) ;Get absolute address. Avoid RIP relative addressing
    cmp     byte [rax], 0
    jz      @2

    mov     rax, ASM_PFX(mSaveStateInMsr)
    cmp     byte [rax], 0
    jnz     @2                          ; DR6/DR7 MSR is RO

    mov     rdx, dr7
    mov     rcx, dr6

    mov     rax, ASM_PFX(mSmmProtectedModeEnable)
    cmp     byte [rax], 1
    jz      @5
    DB      0x48, 0x89, 0x15               ; mov [rip + disp32], rdx
    DD      SSM_DR7 - ($ + 4 - _SmiEntryPoint + 0x8000)
    DB      0x48, 0x89, 0xd               ; mov [rip + disp32], rcx
    DD      SSM_DR6 - ($ + 4 - _SmiEntryPoint + 0x8000)
    jmp     @2
@5:
    DB      0x48, 0x89, 0x15               ; mov [rip + disp32], rdx
    DD      SSM_DR7 - ($ + 4 - _SmiPMEntryPoint + 0x8000)
    DB      0x48, 0x89, 0xd               ; mov [rip + disp32], rcx
    DD      SSM_DR6 - ($ + 4 - _SmiPMEntryPoint + 0x8000)
@2:
    rsm

ASM_PFX(gcSmiHandlerSize): DW $ - _SmiEntryPoint
ASM_PFX(gcSmiPMHandlerSize): DW $ - _SmiPMEntryPoint

