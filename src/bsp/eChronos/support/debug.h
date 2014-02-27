/*
 * debug.h
 *      Debugging support for the echronos freertos wrapper
 *  Created on: Nov 18, 2013
 *      Author: jxie @ NICTA
 */

/* Enable echronos wrapper debug output */
//#define ECHRONOS_WRAPPER_DEBUG_ENABLE

/* These are provided by eChronos.  We can't include the appropriate header 
   file because it hasn't been (re)generated yet at the time we build this. */
extern void debug_println(const char *msg);
extern void debug_print(const char *msg);
extern void debug_printhex32(uint32_t val);
