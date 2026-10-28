#include "system_cfg.h"

#include "drivers/timer_0/timer_0.h"
#include "drivers/timer_0/timer_0_cfg.h"

#include "main.h"

#include <stdint.h>

#include <avr/io.h>
#include <avr/interrupt.h>

/* Calculates Clock Selection Bits */
#if (TIMER_0_PRESCALER_VALUE == 1)
#define TIMER_0_PRESCALER_BITS 0x01
#elif(TIMER_0_PRESCALER_VALUE == 8)
#define TIMER_0_PRESCALER_BITS 0x02
#elif(TIMER_0_PRESCALER_VALUE == 64)
#define TIMER_0_PRESCALER_BITS 0x03
#elif(TIMER_0_PRESCALER_VALUE == 256)
#define TIMER_0_PRESCALER_BITS 0x04
#elif(TIMER_0_PRESCALER_VALUE == 1024)
#define TIMER_0_PRESCALER_BITS 0x05
#else
#error "Timer 0 - Invalid prescaler value!"
#endif

/****************************************************************************\
 * Software timer counter
\****************************************************************************/
volatile uint16_t Timer0_Cnt = 0;

/****************************************************************************\
 * Initializes Timer 0
 *
 * Context: User
 *
\****************************************************************************/
void Timer0_Init(void)
{
   /* Initializes SW Timer */
   Timer0_Cnt = (((uint32_t)TIMER_0_FREQ * (uint32_t)TIMER_0_PERIOD_MS) / 1000) - 1;

   /* Enables Timer 0 */
   PRR &= ~(1 << PRTIM0);

   /* Control Register A */
   TCCR0A =
   (
      (0 << COM0A1)  |  /* Compare Match Output A Mode - Normal pin operation, OC0A disconnected */
      (0 << COM0A0)  |  /* Compare Match Output A Mode - Normal pin operation, OC0A disconnected */
      (0 << COM0B1)  |  /* Compare Match Output B Mode - Normal pin operation, OC0B disconnected */
      (0 << COM0B0)  |  /* Compare Match Output B Mode - Normal pin operation, OC0B disconnected */
      (1 << WGM01)   |  /* Waveform Generation Mode - CTC */
      (0 << WGM00)      /* Waveform Generation Mode - CTC */
   );

   /* Control Register B */
   TCCR0B =
   (
      (0 << WGM02)   |  /* Waveform Generation Mode - CTC */
      (0 << CS02)    |  /* Clock Select - No clock */
      (0 << CS01)    |  /* Clock Select - No clock */
      (0 << CS00)       /* Clock Select - No clock */
   );

   /* Timer/Counter Register */
   TCNT0 = 0;  /* Resets Timer value */

   /* Output Compare Register A - Timer period */
   OCR0A = ((uint32_t)F_CPU / ((uint32_t)TIMER_0_FREQ * (uint32_t)TIMER_0_PRESCALER_VALUE)) - 1;

   /* Output Compare Register B */
   OCR0B = 0;  /* Not used */

   /* Interrupt Mask Register */
   TIMSK0 =
   (
      (0 << OCIE0B)  |  /* Timer/Counter Output Compare Match B Interrupt Enable */
      (1 << OCIE0A)  |  /* Timer/Counter0 Output Compare Match A Interrupt Enable */
      (0 << TOIE0)      /* Timer/Counter0 Overflow Interrupt Enable */
   );

   /* Interrupt Flag Register */
   TIFR0 =
   (
      (1 << OCF0B)   |  /* Timer/Counter 0 Output Compare B Match Flag - clears interrupt */
      (1 << OCF0A)   |  /* Timer/Counter 0 Output Compare A Match Flag  - clears interrupt*/
      (1 << TOV0)       /* Timer/Counter0 Overflow Flag  - clears interrupt */
   );
}

/****************************************************************************\
 * Starts Timer 0
 *
 * Context: User
 *
\****************************************************************************/
void Timer0_Start(void)
{
   /* Initializes SW Timer */
   Timer0_Cnt = (((uint32_t)TIMER_0_FREQ * (uint32_t)TIMER_0_PERIOD_MS) / 1000) - 1;

   /* Starts HW Timer */
   TCCR0B |= TIMER_0_PRESCALER_BITS;
}

/****************************************************************************\
 * Interrupt: Data Register Empty
\****************************************************************************/
ISR(TIMER0_COMPA_vect)
{
   /* Local copy of volatile SW Timer value */
   uint16_t cnt;

   /* Reads SW Timer value */
   cnt = Timer0_Cnt;

   /* Checks if counter has been decremented to zero */
   if(cnt == 0)
   {
      /* The SW Timer has reached zero - Restarts SW Timer */
      cnt = (((uint32_t)TIMER_0_FREQ * (uint32_t)TIMER_0_PERIOD_MS) / 1000) - 1;

      /* Sets Timer 0 event */
      FromIsr_Set_Event(EVENT_ID_TIMER_0);
   }
   else
   {
      /* Decrement counter */
      cnt--;
   }

   /* Updates SW Timer value */
   Timer0_Cnt = cnt;
}
