
#include <stdint.h>
#include <ctype.h>

#include <assert.h>

#include <eChronos.h>
#include <rtos-kochab.h>

#define IRQ_WRAPPER_NAKED(x) \
        extern void x##_IRQHandler(void);                 \
        bool eChronos_##x##_IRQHandler(void)              \
        {                                                 \
            x##_IRQHandler();                         	  \
            return true;                                  \
        }                                                 \
        void x##_IRQHandler_wrapper(void)                 \
        {                                                 \
            while (1) {                                   \
                rtos_signal_wait(RTOS_SIGNAL_ID_GENERIC); \
            }                                             \
        }

/* If an interrupt handler intends to post to a semaphore, it must return the
 * handle of that semaphore, otherwise NULL.
 * The assertion below tests the assumption that no x_IRQHandler() can return
 * two different semaphore handles on subsequent calls before a previous call
 * has been processed by the x_IRQHandler_wrapper() thread. */

#define IRQ_WRAPPER_DEFER_SEM(x) \
        extern void *x##_IRQHandler(void);                \
        static void *x##_sem;                             \
        static int x##_sem_give_count;                    \
        bool eChronos_##x##_IRQHandler(void)              \
        {                                                 \
            void *sem;                                    \
            sem = x##_IRQHandler();                       \
            if (sem) {                                    \
                if (x##_sem_give_count) {                 \
                    assert(sem == x##_sem);               \
                } else {                                  \
                    x##_sem = sem;                        \
                }                                         \
                x##_sem_give_count++;                     \
                rtos_interrupt_event_raise(RTOS_INTERRUPT_EVENT_ID_##x); \
                return true;                              \
            }                                             \
            return false;                                 \
        }                                                 \
        void x##_IRQHandler_wrapper(void)                 \
        {                                                 \
            while (1) {                                   \
                int i, give_count;                        \
                void *give_sem;                           \
                rtos_signal_wait(RTOS_SIGNAL_ID_GENERIC); \
                ulPortSetInterruptMask();                 \
                give_count = x##_sem_give_count;          \
                give_sem = x##_sem;                       \
                x##_sem_give_count = 0;                   \
                vPortClearInterruptMask(0);               \
                for (i = 0; i < give_count; i++) {        \
                    xSemaphoreGive(give_sem);             \
                }                                         \
            }                                             \
        }

#define IRQ_WRAPPER(x) \
        bool eChronos_##x##_IRQHandler(void)              \
        {                                                 \
            rtos_interrupt_event_raise(RTOS_INTERRUPT_EVENT_ID_##x); \
            NVIC_DisableIRQ(x##_IRQn);                    \
            return true;                                  \
        }                                                 \
        void x##_IRQHandler_wrapper(void)                 \
        {                                                 \
            extern void x##_IRQHandler(void);             \
            while (1) {                                   \
                NVIC_EnableIRQ(x##_IRQn);                 \
                rtos_signal_wait(RTOS_SIGNAL_ID_GENERIC); \
                x##_IRQHandler();                         \
            }                                             \
        }

IRQ_WRAPPER_DEFER_SEM(I2C2_EV)
IRQ_WRAPPER_DEFER_SEM(I2C2_ER)
IRQ_WRAPPER_DEFER_SEM(SPI1)
IRQ_WRAPPER_NAKED(UART5)
//IRQ_WRAPPER_NAKED(USART1)
IRQ_WRAPPER_NAKED(USART2)
IRQ_WRAPPER_NAKED(USART6)
IRQ_WRAPPER_NAKED(TIM1_UP_TIM10)
IRQ_WRAPPER_NAKED(TIM1_CC)

int ivory_isr_sem_count = 0;
void *ivory_isr_sem_handle = NULL;

extern void USART1_IRQHandler(void);
static void *USART1_sem;
static int USART1_sem_give_count;

bool eChronos_USART1_IRQHandler(void)
{
    assert(ivory_isr_sem_count == 0);
    assert(ivory_isr_sem_handle == NULL);

    USART1_IRQHandler();

    /* This tests the theory that it just so happens that xSemaphoreGiveFromISR
     * is never called twice in a single run of the USART1 interrupt handler. */
    assert(ivory_isr_sem_count <= 1);

    if (ivory_isr_sem_count) {
        if (USART1_sem_give_count) {
            /* Assumes:
             * - USART1 is the only interrupt whose handler originates calls to
             *     ivory_echronos_semaphore_give_isr.
             * - ivory_echronos_semaphore_give_isr is the only caller of
             *     xSemaphoreGiveFromISR. */
            assert(ivory_isr_sem_handle == USART1_sem);
        } else {
            USART1_sem = ivory_isr_sem_handle;
        }
        ivory_isr_sem_handle = NULL;
        USART1_sem_give_count += ivory_isr_sem_count;
        ivory_isr_sem_count = 0;
        rtos_interrupt_event_raise(RTOS_INTERRUPT_EVENT_ID_USART1);
        return true;
    }

    return false;
}

/* NB: this is just the same as the version in DEFER_SEM macro */
void USART1_IRQHandler_wrapper(void)
{
    while (1) {
        int i, give_count;
        void *give_sem;

        rtos_signal_wait(RTOS_SIGNAL_ID_GENERIC);

        ulPortSetInterruptMask();
        give_count = USART1_sem_give_count;
        give_sem = USART1_sem;
        USART1_sem_give_count = 0;
        vPortClearInterruptMask(0);

        for (i = 0; i < give_count; i++) {
            xSemaphoreGive(give_sem);
        }
    }
}
