#include "system_cfg.h"

#include "uart.h"
#include "uart_cfg.h"

#include "main.h"

#include <stdint.h>

#include <avr/io.h>
#include <avr/interrupt.h>

/****************************************************************************\
 * Buffers
\****************************************************************************/
static volatile uint8_t    Uart_RxBuffer[UART_BUFFER_SIZE];
static volatile uint8_t    Uart_TxBuffer[UART_BUFFER_SIZE];
static volatile uint16_t   Uart_RxReadIndex = 0;
static volatile uint16_t   Uart_RxWriteIndex = 0;
static volatile uint16_t   Uart_TxReadIndex = 0;
static volatile uint16_t   Uart_TxWriteIndex = 0;

static volatile uint8_t    Uart_Errors = 0;

/****************************************************************************\
 * Initializes UART
 *
 * Context: User
 *
\****************************************************************************/
void Uart_Init(void)
{
   /* Reset variables */
   Uart_RxReadIndex = 0;
   Uart_RxWriteIndex = 0;
   Uart_TxReadIndex = 0;
   Uart_TxWriteIndex = 0;
   Uart_Errors = 0;

   /* Enables USART */
   PRR &= ~(1 << PRUSART0);

   /* USART Initialization */
   UCSR0A = 1 << U2X0; /* Double Baud-rate */
   UCSR0B =
   (
      (0 << RXCIE0)  |  /* RX Complete Interrupt Enable */
      (0 << TXCIE0)  |  /* TX Complete Interrupt Enable */
      (0 << UDRIE0)  |  /* USART Data Register Empty Interrupt Enable */
      (0 << RXEN0)   |  /* Receiver Enable */
      (0 << TXEN0)   |  /* Transmitter Enable */
      (0 << UCSZ02)     /* Character Size - 8-bits */
   );

   UCSR0C =
   (
      (0 << UMSEL01) |  /* Mode Select - Asynchronous USART */
      (0 << UMSEL00) |  /* Mode Select - Asynchronous USART */
      (0 << UPM01)   |  /* Parity Mode - Disabled */
      (0 << UPM00)   |  /* Parity Mode - Disabled */
      (0 << USBS0)   |  /* Stop Bit Select - 1-bit */
      (1 << UCSZ01)  |  /* Character Size - 8-bits */
      (1 << UCSZ00)  |  /* Character Size - 8-bits */
      (0 << UCPOL0)     /* Clock Polarity -  zero when asynchronous mode */
   );

   /* Sets Baud Rate */
   UBRR0L = ((F_CPU / (8 * UART_BAUDRATE)) - 1) & 0xFF;
   UBRR0H = (((F_CPU / (8 * UART_BAUDRATE)) - 1) >> 8) & 0x0F;

   /* Starts USART */
   UCSR0B |=
   (
      (1 << RXCIE0)  |  /* RX Complete Interrupt Enable */
      (1 << RXEN0)   |  /* Receiver Enable */
      (1 << TXEN0)      /* Transmitter Enable */
   );

}

/****************************************************************************\
 * Returns active UART errors
 *
 * Context: User
 *
 * Return:
 *    bitmask of active errors
 *
\****************************************************************************/
uint8_t Uart_Get_Errors(void)
{
   /* Local copy of volatile errors */
   uint8_t errors;

   /* Atomic read & clear */
   cli();
   errors = Uart_Errors;
   Uart_Errors = 0;
   sei();

   /* Returns errors */
   return(errors);
}

/****************************************************************************\
 * Pulls (reads) already received data from Rx Buffer
 *
 * Context: User
 *
 * In:
 *    size     - maximal number of bytes to pull from the Rx Buffer
 *
 * Out:
 *    buffer   - pointer to buffer for data
 *
 * Return:
 *    number of bytes pulled from the Rx Buffer
 *
\****************************************************************************/
uint16_t Uart_Pull(uint16_t size, uint8_t *buffer)
{
   /* Number of pulled bytes */
   uint16_t data_cnt;

   /* Local copy of read index */
   uint16_t read_index;

   /* Local copy of write index */
   uint16_t write_index;

   /* Atomic indexes read */
   cli();
   read_index = Uart_RxReadIndex;
   write_index = Uart_RxWriteIndex;
   sei();

   /* Copies data from Rx Buffer */
   data_cnt = 0;
   while((data_cnt < size) && (read_index != write_index))
   {
      /* Copies byte */
      buffer[data_cnt] = Uart_RxBuffer[read_index];

      /* Increments counters */
      data_cnt++;
      read_index = (read_index < (UART_BUFFER_SIZE - 1)) ? (read_index + 1) : (0);
   }

   /* Atomic Indexes update - frees space in Rx Buffer */
   cli();
   Uart_RxReadIndex = read_index;
   write_index = Uart_RxWriteIndex;
   sei();

   /* Sets event if any data in buffer */
   if(write_index != read_index)
   {
      FromUser_Set_Event(EVENT_ID_UART_DATA_READY);
   }

   /* Returns number of bytes */
   return(data_cnt);
}

/****************************************************************************\
 * Pushes (writes) data to Tx Buffer
 *
 * Context: User
 *
 * In:
 *    size     - maximal number of bytes to push to the Rx Buffer
 *    buffer   - pointer to data buffer
 *
 * Return:
 *    number of bytes pushed to the Tx Buffer
 *
\****************************************************************************/
uint16_t Uart_Push(uint16_t size, const uint8_t *buffer)
{
   /* Number of pulled bytes */
   uint16_t data_cnt;

   /* Local copy of read index */
   uint16_t read_index;

   /* Local copy of write index */
   uint16_t write_index;

   /* Next value of write index */
   uint16_t next_write_index;

   /* Atomic indexes read */
   cli();
   read_index = Uart_TxReadIndex;
   write_index = Uart_TxWriteIndex;
   sei();

   /* Copies data to Tx Buffer */
   data_cnt = 0;
   next_write_index = (write_index < (UART_BUFFER_SIZE - 1)) ? (write_index + 1) : (0);
   while((data_cnt < size) && (next_write_index != read_index))
   {
      /* Copies byte */
      Uart_TxBuffer[write_index] = buffer[data_cnt];

      /* Increments counters */
      data_cnt++;
      write_index = next_write_index;
      next_write_index = (write_index < (UART_BUFFER_SIZE - 1)) ? (write_index + 1) : (0);
   }

   /* Atomic:
    *    Updates the Write Index
    *       - makes new data in Tx Buffer ready to send
    *    Enables UART Data Register Empty interrupt
    *       - restarts sending data by interrupts
    */
   cli();
   Uart_TxWriteIndex = write_index;
   UCSR0B |= 1 << UDRIE0;
   sei();

   /* Returns number of bytes */
   return(data_cnt);
}

/****************************************************************************\
 * Restores the Data Ready Event activation
 *
 * Comment:
 *    In response to the UART_DATA_READY event, the application should call
 *    the Uart_Pull() function, otherwise the event may not be activated
 *    again even if RxBuffer is not empty.
 *
 *    This function is dedicated for application to restore the event
 *    activation in this same way as by calling Uart_Pull() but without
 *    pulling any data from RxBuffer
 *
 * Context: User
 *
\****************************************************************************/
void Uart_Restore_DataReadyEvent(void)
{
   /* Local copy of read index */
   uint16_t read_index;

   /* Local copy of write index */
   uint16_t write_index;

   /* Atomic indexes read */
   cli();
   read_index = Uart_RxReadIndex;
   write_index = Uart_RxWriteIndex;
   sei();

   /* Sets event if any data in buffer */
   if(write_index != read_index)
   {
      FromUser_Set_Event(EVENT_ID_UART_DATA_READY);
   }
}

/****************************************************************************\
 * Interrupt: RX Complete
\****************************************************************************/
ISR(USART_RX_vect)
{
   /* Local copy or received byte */
   uint8_t data;

   /* Local copy of volatile write index */
   uint16_t write_index;

   /* Next value of write index */
   uint16_t next_write_index;

   /* Reads received data byte */
   data = UDR0;

   /* Reads write index */
   write_index = Uart_RxWriteIndex;

   /* Increments write index */
   next_write_index = (write_index < (UART_BUFFER_SIZE - 1)) ? (write_index + 1) : (0);

   /* Checks if is space in buffer */
   if(next_write_index != Uart_RxReadIndex)
   {
      /* Updates RX Buffer */
      Uart_RxBuffer[write_index] = data;
      Uart_RxWriteIndex = next_write_index;

      /* Sets event if first data in buffer */
      if(write_index == Uart_RxReadIndex)
      {
         FromIsr_Set_Event(EVENT_ID_UART_DATA_READY);
      }
   }
   else
   {
      /* Rx buffer overflows */
      Uart_Errors |= UART_ERROR_RX_BUFFER_OVERFLOW;
      FromIsr_Set_Event(EVENT_ID_UART_ERROR);
   }
}

/****************************************************************************\
 * Interrupt: Data Register Empty
\****************************************************************************/
ISR(USART_UDRE_vect)
{
   /* Local copy of volatile read index */
   uint16_t read_index;

   /* Reads read index */
   read_index = Uart_TxReadIndex;

   /* Checks if Tx Buffer is not empty */
   if(read_index != Uart_TxWriteIndex)
   {
      /* Writes the next data to send from data from the Tx Buffer */
      UDR0 = Uart_TxBuffer[read_index];

      /* Increments and updates the read index */
      Uart_TxReadIndex = (read_index < (UART_BUFFER_SIZE - 1)) ? (read_index + 1) : (0);
   }
   else
   {
      /* Empty Tx buffer - Stops sending by interrupts */
      UCSR0B &= ~(1 << UDRIE0);

      /* Set events */
      FromIsr_Set_Event(EVENT_ID_UART_TX_READY);
   }
}
