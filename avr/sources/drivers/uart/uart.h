#ifndef UART_H_
#define UART_CFG_H_

#include <stdint.h>

/* Errors bits */
#define UART_ERROR_RX_BUFFER_OVERFLOW     (1 << 0)

/* Functions */
void Uart_Init(void);
uint8_t Uart_Get_Errors(void);
uint16_t Uart_Pull(uint16_t size, uint8_t *buffer);
uint16_t Uart_Push(uint16_t size, const uint8_t *buffer);

void Uart_Restore_DataReadyEvent(void);

#endif
