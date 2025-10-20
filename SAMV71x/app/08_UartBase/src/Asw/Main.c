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
/**
 * @brief UART test function - sends messages on all configured channels.
 * @note This is an AUTOSAR-compliant implementation using only ECUAL APIs.
 */
static void UART_Tester( void )
{
	static uint8_t dataUART0[] = "Hello from UART0";
	static uint8_t dataUART1[] = "Hello from UART1";
	static uint8_t dataUART2[] = "Hello from UART2";
	static uint8_t dataUART3[] = "Hello from UART3";
	static uint8_t dataUART4[] = "Hello from UART4";

	/* Send test messages on all configured channels */
	Uart_SendBuffer( UART_CFG_CHANNEL0, dataUART0, strlen((const char*)dataUART0) );
	Uart_SendBuffer( UART_CFG_CHANNEL1, dataUART1, strlen((const char*)dataUART1) );
	Uart_SendBuffer( UART_CFG_CHANNEL2, dataUART2, strlen((const char*)dataUART2) );
	Uart_SendBuffer( UART_CFG_CHANNEL3, dataUART3, strlen((const char*)dataUART3) );
	Uart_SendBuffer( UART_CFG_CHANNEL4, dataUART4, strlen((const char*)dataUART4) );
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

	/* Configure LEDs */
	printf( "-- Led Control --\n\r" ) ;
	LedCtrl_Configure();
  
	printf( "-- Button Control --\n\r" ) ;  
	ButtonCtrl_ConfigureSW0Button();
  
	/* UART Initialization - AUTOSAR compliant */
	printf( "-- Uart Initialization --\n\r" ) ;
	Uart_Init(&Uart_Config);

	/* Run UART test to send messages on all channels */
	printf( "-- UART Test --\n\r" ) ;
	UART_Tester();

	/* Scheduler Initialization */
	printf( "-- Scheduler Initialization --\n\r" ) ;
	SchM_Init(ScheduleConfig);
	
	/* Should never reach this code */
	for(;;)
    {
		printf( "-- Unexpected Error at Scheduler Initialization --\n\r" ) ;
	}
}
