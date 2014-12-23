/*
Unpublished copyright (c) 2013-2014 National ICT Australia (NICTA),
ABN 62 102 206 173.  All rights reserved.

The contents of this document are proprietary to NICTA and you may not
use, copy, modify, sublicense or distribute the contents in any form
except as permitted under the terms of a separately executed licence
agreement with NICTA.

Licensee acknowledges that the Licensed Materials will contain certain
proprietary notices, including without limitation a notice pursuant to
FAR 52.227-7202-3(c) in the following form:

"Notwithstanding any other lease or license agreement that may pertain
to, or accompany the delivery of, this computer software, the rights of
the Government regarding its use, reproduction and disclosure are as
set forth in the corresponding software Evaluation License Agreement."

*/
.syntax unified

/* See ARMv7M Architecture Reference Manual */
.set reset_register, 0xe000ed0c
.set reset_value, 0x05fa0004

.section .vectors, "a"
.global _vector_table
_vector_table:
        .word _stack
        .word _entry
        .word {{nmi}}
        .word {{hardfault}}
        .word {{memmanage}}
        .word {{busfault}}
        .word {{usagefault}}
        .word reset
        .word reset
        .word reset
        .word reset
        .word {{svcall}}
        .word {{debug_monitor}}
        .word reset
        .word {{pendsv}}
        .word {{systick}}
{{#external_irqs}}
        .word {{handler}}
{{/external_irqs}}


.section .text
.type reset,#function
reset:
        ldr r0, =reset_register
        ldr r1, =reset_value
        str r1, [r0]
        dsb
1:      b 1b
        .ltorg
.size reset, .-reset

/*
The _entry function initialises the C run-time and then jumps to main. (Which should never return!)

Specifically, this loads the .data section from flash in to SRAM, and then zeros the .bss section.

If there is a Reset_Handler function defined, then this will be invoked.  It should never return.
*/
.weak Reset_Handler

.type _entry,#function
_entry:
        /* If there is a Reset_Handler call it.  It shouldn't return */
        bl Reset_Handler

        /* Load .data section */
        ldr r0, =_data_load_addr
        ldr r1, =_data_virt_addr
        ldr r2, =_data_size
1:      cbz r2, 2f
        ldm r0!, {r3}
        stm r1!, {r3}
        sub r2, #4
        b 1b
2:

        /* Zero .bss section */
        ldr r1, =_bss_virt_addr
        ldr r2, =_bss_size
        mov r3, #0
1:      cbz r2, 2f
        stm r1!, {r3}
        subs r2, #4
        b 1b
2:

        b main
.size _entry, .-_entry
