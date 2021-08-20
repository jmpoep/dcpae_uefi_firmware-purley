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
;   CpuAsm.nasm
;
; Abstract:
;
;   Assembly code that supports IA32 CPU architectural protocol
;
;------------------------------------------------------------------------------

SECTION .text

;---------------------------------------;
; _InitializeSelectors                  ;
;----------------------------------------------------------------------------;
;
; Protocol prototype
;   InitializeSelectors (
;   )
;
; Routine Description:
;
;  Creates an new GDT in RAM.  The problem is that our former selectors
;  were ROM based and the EFI OS Loader does not manipulate the machine state
;  to change them (as it would for a 16-bit PC/AT startup code that had to
;  go from Real Mode to flat mode).
;
; Arguments:
;
;
; Returns:
;
;   Nothing
;
;
; Input:  [ebp][0]  = Original ebp
;         [ebp][4]  = Return address
;
; Output: Nothing
;
; Destroys: Nothing
;-----------------------------------------------------------------------------;

%define CODE_SELECTOR 0x10
%define DATA_SELECTOR 0x18

global ASM_PFX(InitializeGdt)
ASM_PFX(InitializeGdt):
  push    ebp                 ; C prolog
  mov     ebp, esp
  pushad
  mov     edi, gdtr           ; Load GDT register

  mov     ax,cs               ; Get the selector data from our code image
o16 mov     es,ax
  lgdt    [es:edi]            ; and update the GDTR

  db      0x67
  db      0xea              ; Far Jump Offset:Selector to reload CS
  dd      SelectorRld       ; Offset is ensuing instruction boundary
  dw      CODE_SELECTOR     ; Selector is our code selector, 10h
SelectorRld:
  mov     ax, DATA_SELECTOR ; Update the Base for the new selectors, too
o16 mov     ds, ax
o16 mov     es, ax
o16 mov     fs, ax
o16 mov     gs, ax
o16 mov     ss, ax

  popad
  pop     ebp
  ret

;-----------------------------------------------------------------------------;
; data
;-----------------------------------------------------------------------------;

        align 16

gdtr:   dw (GDT_END - GDT_BASE - 1) ; GDT limit
        dd GDT_BASE                 ; (GDT base gets set above)

;-----------------------------------------------------------------------------;
;   global descriptor table (GDT)
;-----------------------------------------------------------------------------;

        align 16

GDT_BASE:
; null descriptor
%define NULL_SEL $-GDT_BASE    ; Selector [0]
        dw 0            ; limit 15:0
        dw 0            ; base 15:0
        db 0            ; base 23:16
        db 0            ; type
        db 0            ; limit 19:16, flags
        db 0            ; base 31:24

; linear data segment descriptor
%define LINEAR_SEL $-GDT_BASE        ; Selector [0x8]
        dw 0xFFFF       ; limit 0xFFFFF
        dw 0            ; base 0
        db 0
        db 0x92         ; present, ring 0, data, expand-up, writable
        db 0xCF         ; page-granular, 32-bit
        db 0

; linear code segment descriptor
%define LINEAR_CODE_SEL $-GDT_BASE        ; Selector [0x10]
        dw 0xFFFF       ; limit 0xFFFFF
        dw 0            ; base 0
        db 0
        db 0x9A         ; present, ring 0, data, expand-up, writable
        db 0xCF         ; page-granular, 32-bit
        db 0

; system data segment descriptor
%define SYS_DATA_SEL $-GDT_BASE        ; Selector [0x18]
        dw 0xFFFF       ; limit 0xFFFFF
        dw 0            ; base 0
        db 0
        db 0x92         ; present, ring 0, data, expand-up, writable
        db 0xCF         ; page-granular, 32-bit
        db 0

; system code segment descriptor
%define SYS_CODE_SEL $-GDT_BASE
        dw 0xFFFF       ; limit 0xFFFFF
        dw 0            ; base 0
        db 0
        db 0x9A         ; present, ring 0, data, expand-up, writable
        db 0xCF         ; page-granular, 32-bit
        db 0

; spare segment descriptor
%define SPARE3_SEL $-GDT_BASE
        dw 0            ; limit 0xFFFFF
        dw 0            ; base 0
        db 0
        db 0            ; present, ring 0, data, expand-up, writable
        db 0            ; page-granular, 32-bit
        db 0

; spare segment descriptor
%define SPARE4_SEL $-GDT_BASE
        dw 0            ; limit 0xFFFFF
        dw 0            ; base 0
        db 0
        db 0            ; present, ring 0, data, expand-up, writable
        db 0            ; page-granular, 32-bit
        db 0

; spare segment descriptor
%define SPARE5_SEL $-GDT_BASE
        dw 0            ; limit 0xFFFFF
        dw 0            ; base 0
        db 0
        db 0            ; present, ring 0, data, expand-up, writable
        db 0            ; page-granular, 32-bit
        db 0

GDT_END: 

