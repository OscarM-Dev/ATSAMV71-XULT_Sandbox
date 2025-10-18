/*******************************************************************************/
/**
\file       Uart.h
\brief      
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   \version       1.0
   \author        
   \description   
   \date          
*/

#ifndef _UART_H        /*prevent duplicated includes*/
#define _UART_H

/** Standard Types */
#include "Std_Types.h"
#include "Uart_Types.h"
#include "Uart_Cfg.h"

#define UART_MASK_RXRDT         ((uint8)1)
#define UART_MASK_TXRDY         ((uint8)2)
#define UART_MASK_OVRE          ((uint32)32)
#define UART_MASK_FRAME         ((uint32)64)
#define UART_MASK_PARE          ((uint32)128)
#define UART_MASK_TXEMPTY       ((uint32)512)

extern UartErrorType Uart_Init        (const UartConfigType * Cfg);
extern UartErrorType Uart_SetBaudrate (UartChannelType Ch, Uart_BaudrateType Baud);
extern UartErrorType Uart_SetTxEnable (UartChannelType Ch, boolean Enable);
extern UartErrorType Uart_SendByte    (UartChannelType Ch, uint8 Byte);
extern UartErrorType Uart_SendBuffer  (UartChannelType Ch, const uint8 * Buf, uint32 Len);
extern UartErrorType Uart_GetStatus   (UartChannelType Ch, UartStatusType * Out);
extern void          Uart_Isr         (UartChannelType ChannelId);

extern void UART0_Handler(void);
extern void UART1_Handler(void);
extern void UART2_Handler(void);
extern void UART3_Handler(void);
extern void UART4_Handler(void);

/*============================================================================*/
#endif /* _UART_H */