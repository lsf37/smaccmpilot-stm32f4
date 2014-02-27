
#include "rtos.h"
#include "queue.h"
#include "echronos_wrapper.h"

#include <eChronos.h>


uint8_t eChronosQueueGenericCreate( unsigned long uxQueueLength, unsigned long uxItemSize, unsigned char ucQueueType)
{
	UNIMPLEMENTED();
	return 0;
}

signed long eChronosQueueGenericSend( uint8_t pxQueue, const void * const pvItemToQueue, unsigned long xTicksToWait, long xCopyPosition)
{
	UNIMPLEMENTED();
	return 0;
}

signed long eChrononsQueueGenericReceive( uint8_t pxQueue, void * const pvBuffer, unsigned long xTicksToWait, long xJustPeeking)
{
	UNIMPLEMENTED();
	return 0;
}

signed long eChronosQueueGenericSendFromISR( uint8_t pxQueue, const void * const pvItemToQueue, signed long *pxHigherPriorityTaskWoken, long xCopyPosition )
{
	UNIMPLEMENTED();
	return 0;
}

signed long eChronosQueueReceiveFromISR( uint8_t pxQueue, void * const pvBuffer, signed long *pxHigherPriorityTaskWoken)
{
	UNIMPLEMENTED();
	return 0;
}

unsigned long eChronosQueueMessagesWaiting( const uint8_t pxQueue)
{
	UNIMPLEMENTED();
	return 0;
}
