#include "blink.h"
#include "blink_cfg.h"

#include <avr/io.h>

/****************************************************************************\
 * Blink application initialization
 *
 * Context: User
 *
\****************************************************************************/
void Blink_Init(void)
{
   /* Configures LED pin as output */
   LED_DDR |= 1 << LED_DDR_BIT;
}

/****************************************************************************\
 * Blink application periodic function
 *
 * Context: User
 *
\****************************************************************************/
void Blink_PeriodicFunction(void)
{
   LED_PORT ^= 1 << LED_PORT_BIT;
}
