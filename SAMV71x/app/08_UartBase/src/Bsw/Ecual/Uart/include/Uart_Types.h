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
/* ************************************************************************** */
/* Uart Types. 
/* ************************************************************************** */
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
typedef uint32_t Uart_BaudrateType;

/* ************************************************************************** */
/* Control and configuration structures. 
/* ************************************************************************** */
/**
 * @brief Tx control structure for each channel. 
 * 
 */
typedef struct
{

} UartChannelTxType;

/**
 * @brief Configuration structure for each UART channel.
 * 
 */
typedef struct 
{
  uint8_t                 ChannelId;
  uint8_t                 IsrEn;
  uint8_t                 Mode;
  uint8_t                 Parity;
  uint32_t                Baudrate;
  UartNotification      TxNotification;
  UartNotification      RxNotification;
  UartErrorNotification ErrorNotification;
} UartChannelCfgType;

/**
 * @brief Configuration structure for all UART channels.
 * 
 */
typedef struct 
{
  uint8_t                       UartNumberOfChannels;
  uint8_t                       ClkSrc;         
  UartChannelCfgType  * UartChannel;
} UartConfigType;
  
/*============================================================================*/
#endif /* UART_TYPES */
