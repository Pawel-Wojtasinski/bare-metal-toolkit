#ifndef ECHO_H_
#define ECHO_H_

/* Application API */
void Echo_Init(void);
void Echo_Event_DataReady(void);
void Echo_Event_TxReady(void);
void Echo_Event_Error(void);

#endif
