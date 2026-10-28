#ifndef SYSTEM_CFG_H_
#define SYSTEM_CFG_H_

#ifndef __AVR_ATmega328P__
#define __AVR_ATmega328P__
#endif
/* CPU Frequency */
#define F_CPU 16000000UL

/* System Tick Period [ms] */
#define SYSTEM_TICK_PERIOD_MS 100

/* Watchdog Period [ms] */
#define WATCHDOG_PERIOD_MS 1000

#endif
