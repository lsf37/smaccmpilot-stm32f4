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
#ifndef RTOS_KOCHAB_H
#define RTOS_KOCHAB_H




#include <stdbool.h>
#include <stdint.h>

#include <stdint.h>

#include <stdint.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdint.h>
#include <stdint.h>
#include <stdint.h>




typedef uint8_t RtosSignalSet;
typedef RtosSignalSet RtosSignalId;

typedef uint8_t RtosTimerId;
typedef uint32_t RtosTicksAbsolute;
typedef uint16_t RtosTicksRelative;

typedef uint8_t RtosInterruptEventId;

typedef uint8_t RtosMutexId;
typedef uint8_t RtosSemId;
typedef uint8_t RtosSemValue;
typedef uint8_t RtosErrorId;
typedef uint8_t RtosTaskId;
















#define RTOS_REENTRANT 



#define RTOS_SIGNAL_SET_EMPTY ((RtosSignalSet) UINT8_C(0))
#define RTOS_SIGNAL_SET_ALL ((RtosSignalSet) UINT8_MAX)
#define RTOS_SIGNAL_SET_GENERIC ((RtosSignalSet) UINT8_C(1))
#define RTOS_SIGNAL_ID_GENERIC ((RtosSignalId) RTOS_SIGNAL_SET_GENERIC)
#define RTOS_SIGNAL_SET__TASK_TIMER ((RtosSignalSet) UINT8_C(2))
#define RTOS_SIGNAL_ID__TASK_TIMER ((RtosSignalId) RTOS_SIGNAL_SET__TASK_TIMER)

#define RTOS_TIMER_ID__TASK_INIT_THREAD ((RtosTimerId) UINT8_C(0))
#define RTOS_TIMER_ID__TASK_I2C2_EV_IRQHANDLER ((RtosTimerId) UINT8_C(1))
#define RTOS_TIMER_ID__TASK_I2C2_ER_IRQHANDLER ((RtosTimerId) UINT8_C(2))
#define RTOS_TIMER_ID__TASK_SPI1_IRQHANDLER ((RtosTimerId) UINT8_C(3))
#define RTOS_TIMER_ID__TASK_TIM1_UP_TIM10_IRQHANDLER ((RtosTimerId) UINT8_C(4))
#define RTOS_TIMER_ID__TASK_TIM1_CC_IRQHANDLER ((RtosTimerId) UINT8_C(5))
#define RTOS_TIMER_ID__TASK_UART5_IRQHANDLER ((RtosTimerId) UINT8_C(6))
#define RTOS_TIMER_ID__TASK_USART1_IRQHANDLER ((RtosTimerId) UINT8_C(7))
#define RTOS_TIMER_ID__TASK_USART2_IRQHANDLER ((RtosTimerId) UINT8_C(8))
#define RTOS_TIMER_ID__TASK_USART6_IRQHANDLER ((RtosTimerId) UINT8_C(9))
#define RTOS_TIMER_ID__TASK_SCHEDULER ((RtosTimerId) UINT8_C(10))
#define RTOS_TIMER_ID__TASK_TOWER_TASK_LOOP_SENSORSCAPTURETASK_72_PROC ((RtosTimerId) UINT8_C(11))
#define RTOS_TIMER_ID__TASK_TOWER_TASK_LOOP_PROXY_CONTROLLAW_367_PROC ((RtosTimerId) UINT8_C(12))
#define RTOS_TIMER_ID__TASK_TOWER_TASK_LOOP_PROXY_USERINPUT_358_PROC ((RtosTimerId) UINT8_C(13))
#define RTOS_TIMER_ID__TASK_TOWER_TASK_LOOP_UIMUXTASK_170_PROC ((RtosTimerId) UINT8_C(14))
#define RTOS_TIMER_ID__TASK_TOWER_TASK_LOOP_CONTROLLAWTASK_146_PROC ((RtosTimerId) UINT8_C(15))
#define RTOS_TIMER_ID__TASK_TOWER_TASK_LOOP_PPMINPUTTOWER_102_PROC ((RtosTimerId) UINT8_C(16))
#define RTOS_TIMER_ID__TASK_TOWER_TASK_LOOP_MOTMIX_552_PROC ((RtosTimerId) UINT8_C(17))
#define RTOS_TIMER_ID__TASK_TOWER_TASK_LOOP_MOTORS_STATE_572_PROC ((RtosTimerId) UINT8_C(18))
#define RTOS_TIMER_ID__TASK_TOWER_TASK_LOOP_CONTROL_STATE_563_PROC ((RtosTimerId) UINT8_C(19))
#define RTOS_TIMER_ID__TASK_TOWER_TASK_LOOP_CONTROL_396_PROC ((RtosTimerId) UINT8_C(20))
#define RTOS_TIMER_ID__TASK_TOWER_TASK_LOOP_POSITION_STATE_633_PROC ((RtosTimerId) UINT8_C(21))
#define RTOS_TIMER_ID__TASK_TOWER_TASK_LOOP_PX4IOAR_587_PROC ((RtosTimerId) UINT8_C(22))
#define RTOS_TIMER_ID__TASK_TOWER_TASK_LOOP_COMMSECRECOVERYTASK_1433_PROC ((RtosTimerId) UINT8_C(23))
#define RTOS_TIMER_ID__TASK_TOWER_TASK_LOOP_PROXY_NAV_LAW_385_PROC ((RtosTimerId) UINT8_C(24))
#define RTOS_TIMER_ID__TASK_TOWER_TASK_LOOP_PROXY_NAVSETPT_376_PROC ((RtosTimerId) UINT8_C(25))
#define RTOS_TIMER_ID__TASK_TOWER_TASK_LOOP_NAVIGATION_199_PROC ((RtosTimerId) UINT8_C(26))
#define RTOS_TIMER_ID__TASK_TOWER_TASK_LOOP_MAVLINKINPUTTASK_124_PROC ((RtosTimerId) UINT8_C(27))
#define RTOS_TIMER_ID__TASK_TOWER_TASK_LOOP_GCSRECEIVETASK_UART1_709_PROC ((RtosTimerId) UINT8_C(28))
#define RTOS_TIMER_ID__TASK_TOWER_TASK_LOOP_GCSTRANSMITTASK_UART1_906_PROC ((RtosTimerId) UINT8_C(29))
#define RTOS_TIMER_ID__TASK_TOWER_TASK_LOOP_GCSRECEIVETASK_UART5_1105_PROC ((RtosTimerId) UINT8_C(30))
#define RTOS_TIMER_ID__TASK_TOWER_TASK_LOOP_GCSTRANSMITTASK_UART5_1302_PROC ((RtosTimerId) UINT8_C(31))
#define RTOS_TIMER_ID__TASK_TOWER_TASK_LOOP_ENCRYPTTASK_UART5_1294_PROC ((RtosTimerId) UINT8_C(32))
#define RTOS_TIMER_ID__TASK_TOWER_TASK_LOOP_DECRYPTTASK_UART5_1092_PROC ((RtosTimerId) UINT8_C(33))
#define RTOS_TIMER_ID__TASK_TOWER_TASK_LOOP_RADIO_STATUS_UART5_1083_PROC ((RtosTimerId) UINT8_C(34))
#define RTOS_TIMER_ID__TASK_TOWER_TASK_LOOP_DATALINK_UART5_1050_PROC ((RtosTimerId) UINT8_C(35))
#define RTOS_TIMER_ID__TASK_TOWER_TASK_LOOP_ENCRYPTTASK_UART1_898_PROC ((RtosTimerId) UINT8_C(36))
#define RTOS_TIMER_ID__TASK_TOWER_TASK_LOOP_DECRYPTTASK_UART1_696_PROC ((RtosTimerId) UINT8_C(37))
#define RTOS_TIMER_ID__TASK_TOWER_TASK_LOOP_RADIO_STATUS_UART1_687_PROC ((RtosTimerId) UINT8_C(38))
#define RTOS_TIMER_ID__TASK_TOWER_TASK_LOOP_DATALINK_UART1_654_PROC ((RtosTimerId) UINT8_C(39))
#define RTOS_TIMER_ID__TASK_TOWER_TASK_LOOP_UBLOXGPS_54_PROC ((RtosTimerId) UINT8_C(40))
#define RTOS_TIMER_ID__TASK_TOWER_TASK_LOOP_BLINK_542_PROC ((RtosTimerId) UINT8_C(41))

#define RTOS_INTERRUPT_EVENT_ID_I2C2_EV ((RtosInterruptEventId) UINT8_C(0))
#define RTOS_INTERRUPT_EVENT_ID_I2C2_ER ((RtosInterruptEventId) UINT8_C(1))
#define RTOS_INTERRUPT_EVENT_ID_SPI1 ((RtosInterruptEventId) UINT8_C(2))
#define RTOS_INTERRUPT_EVENT_ID_TIM1_UP_TIM10 ((RtosInterruptEventId) UINT8_C(3))
#define RTOS_INTERRUPT_EVENT_ID_TIM1_CC ((RtosInterruptEventId) UINT8_C(4))
#define RTOS_INTERRUPT_EVENT_ID_UART5 ((RtosInterruptEventId) UINT8_C(5))
#define RTOS_INTERRUPT_EVENT_ID_USART1 ((RtosInterruptEventId) UINT8_C(6))
#define RTOS_INTERRUPT_EVENT_ID_USART2 ((RtosInterruptEventId) UINT8_C(7))
#define RTOS_INTERRUPT_EVENT_ID_USART6 ((RtosInterruptEventId) UINT8_C(8))
#define RTOS_INTERRUPT_EVENT_ID_MAINIDLE ((RtosInterruptEventId) UINT8_C(9))

#define RTOS_MUTEX_ID_ZERO ((RtosMutexId) UINT8_C(0))
#define RTOS_MUTEX_ID_MAX ((RtosMutexId) UINT8_C(72 - 1))
#define RTOS_MUTEX_ID_M00 ((RtosMutexId) UINT8_C(0))
#define RTOS_MUTEX_ID_M01 ((RtosMutexId) UINT8_C(1))
#define RTOS_MUTEX_ID_M02 ((RtosMutexId) UINT8_C(2))
#define RTOS_MUTEX_ID_M03 ((RtosMutexId) UINT8_C(3))
#define RTOS_MUTEX_ID_M04 ((RtosMutexId) UINT8_C(4))
#define RTOS_MUTEX_ID_M05 ((RtosMutexId) UINT8_C(5))
#define RTOS_MUTEX_ID_M06 ((RtosMutexId) UINT8_C(6))
#define RTOS_MUTEX_ID_M07 ((RtosMutexId) UINT8_C(7))
#define RTOS_MUTEX_ID_M08 ((RtosMutexId) UINT8_C(8))
#define RTOS_MUTEX_ID_M09 ((RtosMutexId) UINT8_C(9))
#define RTOS_MUTEX_ID_M10 ((RtosMutexId) UINT8_C(10))
#define RTOS_MUTEX_ID_M11 ((RtosMutexId) UINT8_C(11))
#define RTOS_MUTEX_ID_M12 ((RtosMutexId) UINT8_C(12))
#define RTOS_MUTEX_ID_M13 ((RtosMutexId) UINT8_C(13))
#define RTOS_MUTEX_ID_M14 ((RtosMutexId) UINT8_C(14))
#define RTOS_MUTEX_ID_M15 ((RtosMutexId) UINT8_C(15))
#define RTOS_MUTEX_ID_M16 ((RtosMutexId) UINT8_C(16))
#define RTOS_MUTEX_ID_M17 ((RtosMutexId) UINT8_C(17))
#define RTOS_MUTEX_ID_M18 ((RtosMutexId) UINT8_C(18))
#define RTOS_MUTEX_ID_M19 ((RtosMutexId) UINT8_C(19))
#define RTOS_MUTEX_ID_M20 ((RtosMutexId) UINT8_C(20))
#define RTOS_MUTEX_ID_M21 ((RtosMutexId) UINT8_C(21))
#define RTOS_MUTEX_ID_M22 ((RtosMutexId) UINT8_C(22))
#define RTOS_MUTEX_ID_M23 ((RtosMutexId) UINT8_C(23))
#define RTOS_MUTEX_ID_M24 ((RtosMutexId) UINT8_C(24))
#define RTOS_MUTEX_ID_M25 ((RtosMutexId) UINT8_C(25))
#define RTOS_MUTEX_ID_M26 ((RtosMutexId) UINT8_C(26))
#define RTOS_MUTEX_ID_M27 ((RtosMutexId) UINT8_C(27))
#define RTOS_MUTEX_ID_M28 ((RtosMutexId) UINT8_C(28))
#define RTOS_MUTEX_ID_M29 ((RtosMutexId) UINT8_C(29))
#define RTOS_MUTEX_ID_M30 ((RtosMutexId) UINT8_C(30))
#define RTOS_MUTEX_ID_M31 ((RtosMutexId) UINT8_C(31))
#define RTOS_MUTEX_ID_M32 ((RtosMutexId) UINT8_C(32))
#define RTOS_MUTEX_ID_M33 ((RtosMutexId) UINT8_C(33))
#define RTOS_MUTEX_ID_M34 ((RtosMutexId) UINT8_C(34))
#define RTOS_MUTEX_ID_M35 ((RtosMutexId) UINT8_C(35))
#define RTOS_MUTEX_ID_M36 ((RtosMutexId) UINT8_C(36))
#define RTOS_MUTEX_ID_M37 ((RtosMutexId) UINT8_C(37))
#define RTOS_MUTEX_ID_M38 ((RtosMutexId) UINT8_C(38))
#define RTOS_MUTEX_ID_M39 ((RtosMutexId) UINT8_C(39))
#define RTOS_MUTEX_ID_M40 ((RtosMutexId) UINT8_C(40))
#define RTOS_MUTEX_ID_M41 ((RtosMutexId) UINT8_C(41))
#define RTOS_MUTEX_ID_M42 ((RtosMutexId) UINT8_C(42))
#define RTOS_MUTEX_ID_M43 ((RtosMutexId) UINT8_C(43))
#define RTOS_MUTEX_ID_M44 ((RtosMutexId) UINT8_C(44))
#define RTOS_MUTEX_ID_M45 ((RtosMutexId) UINT8_C(45))
#define RTOS_MUTEX_ID_M46 ((RtosMutexId) UINT8_C(46))
#define RTOS_MUTEX_ID_M47 ((RtosMutexId) UINT8_C(47))
#define RTOS_MUTEX_ID_M48 ((RtosMutexId) UINT8_C(48))
#define RTOS_MUTEX_ID_M49 ((RtosMutexId) UINT8_C(49))
#define RTOS_MUTEX_ID_M50 ((RtosMutexId) UINT8_C(50))
#define RTOS_MUTEX_ID_M51 ((RtosMutexId) UINT8_C(51))
#define RTOS_MUTEX_ID_M52 ((RtosMutexId) UINT8_C(52))
#define RTOS_MUTEX_ID_M53 ((RtosMutexId) UINT8_C(53))
#define RTOS_MUTEX_ID_M54 ((RtosMutexId) UINT8_C(54))
#define RTOS_MUTEX_ID_M55 ((RtosMutexId) UINT8_C(55))
#define RTOS_MUTEX_ID_M56 ((RtosMutexId) UINT8_C(56))
#define RTOS_MUTEX_ID_M57 ((RtosMutexId) UINT8_C(57))
#define RTOS_MUTEX_ID_M58 ((RtosMutexId) UINT8_C(58))
#define RTOS_MUTEX_ID_M59 ((RtosMutexId) UINT8_C(59))
#define RTOS_MUTEX_ID_M60 ((RtosMutexId) UINT8_C(60))
#define RTOS_MUTEX_ID_M61 ((RtosMutexId) UINT8_C(61))
#define RTOS_MUTEX_ID_M62 ((RtosMutexId) UINT8_C(62))
#define RTOS_MUTEX_ID_M63 ((RtosMutexId) UINT8_C(63))
#define RTOS_MUTEX_ID_M64 ((RtosMutexId) UINT8_C(64))
#define RTOS_MUTEX_ID_M65 ((RtosMutexId) UINT8_C(65))
#define RTOS_MUTEX_ID_M66 ((RtosMutexId) UINT8_C(66))
#define RTOS_MUTEX_ID_M67 ((RtosMutexId) UINT8_C(67))
#define RTOS_MUTEX_ID_M68 ((RtosMutexId) UINT8_C(68))
#define RTOS_MUTEX_ID_M69 ((RtosMutexId) UINT8_C(69))
#define RTOS_MUTEX_ID_M70 ((RtosMutexId) UINT8_C(70))
#define RTOS_MUTEX_ID_M71 ((RtosMutexId) UINT8_C(71))
#define RTOS_SEM_ID_ZERO ((RtosSemId) UINT8_C(0))
#define RTOS_SEM_ID_MAX ((RtosSemId) UINT8_C(82 - 1))
#define RTOS_SEM_ID_SEM00 ((RtosSemId) UINT8_C(0))
#define RTOS_SEM_ID_SEM01 ((RtosSemId) UINT8_C(1))
#define RTOS_SEM_ID_SEM02 ((RtosSemId) UINT8_C(2))
#define RTOS_SEM_ID_SEM03 ((RtosSemId) UINT8_C(3))
#define RTOS_SEM_ID_SEM04 ((RtosSemId) UINT8_C(4))
#define RTOS_SEM_ID_SEM05 ((RtosSemId) UINT8_C(5))
#define RTOS_SEM_ID_SEM06 ((RtosSemId) UINT8_C(6))
#define RTOS_SEM_ID_SEM07 ((RtosSemId) UINT8_C(7))
#define RTOS_SEM_ID_SEM08 ((RtosSemId) UINT8_C(8))
#define RTOS_SEM_ID_SEM09 ((RtosSemId) UINT8_C(9))
#define RTOS_SEM_ID_SEM10 ((RtosSemId) UINT8_C(10))
#define RTOS_SEM_ID_SEM11 ((RtosSemId) UINT8_C(11))
#define RTOS_SEM_ID_SEM12 ((RtosSemId) UINT8_C(12))
#define RTOS_SEM_ID_SEM13 ((RtosSemId) UINT8_C(13))
#define RTOS_SEM_ID_SEM14 ((RtosSemId) UINT8_C(14))
#define RTOS_SEM_ID_SEM15 ((RtosSemId) UINT8_C(15))
#define RTOS_SEM_ID_SEM16 ((RtosSemId) UINT8_C(16))
#define RTOS_SEM_ID_SEM17 ((RtosSemId) UINT8_C(17))
#define RTOS_SEM_ID_SEM18 ((RtosSemId) UINT8_C(18))
#define RTOS_SEM_ID_SEM19 ((RtosSemId) UINT8_C(19))
#define RTOS_SEM_ID_SEM20 ((RtosSemId) UINT8_C(20))
#define RTOS_SEM_ID_SEM21 ((RtosSemId) UINT8_C(21))
#define RTOS_SEM_ID_SEM22 ((RtosSemId) UINT8_C(22))
#define RTOS_SEM_ID_SEM23 ((RtosSemId) UINT8_C(23))
#define RTOS_SEM_ID_SEM24 ((RtosSemId) UINT8_C(24))
#define RTOS_SEM_ID_SEM25 ((RtosSemId) UINT8_C(25))
#define RTOS_SEM_ID_SEM26 ((RtosSemId) UINT8_C(26))
#define RTOS_SEM_ID_SEM27 ((RtosSemId) UINT8_C(27))
#define RTOS_SEM_ID_SEM28 ((RtosSemId) UINT8_C(28))
#define RTOS_SEM_ID_SEM29 ((RtosSemId) UINT8_C(29))
#define RTOS_SEM_ID_SEM30 ((RtosSemId) UINT8_C(30))
#define RTOS_SEM_ID_SEM31 ((RtosSemId) UINT8_C(31))
#define RTOS_SEM_ID_SEM32 ((RtosSemId) UINT8_C(32))
#define RTOS_SEM_ID_SEM33 ((RtosSemId) UINT8_C(33))
#define RTOS_SEM_ID_SEM34 ((RtosSemId) UINT8_C(34))
#define RTOS_SEM_ID_SEM35 ((RtosSemId) UINT8_C(35))
#define RTOS_SEM_ID_SEM36 ((RtosSemId) UINT8_C(36))
#define RTOS_SEM_ID_SEM37 ((RtosSemId) UINT8_C(37))
#define RTOS_SEM_ID_SEM38 ((RtosSemId) UINT8_C(38))
#define RTOS_SEM_ID_SEM39 ((RtosSemId) UINT8_C(39))
#define RTOS_SEM_ID_SEM40 ((RtosSemId) UINT8_C(40))
#define RTOS_SEM_ID_SEM41 ((RtosSemId) UINT8_C(41))
#define RTOS_SEM_ID_SEM42 ((RtosSemId) UINT8_C(42))
#define RTOS_SEM_ID_SEM43 ((RtosSemId) UINT8_C(43))
#define RTOS_SEM_ID_SEM44 ((RtosSemId) UINT8_C(44))
#define RTOS_SEM_ID_SEM45 ((RtosSemId) UINT8_C(45))
#define RTOS_SEM_ID_SEM46 ((RtosSemId) UINT8_C(46))
#define RTOS_SEM_ID_SEM47 ((RtosSemId) UINT8_C(47))
#define RTOS_SEM_ID_SEM48 ((RtosSemId) UINT8_C(48))
#define RTOS_SEM_ID_SEM49 ((RtosSemId) UINT8_C(49))
#define RTOS_SEM_ID_SEM50 ((RtosSemId) UINT8_C(50))
#define RTOS_SEM_ID_SEM51 ((RtosSemId) UINT8_C(51))
#define RTOS_SEM_ID_SEM52 ((RtosSemId) UINT8_C(52))
#define RTOS_SEM_ID_SEM53 ((RtosSemId) UINT8_C(53))
#define RTOS_SEM_ID_SEM54 ((RtosSemId) UINT8_C(54))
#define RTOS_SEM_ID_SEM55 ((RtosSemId) UINT8_C(55))
#define RTOS_SEM_ID_SEM56 ((RtosSemId) UINT8_C(56))
#define RTOS_SEM_ID_SEM57 ((RtosSemId) UINT8_C(57))
#define RTOS_SEM_ID_SEM58 ((RtosSemId) UINT8_C(58))
#define RTOS_SEM_ID_SEM59 ((RtosSemId) UINT8_C(59))
#define RTOS_SEM_ID_SEM60 ((RtosSemId) UINT8_C(60))
#define RTOS_SEM_ID_SEM61 ((RtosSemId) UINT8_C(61))
#define RTOS_SEM_ID_SEM62 ((RtosSemId) UINT8_C(62))
#define RTOS_SEM_ID_SEM63 ((RtosSemId) UINT8_C(63))
#define RTOS_SEM_ID_SEM64 ((RtosSemId) UINT8_C(64))
#define RTOS_SEM_ID_SEM65 ((RtosSemId) UINT8_C(65))
#define RTOS_SEM_ID_SEM66 ((RtosSemId) UINT8_C(66))
#define RTOS_SEM_ID_SEM67 ((RtosSemId) UINT8_C(67))
#define RTOS_SEM_ID_SEM68 ((RtosSemId) UINT8_C(68))
#define RTOS_SEM_ID_SEM69 ((RtosSemId) UINT8_C(69))
#define RTOS_SEM_ID_SEM70 ((RtosSemId) UINT8_C(70))
#define RTOS_SEM_ID_SEM71 ((RtosSemId) UINT8_C(71))
#define RTOS_SEM_ID_SEM72 ((RtosSemId) UINT8_C(72))
#define RTOS_SEM_ID_SEM73 ((RtosSemId) UINT8_C(73))
#define RTOS_SEM_ID_SEM74 ((RtosSemId) UINT8_C(74))
#define RTOS_SEM_ID_SEM75 ((RtosSemId) UINT8_C(75))
#define RTOS_SEM_ID_SEM76 ((RtosSemId) UINT8_C(76))
#define RTOS_SEM_ID_SEM77 ((RtosSemId) UINT8_C(77))
#define RTOS_SEM_ID_SEM78 ((RtosSemId) UINT8_C(78))
#define RTOS_SEM_ID_SEM79 ((RtosSemId) UINT8_C(79))
#define RTOS_SEM_ID_SEM80 ((RtosSemId) UINT8_C(80))
#define RTOS_SEM_ID_INIT_SEM ((RtosSemId) UINT8_C(81))

#define RTOS_TASK_ID_ZERO ((RtosTaskId) UINT8_C(0))
#define RTOS_TASK_ID_MAX ((RtosTaskId)UINT8_C(42 - 1))
#define RTOS_TASK_ID_INIT_THREAD ((RtosTaskId) UINT8_C(0))
#define RTOS_TASK_ID_I2C2_EV_IRQHANDLER ((RtosTaskId) UINT8_C(1))
#define RTOS_TASK_ID_I2C2_ER_IRQHANDLER ((RtosTaskId) UINT8_C(2))
#define RTOS_TASK_ID_SPI1_IRQHANDLER ((RtosTaskId) UINT8_C(3))
#define RTOS_TASK_ID_TIM1_UP_TIM10_IRQHANDLER ((RtosTaskId) UINT8_C(4))
#define RTOS_TASK_ID_TIM1_CC_IRQHANDLER ((RtosTaskId) UINT8_C(5))
#define RTOS_TASK_ID_UART5_IRQHANDLER ((RtosTaskId) UINT8_C(6))
#define RTOS_TASK_ID_USART1_IRQHANDLER ((RtosTaskId) UINT8_C(7))
#define RTOS_TASK_ID_USART2_IRQHANDLER ((RtosTaskId) UINT8_C(8))
#define RTOS_TASK_ID_USART6_IRQHANDLER ((RtosTaskId) UINT8_C(9))
#define RTOS_TASK_ID_SCHEDULER ((RtosTaskId) UINT8_C(10))
#define RTOS_TASK_ID_TOWER_TASK_LOOP_SENSORSCAPTURETASK_72_PROC ((RtosTaskId) UINT8_C(11))
#define RTOS_TASK_ID_TOWER_TASK_LOOP_PROXY_CONTROLLAW_367_PROC ((RtosTaskId) UINT8_C(12))
#define RTOS_TASK_ID_TOWER_TASK_LOOP_PROXY_USERINPUT_358_PROC ((RtosTaskId) UINT8_C(13))
#define RTOS_TASK_ID_TOWER_TASK_LOOP_UIMUXTASK_170_PROC ((RtosTaskId) UINT8_C(14))
#define RTOS_TASK_ID_TOWER_TASK_LOOP_CONTROLLAWTASK_146_PROC ((RtosTaskId) UINT8_C(15))
#define RTOS_TASK_ID_TOWER_TASK_LOOP_PPMINPUTTOWER_102_PROC ((RtosTaskId) UINT8_C(16))
#define RTOS_TASK_ID_TOWER_TASK_LOOP_MOTMIX_552_PROC ((RtosTaskId) UINT8_C(17))
#define RTOS_TASK_ID_TOWER_TASK_LOOP_MOTORS_STATE_572_PROC ((RtosTaskId) UINT8_C(18))
#define RTOS_TASK_ID_TOWER_TASK_LOOP_CONTROL_STATE_563_PROC ((RtosTaskId) UINT8_C(19))
#define RTOS_TASK_ID_TOWER_TASK_LOOP_CONTROL_396_PROC ((RtosTaskId) UINT8_C(20))
#define RTOS_TASK_ID_TOWER_TASK_LOOP_POSITION_STATE_633_PROC ((RtosTaskId) UINT8_C(21))
#define RTOS_TASK_ID_TOWER_TASK_LOOP_PX4IOAR_587_PROC ((RtosTaskId) UINT8_C(22))
#define RTOS_TASK_ID_TOWER_TASK_LOOP_COMMSECRECOVERYTASK_1433_PROC ((RtosTaskId) UINT8_C(23))
#define RTOS_TASK_ID_TOWER_TASK_LOOP_PROXY_NAV_LAW_385_PROC ((RtosTaskId) UINT8_C(24))
#define RTOS_TASK_ID_TOWER_TASK_LOOP_PROXY_NAVSETPT_376_PROC ((RtosTaskId) UINT8_C(25))
#define RTOS_TASK_ID_TOWER_TASK_LOOP_NAVIGATION_199_PROC ((RtosTaskId) UINT8_C(26))
#define RTOS_TASK_ID_TOWER_TASK_LOOP_MAVLINKINPUTTASK_124_PROC ((RtosTaskId) UINT8_C(27))
#define RTOS_TASK_ID_TOWER_TASK_LOOP_GCSRECEIVETASK_UART1_709_PROC ((RtosTaskId) UINT8_C(28))
#define RTOS_TASK_ID_TOWER_TASK_LOOP_GCSTRANSMITTASK_UART1_906_PROC ((RtosTaskId) UINT8_C(29))
#define RTOS_TASK_ID_TOWER_TASK_LOOP_GCSRECEIVETASK_UART5_1105_PROC ((RtosTaskId) UINT8_C(30))
#define RTOS_TASK_ID_TOWER_TASK_LOOP_GCSTRANSMITTASK_UART5_1302_PROC ((RtosTaskId) UINT8_C(31))
#define RTOS_TASK_ID_TOWER_TASK_LOOP_ENCRYPTTASK_UART5_1294_PROC ((RtosTaskId) UINT8_C(32))
#define RTOS_TASK_ID_TOWER_TASK_LOOP_DECRYPTTASK_UART5_1092_PROC ((RtosTaskId) UINT8_C(33))
#define RTOS_TASK_ID_TOWER_TASK_LOOP_RADIO_STATUS_UART5_1083_PROC ((RtosTaskId) UINT8_C(34))
#define RTOS_TASK_ID_TOWER_TASK_LOOP_DATALINK_UART5_1050_PROC ((RtosTaskId) UINT8_C(35))
#define RTOS_TASK_ID_TOWER_TASK_LOOP_ENCRYPTTASK_UART1_898_PROC ((RtosTaskId) UINT8_C(36))
#define RTOS_TASK_ID_TOWER_TASK_LOOP_DECRYPTTASK_UART1_696_PROC ((RtosTaskId) UINT8_C(37))
#define RTOS_TASK_ID_TOWER_TASK_LOOP_RADIO_STATUS_UART1_687_PROC ((RtosTaskId) UINT8_C(38))
#define RTOS_TASK_ID_TOWER_TASK_LOOP_DATALINK_UART1_654_PROC ((RtosTaskId) UINT8_C(39))
#define RTOS_TASK_ID_TOWER_TASK_LOOP_UBLOXGPS_54_PROC ((RtosTaskId) UINT8_C(40))
#define RTOS_TASK_ID_TOWER_TASK_LOOP_BLINK_542_PROC ((RtosTaskId) UINT8_C(41))





#define rtos_signal_wait(requested_signal) \
    (void) rtos_signal_wait_set(requested_signal)

#define rtos_signal_poll(requested_signal) \
    (rtos_signal_poll_set(requested_signal) != RTOS_SIGNAL_SET_EMPTY)

#define rtos_signal_peek(requested_signal) \
    (rtos_signal_peek_set(requested_signal) != RTOS_SIGNAL_SET_EMPTY)

#define rtos_signal_send(task_id, signal) \
    rtos_signal_send_set(task_id, signal)


/* Map of task ID to entry function */
extern void *entry_fn[42];














extern RtosTicksAbsolute rtos_timer_current_ticks;











RtosSignalSet rtos_signal_wait_set(RtosSignalSet requested_signals) RTOS_REENTRANT;
RtosSignalSet rtos_signal_poll_set(RtosSignalSet requested_signals);
RtosSignalSet rtos_signal_peek_set(RtosSignalSet requested_signals);
void rtos_signal_send_set(RtosTaskId task_id, RtosSignalSet signals);
void rtos_timer_tick(void);
void rtos_timer_enable(RtosTimerId timer_id);
void rtos_timer_disable(RtosTimerId timer_id);
void rtos_timer_oneshot(RtosTimerId timer_id, RtosTicksRelative timeout);
bool rtos_timer_check_overflow(RtosTimerId timer_id);
RtosTicksRelative rtos_timer_remaining(RtosTimerId timer_id);
void rtos_timer_reload_set(RtosTimerId timer_id, RtosTicksRelative reload);
void rtos_timer_error_set(RtosTimerId timer_id, RtosErrorId error_id);
void rtos_timer_signal_set(RtosTimerId timer_id, RtosTaskId task_id, RtosSignalSet signal_set);
void rtos_interrupt_event_raise(RtosInterruptEventId event);


void rtos_mutex_lock(RtosMutexId) RTOS_REENTRANT;
bool rtos_mutex_lock_timeout(RtosMutexId, RtosTicksRelative) RTOS_REENTRANT;
bool rtos_mutex_try_lock(RtosMutexId);
void rtos_mutex_unlock(RtosMutexId);
bool rtos_mutex_holder_is_current(RtosMutexId);
void rtos_sem_post(RtosSemId);
bool rtos_sem_try_wait(RtosSemId);
void rtos_sem_wait(RtosSemId) RTOS_REENTRANT;
bool rtos_sem_wait_timeout(RtosSemId, RtosTicksRelative timeout)
        RTOS_REENTRANT;
void rtos_sem_max_init(RtosSemId s, RtosSemValue max);

RtosTaskId rtos_task_current(void);
void rtos_start(void);
void rtos_sleep(RtosTicksRelative ticks) RTOS_REENTRANT;

#endif /* RTOS_KOCHAB_H */
