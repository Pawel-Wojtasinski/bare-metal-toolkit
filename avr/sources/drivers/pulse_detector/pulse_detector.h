#ifndef PULSE_DETECTOR_H_
#define PULSE_DETECTOR_H_

#include <stdbool.h>

#include "pulse_detector_cfg.h"

/* Functions */
void PulseDetector_Init(void);

#ifdef PULSE_DETECTOR_CHANNEL_A_CONFIG
#if (PULSE_DETECTOR_CHANNEL_A_CONFIG != PULSE_DETECTOR_CONFIG_CHANNEL_DISABLED)
bool PulseDetector_Get_A(void);
#endif
#endif

#ifdef PULSE_DETECTOR_CHANNEL_B_CONFIG
#if (PULSE_DETECTOR_CHANNEL_B_CONFIG != PULSE_DETECTOR_CONFIG_CHANNEL_DISABLED)
bool PulseDetector_Get_B(void);
#endif
#endif

#ifdef PULSE_DETECTOR_CHANNEL_C_CONFIG
#if (PULSE_DETECTOR_CHANNEL_C_CONFIG != PULSE_DETECTOR_CONFIG_CHANNEL_DISABLED)
bool PulseDetector_Get_C(void);
#endif
#endif

#ifdef PULSE_DETECTOR_CHANNEL_D_CONFIG
#if (PULSE_DETECTOR_CHANNEL_D_CONFIG != PULSE_DETECTOR_CONFIG_CHANNEL_DISABLED)
bool PulseDetector_Get_D(void);
#endif
#endif

#ifdef PULSE_DETECTOR_CHANNEL_E_CONFIG
#if (PULSE_DETECTOR_CHANNEL_E_CONFIG != PULSE_DETECTOR_CONFIG_CHANNEL_DISABLED)
bool PulseDetector_Get_E(void);
#endif
#endif
#endif
