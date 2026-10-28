#ifndef MAIN_H_
#define MAIN_H_

#include <stdint.h>

#include "main_cfg.h"

void FromIsr_Set_Event(uint8_t event_id);
void FromUser_Set_Event(uint8_t event_id);

#endif
