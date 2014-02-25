/**
 * This file should be copied and compiled with the application
 * because the library the number of objects is application specific
 * NOTE: You can override these values to force an upper bound
 *       on the number of objects that the wrapper uses.
 */
#include <rtos-kochab.h>
#include <mutex.h>

int _muxid_count = MUTEX_ID_MAX + 1;
int _semid_count = SEM_ID_MAX + 1;
int _taskid_count = TASK_ID_MAX + 1;

#ifndef MUTEX_ID_m00
#define MUTEX_ID_m00 0
#endif

#ifndef SEM_ID_sem00
#define SEM_ID_sem00 0
#endif

int _wrapper_mux_start = MUTEX_ID_m00;
int _wrapper_sem_start = SEM_ID_sem00;

struct MUX_t _mux_array[MUTEX_ID_MAX + 1];
struct SEM_t _sem_array[SEM_ID_MAX + 1];
struct xSemMux_t _semmux_array[MUTEX_ID_MAX + 1 + SEM_ID_MAX + 1];
