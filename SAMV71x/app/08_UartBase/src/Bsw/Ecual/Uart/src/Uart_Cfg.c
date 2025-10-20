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

/**
 * @brief UART pin configurations per channel.
 * @note Pin definitions from SAMV71 Xplained Ultra board.
 */
static UartChannelCfgType Uart_ChannelPB[] =
{
  /* ChannelId,         IsrEn,                 Mode,                   Parity,                   Baudrate,                  TxNotif,        RxNotif,      ErrNotif,     UART addr,  Pin config */
  { 
    UART_CFG_CHANNEL0,  UART_CFG_INT_TXRDY,    UART_CFG_MODE_NORMAL,   UART_CFG_PARITY_EVEN,     UART_CFG_BAUDRATE_115200,  Uart_TxDone_CB, NULL,         Uart_Err_CB,  UART0,
    { .mask = PIO_PA10A_UTXD0, .pio = PIOA, .id = ID_PIOA, .type = PIO_PERIPH_A, .attribute = PIO_DEFAULT }
  },
  { 
    UART_CFG_CHANNEL1,  UART_CFG_INT_TXRDY,    UART_CFG_MODE_NORMAL,   UART_CFG_PARITY_EVEN,     UART_CFG_BAUDRATE_57600,  Uart_TxDone_CB, NULL,         Uart_Err_CB,  UART1,
    { .mask = PIO_PA6C_UTXD1, .pio = PIOA, .id = ID_PIOA, .type = PIO_PERIPH_C, .attribute = PIO_DEFAULT }
  },
  { 
    UART_CFG_CHANNEL2,  UART_CFG_INT_TXRDY,    UART_CFG_MODE_NORMAL,   UART_CFG_PARITY_EVEN,     UART_CFG_BAUDRATE_19200,  Uart_TxDone_CB, NULL,         Uart_Err_CB,  UART2,
    { .mask = PIO_PD26C_UTXD2, .pio = PIOD, .id = ID_PIOD, .type = PIO_PERIPH_C, .attribute = PIO_DEFAULT }
  },
  { 
    UART_CFG_CHANNEL3,  UART_CFG_INT_TXRDY,    UART_CFG_MODE_NORMAL,   UART_CFG_PARITY_EVEN,     UART_CFG_BAUDRATE_9600,  Uart_TxDone_CB, NULL,         Uart_Err_CB,  UART3,
    { .mask = PIO_PD30A_UTXD3, .pio = PIOD, .id = ID_PIOD, .type = PIO_PERIPH_A, .attribute = PIO_DEFAULT }
  },
  { 
    UART_CFG_CHANNEL4,  UART_CFG_INT_TXRDY,    UART_CFG_MODE_NORMAL,   UART_CFG_PARITY_EVEN,     UART_CFG_BAUDRATE_4800,  Uart_TxDone_CB, NULL,         Uart_Err_CB,  UART4,
    { .mask = PIO_PD19C_UTXD4, .pio = PIOD, .id = ID_PIOD, .type = PIO_PERIPH_C, .attribute = PIO_DEFAULT }
  }
};

static UartChannelTxType Uart_ChannelTx[UART_CFG_CHANNELS];

UartConfigType Uart_Config =
{
  .UartNumberOfChannels = UART_CFG_CHANNELS,
  .ClkSrc               = UART_CFG_PER_CLK,
  .UartChannel          = Uart_ChannelPB,
  .UartChannelTx        = Uart_ChannelTx
};