
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
                rtos_signal_wait_set(SIGNAL_SET_IRQ_##x); \
            }                                             \
        }

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
                    assert(sem == x##_sem); /* XXX */     \
                } else {                                  \
                    x##_sem = sem;                        \
                }                                         \
                x##_sem_give_count++;                     \
                rtos_irq_event_raise(IRQ_EVENT_ID_##x);   \
                return true;                              \
            }                                             \
            return false;                                 \
        }                                                 \
        void x##_IRQHandler_wrapper(void)                 \
        {                                                 \
            while (1) {                                   \
                int i, give_count;                        \
                void *give_sem;                           \
                rtos_signal_wait_set(SIGNAL_SET_IRQ_##x); \
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
            rtos_irq_event_raise(IRQ_EVENT_ID_##x);       \
            NVIC_DisableIRQ(x##_IRQn);                    \
            return true;                                  \
        }                                                 \
        void x##_IRQHandler_wrapper(void)                 \
        {                                                 \
            extern void x##_IRQHandler(void);             \
            while (1) {                                   \
                NVIC_EnableIRQ(x##_IRQn);                 \
                rtos_signal_wait_set(SIGNAL_SET_IRQ_##x); \
                x##_IRQHandler();                         \
            }                                             \
        }


IRQ_WRAPPER_DEFER_SEM(I2C2_EV)
IRQ_WRAPPER_DEFER_SEM(I2C2_ER)
IRQ_WRAPPER_DEFER_SEM(SPI1)
//IRQ_WRAPPER_NAKED(UART5)
IRQ_WRAPPER_NAKED(USART1)
// IRQ_WRAPPER_NAKED(USART2)
IRQ_WRAPPER_NAKED(USART6)
IRQ_WRAPPER_NAKED(TIM1_UP_TIM10)
IRQ_WRAPPER_NAKED(TIM1_CC)

int ivory_isr_sem_count = 0;
void *ivory_isr_sem_handle = NULL;

extern void UART5_IRQHandler(void);
static void *UART5_sem;
static int UART5_sem_give_count;

bool eChronos_UART5_IRQHandler(void)
{
    assert(ivory_isr_sem_count == 0);
    assert(ivory_isr_sem_handle == NULL);

    UART5_IRQHandler();

    if (ivory_isr_sem_count > 1) {
        assert(ivory_isr_sem_count <= 1); /* XXX */
    }

    if (ivory_isr_sem_count) {
        if (UART5_sem_give_count) {
            assert(ivory_isr_sem_handle == UART5_sem); /* XXX */
        } else {
            UART5_sem = ivory_isr_sem_handle;
        }
        ivory_isr_sem_handle = NULL;
        UART5_sem_give_count += ivory_isr_sem_count;
        ivory_isr_sem_count = 0;
        rtos_irq_event_raise(IRQ_EVENT_ID_UART5);
        return true;
    }

    return false;
}

/* NB: this is just the same as the version in DEFER_SEM macro */
void UART5_IRQHandler_wrapper(void)
{
    while (1) {
        int i, give_count;
        void *give_sem;

        rtos_signal_wait_set(SIGNAL_SET_IRQ_UART5);

        ulPortSetInterruptMask();
        give_count = UART5_sem_give_count;
        give_sem = UART5_sem;
        UART5_sem_give_count = 0;
        vPortClearInterruptMask(0);

        for (i = 0; i < give_count; i++) {
            xSemaphoreGive(give_sem);
        }
    }
}
