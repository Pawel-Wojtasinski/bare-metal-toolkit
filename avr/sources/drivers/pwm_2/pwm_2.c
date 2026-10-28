#include "system_cfg.h"

#include "drivers/pwm_2/pwm_2.h"
#include "drivers/pwm_2/pwm_2_cfg.h"

#include "main.h"

#include <stdint.h>

#include <avr/io.h>
#include <avr/interrupt.h>

/* Calculates Clock Selection Bits */
#if (PWM_2_PRESCALER_VALUE == 1)
#define PWM_2_PRESCALER_BITS 0x01
#elif(PWM_2_PRESCALER_VALUE == 8)
#define PWM_2_PRESCALER_BITS 0x02
#elif(PWM_2_PRESCALER_VALUE == 64)
#define PWM_2_PRESCALER_BITS 0x03
#elif(PWM_2_PRESCALER_VALUE == 256)
#define PWM_2_PRESCALER_BITS 0x04
#elif(PWM_2_PRESCALER_VALUE == 1024)
#define PWM_2_PRESCALER_BITS 0x05
#else
#error "PWM 2 - Invalid prescaler value!"
#endif

/* Calculates timer ticks */
#define PWM_2_TIMER_TICKS (F_CPU / (2UL * PWM_2_FREQ * PWM_2_PRESCALER_VALUE))
#if(PWM_2_TIMER_TICKS > 255)
#error "PWM 2 - configured frequency is out of range!"
#endif

/****************************************************************************\
 * Initializes Timer 0
 *
 * Context: User
 *
\****************************************************************************/
void Pwm2_Init(void)
{
   /* Enables Timer 1 */
   PRR &= ~(1 << PRTIM2);

   /* Control Register A */
   TCCR2A =
   (
      (0 << COM2A1)  |  /* Compare Match Output A Mode - Normal port operation, OC0A disconnected */
      (0 << COM2A0)  |  /* Compare Match Output A Mode - Normal port operation, OC0A disconnected */

      #if (PWM_2_CHANNEL_CONFIG== PWM_2_CONFIG_CHANNEL_POLARITY_NORMAL)
      (1 << COM2B1)  |  /* Compare Match Output B Mode - Clear pin on compare match */
      (0 << COM2B0)  |  /* Compare Match Output B Mode - Clear pin on compare match */
      #elif (PWM_2_CHANNEL_CONFIG== PWM_2_CONFIG_CHANNEL_POLARITY_INVERTED)
      (1 << COM2B1)  |  /* Compare Match Output B Mode - Clear pin on compare match */
      (1 << COM2B0)  |  /* Compare Match Output B Mode - Clear pin on compare match */
      #else
      #error "PWM_2 - Invalid channel configuration"
      #endif

      (0 << WGM21)   |  /* Waveform Generation Mode - Phase correct PWM, TOP = OCRA */
      (1 << WGM20)      /* Waveform Generation Mode - Phase correct PWM, TOP = OCRA */
   );

   /* Control Register B */
   TCCR2B =
   (
      (1 << WGM22)   |  /* Waveform Generation Mode - Phase correct PWM, TOP = OCRA */
      (0 << CS22)    |  /* Clock Select - No clock */
      (0 << CS21)    |  /* Clock Select - No clock */
      (0 << CS20)       /* Clock Select - No clock */
   );

   /* Timer/Counter Register */
   TCNT2 = 0;  /* Resets Timer value */

   /* Output Compare Register A - Timer period */
   OCR2A = PWM_2_TIMER_TICKS & 0xFF;

   /* Output Compare Register B - PWM value */
   OCR2B = 0;

   /* Interrupt Mask Register */
   TIMSK2 =
   (
      (0 << OCIE2B)  |  /* Timer/Counter Output Compare Match B Interrupt Enable */
      (0 << OCIE2A)  |  /* Timer/Counter0 Output Compare Match A Interrupt Enable */
      (0 << TOIE2)      /* Timer/Counter0 Overflow Interrupt Enable */
   );

   /* Interrupt Flag Register */
   TIFR2 =
   (
      (1 << OCF2B)   |  /* Timer/Counter 0 Output Compare B Match Flag - clears interrupt */
      (1 << OCF2A)   |  /* Timer/Counter 0 Output Compare A Match Flag  - clears interrupt*/
      (1 << TOV2)       /* Timer/Counter0 Overflow Flag  - clears interrupt */
   );

   /* Starts HW Timer */
   TCCR2B |= PWM_2_PRESCALER_BITS;

   /* Initializes pin state */
   #if (PWM_2_CHANNEL_CONFIG == PWM_2_CONFIG_CHANNEL_POLARITY_NORMAL)
   PORTD &= ~(1 << PORTD3);
   #elif (PWM_2_CHANNEL_CONFIG == PWM_2_CONFIG_CHANNEL_POLARITY_INVERTED)
   PORTD |= 1 << PORTD3;
   #else
   #error "PWM_2 - Invalid channel configuration"
   #endif

   /* Configures pins as outputs */
   DDRD |= 1 << DDD3;
}

/****************************************************************************\
 * Sets new PWM value
 *
 * Context: User
 *
 * In:
 *    pwm   - pwm value to set (0 to 100[%])
 *
\****************************************************************************/
void Pwm2_Set(uint8_t pwm)
{
   /* PWM value calculated to register value */
   uint32_t pwm_regvalue;

   /* Checks if PWM value is below 100% */
   if(pwm < 100)
   {
      /* Calculates new PWM register value */
      pwm_regvalue = pwm;
      pwm_regvalue *= PWM_2_TIMER_TICKS;
      pwm_regvalue = pwm_regvalue / 100L;

      /* Checks if PWM is over 0% but calculated value is 0% */
      if ((pwm > 0) && (pwm_regvalue == 0))
      {
         /* Prevents setting the permanent low state for non-zero values */
         pwm_regvalue = 1;
      }

      /* Checks if PWM is below 100% but calculated value is 100% */
      if ((pwm < 100) && (pwm_regvalue == PWM_2_TIMER_TICKS))
      {
         /* Prevents setting the permanent high state for value below 100% */
         pwm_regvalue = PWM_2_TIMER_TICKS - 1;
      }
   }
   else
   {
      /* Saturates pwm to PWM Limit (100 [%]) */
      pwm_regvalue = PWM_2_TIMER_TICKS;
   }

   /* Updates PWM register */
   OCR2B = pwm_regvalue & 0xFF;
}
