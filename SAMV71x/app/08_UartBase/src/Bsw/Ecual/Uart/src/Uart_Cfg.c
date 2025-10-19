/*******************************************************************************/
/**
\file       Uart_Cfg.c
\brief      
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   \version       1.0
   \author        
   \description   
   \date          
*/

#include "Uart.h"

static void Uart_TxDone_CB(UartChannelType ch) { (void)ch; }
static void Uart_Err_CB   (UartChannelType ch, UartErrorType e) { (void)ch; (void)e; }

static UartChannelCfgType Uart_ChannelPB[] =
{
  /* ChannelId,         IsrEn,                 Mode,                   Parity,                   Baudrate,                  TxNotif,        RxNotif,      ErrNotif */
  { UART_CFG_CHANNEL0,  UART_CFG_INT_TXRDY,    UART_CFG_MODE_NORMAL,   UART_CFG_PARITY_EVEN,     UART_CFG_BAUDRATE_115200,  Uart_TxDone_CB, NULL,         Uart_Err_CB },
  { UART_CFG_CHANNEL1,  UART_CFG_INT_TXRDY,    UART_CFG_MODE_NORMAL,   UART_CFG_PARITY_EVEN,     UART_CFG_BAUDRATE_115200,  Uart_TxDone_CB, NULL,         Uart_Err_CB },
  { UART_CFG_CHANNEL2,  UART_CFG_INT_TXRDY,    UART_CFG_MODE_NORMAL,   UART_CFG_PARITY_EVEN,     UART_CFG_BAUDRATE_115200,  Uart_TxDone_CB, NULL,         Uart_Err_CB },
  { UART_CFG_CHANNEL3,  UART_CFG_INT_TXRDY,    UART_CFG_MODE_NORMAL,   UART_CFG_PARITY_EVEN,     UART_CFG_BAUDRATE_115200,  Uart_TxDone_CB, NULL,         Uart_Err_CB },
  { UART_CFG_CHANNEL4,  UART_CFG_INT_TXRDY,    UART_CFG_MODE_NORMAL,   UART_CFG_PARITY_EVEN,     UART_CFG_BAUDRATE_115200,  Uart_TxDone_CB, NULL,         Uart_Err_CB }
};

UartConfigType Uart_Config =
{
  .UartNumberOfChannels = UART_CFG_CHANNELS,
  .ClkSrc               = UART_CFG_PER_CLK,
  .UartChannel          = Uart_ChannelPB
};