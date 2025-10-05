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
/** Memory Allocator definitions */
#include    "Mem_Alloc.h"

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
		vfnCycleLed1Priority();
		printf("Button pressed: LED1 new priority = %u\n\r", u8GetLed1Priority());
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

/**
 * @brief Comprehensive test for Memory Allocator and Linker configuration
 */
static void memory_allocator_test(void)
{
    printf("\n\r=== MEMORY ALLOCATOR TEST START ===\n\r");
    
    /* Initialize Memory Allocator */
    Mem_Init();
    printf("Memory Allocator initialized\n\r");
    
    /* Test 1: Small allocation */
    printf("\nTest 1: Allocating 64 bytes...\n\r");
    void *ptr1 = Mem_Alloc(64);
    if (ptr1 != NULL) {
        printf("SUCCESS: Allocated 64 bytes at 0x%08X\n\r", (uint32_t)ptr1);
        
        /* Write test pattern */
        *(uint32_t*)ptr1 = 0xDEADBEEF;
        if (*(uint32_t*)ptr1 == 0xDEADBEEF) {
            printf("SUCCESS: Memory write/read test passed\n\r");
        } else {
            printf("ERROR: Memory write/read test failed\n\r");
        }
    } else {
        printf("ERROR: Failed to allocate 64 bytes\n\r");
    }
    
    /* Test 2: Medium allocation */
    printf("\nTest 2: Allocating 256 bytes...\n\r");
    void *ptr2 = Mem_Alloc(256);
    if (ptr2 != NULL) {
        printf("SUCCESS: Allocated 256 bytes at 0x%08X\n\r", (uint32_t)ptr2);
    } else {
        printf("ERROR: Failed to allocate 256 bytes\n\r");
    }
    
    /* Test 3: Large allocation */
    printf("\nTest 3: Allocating 1024 bytes...\n\r");
    void *ptr3 = Mem_Alloc(1024);
    if (ptr3 != NULL) {
        printf("SUCCESS: Allocated 1024 bytes at 0x%08X\n\r", (uint32_t)ptr3);
    } else {
        printf("ERROR: Failed to allocate 1024 bytes\n\r");
    }
    
    /* Test 4: Check alignment */
    printf("\nTest 4: Checking 32-bit alignment...\n\r");
    if (ptr1 && ((uint32_t)ptr1 % 4 == 0)) {
        printf("SUCCESS: ptr1 is 32-bit aligned\n\r");
    } else {
        printf("ERROR: ptr1 is not 32-bit aligned\n\r");
    }
    
    if (ptr2 && ((uint32_t)ptr2 % 4 == 0)) {
        printf("SUCCESS: ptr2 is 32-bit aligned\n\r");
    } else {
        printf("ERROR: ptr2 is not 32-bit aligned\n\r");
    }
    
    /* Test 5: Try to allocate more than available */
    printf("\nTest 5: Trying to allocate 5000 bytes (should fail)...\n\r");
    void *ptr4 = Mem_Alloc(5000);
    if (ptr4 == NULL) {
        printf("SUCCESS: Large allocation correctly failed\n\r");
    } else {
        printf("ERROR: Large allocation should have failed\n\r");
    }
    
    printf("\n=== MEMORY ALLOCATOR TEST END ===\n\r");
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

	/* Test Memory Allocator functionality */
	memory_allocator_test();

	/* Once all the basic services have been started, go to infinite loop to serviced activated tasks */
	for(;;)
    {
		vfnTask_Scheduler();
	}
}
