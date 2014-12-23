/*
 * Copyright (C) 2014, NICTA, Inc.
 * All Rights Reserved.
 *
 * This software is released under the "BSD3" license.  Read the file
 * "LICENSE" for more information.
 */

#include <rtos-kochab.h>
#include "entry-fn.h"

{{#tasks}}
extern void {{function}}(void);
{{/tasks}}

void *entry_fn[{{tasks.length}}] = {
{{#tasks}}
    [{{prefix_const}}TASK_ID_{{name|u}}] = (void*)&{{function}},
{{/tasks}}
};
