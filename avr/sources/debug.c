#include "debug.h"

#include "drivers/uart/uart.h"

#include <string.h>

void Debug_Send(uint16_t size, uint8_t *data)
{
   uint16_t data_cnt = 0;

   while(data_cnt < size)
   {
      data_cnt += Uart_Push(size - data_cnt, &data[data_cnt]);
   }
}

void Debug_Send_Text(char *text)
{
   Debug_Send(strlen(text), (uint8_t *)text);
}

void Debug_Send_Line(char *text)
{
   Debug_Send_Text(text);
   Debug_Send_Text("\r\n");
}

void Debug_Dump(char *text, uint8_t data)
{
   uint8_t b[2];

   b[0] = (data >> 4) & 0x0F;
   b[1] = data & 0x0F;

   b[0] += (b[0] < 10) ? ('0') : ('A' - 10);
   b[1] += (b[1] < 10) ? ('0') : ('A' - 10);

   Debug_Send_Text(text);
   Debug_Send(sizeof(b), b);
   Debug_Send_Text("\r\n");
}
