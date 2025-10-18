/*******************************************************************************/
/**
\file       Uart_Types.h
\brief      
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   \version       1.0
   \author        
   \description   
   \date          
*/

#ifndef UART_TYPES
#define UART_TYPES

/** Standard Types */
#include "Std_Types.h"

/* Error type */
typedef enum
{
    UART_ERROR_OVERRUN = 0u,
    UART_ERROR_FRAMING = 1u,
    UART_ERROR_PARITY = 2u
} UartErrorType;

/* Notification function pointer types */
typedef void (*UartNotification)(uint8_t channel);
typedef void (*UartErrorNotification)(uint8_t channel, UartErrorType error);

typedef uint8_t UartChannelType;

typedef struct
{
  UartChannelType physicalChannel;
  uint32_t baudrate;
  uint8_t parity;
  uint8_t stopBits;
} UartChannelCfgType;

typedef struct
{
  const UartChannelCfgType* channels;
  uint8_t numChannels;
} UartConfigType;

typedef struct
{
  uint8_t ChannelId;
  uint8_t Counter;
  uint8_t TriggerCounter;
} UartStatusType;
  
/*============================================================================*/
#endif /* UART_TYPES */
