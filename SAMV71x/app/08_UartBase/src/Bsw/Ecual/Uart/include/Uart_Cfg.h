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

/* ************************************************************************** */
/* Control and configuration defines. 
/* ************************************************************************** */
#define UART_CFG_CHANNELS ((uint8_t)5)

//UART channel IDs.
#define UART_CFG_CHANNEL0  ((uint8_t)0)
#define UART_CFG_CHANNEL1  ((uint8_t)1)
#define UART_CFG_CHANNEL2  ((uint8_t)2)
#define UART_CFG_CHANNEL3  ((uint8_t)3)
#define UART_CFG_CHANNEL4  ((uint8_t)4)

//UART channel interrupt configuration.
#define UART_CFG_INT_ENABLE        ((uint8_t)1)
#define UART_CFG_INT_DISABLE       ((uint8_t)0)
#define UART_CFG_INT_TXRDY         ((uint8_t)2)
#define UART_CFG_INT_OVR_ERROR     ((uint32_t)32)
#define UART_CFG_INT_FRAMING_ERROR ((uint32_t)64)
#define UART_CFG_INT_PARITY_ERROR  ((uint32_t)128)
#define UART_CFG_INT_TXEMPTY       ((uint32_t)512)

//UART channel parity configuration.
#define UART_CFG_PARITY_EVEN    ((uint8_t)0)
#define UART_CFG_PARITY_ODD     ((uint8_t)1)
#define UART_CFG_PARITY_SPACE   ((uint8_t)2)
#define UART_CFG_PARITY_MARK    ((uint8_t)3)
#define UART_CFG_PARITY_NONE    ((uint8_t)4)

//UART channel baudrate configuration.
#define UART_CFG_BAUDRATE_4800     ((uint32_t)4800)
#define UART_CFG_BAUDRATE_9600     ((uint32_t)9600)
#define UART_CFG_BAUDRATE_19200    ((uint32_t)19200)
#define UART_CFG_BAUDRATE_38400    ((uint32_t)38400)
#define UART_CFG_BAUDRATE_57600    ((uint32_t)57600)
#define UART_CFG_BAUDRATE_115200   ((uint32_t)115200)

//UART channel mode configuration.
#define UART_CFG_MODE_NORMAL   ((uint8_t)0)
#define UART_CFG_MODE_AUTO     ((uint8_t)1)
#define UART_CFG_MODE_LOOPBACK ((uint8_t)2)

//UART channel clk src configuration.
#define UART_CFG_PER_CLK   ((uint8_t)0)
#define UART_CFG_PMC_CLK   ((uint8_t)1)

extern UartConfigType Uart_Config;

#endif /* _UART_CFG_H */