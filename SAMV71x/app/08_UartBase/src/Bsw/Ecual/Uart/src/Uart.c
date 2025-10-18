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

void Uart_Isr( uint8_t Channel )
{
  const Uart * LocUartReg = UartRegAddr[Channel];
  uint8_t LocUartLogicChannel = Uart_GetLogChannel(Channel);
  
  /* Example Code */
  /* UART_CFG_CHANNELS represents the number of configured channels from configuration structure */
  if (LocUartLogicChannel<UART_CFG_CHANNELS)
    UartStatus[LocUartLogicChannel].TriggerCounter++;
	    
}

/* ===================== Handlers físicos ===================== */
void UART0_Handler(void){ Uart_Isr(UART_CH0); }
void UART1_Handler(void){ Uart_Isr(UART_CH1); }
void UART2_Handler(void){ Uart_Isr(UART_CH2); }
void UART3_Handler(void){ Uart_Isr(UART_CH3); }
void UART4_Handler(void){ Uart_Isr(UART_CH4); }
