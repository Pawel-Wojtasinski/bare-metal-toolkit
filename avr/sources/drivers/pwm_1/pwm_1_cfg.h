#ifndef PWM_1_CFG_H_
#define PWM_1_CFG_H_

#include "system_cfg.h"

/* Channel configuration */
#define PWM_1_CONFIG_CHANNEL_POLARITY_NORMAL      0
#define PWM_1_CONFIG_CHANNEL_POLARITY_INVERTED    1

/* Internal Timer Frequency */
#define  PWM_1_FREQ              25000
#define  PWM_1_PRESCALER_VALUE   1

/* Configures channel A - OC1A (PD9) */
#define PWM_1_CHANNEL_A_CONFIG PWM_1_CONFIG_CHANNEL_POLARITY_NORMAL

/* Configures channel B - OC1B (PD10) */
#define PWM_1_CHANNEL_B_CONFIG PWM_1_CONFIG_CHANNEL_POLARITY_NORMAL

#endif
