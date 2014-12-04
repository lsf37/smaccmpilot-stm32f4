/**
 * This file should be copied and compiled with the application
 * because the library the number of objects is application specific
 * NOTE: You can override these values to force an upper bound
 *       on the number of objects that the wrapper uses.
 */
#include <rtos-kochab.h>

int _muxid_count = RTOS_MUTEX_ID_MAX + 1;
int _semid_count = RTOS_SEM_ID_MAX + 1;
int _taskid_count = RTOS_TASK_ID_MAX + 1;
