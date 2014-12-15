"""
Copyright (C) 2014, NICTA, Inc.
All Rights Reserved.

This software is released under the "BSD3" license.  Read the file
"LICENSE" for more information.
"""

from prj import Module


class EntryFnModule(Module):
    xml_schema = """<schema></schema>"""

    files = [
        {'input': 'entry-fn.h', 'render': True},
        {'input': 'entry-fn.c', 'render': True, 'type': 'c'},
    ]

module = EntryFnModule()
