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

OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_vector_table)
SECTIONS
{
        . = {{code_addr}};
        .ro_start = .;
        .vectors : AT ({{flash_load_addr}})
        { *(.vectors) }

        g_pfnVectors = ADDR(.vectors);

        .text :
        { *(.text.startup) *(.text*)
            *(.fixup)
            *(.gnu.warning)
            *(.glue_7)
            *(.glue_7t)
            *(.eh_frame)
            *(.gnu.linkonce.t.*)
            *(.got)
            *(.gcc_except_table)
            *(.gnu.linkonce.r.*)

            KEEP(*(.init))
            KEEP(*(.fini))
        }

        .rodata :
        { *(.rodata*) }

        .ARM.extab :
        {
            *(.ARM.extab*)
        }

        .ARM.exidx :
        {
            *(.ARM.exidx*)
        }

        .preinit_array :
        {
            PROVIDE_HIDDEN(__preinit_array_start = .);
            KEEP(*(.preinit_array))
            PROVIDE_HIDDEN(__preinit_array_end = .);
        }

        .init_array :
        {
            PROVIDE_HIDDEN(__init_array_start = .);
            KEEP(*(.init_array))
            PROVIDE_HIDDEN(__init_array_end = .);
        }

        .ctors :
        {
            KEEP(*crtbegin.o(.ctors))
            KEEP(*crtbegin?.o(.ctors))
            KEEP(*(EXCLUDE_FILE(*crtend.o *crtend?.o) .ctors))

            KEEP(*(SORT(.ctors.*)))
            KEEP(*(.ctors))
        }

        .ro_end = ALIGN(8);

        . = {{data_addr}};
        .data.bitband_end = . ;
        .data : AT ({{flash_load_addr}} + (.ro_end - .ro_start))
        {
            *(.data*)
            CONSTRUCTORS
            . = ALIGN(4);
        }

        _data_load_addr = LOADADDR(.data);
        _data_virt_addr = ADDR(.data);
        _data_size = . - ADDR(.data);
        _sidata = LOADADDR(.data);
        _sdata = ADDR(.data);
        _edata = .;

        /* Check addresses are correct */
        _ = ASSERT(_data_load_addr % 4 == 0, "_data_load_addr not word aligned");
        _ = ASSERT(_data_virt_addr % 4 == 0, "_data_virt_addr not word aligned");
        _ = ASSERT(_data_size % 4 == 0, "_data_size not word aligned");

        .bss :
        {
             *(.bss*)
             *(COMMON)
             . = ALIGN(4);
        }

        _bss_virt_addr = ADDR(.bss);
        _bss_size = SIZEOF(.bss);
        _sbss = ADDR(.bss);
        __bss_start__ = _sbss;
        __bss_end__ = _ebss;
        _ebss = .;

        .stack : {
               . = . + {{stack_size}};
               _stack = .;
        }

        /* Aliases for all the bitband variables */
        {{#bit_aliases}}
        {{.}}_bitband = {{bitband_alias}} + (({{.}} - {{bitband_base}}) * 32);
        {{/bit_aliases}}

        /* Debug sections */

        .debug_aranges  0 : { *(.debug_aranges) }
        .debug_pubnames 0 : { *(.debug_pubnames) }

        .debug_info     0 : { *(.debug_info .gnu.linkonce.wi.*) }
        .debug_abbrev   0 : { *(.debug_abbrev) }
        .debug_line     0 : { *(.debug_line) }
        .debug_frame    0 : { *(.debug_frame) }
        .debug_str      0 : { *(.debug_str) }
        .debug_loc      0 : { *(.debug_loc) }
        .debug_macinfo  0 : { *(.debug_macinfo) }

        .debug_pubtypes 0 : { *(.debug_pubtypes) }
        .debug_ranges   0 : { *(.debug_ranges) }

        .debug_macro    0 : { *(.debug_macro) }

        /* /DISCARD/ : { *(*) } */
}
