/*******************************************************************************/
/**
\file       
\brief      
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   \version       1.0
   \author        
   \description   
   \date          
*/

/** Scheduler function prototypes definitions */
#include "Uart.h"
#include "MemAlloc.h"


/*****************************************************************************************************
* Defines - 
*****************************************************************************************************/

#define UART_CHANNEL_UNDEF (255)

#define UART_CH0  (0) 
#define UART_CH1  (1)
#define UART_CH2  (2)
#define UART_CH3  (3)
#define UART_CH4  (4)

/*****************************************************************************************************
* Definition of  VARIABLEs - 
*****************************************************************************************************/

/* Uart Status Structure Example */
UartStatusType *UartStatus;
  

/*****************************************************************************************************
* Definition of module wide (CONST-) CONSTANTs 
*****************************************************************************************************/
/* Array of Uart Register Base Address */
static const Uart * UartRegAddr[]={ UART0, UART1, UART2, UART3, UART4 };

/*****************************************************************************************************
* Code of module wide Private FUNCTIONS
*****************************************************************************************************/

uint8_t Uart_GetLogChannel(uint8_t PhyChannel)
{
  uint8_t LogicalChannel = UART_CHANNEL_UNDEF; 
  uint8_t LocChIdx = 0; /* LocChIdx represent the logical channel */
  /* UART_CFG_CHANNELS represents the number of configured channels from configuration structure */
  do
  {
    if (UartStatus[LocChIdx].ChannelId == PhyChannel)
    {
      LogicalChannel = LocChIdx; 
    }
    LocChIdx++;
  }while( (UartStatus[LocChIdx-1].ChannelId != PhyChannel) && (LocChIdx < UART_CFG_CHANNELS) );
  return (LogicalChannel);
}

/*****************************************************************************************************
* Code of module wide Public FUNCTIONS
*****************************************************************************************************/

void Uart_Init(  const uint8_t * ChannelConfigure )
{
  const Uart * LocUartReg;
  uint8_t LocChIdx = 0; /* LocChIdx represent the logical channel */
   
  /* Memory allocation for all Channel Status example */
  /* UART_CFG_CHANNELS represents the number of configured channels from configuration structure */
  UartStatus = (UartStatusType*) MemAlloc( sizeof(UartStatusType) * UART_CFG_CHANNELS  );  
  
  for (LocChIdx = 0; LocChIdx < UART_CFG_CHANNELS; LocChIdx++)
  {
    /* Point to register address based of physical channel */
    LocUartReg = UartRegAddr[ChannelConfigure[LocChIdx]];
    /* Access to register for the configured channel with LocUartReg */
    /* Access to channel status structure with LocChIdx */
    UartStatus[LocChIdx].ChannelId = ChannelConfigure[LocChIdx];    
  }
}

void Uart_Send(uint8_t Channel)
{
  const Uart * LocUartReg = UartRegAddr[UartStatus[Channel].ChannelId];
  /* Example Code */
  UartStatus[Channel].Counter++;
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