/*
 * mutex.h
 *
 *  Created on: Nov 18, 2013
 *      Author: jxie @ NICTA
 */

#ifndef MUTEX_H_
#define MUTEX_H_

struct MUX_t{
        int             mux_type;                                                               //type of semaphore / mutex
        uint8_t         muxid;
        int             TskHoldCnt;
};

struct SEM_t{
        int             sem_type;
        uint8_t         semid;
        int             max_count;
        int             initial_count;
};

struct xSemMux_t{
        int     created;
        int     type;
        void *  priv;
};

void* eChronosCreateSemaphoreCounting( unsigned long uxCountValue, unsigned long uxInitialCount);
void * eChronosCreateSemaphore( unsigned long uxCountValue, unsigned long uxInitialCount);
void * eChronosCreateMutex(void);
void * eChronosCreateRecursiveMutex(void);
signed long eChronosMutexTake(void * xMuxId, unsigned long xBlockTime);
signed long eChronosMutexGive(void * xMuxId);
long eChronosMutexTakeRecursive(void * xMutex, unsigned long xBlockTime);
long eChronosMutexGiveRecursive(void * xMutex);
void * eChronosGetMutexHolder(void * xMuxId);


#endif /* MUTEX_H_ */
