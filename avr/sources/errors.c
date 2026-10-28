#include "system_cfg.h"

#include <avr/io.h>
#include <avr/interrupt.h>

#include <util/delay.h>

/****************************************************************************\
 * Debug: Reports Error
 *
 * Context: All
 *
\****************************************************************************/
void Error_Report(char *func, char *error)
{
   while(!0)
   {
      PORTB |= 1 << PORTB5;
      _delay_ms(250);

      PORTB &= ~(1 << PORTB5);
      _delay_ms(2500);
   }
}
