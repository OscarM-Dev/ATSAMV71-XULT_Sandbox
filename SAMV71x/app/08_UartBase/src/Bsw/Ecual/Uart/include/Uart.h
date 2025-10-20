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
#include "Uart_Driver.h"

#define UART_MASK_RXRDT         ((uint8)1)
#define UART_MASK_TXRDY         ((uint8)2)
#define UART_MASK_OVRE          ((uint32)32)
#define UART_MASK_FRAME         ((uint32)64)
#define UART_MASK_PARE          ((uint32)128)
#define UART_MASK_TXEMPTY       ((uint32)512)

/* ************************************************************************** */
/* Public function prototypes  
/* ************************************************************************** */
Std_ReturnType Uart_Init        (UartConfigType * Cfg);
Std_ReturnType Uart_SetBaudrate (UartChannelType Ch, Uart_BaudrateType Baud);
Std_ReturnType Uart_SetTxEnable (UartChannelType Ch, uint8_t Enable);
Std_ReturnType Uart_SendByte    (UartChannelType Ch, uint8_t * Byte);
Std_ReturnType Uart_SendBuffer  (UartChannelType Ch, uint8_t * Buf, uint16_t Len);
Std_ReturnType Uart_GetStatus   (UartChannelType Ch, uint32_t * Out);
Std_ReturnType Uart_EnableInt   (UartChannelType Ch, uint32_t IntMode, uint8_t Enable);

/*============================================================================*/
#endif /* _UART_H */