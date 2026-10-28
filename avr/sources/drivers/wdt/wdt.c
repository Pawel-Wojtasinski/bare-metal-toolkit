#include "system_cfg.h"

#include "drivers/wdt/wdt.h"
#include "drivers/wdt/wdt_cfg.h"

#include <stdint.h>

#include <avr/io.h>

/* Calculates Clock Selection Bits */
#if (WDT_PERIOD_MS == 16)
#define WDT_PRESCALER_BITS 0x00
#elif(WDT_PERIOD_MS == 32)
#define WDT_PRESCALER_BITS 0x01
#elif(WDT_PERIOD_MS == 64)
#define WDT_PRESCALER_BITS 0x02
#elif(WDT_PERIOD_MS == 125)
#define WDT_PRESCALER_BITS 0x03
#elif(WDT_PERIOD_MS == 250)
#define WDT_PRESCALER_BITS 0x04
#elif(WDT_PERIOD_MS == 500)
#define WDT_PRESCALER_BITS 0x05
#elif(WDT_PERIOD_MS == 1000)
#define WDT_PRESCALER_BITS 0x06
#elif(WDT_PERIOD_MS == 2000)
#define WDT_PRESCALER_BITS 0x07
#elif(WDT_PERIOD_MS == 4000)
#define WDT_PRESCALER_BITS 0x08
#elif(WDT_PERIOD_MS == 8000)
#define WDT_PRESCALER_BITS 0x09
#else
#error "Watch-dog Timer - Invalid prescaler value!"
#endif

/****************************************************************************\
 * Initializes Watch-dog Timer
 *
 * Context: User
 *
\****************************************************************************/
void Wdt_Init(void)
{
   /* Clears Wat-dog */
   asm("wdr");

   /* Clears Watch-dog reset flag */
   MCUSR &= ~(1 << WDRF);

   /* Enables Watch-dog Change */
   WDTCSR = (1 << WDCE) | (1 << WDE);

   /* Sets Watchdog Timer prescaler */
   WDTCSR = WDT_PRESCALER_BITS;

   /* Starts Watch-dog Timer */
   WDTCSR |= 1 << WDE;
}

/****************************************************************************\
 * Resets Watch-dog Timer
 *
 * Context: User
 *
\****************************************************************************/
void Wdt_Reset(void)
{
   asm("wdr");
}
