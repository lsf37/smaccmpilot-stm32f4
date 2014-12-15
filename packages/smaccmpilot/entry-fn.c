/*
 * Copyright (C) 2014, NICTA, Inc.
 * All Rights Reserved.
 *
 * This software is released under the "BSD3" license.  Read the file
 * "LICENSE" for more information.
 */

#include <rtos-kochab.h>
#include "entry-fn.h"

{{#armv7m_rtos-kochab_tasks}}
extern void {{function}}(void);
{{/armv7m_rtos-kochab_tasks}}

void *entry_fn[{{armv7m_rtos-kochab_tasks.length}}] = {
{{#armv7m_rtos-kochab_tasks}}
    [{{armv7m_rtos-kochab_prefix_const}}TASK_ID_{{name|u}}] = (void*)&{{function}},
{{/armv7m_rtos-kochab_tasks}}
};
