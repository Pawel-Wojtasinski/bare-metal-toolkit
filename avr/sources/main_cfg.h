#ifndef MAIN_CFG_H_
#define MAIN_CFG_H_

#include <stdint.h>

/*--------------------------------------------------------------------------*/

#include "drivers/wdt/wdt.h"
#include "drivers/uart/uart.h"

/*--------------------------------------------------------------------------*/

#include "apps/echo/echo.h"
#include "apps/blink/blink.h"

/*--------------------------------------------------------------------------*/
/* Drivers initialization                                                   */
/*--------------------------------------------------------------------------*/
#define Main_DrvInit()  \
{                       \
   Wdt_Init();          \
   Uart_Init();         \
}                       \
/* End */

/*--------------------------------------------------------------------------*/
/* Apps initialization                                                      */
/*--------------------------------------------------------------------------*/
#define Main_AppInit()  \
{                       \
   Echo_Init();         \
   Blink_Init();        \
}                       \
/* End */

/*--------------------------------------------------------------------------*/
/* Event IDs - may be equal if this same handl is used                      */
/*--------------------------------------------------------------------------*/
#define EVENT_ID_SYSTEM_TICK           0
#define EVENT_ID_TIMER_0               EVENT_ID_SYSTEM_TICK

#define EVENT_ID_APP_BLINK_PERIODIC    1

#define EVENT_ID_UART_DATA_READY       2
#define EVENT_ID_UART_TX_READY         3
#define EVENT_ID_UART_ERROR            4

#define EVENT_ID_WDT_PERIODIC          31

/*--------------------------------------------------------------------------*/
/* Event ID to function mapping                                             */
/*--------------------------------------------------------------------------*/
#define Main_EventsHandler()                                                  \
{                                                                             \
   /*            Event ID,                      Function                   */ \
   EVENT_HANDLER(EVENT_ID_SYSTEM_TICK,          Main_Timer);                  \
   EVENT_HANDLER(EVENT_ID_APP_BLINK_PERIODIC,   Blink_PeriodicFunction);      \
   EVENT_HANDLER(EVENT_ID_UART_DATA_READY,      Echo_Event_DataReady);        \
   EVENT_HANDLER(EVENT_ID_UART_TX_READY,        Echo_Event_TxReady);          \
   EVENT_HANDLER(EVENT_ID_UART_ERROR,           Echo_Event_Error);            \
   EVENT_HANDLER(EVENT_ID_WDT_PERIODIC,         Wdt_Reset);                   \
}
/* End */

/*--------------------------------------------------------------------------*/
/* System timers configuration                                              */
/*--------------------------------------------------------------------------*/
#define MAIN_TIMERS                                                                                                     \
   /*         Name,                    Start time,             Period,                 Event ID                      */ \
   MAIN_TIMER(Blink_PeriodicFunction,  BLINK_START_MS,         BLINK_PERIOD_MS,        EVENT_ID_APP_BLINK_PERIODIC)     \
   MAIN_TIMER(Wdt_PeriodicFunction,    WATCHDOG_PERIOD_MS / 4, WATCHDOG_PERIOD_MS / 4, EVENT_ID_WDT_PERIODIC)           \
/* End */

#endif
