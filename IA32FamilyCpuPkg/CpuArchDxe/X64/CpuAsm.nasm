;
; This file contains an 'Intel Peripheral Driver' and is
; licensed for Intel CPUs and chipsets under the terms of your
; license agreement with Intel or your vendor.  This file may
; be modified by the user, subject to additional terms of the
; license agreement
;
;-------------------------------------------------------------------------------
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
;
; Module Name:
;
;   CpuAsm.nasm
;
; Abstract:
;
;
;-------------------------------------------------------------------------------

;
;
;
;------------------------------------------------------------------------------
;  Generic IDT Vector Handlers for the Host. They are all the same so they
;  will compress really well.
;
;  By knowing the return address for Vector 00 you can can calculate the
;  vector number by looking at the call CommonInterruptEntry return address.
;  (return address - AsmIdtVector00Base)/8 == IDT index
;
;------------------------------------------------------------------------------

    DEFAULT REL
    SECTION .text

%define CODE_SELECTOR 0x38
%define DATA_SELECTOR 0x30

global ASM_PFX(InitializeGdt)
ASM_PFX(InitializeGdt):

  mov     rdx, gdtr    ; Load GDT register

  mov     ax,cs        ; Get the selector data from our code image
o16 mov     ds,ax
  lgdt    [rdx]        ; and update the GDTR

SelectorRld:
  mov     ax, DATA_SELECTOR ; Update the Base for the new selectors, too
o16 mov     ds, ax
o16 mov     es, ax
o16 mov     fs, ax
o16 mov     gs, ax
o16 mov     ss, ax

  ret

;-----------------------------------------------------------------------------;
; data
;-----------------------------------------------------------------------------;

        align 16

gdtr: dw GDT_END - GDT_BASE - 1   ; GDT limit
      dq  GDT_BASE                ; (GDT base gets set above)

;-----------------------------------------------------------------------------;
;   global descriptor table (GDT)
;-----------------------------------------------------------------------------;

        align 16

GDT_BASE:
; null descriptor
%define NULL_SEL $-GDT_BASE    ; Selector [0]
    dw  0  ; limit 15:0
    dw  0  ; base  15:0
    db  0  ; base  23:16
    db  0  ;
    db  0  ; type & limit 19:16
    db  0  ; base  31:24

; linear data segment descriptor
%define LINEAR_SEL $-GDT_BASE        ; Selector [0x8]
    dw  0xffff  ; limit 15:0
    dw  0       ; base  15:0
    db  0       ; base  23:16
    db  0x92     ;present, ring 0, data, expand-up writable
    db  0xcf    ; type & limit 19:16
    db  0       ; base  31:24

; linear code segment descriptor
%define LINEAR_CODE_SEL $-GDT_BASE        ; Selector [0x10]
    dw  0xffff      ; limit 15:0
    dw  0          ; base  15:0
    db  0          ; base  23:16
    db  0x9a        ; present, ring 0, code, expand-up writable
    db  0xcf       ; type & limit 19:16
    db  0          ; base  31:24

; system data segment descriptor
%define SYS_DATA_SEL $-GDT_BASE        ; Selector [0x18]
    dw  0xffff     ; limit 15:0
    dw  0          ; base  15:0
    db  0          ; base  23:16
    db  0x92       ; type & limit 19:16
    db  0xcf
    db  0          ; base  31:24

; system code segment descriptor
%define SYS_CODE_SEL $-GDT_BASE        ; Selector [0x20]
    dw  0xffff     ; limit 15:0
    dw  0      ; base  15:0
    db  0      ; base  23:16
    db  0x9a ; type & limit 19:16
    db  0xcf
    db  0      ; base  31:24

; spare segment descriptor             ; Selector [0x28]
%define SPARE3_SEL $-GDT_BASE
    dw  0  ; limit 15:0
    dw  0  ; base  15:0
    db  0  ; base  23:16
    db  0  ; type & limit 19:16
    db  0  ; base  31:24
    db  0

; 64bit data segment descriptor
%define LONG_MODE_DATA_SEL $-GDT_BASE        ; Selector [0x30]
    dw  0xffff  ; limit 15:0
    dw  0      ; base  15:0
    db  0      ; base  23:16
    db  0x92    ; type & limit 19:16
    db  0xcf
    db  0      ; base  31:24

; 64bit code segment descriptor
%define LONG_MODE_CODE $-GDT_BASE        ; Selector [0x38]
   dw  0xffff  ; limit 15:0
   dw  0      ; base  15:0
   db  0      ; base  23:16
   db  0x9a ; type & limit 19:16
   db  0xaf
   db  0      ; base  31:24

; spare segment descriptor
%define SPARE5_SEL $-GDT_BASE               ; Selector [0x40]
    dw  0  ; limit 15:0
    dw  0  ; base  15:0
    db  0  ; base  23:16
    db  0  ; type & limit 19:16
    db  0  ; base  31:24
    db  0

GDT_END:

