#ifndef BLINK_H_
#define BLINK_H_

/* Configuration of the application periodic function */
#define BLINK_START_MS  1000
#define BLINK_PERIOD_MS 1000

/* Application API */
void Blink_Init(void);
void Blink_PeriodicFunction(void);

#endif
