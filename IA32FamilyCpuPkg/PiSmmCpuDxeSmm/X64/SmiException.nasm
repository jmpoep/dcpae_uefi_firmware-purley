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
;   SmiException.nasm
;
; Abstract:
;
;   Exception handlers used in SM mode
;
;------------------------------------------------------------------------------

extern  ASM_PFX(gSmiMtrrs)
extern  ASM_PFX(SmiPFHandler)
extern  ASM_PFX(mMceHandlerLongMode)

global  ASM_PFX(gcSmiIdtr)
global  ASM_PFX(gcSmiGdtr)
global  ASM_PFX(gcPsd)
global  ASM_PFX(gSmiExceptionCr3)

    SECTION .data

NullSeg: DQ 0                   ; reserved by architecture
CodeSeg32:
            DW      -1                  ; LimitLow
            DW      0                   ; BaseLow
            DB      0                   ; BaseMid
            DB      0x9b
            DB      0xcf                ; LimitHigh
            DB      0                   ; BaseHigh
ProtModeCodeSeg32:
            DW      -1                  ; LimitLow
            DW      0                   ; BaseLow
            DB      0                   ; BaseMid
            DB      0x9b
            DB      0xcf                ; LimitHigh
            DB      0                   ; BaseHigh
ProtModeSsSeg32:
            DW      -1                  ; LimitLow
            DW      0                   ; BaseLow
            DB      0                   ; BaseMid
            DB      0x93
            DB      0xcf                ; LimitHigh
            DB      0                   ; BaseHigh
DataSeg32:
            DW      -1                  ; LimitLow
            DW      0                   ; BaseLow
            DB      0                   ; BaseMid
            DB      0x93
            DB      0xcf                ; LimitHigh
            DB      0                   ; BaseHigh
CodeSeg16:
            DW      -1
            DW      0
            DB      0
            DB      0x9b
            DB      0x8f
            DB      0
DataSeg16:
            DW      -1
            DW      0
            DB      0
            DB      0x93
            DB      0x8f
            DB      0
CodeSeg64:
            DW      -1                  ; LimitLow
            DW      0                   ; BaseLow
            DB      0                   ; BaseMid
            DB      0x9b
            DB      0xaf                ; LimitHigh
            DB      0                   ; BaseHigh
; TSS Segment for X64 specially
TssSeg:
            DW      TSS_DESC_SIZE       ; LimitLow
            DW      0                   ; BaseLow
            DB      0                   ; BaseMid
            DB      0x89
            DB      0x80                ; LimitHigh
            DB      0                   ; BaseHigh
            DD      0                   ; BaseUpper
            DD      0                   ; Reserved
GDT_SIZE equ $ -   NullSeg

; Create TSS Descriptor just after GDT
TssDescriptor:
            DD      0                   ; Reserved
            DQ      0                   ; RSP0
            DQ      0                   ; RSP1
            DQ      0                   ; RSP2
            DD      0                   ; Reserved
            DD      0                   ; Reserved
            DQ      0                   ; IST1
            DQ      0                   ; IST2
            DQ      0                   ; IST3
            DQ      0                   ; IST4
            DQ      0                   ; IST5
            DQ      0                   ; IST6
            DQ      0                   ; IST7
            DD      0                   ; Reserved
            DD      0                   ; Reserved
            DW      0                   ; Reserved
            DW      0                   ; I/O Map Base Address
TSS_DESC_SIZE equ $ -   TssDescriptor

;
; CODE & DATA segments for SMM runtime
;
%define CODE_SEL       CodeSeg64 -   NullSeg
%define DATA_SEL       DataSeg32 -   NullSeg
%define CODE32_SEL     CodeSeg32 -   NullSeg

;
; This structure serves as a template for all processors.
;
ASM_PFX(gcPsd):
            DB      'PSDSIG  '
            DW      PSD_SIZE
            DW      2
            DW      1 << 2
            DW      CODE_SEL
            DW      DATA_SEL
            DW      DATA_SEL
            DW      DATA_SEL
            DW      0
            DQ      0
            DQ      0
            DQ      0                   ; fixed in InitializeMpServiceData()
            DQ        NullSeg
            DD      GDT_SIZE
            DD      0
            times   24 DB 0
            DQ      ASM_PFX(gSmiMtrrs)
PSD_SIZE  equ $ -   ASM_PFX(gcPsd)

ASM_PFX(gcSmiGdtr):
    DW      GDT_SIZE - 1
    DQ        NullSeg

ASM_PFX(gcSmiIdtr):
    DW      IDT_SIZE - 1
    DQ        _SmiIDT

;
; Here is the IDT. There are 32 (not 255) entries in it since only processor
; generated exceptions will be handled.
;
_SmiIDT:
%rep 32
    DW      0                           ;   0:15
    DW      CODE_SEL                    ; Segment selector
    DB      0                           ; Unused
    DB      0x8e                         ; Interrupt Gate, Present
    DW      0                           ;   16:31
    DQ      0                           ;   32:63
%endrep
_SmiIDTEnd:

IDT_SIZE equ  _SmiIDTEnd -   _SmiIDT

    DEFAULT REL
    SECTION .text

global ASM_PFX(PMIdtHandler)
ASM_PFX(PMIdtHandler):
    jmp    $
    retf

global ASM_PFX(PMIdtHandlerMachineCheck)
ASM_PFX(PMIdtHandlerMachineCheck):
    sub     esp, 8
    sidt    [rsp]            ; save 32bit IDTR
    push    rax
    push    rdx
    push    rcx

    DB      0xb8                       ; mov eax,   gSmiExceptionCr3
ASM_PFX(gSmiExceptionCr3): DD 0
    mov     cr3, rax

    mov     eax, 0x668                  ; as cr4.PGE is not set here, refresh cr3
    mov     cr4, rax                   ; in PreModifyMtrrs() to flush TLB.

; save old ss:esp
    mov     eax, ss
    push    rax
    push    rsp
    cmp     eax, DATA_SEL              ; check if ss:esp have been updated
    jz      Skip                       ; if not, update ss:esp

; change ss:esp
    sgdt    [rsp - 8]
    mov     ecx, dword [rsp - 6]
    add     ecx, eax

    xor     eax, eax
    mov     ax, word [rcx + 2]
    xor     edx, edx
    mov     dl, byte [rcx + 4]
    mov     dh, byte [rcx + 7]
    shl     edx, 0x10
    or      eax, edx
    add     esp, eax
    mov     eax, DATA_SEL
o16 mov     ss, ax

Skip:
    mov     rax, cr4
    push    rax                         ; save cr4
    mov     ecx, 0xC0000080
    rdmsr
    push    rax                         ; save MSR(0xc0000080)
    mov     rax, cr0
    push    rax                         ; save cr0

    mov     eax, CODE_SEL
    push    rax                         ; push cs hardcore
    call    Base                       ; push reture address for retf later
Base:
    add     dword [rsp], LongMode - Base ;   for far retf, seg is the 1st arg
    mov     rax, cr4
    or      al, (1 << 5)
    mov     cr4, rax                    ; enable PAE
    mov     ecx, 0xc0000080
    rdmsr
    or      ah, 1                       ; set LME
    wrmsr
    mov     rbx, cr0
    bts     ebx, 31
    mov     cr0, rbx                    ; enable paging
    retf
LongMode:                               ; long mode starts here
    mov     rax,   ASM_PFX(gcSmiIdtr)       ; load long mode IDT table to handle the exception
    lidt    [rax]             ; that maybe issued in 64bit MC exception handler
    mov     rcx, rsp
    and     rsp, qword 0xfffffff0
    xor     rax, rax
    mov     ax,  ss
    push    rax
    push    rcx
    add     rcx, 32
    mov     eax, dword [rcx + 8]
    push    rax                         ; old eflags
    xor     rax, rax
    mov     ax,  cs                     ; cs
    push    rax
    mov     rax, BackPmIdtHandler      ; return EIP
    push    rax
    mov     rax, ASM_PFX(mMceHandlerLongMode)
    push    qword [rax]                       ; long mode MCA handle
    mov     eax, dword [rcx - 8]
    mov     edx, dword [rcx - 12]
    mov     ecx, dword [rcx - 16]

    add     rsp, 8
    jmp     qword [rsp - 8]         ; jmp to long mode MCA handle

BackPmIdtHandler:
    ;
    ; let rax save DS
    ;
    mov     rax, dword DATA_SEL

    ;
    ; Change to Compatible Segment
    ;
    mov     rcx, dword CODE32_SEL             ; load compatible mode selector
    shl     rcx, 32
    mov     rdx,   Compatible      ; assume address < 4G
    or      rcx, rdx
    push    rcx
    retf

Compatible:
    pop     rax
    mov     cr0, rax                    ; restore cr0 to disable paging.

    mov     ecx, 0xC0000080
    rdmsr
    pop     rax
    wrmsr                               ; restore MSR (0xc0000080) to clear EFER.LME

    pop     rax
    mov     cr4, rax                    ; restore cr4 to clear PAE

    lss     esp, [rsp]        ; restore ss:esp

    add     esp, 4                      ; skip old ss
    pop     rcx
    pop     rdx
    pop     rax
    lidt    [rsp]             ; load saved 32bit IDTR
    add     esp, 12 + 8
    ;
    ; retf maybe used for MCE
    ;
    push    qword [rsp - 8]
    push    qword [rsp - 8]
    retf

;------------------------------------------------------------------------------
; _SmiExceptionEntryPoints is the collection of exception entrypoints followed
; by a common exception handler.
;
; Stack frame would be as follows as specified in IA32 manuals:
;
; +---------------------+ <-- 16-byte aligned ensured by processor
; +    Old SS           +
; +---------------------+
; +    Old RSP          +
; +---------------------+
; +    RFlags           +
; +---------------------+
; +    CS               +
; +---------------------+
; +    RIP              +
; +---------------------+
; +    Error Code       +
; +---------------------+
; +   Vector Number     +
; +---------------------+
; +    RBP              +
; +---------------------+ <-- RBP, 16-byte aligned
;
; RSP set to odd multiple of 8 at @CommonEntryPoint means ErrCode PRESENT
;------------------------------------------------------------------------------
global ASM_PFX(PageFaultIdtHandlerSmmProfile)
ASM_PFX(PageFaultIdtHandlerSmmProfile):
    push    0xe                         ; Page Fault
    test    spl, 8                      ; odd multiple of 8 => ErrCode present
    jnz     .0
    push    qword [rsp]                       ; duplicate INT# if no ErrCode
    mov     qword [rsp + 8], 0
.0:
    push    rbp
    mov     rbp, rsp

    ;
    ; Since here the stack pointer is 16-byte aligned, so
    ; EFI_FX_SAVE_STATE_X64 of EFI_SYSTEM_CONTEXT_x64
    ; is 16-byte aligned
    ;

;; UINT64  Rdi, Rsi, Rbp, Rsp, Rbx, Rdx, Rcx, Rax;
;; UINT64  R8, R9, R10, R11, R12, R13, R14, R15;
    push    r15
    push    r14
    push    r13
    push    r12
    push    r11
    push    r10
    push    r9
    push    r8
    push    rax
    push    rcx
    push    rdx
    push    rbx
    push    qword [rbp + 48]  ; RSP
    push    qword [rbp]       ; RBP
    push    rsi
    push    rdi

;; UINT64  Gs, Fs, Es, Ds, Cs, Ss;  insure high 16 bits of each is zero
    movzx   rax, word [rbp + 56]
    push    rax                      ; for ss
    movzx   rax, word [rbp + 32]
    push    rax                      ; for cs
    mov     rax, ds
    push    rax
    mov     rax, es
    push    rax
    mov     rax, fs
    push    rax
    mov     rax, gs
    push    rax

;; UINT64  Rip;
    push    qword [rbp + 24]

;; UINT64  Gdtr[2], Idtr[2];
    sub     rsp, 16
    sidt    [rsp]
    sub     rsp, 16
    sgdt    [rsp]

;; UINT64  Ldtr, Tr;
    xor     rax, rax
    str     ax
    push    rax
    sldt    ax
    push    rax

;; UINT64  RFlags;
    push    qword [rbp + 40]

;; UINT64  Cr0, Cr1, Cr2, Cr3, Cr4, Cr8;
    mov     rax, cr8
    push    rax
    mov     rax, cr4
    or      rax, 0x208
    mov     cr4, rax
    push    rax
    mov     rax, cr3
    push    rax
    mov     rax, cr2
    push    rax
    xor     rax, rax
    push    rax
    mov     rax, cr0
    push    rax

;; UINT64  Dr0, Dr1, Dr2, Dr3, Dr6, Dr7;
    mov     rax, dr7
    push    rax
    mov     rax, dr6
    push    rax
    mov     rax, dr3
    push    rax
    mov     rax, dr2
    push    rax
    mov     rax, dr1
    push    rax
    mov     rax, dr0
    push    rax

;; FX_SAVE_STATE_X64 FxSaveState;

    sub rsp, 512
    mov rdi, rsp
    db 0xf, 0xae, 00000111y ;fxsave [rdi]

; UEFI calling convention for x64 requires that Direction flag in EFLAGs is clear
    cld

;; UINT32  ExceptionData;
    push    qword [rbp + 16]

;; call into exception handler
    mov     rcx, [rbp + 8]
    mov     rax, ASM_PFX(SmiPFHandler)

;; Prepare parameter and call
    mov     rdx, rsp
    ;
    ; Per X64 calling convention, allocate maximum parameter stack space
    ; and make sure RSP is 16-byte aligned
    ;
    sub     rsp, 4 * 8 + 8
    call    rax
    add     rsp, 4 * 8 + 8
    jmp     .1

.1:
;; UINT64  ExceptionData;
    add     rsp, 8

;; FX_SAVE_STATE_X64 FxSaveState;

    mov rsi, rsp
    db 0xf, 0xae, 00001110y ; fxrstor [rsi]
    add rsp, 512

;; UINT64  Dr0, Dr1, Dr2, Dr3, Dr6, Dr7;
;; Skip restoration of DRx registers to support in-circuit emualators
;; or debuggers set breakpoint in interrupt/exception context
  add     rsp, 8 * 6

;; UINT64  Cr0, Cr1, Cr2, Cr3, Cr4, Cr8;
    pop     rax
    mov     cr0, rax
    add     rsp, 8   ; not for Cr1
    pop     rax
    mov     cr2, rax
    pop     rax
    mov     cr3, rax
    pop     rax
    mov     cr4, rax
    pop     rax
    mov     cr8, rax

;; UINT64  RFlags;
    pop     qword [rbp + 40]

;; UINT64  Ldtr, Tr;
;; UINT64  Gdtr[2], Idtr[2];
;; Best not let anyone mess with these particular registers...
    add     rsp, 48

;; UINT64  Rip;
    pop     qword [rbp + 24]

;; UINT64  Gs, Fs, Es, Ds, Cs, Ss;
    pop     rax
    ; mov     gs, rax ; not for gs
    pop     rax
    ; mov     fs, rax ; not for fs
    ; (X64 will not use fs and gs, so we do not restore it)
    pop     rax
    mov     es, rax
    pop     rax
    mov     ds, rax
    pop     qword [rbp + 32]  ; for cs
    pop     qword [rbp + 56]  ; for ss

;; UINT64  Rdi, Rsi, Rbp, Rsp, Rbx, Rdx, Rcx, Rax;
;; UINT64  R8, R9, R10, R11, R12, R13, R14, R15;
    pop     rdi
    pop     rsi
    add     rsp, 8               ; not for rbp
    pop     qword [rbp + 48] ; for rsp
    pop     rbx
    pop     rdx
    pop     rcx
    pop     rax
    pop     r8
    pop     r9
    pop     r10
    pop     r11
    pop     r12
    pop     r13
    pop     r14
    pop     r15

    mov     rsp, rbp

; Enable TF bit after page fault handler runs
    bts     dword [rsp + 40], 8  ;RFLAGS

    pop     rbp
    add     rsp, 16           ; skip INT# & ErrCode
    iretq

global ASM_PFX(InitializeIDTSmmStackGuard)
ASM_PFX(InitializeIDTSmmStackGuard):
;
; If SMM Stack Guard feature is enabled, set the IST field of
; the interrupe gate for Page Fault Exception to be 1
;
    lea     rax, [_SmiIDT + 14 * 16]
    mov     byte [rax + 4], 1
    ret

