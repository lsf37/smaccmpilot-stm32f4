"""
Unpublished copyright (c) 2013 National ICT Australia (NICTA),
ABN 62 102 206 173.  All rights reserved.

The contents of this document are proprietary to NICTA and you may not
use, copy, modify, sublicense or distribute the contents in any form
except as permitted under the terms of a separately executed licence
agreement with NICTA.

COMMERCIAL LICENSE RIGHTS
Agreement No.: FA8750-12-9-0179
Contractor's Name; Rockwell Collins, Inc.
Contractor's Address: 400 Collins Road N.E., Cedar Rapids, IA 52498

By accepting delivery of the RTOS Code and Documentation, the Licensee
agrees that the software is "commercial" computer software within the
meaning of the applicable acquisition regulations (e.g., FAR 2.101 or
DFARS 227.7202-3).  The terms and conditions of this License shall pertain
to the Licensee's use and disclosure of the software, and shall supersede
any conflicting contractual terms or conditions.

"""
from prj import SystemBuildError, Module, ModuleInstance, pystache_render, xml2dict, xml2schema, xml_parse_string
import logging
import os
import ply.cpp
import shutil
logger = logging.getLogger()


class EntryModule(Module):
    xml_schema = """
<schema>
    <entry name="flash_load_addr" type="int" default="0" />
    <entry name="code_addr" type="int" default="0" />
    <entry name="data_addr" type="int" default="0x20000000" />
    <entry name="stack_size" type="int" default="0x1000" />
    <entry name="bitband_base" type="int" default="0x20000000" />
    <entry name="bitband_size" type="int" default="0x100000" />
    <entry name="bitband_alias" type="int" default="0x22000000" />

    <entry name="nmi" type="c_ident" default="reset" />
    <entry name="hardfault" type="c_ident" default="reset" />
    <entry name="memmanage" type="c_ident" default="reset" />
    <entry name="busfault" type="c_ident" default="reset" />
    <entry name="usagefault" type="c_ident" default="reset" />
    <entry name="svcall" type="c_ident" default="reset" />
    <entry name="debug_monitor" type="c_ident" default="reset" />
    <entry name="pendsv" type="c_ident" default="reset" />
    <entry name="systick" type="c_ident" default="reset" />
    <entry name="external_irqs" type="list" default="[]">
        <entry name="external_irq" type="dict">
          <entry name="number" type="int"/>
          <entry name="handler" type="c_ident" default="reset" />
        </entry>
    </entry>

</schema>"""

    files = [
        {'input': 'bitband.h'},
        {'input': 'vectable.s', 'render': True, 'type': 'asm'},
        {'input': 'default.ld', 'render': True, 'type': 'linker_script', 'stage': 'post_prepare'},
    ]

    def configure(self, xml_config):
        config = {}
        config['external_irqs'] = []
        config['bit_aliases'] = []  # A list of variables that should have bitband aliases created.

        config.update(super().configure(xml_config))
        # Fill in external IRQ vector list
        xirqs = [{'handler':'reset'}] * 240
        for xirq in config['external_irqs']:
            xirqs[xirq['number']] = xirq
        config['external_irqs'] = xirqs

        return config

    def post_prepare(self, system, config):
        # Now find all the BITBAND variables in all the c_files.
        def cb(macro_name, expanded_args):
            bitband_macros = ('BITBAND_VAR', 'BITBAND_VAR_ARRAY',
                              'VOLATILE_BITBAND_VAR', 'VOLATILE_BITBAND_VAR_ARRAY')
            if macro_name in bitband_macros and \
                    len(expanded_args[1]) == 1 and \
                    expanded_args[1][0].type == 'CPP_ID':
                config['bit_aliases'].append(expanded_args[1][0].value)

        p = ply.cpp.Preprocessor(include_paths=system.include_paths,
                                 macro_callback=cb)

        for c_file in system.c_files:
            with open(c_file) as f:
                try:
                    p.parse(f.read(), c_file)
                except ply.cpp.CppError as e:
                    raise SystemBuildError(str(e))

        super().post_prepare(system, config)


module = EntryModule()
