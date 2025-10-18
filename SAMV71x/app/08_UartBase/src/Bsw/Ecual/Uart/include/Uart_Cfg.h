/*******************************************************************************/
/**
\file       Uart_Cfg.h
\brief      
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   \version       1.0
   \author        
   \description   
   \date          
*/
#ifndef _UART_CFG_H
#define _UART_CFG_H

#include "Std_Types.h"

#define UART_CFG_CHANNELS ((uint8)4)

#define UART_CFG_CHANNEL0  ((uint8)0)
#define UART_CFG_CHANNEL1  ((uint8)1)
#define UART_CFG_CHANNEL2  ((uint8)2)
#define UART_CFG_CHANNEL3  ((uint8)3)
#define UART_CFG_CHANNEL4  ((uint8)4)

#define UART_CFG_INT_DISABLE       ((uint8)0)
#define UART_CFG_INT_RXRDT         ((uint8)1)
#define UART_CFG_INT_TXRDY         ((uint8)2)
#define UART_CFG_INT_OVR_ERROR     ((uint32)32)
#define UART_CFG_INT_FRAMING_ERROR ((uint32)64)
#define UART_CFG_INT_PARITY_ERROR  ((uint32)128)
#define UART_CFG_INT_TXEMPTY       ((uint32)512)

#define UART_CFG_PARITY_EVEN    ((uint8)0)
#define UART_CFG_PARITY_ODD     ((uint8)1)
#define UART_CFG_PARITY_SPACE   ((uint8)2)
#define UART_CFG_PARITY_MARK    ((uint8)3)
#define UART_CFG_PARITY_NONE    ((uint8)4)

#define UART_CFG_BAUDRATE_9600     ((uint32)9600)
#define UART_CFG_BAUDRATE_19200    ((uint32)19200)
#define UART_CFG_BAUDRATE_38400    ((uint32)38400)
#define UART_CFG_BAUDRATE_57600    ((uint32)57600)
#define UART_CFG_BAUDRATE_115200   ((uint32)115200)

#define UART_CFG_MODE_NORMAL   ((uint8)0)
#define UART_CFG_MODE_AUTO     ((uint8)1)
#define UART_CFG_MODE_LOOPBACK ((uint8)2)

#define UART_CFG_PER_CLK   ((uint8)0)
#define UART_CFG_PMC_CLK   ((uint8)1)

typedef struct {
  uint8                 ChannelId;
  uint8                 IsrEn;
  uint8                 Mode;
  uint8                 Parity;
  uint32                Baudrate;
  UartNotification      TxNotification;
  UartNotification      RxNotification;
  UartErrorNotification ErrorNotification;
} UartChannelCfgType;

typedef struct {
  uint8                       UartNumberOfChannels;
  uint8                       ClkSrc;         
  const UartChannelCfgType  * UartChannel;
} UartConfigType;

extern const UartConfigType Uart_Config;

#endif /* _UART_CFG_H */