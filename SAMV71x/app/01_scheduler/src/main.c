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
#include    "board.h"
/** Task scheduler definitions */
#include    "app_scheduler.h"
/** LED control definitions */ 
#include    "led_ctrl.h"

/*~~~~~~  Local definitions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/** IRQ priority for PIO (The lower the value, the greater the priority) */
#define IRQ_PRIOR_PIO    0

/*~~~~~~  Local variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static const Pin pinPB1 = PIN_PUSHBUTTON_0;

/*~~~~~~  Global variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


/*~~~~~~  Local functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static void _Button_Handler( const Pin* pPin )
{
	if ( pPin == &pinPB1 ) {
		printf("Button SW0 event confirmed\n\r");
	}
}

static void _ConfigureButtons( void )
{
	/* Initialize PIO interrupts with priority */
	PIO_InitializeInterrupts( IRQ_PRIOR_PIO );

	/* Configure PIO as inputs. */
	PIO_Configure( &pinPB1, 1 ) ;

	/* Adjust PIO denounce filter parameters, uses 10 Hz filter. */
	PIO_SetDebounceFilter( &pinPB1, 10 ) ;

	/* Initialize PIO interrupt handlers, see PIO definition in board.h. */
	PIO_ConfigureIt( &pinPB1, _Button_Handler ) ; /* Interrupt on rising edge  */

	/* Enable PIO controller IRQs. */
	NVIC_EnableIRQ( (IRQn_Type)pinPB1.id ) ;

	/* Enable PIO line interrupts. */
	PIO_EnableIt( &pinPB1 ) ;
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
	vfnWdtCtrl_Disable();

	/* Starting program */
	printf( "\n\r-- Starting program Project1_Task_Scheduler %s --\n\r", SOFTPACK_VERSION ) ;
	printf( "-- %s\n\r", BOARD_NAME ) ;
	printf( "-- Compiled: %s %s With %s--\n\r", __DATE__, __TIME__ , COMPILER_NAME);

	/* Enable I and D cache */
	SCB_EnableICache();
	/* SCB_EnableDCache(); */
	
	/* Configure LEDs */
	vfnLedCtrl_Configure(); 

	/* Configure Non-preemtive scheduler */
	vfnScheduler_Init();
	
	/* Start scheduler */
	vfnScheduler_Start();

	/* Configure button interrupts */
	_ConfigureButtons();

	/* Once all the basic services have been started, go to infinite loop to serviced activated tasks */
	for(;;)
    {
		vfnTask_Scheduler();
	}
}
