/*******************************************************************************/
/**
 * \file       Uart.c
 * \brief      ECUAL UART (TX only) - multi-instance, interrupt-driven
 * \version    1.0
 */
/*******************************************************************************/
#include "Uart.h"
#include "MemAlloc.h"

/* ===================== MCAL interface ===================== */
typedef enum { MCAL_UART_OK=0, MCAL_UART_ERR } McalUartRet;

extern McalUartRet Uart_Init            (UartChannelType ch);
extern McalUartRet Uart_SetBaudrate     (UartChannelType ch, Uart_BaudrateType baud);
extern McalUartRet Uart_SetTxEnable     (UartChannelType ch, uint32 Enable);
extern void        Uart_EnableInterrupt (UartChannelType ch, uint32 mask);
extern void        Uart_DisableInterrupt(UartChannelType ch, uint32 mask);
extern boolean     Uart_IsTxReady       (UartChannelType ch);
extern void        Uart_WriteDR         (UartChannelType ch, uint8 byte);
extern boolean     Uart_Int_TxRdy       (UartChannelType ch);
extern void        Uart_IntClr_TxRdy    (UartChannelType ch);


static UartStatusType * gUartSt = NULL;
static const UartConfigType * gCfg = NULL;

UartErrorType Uart_Init(const UartConfigType * Cfg)
{
  printf("TODO: Uart_Init()\n");
}

UartErrorType Uart_SetBaudrate(UartChannelType Ch, Uart_BaudrateType Baud)
{
  printf("TODO: Uart_SetBaudrate()\n");
}

UartErrorType Uart_SetTxEnable(UartChannelType Ch, uint32 Enable)
{
  printf("TODO: Uart_SetTxEnable()\n");
}

UartErrorType Uart_SendByte(UartChannelType Ch, uint8 Byte)
{
  printf("TODO: Uart_SendByte()\n");  
}

UartErrorType Uart_SendBuffer(UartChannelType Ch, const uint8 * Buf, uint16 Len)
{
  printf("TODO: Uart_SendBuffer()\n");
}

UartErrorType Uart_GetByte(UartChannelType Ch)
{
  printf("TODO: Uart_GetByte()\n");
}

UartErrorType Uart_GetStatus(UartChannelType Ch)
{
  printf("TODO: Uart_GetStatus()\n");
}

UartErrorType Uart_EnableInt(UartChannelType Ch, uint32_t IntMode, uint8_t Enable)
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
void Uart_Isr( uint8_t Channel )
{
  //Data messages for each UART channel.
	static const uint8_t dataUART0[] = "Hello from UART0";
	static const uint8_t dataUART1[] = "Hello from UART1";
	static const uint8_t dataUART2[] = "Hello from UART2";
	static const uint8_t dataUART3[] = "Hello from UART3";
	static const uint8_t dataUART4[] = "Hello from UART4";

  //Message length in bytes.
  static const dataLengthsUART[5] =
  {
    sizeof( dataUART0 ) - 1,
    sizeof( dataUART1 ) - 1,
    sizeof( dataUART2 ) - 1,
    sizeof( dataUART3 ) - 1,
    sizeof( dataUART4 ) - 1
  };

  //Counters.
  static countersUART[5] = { 0, 0, 0, 0, 0 };

  const Uart * LocUartReg = UartRegAddr[Channel];
  uint8_t LocUartLogicChannel = Uart_GetLogChannel(Channel);
  
  /* Example Code */
  /* UART_CFG_CHANNELS represents the number of configured channels from configuration structure */
  if (LocUartLogicChannel<UART_CFG_CHANNELS)
    UartStatus[LocUartLogicChannel].TriggerCounter++;

  //Analizing UART channel.
  switch( Channel )
  {
    case UART_CH0:
      UART_Driver_SendByte( UART0, dataUART0[countersUART[UART_CH0]] );
      countersUART[UART_CH0]++;

      if( countersUART[UART_CH0] >= dataLengthsUART[UART_CH0] )
      { //Disable interrupts and transmitter.
        UART_Driver_SetIt( UART0, 0, UART_IDR_TXRDY );
        UART_Driver_SetTx( UART0, 0 );
      }
    break;

    case UART_CH1:
      UART_Driver_SendByte( UART1, dataUART1[countersUART[UART_CH1]] );
      countersUART[UART_CH1]++;

      if( countersUART[UART_CH1] >= dataLengthsUART[UART_CH1] )
      { //Disable interrupts and transmitter.
        UART_Driver_SetIt( UART1, 0, UART_IDR_TXRDY );
        UART_Driver_SetTx( UART1, 0 );
      }
    break;
    
    case UART_CH2:
        UART_Driver_SendByte( UART2, dataUART2[countersUART[UART_CH2]] );
        countersUART[UART_CH2]++;

        if( countersUART[UART_CH2] >= dataLengthsUART[UART_CH2] )
        { //Disable interrupts and transmitter.
          UART_Driver_SetIt( UART2, 0, UART_IDR_TXRDY );
          UART_Driver_SetTx( UART2, 0 );
        }
    break;

    case UART_CH3:
        UART_Driver_SendByte( UART3, dataUART3[countersUART[UART_CH3]] );
        countersUART[UART_CH3]++;

        if( countersUART[UART_CH3] >= dataLengthsUART[UART_CH3] )
        { //Disable interrupts and transmitter.
          UART_Driver_SetIt( UART3, 0, UART_IDR_TXRDY );
          UART_Driver_SetTx( UART3, 0 );
        }
    break;

    case UART_CH4:
        UART_Driver_SendByte( UART4, dataUART4[countersUART[UART_CH4]] );
        countersUART[UART_CH4]++;

        if( countersUART[UART_CH4] >= dataLengthsUART[UART_CH4] )
        { //Disable interrupts and transmitter.
          UART_Driver_SetIt( UART4, 0, UART_IDR_TXRDY );
          UART_Driver_SetTx( UART4, 0 );
        }
    break;
  }
}

/**
 * @brief This function is the ISR for UART0.
 * @note At the moment the only interrupt source is TXRDY.
 * 
 */
void UART0_Handler( void )
{
	Uart_Isr( UART_CH0 );
}

/**
 * @brief This function is the ISR for UART1.
 * @note At the moment the only interrupt source is TXRDY.
 * 
 */
void UART1_Handler( void )
{
	Uart_Isr( UART_CH1 );
}

/**
 * @brief This function is the ISR for UART2.
 * @note At the moment the only interrupt source is TXRDY.
 * 
 */
void UART2_Handler( void )
{
	Uart_Isr( UART_CH2 );
}

/**
 * @brief This function is the ISR for UART3.
 * @note At the moment the only interrupt source is TXRDY.
 * 
 */
void UART3_Handler( void )
{
	Uart_Isr( UART_CH3 );
}

/**
 * @brief This function is the ISR for UART4.
 * @note At the moment the only interrupt source is TXRDY.
 * 
 */
void UART4_Handler( void )
{
	Uart_Isr( UART_CH4 );
}
