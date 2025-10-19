/*******************************************************************************/
/**
 * \file       Uart.c
 * \brief      ECUAL UART (TX only) - multi-instance, interrupt-driven
 * \version    1.0
 */
/*******************************************************************************/
#include "Uart.h"
#include "MemAlloc.h"

static const UartConfigType * gCfg = NULL;

/* ************************************************************************** */
/* Public functions.
/* ************************************************************************** */
Std_ReturnType Uart_Init(UartConfigType * Cfg)
{
  printf("TODO: Uart_Init()\n");
}

Std_ReturnType Uart_SetBaudrate(UartChannelType Ch, Uart_BaudrateType Baud)
{
  printf("TODO: Uart_SetBaudrate()\n");
}

Std_ReturnType Uart_SetTxEnable(UartChannelType Ch, uint8_t Enable)
{
  printf("TODO: Uart_SetTxEnable()\n");
}

Std_ReturnType Uart_SendByte(UartChannelType Ch, uint8_t *Byte)
{
  //Initializing structure.
  Uart_Config.UartChannelTx[Ch].TxData = Byte;
  Uart_Config.UartChannelTx[Ch].TxDataLen = 1;
  Uart_Config.UartChannelTx[Ch].BytesTransmitted = 0;
  Uart_Config.UartChannelTx[Ch].TransmissionComplete = 0;

  //Enable interrupt and transmitter. //To do, call the corresponding functions and use conf macros.
  UART_Driver_SetIt( Uart_Config.UartChannel[Ch].Addr, 1,  UART_IER_TXRDY );
	UART_Driver_SetTx( Uart_Config.UartChannel[Ch].Addr, 1 );

  return E_OK;
}

Std_ReturnType Uart_SendBuffer(UartChannelType Ch, uint8_t * Buf, uint16_t Len)
{
  //Initializing structure.
  Uart_Config.UartChannelTx[Ch].TxData = Buf;
  Uart_Config.UartChannelTx[Ch].TxDataLen = Len;
  Uart_Config.UartChannelTx[Ch].BytesTransmitted = 0;
  Uart_Config.UartChannelTx[Ch].TransmissionComplete = 0;

  //Enable interrupt and transmitter. //To do, call the corresponding functions and use conf macros.
  UART_Driver_SetIt( Uart_Config.UartChannel[Ch].Addr, 1,  UART_IER_TXRDY );
	UART_Driver_SetTx( Uart_Config.UartChannel[Ch].Addr, 1 );

  return E_OK;
}

Std_ReturnType Uart_GetStatus(UartChannelType Ch, uint32_t * Out)
{
  printf("TODO: Uart_GetStatus()\n");
}

Std_ReturnType Uart_EnableInt(UartChannelType Ch, uint32_t IntMode, uint8_t Enable)
{
  printf("TODO: Uart_EnableInt()\n");
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
