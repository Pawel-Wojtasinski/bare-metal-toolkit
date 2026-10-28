#include "system_cfg.h"

#include "drivers/pwm_1/pwm_1.h"
#include "drivers/pwm_1/pwm_1_cfg.h"

#include "main.h"

#include <stdint.h>

#include <avr/io.h>
#include <avr/interrupt.h>

/* Calculates Clock Selection Bits */
#if (PWM_1_PRESCALER_VALUE == 1)
#define PWM_1_PRESCALER_BITS 0x01
#elif(PWM_1_PRESCALER_VALUE == 8)
#define PWM_1_PRESCALER_BITS 0x02
#elif(PWM_1_PRESCALER_VALUE == 64)
#define PWM_1_PRESCALER_BITS 0x03
#elif(PWM_1_PRESCALER_VALUE == 256)
#define PWM_1_PRESCALER_BITS 0x04
#elif(PWM_1_PRESCALER_VALUE == 1024)
#define PWM_1_PRESCALER_BITS 0x05
#else
#error "PWM 1 - Invalid prescaler value!"
#endif

/* Calculates timer ticks */
#define PWM_1_TIMER_TICKS (F_CPU / (2UL * PWM_1_FREQ * PWM_1_PRESCALER_VALUE))

/****************************************************************************\
 * Calculates PWM in percents to PWM register value
 *
 * Context: User
 *
 * In:
 *    pwm   - pwm value (0 to 100[%])
 *
 * Return:
 *    PWM register value
 *
\****************************************************************************/
static uint32_t Pwm1_Calculate_RwmRegValue(uint8_t pwm)
{
   /* PWM value calculated to register value */
   uint32_t pwm_regvalue;

   /* Checks if PWM value is below 100% */
   if(pwm < 100)
   {
      /* Calculates new PWM register value */
      pwm_regvalue = pwm;
      pwm_regvalue *= PWM_1_TIMER_TICKS;
      pwm_regvalue = pwm_regvalue / 100L;

      /* Checks if PWM is over 0% but calculated value is 0% */
      if ((pwm > 0) && (pwm_regvalue == 0))
      {
         /* Prevents setting the permanent low state for non-zero values */
         pwm_regvalue = 1;
      }

      /* Checks if PWM is below 100% but calculated value is 100% */
      if ((pwm < 100) && (pwm_regvalue == PWM_1_TIMER_TICKS))
      {
         /* Prevents setting the permanent high state for value below 100% */
         pwm_regvalue = PWM_1_TIMER_TICKS - 1;
      }
   }
   else
   {
      /* Saturates pwm to PWM Limit (100 [%]) */
      pwm_regvalue = PWM_1_TIMER_TICKS;
   }

   return(pwm_regvalue);
}


/****************************************************************************\
 * Initializes Timer 1
 *
 * Context: User
 *
\****************************************************************************/
void Pwm1_Init(void)
{
   /* Enables Timer 1 */
   PRR &= ~(1 << PRTIM1);

   /* Control Register A */
   TCCR1A =
   (
      #if (PWM_1_CHANNEL_A_CONFIG == PWM_1_CONFIG_CHANNEL_POLARITY_NORMAL)
      (1 << COM1A1)  |  /* Compare Match Output A Mode - Clear pin on compare match */
      (0 << COM1A0)  |  /* Compare Match Output A Mode - Clear pin on compare match */
      #elif (PWM_1_CHANNEL_A_CONFIG == PWM_1_CONFIG_CHANNEL_POLARITY_INVERTED)
      (1 << COM1A1)  |  /* Compare Match Output A Mode - Clear pin on compare match */
      (1 << COM1A0)  |  /* Compare Match Output A Mode - Clear pin on compare match */
      #else
      #error "PWM_1 - Invalid configuration of channel A"
      #endif

      #if (PWM_1_CHANNEL_B_CONFIG== PWM_1_CONFIG_CHANNEL_POLARITY_NORMAL)
      (1 << COM1B1)  |  /* Compare Match Output B Mode - Clear pin on compare match */
      (0 << COM1B0)  |  /* Compare Match Output B Mode - Clear pin on compare match */
      #elif (PWM_1_CHANNEL_B_CONFIG== PWM_1_CONFIG_CHANNEL_POLARITY_INVERTED)
      (1 << COM1B1)  |  /* Compare Match Output B Mode - Clear pin on compare match */
      (1 << COM1B0)  |  /* Compare Match Output B Mode - Clear pin on compare match */
      #else
      #error "PWM_1 - Invalid configuration of channel B"
      #endif

      (0 << WGM11)   |  /* Waveform Generation Mode - Phase and frequency correct PWM, TOP = ICR1 */
      (0 << WGM10)      /* Waveform Generation Mode - Phase and frequency correct PWM, TOP = ICR1 */
   );

   /* Control Register B */
   TCCR1B =
   (
      (1 << WGM13)   |  /* Waveform Generation Mode - Phase and frequency correct PWM, TOP = ICR1 */
      (0 << WGM12)   |  /* Waveform Generation Mode - Phase and frequency correct PWM, TOP = ICR1 */
      (0 << CS12)    |  /* Clock Select - No clock */
      (0 << CS11)    |  /* Clock Select - No clock */
      (0 << CS10)       /* Clock Select - No clock */
   );

   /* Timer/Counter Register */
   TCNT1H = 0;  /* Resets Timer value */
   TCNT1L = 0;  /* Resets Timer value */

   /* Timer period */
   ICR1H = (PWM_1_TIMER_TICKS >> 8) & 0xFF;
   ICR1L = PWM_1_TIMER_TICKS & 0xFF;

   /* Output Compare Register A - PWM A value */
   OCR1AH = 0;
   OCR1AL = 0;

   /* Output Compare Register B - PWM B value */
   OCR1BH = 0;
   OCR1BL = 0;

   /* Interrupt Mask Register */
   TIMSK1 =
   (
      (0 << OCIE1B)  |  /* Timer/Counter Output Compare Match B Interrupt Enable */
      (0 << OCIE1A)  |  /* Timer/Counter0 Output Compare Match A Interrupt Enable */
      (0 << TOIE1)      /* Timer/Counter0 Overflow Interrupt Enable */
   );

   /* Interrupt Flag Register */
   TIFR1 =
   (
      (1 << OCF1B)   |  /* Timer/Counter 0 Output Compare B Match Flag - clears interrupt */
      (1 << OCF1A)   |  /* Timer/Counter 0 Output Compare A Match Flag  - clears interrupt*/
      (1 << TOV1)       /* Timer/Counter0 Overflow Flag  - clears interrupt */
   );

   /* Starts HW Timer */
   TCCR1B |= PWM_1_PRESCALER_BITS;

   /* Initializes pin state - channel A */
   #if (PWM_1_CHANNEL_A_CONFIG == PWM_1_CONFIG_CHANNEL_POLARITY_NORMAL)
   PORTB &= ~(1 << PORTB1);
   #elif (PWM_1_CHANNEL_A_CONFIG == PWM_1_CONFIG_CHANNEL_POLARITY_INVERTED)
   PORTB |= 1 << PORTB1;
   #else
   #error "PWM_1 - Invalid configuration of channel A"
   #endif

   /* Initializes pin state - channel B */
   #if (PWM_1_CHANNEL_B_CONFIG == PWM_1_CONFIG_CHANNEL_POLARITY_NORMAL)
   PORTB &= ~(1 << PORTB2);
   #elif (PWM_1_CHANNEL_B_CONFIG == PWM_1_CONFIG_CHANNEL_POLARITY_INVERTED)
   PORTB |= 1 << PORTB2;
   #else
   #error "PWM_1 - Invalid configuration of channel B"
   #endif

   /* Configures pins as outputs */
   DDRB |= (1 << DDB1) | (1 << DDB2);
}

/****************************************************************************\
 * Sets new PWM value of channel A
 *
 * Context: User
 *
 * In:
 *    pwm   - pwm value to set (0 to 100[%])
 *
\****************************************************************************/
void Pwm1_Set_A(uint8_t pwm)
{
   /* PWM value calculated to register value */
   uint32_t pwm_regvalue = Pwm1_Calculate_RwmRegValue(pwm);

   /* Updates PWM register */
   OCR1AH = (pwm_regvalue >> 8) & 0xFF;
   OCR1AL = pwm_regvalue & 0xFF;
}

/****************************************************************************\
 * Sets new PWM value of channel B
 *
 * Context: User
 *
 * In:
 *    pwm   - pwm value to set (0 to 100[%])
 *
\****************************************************************************/
void Pwm1_Set_B(uint8_t pwm)
{
   /* PWM value calculated to register value */
   uint32_t pwm_regvalue = Pwm1_Calculate_RwmRegValue(pwm);

   /* Updates PWM register */
   OCR1BH = (pwm_regvalue >> 8) & 0xFF;
   OCR1BL = pwm_regvalue & 0xFF;
}
