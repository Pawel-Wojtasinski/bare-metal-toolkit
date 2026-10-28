#ifndef PWM_2_CFG_H_
#define PWM_2_CFG_H_

#include "system_cfg.h"

/* Channel configuration */
#define PWM_2_CONFIG_CHANNEL_POLARITY_NORMAL      0
#define PWM_2_CONFIG_CHANNEL_POLARITY_INVERTED    1

/* Internal Timer Frequency */
#define  PWM_2_FREQ              25000
#define  PWM_2_PRESCALER_VALUE   8

/* Configures channel - OC2B (PD3) */
#define PWM_2_CHANNEL_CONFIG PWM_2_CONFIG_CHANNEL_POLARITY_NORMAL

#endif
