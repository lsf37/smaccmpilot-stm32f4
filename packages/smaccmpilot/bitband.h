/*
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

*/
#ifndef BITBAND_H
#define BITBAND_H

#define BITBAND_VAR(TYPE, NAME) \
    TYPE NAME __attribute__ ((section (".data.bitband"))); \
    extern uint32_t NAME##_bitband[sizeof NAME]

#define BITBAND_VAR_ARRAY(TYPE, NAME, NELEM) \
    TYPE NAME[NELEM] __attribute__ ((section (".data.bitband"))); \
    extern uint32_t NAME##_bitband[sizeof NAME]

#define VOLATILE_BITBAND_VAR(TYPE, NAME) \
    volatile TYPE NAME __attribute__ ((section (".data.bitband"))); \
    extern volatile uint32_t NAME##_bitband[sizeof NAME]

#define VOLATILE_BITBAND_VAR_ARRAY(TYPE, NAME, NELEM) \
    volatile TYPE NAME[NELEM] __attribute__ ((section (".data.bitband"))); \
    extern volatile uint32_t NAME##_bitband[sizeof NAME]


#endif

