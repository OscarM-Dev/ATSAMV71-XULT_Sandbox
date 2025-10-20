/*******************************************************************************/
/**
 * \file       Uart.c
 * \brief      ECUAL UART (TX only) - multi-instance, interrupt-driven
 * \version    1.0
 */
/*******************************************************************************/
#include "Uart.h"
#include "chip.h"   /* For NVIC, PMC, PIO, BOARD_MCK */
#include "board.h"  /* For board-specific definitions */

static const UartConfigType * gCfg = NULL;

/* Internal table to map channel ID to IRQ numbers and peripheral IDs */
static const struct {
  uint8_t channelId;
  IRQn_Type irqn;
  uint32_t pmcId;
} gUartMap[] = {
  { UART_CFG_CHANNEL0, UART0_IRQn, ID_UART0 },
  { UART_CFG_CHANNEL1, UART1_IRQn, ID_UART1 },
  { UART_CFG_CHANNEL2, UART2_IRQn, ID_UART2 },
  { UART_CFG_CHANNEL3, UART3_IRQn, ID_UART3 },
  { UART_CFG_CHANNEL4, UART4_IRQn, ID_UART4 }
};

/* ************************************************************************** */
/* Public functions.
/* ************************************************************************** */
/**
 * @brief Initialize all configured UART channels.
 * @details Enables peripheral clocks, configures pins, enables NVIC interrupts,
 *          and configures UART hardware via MCAL driver.
 * 
 * @param Cfg Pointer to UART configuration structure.
 * @return E_OK if successful, E_NOT_OK otherwise.
 */
Std_ReturnType Uart_Init(UartConfigType * Cfg)
{
  uint8_t i;
  UartChannelCfgType *ch;
  
  if (Cfg == NULL || Cfg->UartChannel == NULL) {
    return E_NOT_OK;
  }

  gCfg = Cfg;

  /* Enable PIO peripheral clocks for pin configuration */
  PMC_EnablePeripheral(ID_PIOA);
  PMC_EnablePeripheral(ID_PIOD);

  /* Initialize each configured channel */
  for (i = 0; i < Cfg->UartNumberOfChannels; i++)
  {
    ch = &Cfg->UartChannel[i];

    /* Enable UART peripheral clock */
    PMC_EnablePeripheral(gUartMap[ch->ChannelId].pmcId);

    /* Configure UART pins */
    PIO_Configure(&ch->UartPin, 1);

    /* Configure NVIC for this UART */
    NVIC_SetPriority(gUartMap[ch->ChannelId].irqn, 1);
    NVIC_EnableIRQ(gUartMap[ch->ChannelId].irqn);

    /* Configure UART hardware via MCAL driver */
    UART_Driver_Configure(
      ch->Addr,
      ch->Mode,
      Cfg->ClkSrc,
      ch->Parity,
      ch->Baudrate,
      BOARD_MCK  /* Clock source frequency */
    );

    /* Initialize TX control structure */
    Cfg->UartChannelTx[i].TxData = NULL;
    Cfg->UartChannelTx[i].TxDataLen = 0;
    Cfg->UartChannelTx[i].BytesTransmitted = 0;
    Cfg->UartChannelTx[i].TransmissionComplete = 1;
  }

  return E_OK;
}

/**
 * @brief Set baudrate for a specific UART channel.
 * 
 * @param Ch UART channel ID.
 * @param Baud Desired baudrate in Hz.
 * @return E_OK if successful, E_NOT_OK otherwise.
 */
Std_ReturnType Uart_SetBaudrate(UartChannelType Ch, Uart_BaudrateType Baud)
{
  if (gCfg == NULL || Ch >= gCfg->UartNumberOfChannels || Baud == 0) {
    return E_NOT_OK;
  }

  UART_Driver_SetBaudrate(gCfg->UartChannel[Ch].Addr, Baud, BOARD_MCK);
  return E_OK;
}

/**
 * @brief Enable or disable UART transmitter.
 * 
 * @param Ch UART channel ID.
 * @param Enable 1 to enable, 0 to disable.
 * @return E_OK if successful, E_NOT_OK otherwise.
 */
Std_ReturnType Uart_SetTxEnable(UartChannelType Ch, uint8_t Enable)
{
  if (gCfg == NULL || Ch >= gCfg->UartNumberOfChannels) {
    return E_NOT_OK;
  }

  UART_Driver_SetTx(gCfg->UartChannel[Ch].Addr, Enable);
  return E_OK;
}

/**
 * @brief Send a single byte via UART (interrupt-driven).
 * 
 * @param Ch UART channel ID.
 * @param Byte Pointer to byte to send.
 * @return E_OK if successful, E_NOT_OK otherwise.
 */
Std_ReturnType Uart_SendByte(UartChannelType Ch, uint8_t *Byte)
{
  if (gCfg == NULL || Ch >= gCfg->UartNumberOfChannels || Byte == NULL) {
    return E_NOT_OK;
  }

  //Initializing structure.
  Uart_Config.UartChannelTx[Ch].TxData = Byte;
  Uart_Config.UartChannelTx[Ch].TxDataLen = 1;
  Uart_Config.UartChannelTx[Ch].BytesTransmitted = 0;
  Uart_Config.UartChannelTx[Ch].TransmissionComplete = 0;

  //Enable interrupt and transmitter.
  UART_Driver_SetIt( Uart_Config.UartChannel[Ch].Addr, 1,  UART_IER_TXRDY );
  UART_Driver_SetTx( Uart_Config.UartChannel[Ch].Addr, 1 );

  return E_OK;
}

/**
 * @brief Send a buffer via UART (interrupt-driven).
 * 
 * @param Ch UART channel ID.
 * @param Buf Pointer to buffer to send.
 * @param Len Length of buffer in bytes.
 * @return E_OK if successful, E_NOT_OK otherwise.
 */
Std_ReturnType Uart_SendBuffer(UartChannelType Ch, uint8_t * Buf, uint16_t Len)
{
  if (gCfg == NULL || Ch >= gCfg->UartNumberOfChannels || Buf == NULL || Len == 0) {
    return E_NOT_OK;
  }

  //Initializing structure.
  Uart_Config.UartChannelTx[Ch].TxData = Buf;
  Uart_Config.UartChannelTx[Ch].TxDataLen = Len;
  Uart_Config.UartChannelTx[Ch].BytesTransmitted = 0;
  Uart_Config.UartChannelTx[Ch].TransmissionComplete = 0;

  //Enable interrupt and transmitter.
  UART_Driver_SetIt( Uart_Config.UartChannel[Ch].Addr, 1,  UART_IER_TXRDY );
  UART_Driver_SetTx( Uart_Config.UartChannel[Ch].Addr, 1 );

  return E_OK;
}

/**
 * @brief Get UART status register value.
 * 
 * @param Ch UART channel ID.
 * @param Out Pointer to store status register value.
 * @return E_OK if successful, E_NOT_OK otherwise.
 */
Std_ReturnType Uart_GetStatus(UartChannelType Ch, uint32_t * Out)
{
  if (gCfg == NULL || Ch >= gCfg->UartNumberOfChannels || Out == NULL) {
    return E_NOT_OK;
  }

  *Out = UART_Driver_GetStatus(gCfg->UartChannel[Ch].Addr);
  return E_OK;
}

/**
 * @brief Enable or disable UART interrupts.
 * 
 * @param Ch UART channel ID.
 * @param IntMode Interrupt mode flags (e.g., UART_CFG_INT_TXRDY).
 * @param Enable 1 to enable, 0 to disable.
 * @return E_OK if successful, E_NOT_OK otherwise.
 */
Std_ReturnType Uart_EnableInt(UartChannelType Ch, uint32_t IntMode, uint8_t Enable)
{
  if (gCfg == NULL || Ch >= gCfg->UartNumberOfChannels) {
    return E_NOT_OK;
  }

  UART_Driver_SetIt(gCfg->UartChannel[Ch].Addr, Enable, IntMode);
  return E_OK;
}

/*****************************************************************************************************
* Code of module wide Interrupt Handler FUNCTIONS
*****************************************************************************************************/
/**
 * @brief This function is the interrupt callback for all UARTs.
 * @note It controls the message transmission of each UART peripheral byte by byte.
 * 
 * @param Channel UART peripheral who called the callback.
 */
void Uart_Isr( UartChannelType Channel )
{
  //Send byte of corresponding UART channel.
  UART_Driver_SendByte( Uart_Config.UartChannel[Channel].Addr, 
  Uart_Config.UartChannelTx[Channel].TxData[Uart_Config.UartChannelTx[Channel].BytesTransmitted] );
  Uart_Config.UartChannelTx[Channel].BytesTransmitted++;

  //Analize if transmission was completed.
  if( Uart_Config.UartChannelTx[Channel].BytesTransmitted >= Uart_Config.UartChannelTx[Channel].TxDataLen )
  {
    //Disable interrupt and transmitter. //To do, call the corresponding functions and use conf macros.
    UART_Driver_SetIt( Uart_Config.UartChannel[Channel].Addr, 0,  UART_IDR_TXRDY );
	  UART_Driver_SetTx( Uart_Config.UartChannel[Channel].Addr, 0 );
    Uart_Config.UartChannelTx[Channel].TransmissionComplete = 1;
  }
}

/**
 * @brief This function is the ISR for UART0.
 * @note At the moment the only interrupt source is TXRDY.
 * 
 */
void UART0_Handler( void )
{
	Uart_Isr( UART_CFG_CHANNEL0 );
}

/**
 * @brief This function is the ISR for UART1.
 * @note At the moment the only interrupt source is TXRDY.
 * 
 */
void UART1_Handler( void )
{
	Uart_Isr( UART_CFG_CHANNEL1 );
}

/**
 * @brief This function is the ISR for UART2.
 * @note At the moment the only interrupt source is TXRDY.
 * 
 */
void UART2_Handler( void )
{
	Uart_Isr( UART_CFG_CHANNEL2 );
}

/**
 * @brief This function is the ISR for UART3.
 * @note At the moment the only interrupt source is TXRDY.
 * 
 */
void UART3_Handler( void )
{
	Uart_Isr( UART_CFG_CHANNEL3 );
}

/**
 * @brief This function is the ISR for UART4.
 * @note At the moment the only interrupt source is TXRDY.
 * 
 */
void UART4_Handler( void )
{
	Uart_Isr( UART_CFG_CHANNEL4 );
}
