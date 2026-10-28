#ifndef DEBUG_H_
#define DEBUG_H_

#include <stdint.h>

void Debug_Send(uint16_t size, uint8_t *data);
void Debug_Send_Text(char *text);
void Debug_Send_Line(char *text);
void Debug_Dump(char *text, uint8_t data);

#endif
