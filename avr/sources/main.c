#include "system_cfg.h"

#include "main.h"
#include "main_cfg.h"

#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>

#include "errors.h"
#include "drivers/timer_0/timer_0.h"

/****************************************************************************\
 * Event handler definition
\****************************************************************************/
#define EVENT_HANDLER(event_id, event_function) {if((events & ((uint32_t)1 << (event_id))) != 0) event_function();}

/****************************************************************************\
 * Events list
\****************************************************************************/
static volatile uint32_t Main_Events = 0;

/****************************************************************************\
 * System timers
\****************************************************************************/
#define MAIN_TIMER(name, start, period, event_id) \
static uint16_t Main_Timer_##name = (start) / (SYSTEM_TICK_PERIOD_MS);
MAIN_TIMERS
#undef MAIN_TIMER

/****************************************************************************/

static void Main_Timer(void);

/****************************************************************************\
 * Main function
\****************************************************************************/
int main(void)
{
   /* Configures system clock divider */
   CLKPR = 1 << CLKPCE;
   CLKPR = 0;  /* Clock divider = 1 */

   /* Enables Port Pull-Ups */
   MCUCR &= ~(1 << PUD);

   /* Turn off all peripherals - drivers should turn on all peripherals in use */
   PRR |= 0xEF;

   /* System Tick (Timer 0) Initialization */
   Timer0_Init();

   /* Drivers initialization */
   Main_DrvInit();

   /* Apps initialization */
   Main_AppInit();

   /* Starts system */
   Main_Events = 0;
   sei();

   /* Starts System Tick (Timer 0) */
   Timer0_Start();

   /* Basic scheduler */
   while(!0)
   {
      /* Local copy of Main_Events volatile variable */
      uint32_t events;

      /* Handle events - Atomic read & clear */
      cli();
      events = Main_Events;
      Main_Events = 0;
      sei();

      /* Handle events - Basic scheduler */
      Main_EventsHandler();

      /* Sleep handler */
      cli();
      if(Main_Events == 0)
      {
         /* Enables Idle sleep Mode */
         SMCR =
         (
            (0 << SM2)  |  /* Sleep Mode: 0 -> Idle */
            (0 << SM1)  |
            (0 << SM0)  |
            (1 << SE)      /* Sleep Enable */
         );

         /* Enters Sleep Mode */
         sei();
         sleep_cpu();

         /* Disables Sleep Mode */
         SMCR &= ~(1 << SE);
      }
      sei();
   }
}

/****************************************************************************\
 * Implementation of system timers
\****************************************************************************/
static void Main_Timer(void)
{
   #define MAIN_TIMER(name, start, period, event_id)              \
   /* Checks if Timer is started */                               \
   if(Main_Timer_##name > 0)                                      \
   {                                                              \
      /* Checks if Timer is decremented to zero */                \
      if(Main_Timer_##name == 1)                                  \
      {                                                           \
         /* Timer is decremented to zero - restarts Timer */      \
         Main_Timer_##name = (period) / (SYSTEM_TICK_PERIOD_MS);  \
         /* Sets event */                                         \
         FromUser_Set_Event(event_id);                            \
      }                                                           \
      else                                                        \
      {                                                           \
         /* Decrementing timer*/                                  \
         Main_Timer_##name--;                                     \
      }                                                           \
   };                                                             \
   /* End */
   MAIN_TIMERS
   #undef MAIN_TIMER
}

/****************************************************************************\
 * Interrupts: Set event
 *
 * Context: ISR
 *
 * In:
 *    event_id - Event ID of the event to set
\****************************************************************************/
void FromIsr_Set_Event(uint8_t event_id)
{
   if((SREG & (1<< SREG_I)) == 0)
   {
      if(event_id < 32)
      {
         Main_Events |= 1UL << event_id;
      }
      else
      {
         Error_Report("FromIsr_Set_Event" ,Error_EventIdOutOfRange);
      }
   }
   else
   {
      Error_Report("FromIsr_Set_Event" ,Error_IsrFromUser);
   }
}

/****************************************************************************\
 * Interrupts: Set event
 *
 * Context: User
 *
 * In:
 *    event_id - Event ID of the event to set
\****************************************************************************/
void FromUser_Set_Event(uint8_t event_id)
{
   if((SREG & (1<< SREG_I)) != 0)
   {
      if(event_id < 32)
      {
         Main_Events |= 1UL << event_id;
      }
      else
      {
         Error_Report("FromUser_Set_Event" ,Error_EventIdOutOfRange);
      }
   }
   else
   {
      Error_Report("FromUser_Set_Event" ,Error_UserFromIsr);
   }
}
