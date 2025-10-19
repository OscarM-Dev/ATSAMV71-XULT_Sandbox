/****************************************************************************************************/
/**
  \mainpage
  \n 
  \brief        Main application (main module)
  \author       Abraham Tezmol Otero, M.S.E.E
  \project      Tau 
  \version      1.0
  \date         12/Jun/2016
   
  Program compiled with  WinIDEA Open Build 9.12.256 (54401), tested on Atmel SAMV71 Xplained Ultra
*/
/****************************************************************************************************/


/*~~~~~~  Headers ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/** Main group of includes for board definitions, chip definitions and type definitions */
#include    "Std_Types.h"
/** Task scheduler definitions */
#include    "SchM.h"
/** LED control definitions */ 
#include    "Led_Ctrl.h"
/** Watchdog control function prototypes definitions */
#include    "Wdg.h"
/** Button Control interfaces */
#include    "Button_Ctrl.h"
/** Uart interfaces */
#include    "Uart.h"

/*~~~~~~  Local definitions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*~~~~~~  Global variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


/*~~~~~~  Local functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
static void UART_Tester( void )
{
	static uint8_t dataUART0[] = "Hello from UART0";
	static uint8_t dataUART1[] = "Hello from UART1";
	static uint8_t dataUART2[] = "Hello from UART2";
	static uint8_t dataUART3[] = "Hello from UART3";
	static uint8_t dataUART4[] = "Hello from UART4";

	//Pin configuration.
	const Pin UARTPins[5] =
	{
		{ 
			.mask = PIO_PA10A_UTXD0, 
			.pio = PIOA, 
			.id = ID_PIOA, 
			.type = PIO_PERIPH_A, 
			.attribute = PIO_DEFAULT 
		},

		{ 
			.mask = PIO_PA6C_UTXD1, 
			.pio = PIOA, 
			.id = ID_PIOA, 
			.type = PIO_PERIPH_C, 
			.attribute = PIO_DEFAULT 
		},

		{
			.mask = PIO_PD26C_UTXD2, 
			.pio = PIOD, 
			.id = ID_PIOD, 
			.type = PIO_PERIPH_C, 
			.attribute = PIO_DEFAULT 
		},

		{
			.mask = PIO_PD30A_UTXD3, 
			.pio = PIOD, 
			.id = ID_PIOD, 
			.type = PIO_PERIPH_A, 
			.attribute = PIO_DEFAULT 
		},

		{
			.mask = PIO_PD19C_UTXD4, 
			.pio = PIOD, 
			.id = ID_PIOD, 
			.type = PIO_PERIPH_C, 
			.attribute = PIO_DEFAULT 
		}
	};

	//Enabling and configuring pins.
	PMC_EnablePeripheral( ID_PIOA );
	PMC_EnablePeripheral( ID_PIOD );
	PIO_Configure( UARTPins, 5 );

	//Enabling and configuring global interrupts.
	NVIC_SetPriority( (IRQn_Type)ID_UART0, 1 );
	NVIC_EnableIRQ( (IRQn_Type)ID_UART0 );
	NVIC_SetPriority( (IRQn_Type)ID_UART1, 1 );
	NVIC_EnableIRQ( (IRQn_Type)ID_UART1 );
	NVIC_SetPriority( (IRQn_Type)ID_UART2, 1 );
	NVIC_EnableIRQ( (IRQn_Type)ID_UART2 );
	NVIC_SetPriority( (IRQn_Type)ID_UART3, 1 );
	NVIC_EnableIRQ( (IRQn_Type)ID_UART3 );
	NVIC_SetPriority( (IRQn_Type)ID_UART4, 1 );
	NVIC_EnableIRQ( (IRQn_Type)ID_UART4 );

	//UART0.
	PMC_EnablePeripheral( ID_UART0 );
	UART_Driver_Configure( UART0, 0, 0, 0, 115200, 150000000 );
	Uart_SendBuffer( 0, dataUART0, strlen( dataUART0 ) );

	//UART1.
	PMC_EnablePeripheral( ID_UART1 );
	UART_Driver_Configure( UART1, 0, 0, 0, 57600, 150000000 );
	Uart_SendBuffer( 1, dataUART1, strlen( dataUART1 ) );

	//UART2.
	PMC_EnablePeripheral( ID_UART2 );
	UART_Driver_Configure( UART2, 0, 0, 0, 19200, 150000000 );
	Uart_SendBuffer( 2, dataUART2, strlen( dataUART2 ) );

	//UART3.
	PMC_EnablePeripheral( ID_UART3 );
	UART_Driver_Configure( UART3, 0, 0, 0, 9600, 150000000 );
	Uart_SendBuffer( 3, dataUART3, strlen( dataUART3 ) );

	//UART4.
	PMC_EnablePeripheral( ID_UART4 );
	UART_Driver_Configure( UART4, 0, 0, 0, 4800, 150000000 );
	Uart_SendBuffer( 4, dataUART4, strlen( dataUART4 ) );
}		
/*----------------------------------------------------------------------------
 *        Exported functions
 *----------------------------------------------------------------------------*/
/**
 *  \brief getting-started Application entry point.
 *
 *  \return Unused (ANSI-C compatibility).
 */
extern int main( void )
{
	/* Disable watchdog */
	Wdg_Disable();
	printf( "\n\r-- Scheduler Project %s --\n\r", SOFTPACK_VERSION ) ;
	printf( "-- %s\n\r", BOARD_NAME ) ;
	printf( "-- Compiled: %s %s With %s --\n\r", __DATE__, __TIME__ , COMPILER_NAME);
	/* Enable I and D cache */
	SCB_EnableICache();
	/* SCB_EnableDCache(); */
	UART_Tester();

	/* Configure LEDs */
	printf( "-- Led Control --\n\r" ) ;
	LedCtrl_Configure();
  
    printf( "-- Button Control --\n\r" ) ;  
    ButtonCtrl_ConfigureSW0Button();
  
    /* Uart Inititalization */
    printf( "-- Uart Initialization --\n\r" ) ;

    //Uart_Init(&UartConfiguredChannels[0]);
	//UartMiniTester_Init();

	/*{
		static const char* preMsgs[] = {
			"PRE: CH0 ready\r\n",
			"PRE: CH1 ready\r\n",
			"PRE: CH2 ready\r\n",
			"PRE: CH3 ready\r\n"
		};
		for (uint8_t ch = 0u; ch < UART_CFG_CHANNELS; ch++) {
			const char* m = preMsgs[ch];
			(void)Uart_Write(ch, (const uint8_t*)m, (uint16_t)strlen(m));
		}
	}*/

	/* Scheduler Inititalization */
	printf( "-- Scheduler Initialization --\n\r" ) ;
	SchM_Init(ScheduleConfig);
	
	/* Should never reach this code */
	for(;;)
    {
		printf( "-- Unexpected Error at Scheduler Initialization --\n\r" ) ;
	}
}
