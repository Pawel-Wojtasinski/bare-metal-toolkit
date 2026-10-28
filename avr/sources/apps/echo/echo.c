#include "echo.h"
#include "echo_cfg.h"

#include <stdbool.h>

#include "drivers/uart/uart.h"
#include "debug.h"

#if (ECHO_BUFFER_SIZE < 255)
typedef uint8_t Echo_BufferSize_t;
#else
typedef uint16_t Echo_BufferSize_t;
#endif

/****************************************************************************\
 * Data buffer
\****************************************************************************/
static uint8_t Echo_Buffer[ECHO_BUFFER_SIZE];
static Echo_BufferSize_t Echo_Buffer_ReadIndex = 0;
static Echo_BufferSize_t Echo_Buffer_WriteIndex = 0;

/****************************************************************************\
 * Local variables
\****************************************************************************/

/* Informs that receiving is stopped due to out of buffer */
static bool Echo_ReceivingIsStopped = false;

/****************************************************************************\
 * Sends data from buffer
\****************************************************************************/
static void Echo_Send(void)
{
   /* Number of pushed bytes */
   Echo_BufferSize_t data_cnt;

   /* Checks if the buffer is wrapped and there is data in the buffer */
   if(Echo_Buffer_ReadIndex > Echo_Buffer_WriteIndex)
   {
      /* Buffer is wrapped and there is data in the buffer - push data up to end of buffer */
      data_cnt = Uart_Push(ECHO_BUFFER_SIZE - Echo_Buffer_ReadIndex, &Echo_Buffer[Echo_Buffer_ReadIndex]);
      Echo_Buffer_ReadIndex += data_cnt;
      if(Echo_Buffer_ReadIndex >= ECHO_BUFFER_SIZE)
      {
         Echo_Buffer_ReadIndex -= ECHO_BUFFER_SIZE;
      }
   }

   /* Checks if the buffer is not wrapped and there is data in the buffer */
   if(Echo_Buffer_ReadIndex < Echo_Buffer_WriteIndex)
   {
      /* Buffer is not wrapped and there is data in the buffer - push data up to Write index */
      data_cnt = Uart_Push(Echo_Buffer_WriteIndex - Echo_Buffer_ReadIndex, &Echo_Buffer[Echo_Buffer_ReadIndex]);
      Echo_Buffer_ReadIndex += data_cnt;
   }
}

/****************************************************************************\
 * Echo application initialization
 *
 * Context: User
 *
\****************************************************************************/
void Echo_Init(void)
{
   Debug_Send_Text("\r\n");
   Debug_Send_Line("---> Reset! <---");
   Debug_Send_Text("\r\n");

   /* Initializes variables */
   Echo_Buffer_ReadIndex = 0;
   Echo_Buffer_WriteIndex = 0;
   Echo_ReceivingIsStopped = false;
}

/****************************************************************************\
 * Echo application event: Data is ready to pull form RX buffer
 *
 * Context: User
 *
\****************************************************************************/
void Echo_Event_DataReady(void)
{
   /* Number of pulled bytes */
   Echo_BufferSize_t data_cnt;

   /* Deadlocks prevention:
    * This event is activated only if:
    *  - The byte is received bu UART and the RxBuffer is empty (first byte of bulk)
    *  - The Uart_Pull() has been called and some data stays in RxBuffer
    * If buffer is full, the Uart_Pull() is not called and this event may not be activated again.
    *
    * This flag is set to inform that, the event may be lost and is cleared if Uart_Pull() has been called
   */
   Echo_ReceivingIsStopped = true;

   /* Checks if the buffer is not wrapped and is not full */
   if
   (
      (Echo_Buffer_WriteIndex >= Echo_Buffer_ReadIndex)
      &&
      (
         (Echo_Buffer_ReadIndex > 0)
         ||
         (Echo_Buffer_WriteIndex < (ECHO_BUFFER_SIZE - 1))
      )
   )
   {
      /* Buffer is not wrapped - Fills buffer to the end of buffer */
      data_cnt = Uart_Pull(ECHO_BUFFER_SIZE - Echo_Buffer_WriteIndex, &Echo_Buffer[Echo_Buffer_WriteIndex]);
      Echo_Buffer_WriteIndex += data_cnt;
      if(Echo_Buffer_WriteIndex >= ECHO_BUFFER_SIZE)
      {
         Echo_Buffer_WriteIndex -= ECHO_BUFFER_SIZE;
      }
      Echo_ReceivingIsStopped = false;
   }

   /* Checks if the buffer is wrapped and is not full */
   if((Echo_Buffer_WriteIndex + 1) < Echo_Buffer_ReadIndex)
   {
      /* Buffer is wrapped - Fills buffer from write index to the read index */
      data_cnt = Uart_Pull((Echo_Buffer_ReadIndex - Echo_Buffer_WriteIndex) - 1, &Echo_Buffer[Echo_Buffer_WriteIndex]);
      Echo_Buffer_WriteIndex += data_cnt;
      Echo_ReceivingIsStopped = false;
   }

   /* Tries to send data back */
   Echo_Send();
}

/****************************************************************************\
 * Echo application event: Tx Buffer is ready to push data
 *
 * Context: User
 *
\****************************************************************************/
void Echo_Event_TxReady(void)
{
   /* Tx Buffer is ready - tries to send next data back */
   Echo_Send();

   /* Deadlock prevention - Restart receiving if is stopped */
   if(Echo_ReceivingIsStopped)
   {
      Echo_ReceivingIsStopped = false;
      Uart_Restore_DataReadyEvent();
   }
}

/****************************************************************************\
 * Echo application event: UART Error occurred
 *
 * Context: User
 *
\****************************************************************************/
void Echo_Event_Error(void)
{
   /* UART Error bits */
   uint8_t errors;

   /* Reads error */
   errors = Uart_Get_Errors();

   Debug_Send_Text("\n\r");
   Debug_Dump("!!! UART Error occurred: 0x", errors);
   Debug_Send_Text("\n\r");
}
