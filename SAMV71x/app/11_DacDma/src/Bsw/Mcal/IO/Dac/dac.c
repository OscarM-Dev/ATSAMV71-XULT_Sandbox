/****************************************************************************************************/
/**
\file       dac.c
\brief      MCAL abstraction level - Digital to Analog Converter Controller configuration and interrupt handling.
\author     Abraham Tezmol
\version    1.0
\project    Tau 
\date       26/September/2016
*/
/****************************************************************************************************/

/*****************************************************************************************************
* Include files
*****************************************************************************************************/
/** dac function prototypes and definitions */
#include "dac.h"
/** ECG sample data */
#include "ecg_data.h"
/** DAC DMA types/APIs */
#include "dac_dma.h"
/** Chip-level peripherals (PMC/TC/DACC) */
#include "chip.h"
/** Board frequencies */
#include "board.h"
/* (Diagnostics via LED removed; use TIOA0 pin for scope verification) */

/*****************************************************************************************************
* Definition of module wide MACROs / #DEFINE-CONSTANTs 
*****************************************************************************************************/
/** SAMPLES per cycle*/
#define SAMPLES         1024
#define TEST_CHANNEL    5

/*****************************************************************************************************
* Definition of  VARIABLEs - 
*****************************************************************************************************/
/** */
//uint32_t dacBuffer[SAMPLES];  
//Experiment
__attribute__((aligned(64))) uint16_t dacBuffer[SAMPLES];

/** Global DMA driver for all transfer */
sXdmad dmad;
/** Global DAC DMA instance */
DacDma Dacd;
/** DAC command instance */
DacCmd DacCommand;

/*****************************************************************************************************
* Definition of module wide (CONST-) CONSTANTs 
*****************************************************************************************************/

/*****************************************************************************************************
* Code of module wide FUNCTIONS
*****************************************************************************************************/

/* ----------------------------------------------------------------------------------------------- */
/* Timer Counter configuration to generate 1 kHz HW trigger for DACC                               */
/* ----------------------------------------------------------------------------------------------- */
static void tc0_ch0_1khz(void)
{
	const uint32_t mck  = BOARD_MCK;
    const uint32_t div  = 128U;
    const uint32_t rc   = (mck/div)/1000U;
    const uint32_t ra   = rc/2U;
    TcChannel *tc = &TC0->TC_CHANNEL[0];

    PMC_EnablePeripheral(ID_PIOA);
    PIOA->PIO_PDR = PIO_PA0; 
    PIOA->PIO_ABCDSR[0] |=  PIO_PA0; 
    PIOA->PIO_ABCDSR[1] &= ~PIO_PA0;
    PIOA->PIO_PUDR = PIO_PA0; 

    PMC_EnablePeripheral(ID_TC0);
    tc->TC_CCR = TC_CCR_CLKDIS;
    tc->TC_CMR = TC_CMR_WAVE
               | TC_CMR_WAVSEL_UP_RC
               | TC_CMR_TCCLKS_TIMER_CLOCK4
               | TC_CMR_ACPA_SET
               | TC_CMR_ACPC_CLEAR;
    tc->TC_RA  = ra;
    tc->TC_RC  = rc;
    tc->TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;
}

/* ----------------------------------------------------------------------------------------------- */
/* DACC SETUP                                                                                     */
/* ----------------------------------------------------------------------------------------------- */
static void dac_setup(void) 
{
	PMC_EnablePeripheral(ID_DACC);
	DACC_SoftReset(DACC);
	DACC_EnableChannel(DACC, 0);
	DACC->DACC_MR &= ~DACC_MR_MAXS0;
	DACC->DACC_TRIGR = DACC_TRIGR_TRGEN0_EN | DACC_TRIGR_TRGSEL0_TRGSEL1;
	/* No IRQ */
	DACC->DACC_IDR = 0xFFFFFFFFu; //temp
}

static void dac_prepare_buffer(void)
{
	uint32_t i;
	uint32_t v;
	for (i = 0; i < SAMPLES; i++) {
		v = ecg_resampled_integer[i];
		if (v > 4095u) {
			v = 4095u;
		}
		dacBuffer[i] = (uint16_t)v;
	}
}

/* ----------------------------------------------------------------------------------------------- */
/* DACC Trigger Test                                                                               */
/* ----------------------------------------------------------------------------------------------- */
static inline int dacc_txrdy(void){
return (DACC->DACC_ISR & (DACC_TRIGR_TRGEN0_EN | DACC_TRIGR_TRGSEL0_TRGSEL1)) ? 1 : 0;
}

void dacc_feed_cpu_test(const uint16_t *buf, uint32_t n){
    uint32_t i = 0;
    DACC->DACC_CDR[0] = buf[i++ % n];
    while (1){
        if (dacc_txrdy()){
            DACC->DACC_CDR[0] = buf[i++ % n];
        }
    }
}

void prepare_buffer_test(void){
	uint32_t k;
	for (k=0;k<SAMPLES;k++) dacBuffer[k] = (k & 1) ? 4095 : 0;
}

/****************************************************************************************************/
/**
* \brief    SysTick - Initialization
* \author   Abraham Tezmol
* \param    int32_t base_freq	--> Requested SysTick interrupt frequency
* \param    tPtr_to_function sysTick_handler	--> Callback function to handle SysTick ISR.
* \return   int8_t --> 0 - Successful configuration of SysTick IRQ and SysTick Timer, 1 - Invalid configuration
* \todo
*/
void dac_initialization(void)
{
	//Just a test to validate DAC + Timer
	//prepare_buffer_test();
	//Put the ECG data into DAC buffer
	dac_prepare_buffer();
    tc0_ch0_1khz();
    dac_setup();
	//Just a test to validate DAC + Timer
	//dacc_feed_cpu_test(dacBuffer, SAMPLES);
    PMC_EnablePeripheral(ID_XDMAC); //temp
}

/**
 *  \brief Configure DAC DMA and start DMA transfer.
 * @note Initializes DACDMA and DACCMD control structures.
* \param    void
* \return   void
* \todo
*/
void dac_dmaTransfer( void )
{
	//Initialize DAC_DMA control struct.
	Dac_ConfigureDma( &Dacd, DACC, ID_DACC, &dmad );

	//Initialize DAC command control struct.
	DacCommand.dacChannel = DACC_CHANNEL_0;
	DacCommand.TxSize = SAMPLES;
	DacCommand.pTxBuff = ( uint8_t * ) dacBuffer;
	DacCommand.loopback = 0;

	/* Configure NVIC for DMA*/
  	NVIC_SetPriority( XDMAC_IRQn, 1 );
  	NVIC_EnableIRQ( XDMAC_IRQn );
	
	//Configure and initialize DMA channel for DAC0.
	Dac_SendData( &Dacd, &DacCommand );
}
