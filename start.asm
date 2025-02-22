; Kernel's entry point. 
[BITS 32]
global start
start:
  mov esp, _sys_stack   ; Points stack to new stack area
  jmp stublet

; This part must be 4 byte aligned
ALIGN 4
mboot:
  ; Multiboot macros
  MULTIBOOT_PAGE_ALIGN      equ 1 << 0
  MULTIBOOT_MEMORY_INFO     equ 1 << 1
  MULTIBOOT_AOUT_KLUDGE     equ 1 << 16
  MULTIBOOT_HEADER_MAGIC    equ 0x1BADB002
  MULTIBOOT_HEADER_FLAGS    equ MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO | MULTIBOOT_AOUT_KLUDGE
  MULTIBOOT_CHECKSUM        equ -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)
  EXTERN code, bss, end

  ; GRUB multiboot header. A boot signature
  dd MULTIBOOT_HEADER_MAGIC
  dd MULTIBOOT_HEADER_FLAGS
  dd MULTIBOOT_CHECKSUM

  ; AOUT kludge - must be physical addresses.
  ; Linker script fills these
  dd mboot
  dd code
  dd bss
  dd end
  dd start

stublet:
  extern kmain
  call kmain

  jmp $

; TODO - Add code for loading GDT
global gdt_flush       ; Allows the C code to link to this function
extern gp              ; Says that '_gp' is in another file
gdt_flush:
    lgdt[gp]           ; Load the GDT with '_gp' a special pointer
    mov ax, 0x10        ; 0x10 is the offset in the GDT to our data segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:flush2     ; 0x08 is the offset to code segment
flush2:
    ret                 ; Return back to C code 

; Loads IDT defined in '_idtp' into the processor
; This is declared in 'idt.c'.
global idt_load
extern idtp
idt_load:
    lidt[idtp]
    ret

; TODO - Add Interrupt service routines (ISRs)


; Definition of bss section
SECTION .bss
  resb 8192
_sys_stack:

