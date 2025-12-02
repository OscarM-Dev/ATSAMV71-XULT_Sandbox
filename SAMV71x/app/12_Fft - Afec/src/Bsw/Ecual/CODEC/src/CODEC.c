/**
 * @file CODEC.c
 * @author Oscar Mercado omercadorico@gmail.com
 * @brief This file contains the definition of the CODEC related functions.
 * 
 */
/* ************************************************************************** */
/* Includes.
/* ************************************************************************** */
#include "CODEC.h"
#include "AUDIO_FFT.h"

/* ************************************************************************** */
/* Private Macros.
/* ************************************************************************** */
//CLK related macros.
#define FREQ_SCL            400000      //Desired SCL frequency.
#define FREQ_PERIPH_CLK     150000000   //Peripheral CLK frequency.
#define PMC_PCK2_PRES_VALUE 0           //Prescaler value for the PCK2 input clk.
#define SSC_NULL_MST_CLK    0           //Null freq for SSC master clock.
#define SSC_NULL_BITRATE    0           //Null bit rate for SSC.

//Pin related macros.
#define PCK2_PIN            1
#define I2C_PINS            2
#define I2S_PINS            3

//SSC related macros.
#define SSC_TCMR_CONFIG     SSC_TCMR_PERIOD( 0 ) | SSC_TCMR_STTDLY( 1 ) | SSC_TCMR_START_TF_EDGE | SSC_TCMR_CKG_CONTINUOUS | SSC_TCMR_CKO_NONE | SSC_TCMR_CKS_TK
#define SSC_TFMR_CONFIG     SSC_TFMR_FSEDGE_POSITIVE | SSC_TFMR_FSOS_NONE | SSC_TFMR_DATNB( 0 ) | SSC_TFMR_MSBF | SSC_TFMR_DATLEN( 15 )
#define SSC_RCMR_CONFIG     SSC_RCMR_PERIOD( 0 ) | SSC_RCMR_STTDLY( 1 ) | SSC_RCMR_START_RF_FALLING | SSC_RCMR_CKG_CONTINUOUS | SSC_RCMR_CKI | SSC_RCMR_CKO_NONE | SSC_RCMR_CKS_TK
#define SSC_RFMR_CONFIG     SSC_RFMR_FSEDGE_POSITIVE | SSC_RFMR_FSOS_NONE | SSC_RFMR_DATNB( 0 ) | SSC_RFMR_MSBF | SSC_RFMR_DATLEN( 31 )
#define SSC_IER_CONFIG      SSC_IER_RXRDY
#define SSC_IDR_CONFIG      SSC_IDR_RXRDY
#define SSC_IDR_DISABLE_ALL 0xFFFFFFFF
#define SSC_RXRDY_ISR_PRIO  1

//CODEC related.
#define DATA_BUFFER_SIZE    64000  //For 8s audio capture.

/* ************************************************************************** */
/* Global data.
/* ************************************************************************** */
volatile uint8_t CaptureAudioFlag = 0;
int32_t CODEC_Data[DATA_BUFFER_SIZE];

/* ************************************************************************** */
/* Private data.
/* ************************************************************************** */
static uint32_t i = 0;

/**
 * @brief I2C0 pin configuration struct.
 * 
 */
static Pin I2C0_PinConf[2] = 
{
    { .mask = PIO_PA3A_TWD0, .pio = PIOA, .id = ID_PIOA, .type = PIO_PERIPH_A, .attribute = PIO_DEFAULT },  //SDA pin.
    { .mask = PIO_PA4A_TWCK0, .pio = PIOA, .id = ID_PIOA, .type = PIO_PERIPH_A, .attribute = PIO_DEFAULT }   //SCL pin.
};

/**
 * @brief PCK2 pin configuration struct.
 * 
 */
static Pin PCK2_PinConf = { .mask = PIO_PA18B_PCK2, .pio = PIOA, .id = ID_PIOA, .type = PIO_PERIPH_B, .attribute = PIO_DEFAULT };

/**
 * @brief SSC pin configuration struct. 
 * 
 */
static Pin SSC_PinConf[3] =
{
    { .mask = PIO_PB1D_TK, .pio = PIOB, .id = ID_PIOB, .type = PIO_PERIPH_D, .attribute = PIO_DEFAULT },    //SCK pin , TK.
    { .mask = PIO_PD24B_RF, .pio = PIOD, .id = ID_PIOD, .type = PIO_PERIPH_B, .attribute = PIO_DEFAULT },   //WS pin, RF.
    { .mask = PIO_PA10C_RD, .pio = PIOA, .id = ID_PIOA, .type = PIO_PERIPH_C, .attribute = PIO_DEFAULT }    //SDIN pin, RD.
};

/**
 * @brief I2C0 control structure.
 * 
 */
static Twid I2C0_control;

/* ************************************************************************** */
/* Private functions.
/* ************************************************************************** */
/**
 * @brief This function initializes the I2C0 MCU peripheral.
 * 
 */
static void I2C0_Init( void )
{
    //Enable required peripherals.
    PMC_EnablePeripheral( ID_TWIHS0 );

    //Configure pins.
    PIO_Configure( I2C0_PinConf, I2C_PINS );

    //Initialize control structure.
    TWID_Initialize( &I2C0_control, TWIHS0 );

    //Configure I2C0 as master and desired bit rate -->400Kbps, aprox 403.225Kbps, data hold time = 20ns.
    TWI_ConfigureMaster( I2C0_control.pTwi, FREQ_SCL, FREQ_PERIPH_CLK );
}

/**
 * @brief This function initializes the PCK2 clk for the MCLK clk used by the CODEC.
 * @note the PCK2 clk freq is 32.768Khz.
 */
static void MCLK_Init( void )
{
    //Initialize pin.
    PIO_Configure( &PCK2_PinConf, PCK2_PIN );

    //SLCK input src clk to external crystals osc 32.768Khz.
    SUPC_SelectExtCrystal32K();

    //Configure PCK2 
    PMC_ConfigurePCK2( PMC_PCK_CSS_SLOW_CLK, PMC_PCK2_PRES_VALUE );
}

/**
 * @brief This function initializes SSC MCU peripheral as I2S.
 * @note RX clk is selected as TX clk which is TK input ( SCK ) 1.5Mhz.
 */
static void SSC_Init( void )
{
    //Enable and initialize required peripheral. -->SSC clk = 75Mhz.
    SSC_Configure( SSC, SSC_NULL_BITRATE, SSC_NULL_MST_CLK );

    //Initialize pins.
    PIO_Configure( SSC_PinConf, I2S_PINS );

    //Configure transmitter -->just for selecting TX clk as TK input ( SCK ).
    SSC_ConfigureTransmitter( SSC, SSC_TCMR_CONFIG, SSC_TFMR_CONFIG );

    //Configure receiver.
    SSC_ConfigureReceiver( SSC, SSC_RCMR_CONFIG, SSC_RFMR_CONFIG );

    //Disable transmitter and receiver.
    SSC_DisableTransmitter( SSC );
    SSC_DisableReceiver( SSC );

    //Disable interrupts.
    SSC_DisableInterrupts( SSC, SSC_IDR_DISABLE_ALL );

    //Configure NVIC for SSC interrupts.
    NVIC_SetPriority( SSC_IRQn, SSC_RXRDY_ISR_PRIO );
    NVIC_EnableIRQ( SSC_IRQn );
}

/**
 * @brief This function clears the data buffer for captured audio.
 * 
 * @param data Pointer to data buffer.
 * @param bufferSize Number of buffer data elements.
 */
static void Clear_DataBuffer( int32_t *data, uint32_t bufferSize )
{
    uint32_t i = 0;

    for ( i = 0; i < bufferSize; i++ )
    {
        data[i] = 0;
    }
}

/* ************************************************************************** */
/* Public functions.
/* ************************************************************************** */
/**
 * @brief This function is the SSC ISR.
 * @note For now only RXRDY interrupt calls this function.
 */
void SSC_Handler( void )
{
    if ( i < DATA_BUFFER_SIZE )
    {   //Store data received.
        CODEC_Data[i] = ( ( int32_t ) SSC_Read( SSC ) ) >> 8;
        i++;
    }

    else
    {   //Store and ignore data received.
        (void)SSC_Read(SSC);
    }
}

void WM8904_BoostADCVolume(Twid *pTwid, uint32_t device)
{
    uint16_t vol = 0x01F0;
    TWI_EnableMaster(pTwid->pTwi);
    WM8904_Write(pTwid, device, 0x24, vol);  // Left
    WM8904_Write(pTwid, device, 0x25, vol);  // Right
    TWI_DisableMaster(pTwid->pTwi);
}

/**
 * @brief This function initializes all the MCU configurations and the CODEC configuration for audio capture.
 * @note Mono audio capture is used with the Left MIC channel due to RAM limitations.
 */
void CODEC_Init( void )
{   
    printf( "Initializing CODEC \n\r" );
    //Initialize related MCU peripherals.
    PMC_EnablePeripheral( ID_PIOA );
    PMC_EnablePeripheral( ID_PIOB );
    PMC_EnablePeripheral( ID_PIOD );
    MCLK_Init();
    SSC_Init();
    I2C0_Init();

    //Configuring CODEC via I2C.
    WM8904_Init( &I2C0_control, WM8904_SLAVE_ADDRESS, PMC_PCK_CSS_SLOW_CLK );

    WM8904_IN2R_IN1L(&I2C0_control, WM8904_SLAVE_ADDRESS);

    WM8904_BoostADCVolume(&I2C0_control, WM8904_SLAVE_ADDRESS);

    TWI_EnableMaster(I2C0_control.pTwi);
    uint16_t id = WM8904_Read(&I2C0_control, WM8904_SLAVE_ADDRESS, 0x00);
    printf("WM8904 ID (R0) = 0x%04X\r\n", id);
    TWI_DisableMaster(I2C0_control.pTwi);
}

/**
 * @brief This function triggers the start for the mono audio capture.
 * 
 */
void CODEC_StartAudioCapture_MONO( void )
{
    printf( "Starting audio capture \n\r" );
    Clear_DataBuffer( CODEC_Data, DATA_BUFFER_SIZE );
    WM8904_EnableLeftADC( &I2C0_control, WM8904_SLAVE_ADDRESS );
    SSC_EnableInterrupts( SSC, SSC_IER_CONFIG );
    SSC_EnableReceiver( SSC );
}

/**
 * @brief This function triggers the stop for the mono audio capture.
 * 
 */
void CODEC_StopAudioCapture_MONO( void )
{
    printf( "Stoping audio capture \n\r" );
    SSC_DisableInterrupts( SSC, SSC_IDR_CONFIG );
    SSC_DisableReceiver( SSC );
    
    uint32_t used = i;

    WM8904_DisableLeftADC( &I2C0_control, WM8904_SLAVE_ADDRESS );

    uint16_t sample = CODEC_Data[100];
    printf("Sample[100] = %u\r\n", sample);

    uint16_t raw;
    TWI_EnableMaster(I2C0_control.pTwi);
    raw = WM8904_Read(&I2C0_control, WM8904_SLAVE_ADDRESS, 0x2C);
    printf("R44 Analogue Left Input 0 = 0x%04X\r\n", raw);
    raw = WM8904_Read(&I2C0_control, WM8904_SLAVE_ADDRESS, 0x2D);
    printf("R45 Analogue Right Input 0 = 0x%04X\r\n", raw);
    raw = WM8904_Read(&I2C0_control, WM8904_SLAVE_ADDRESS, 0x2E);
    printf("R46 Analogue Left Input 1 = 0x%04X\r\n", raw);
    raw = WM8904_Read(&I2C0_control, WM8904_SLAVE_ADDRESS, 0x2F);
    printf("R47 Analogue Right Input 1 = 0x%04X\r\n", raw);
    raw = WM8904_Read(&I2C0_control, WM8904_SLAVE_ADDRESS, 0x0A);
    printf("R10 Analogue ADC 0 = 0x%04X\r\n", raw);
    raw = WM8904_Read(&I2C0_control, WM8904_SLAVE_ADDRESS, 0x24);
    printf("R36 ADC Digital Volume Left = 0x%04X\r\n", raw);
    raw = WM8904_Read(&I2C0_control, WM8904_SLAVE_ADDRESS, 0x25);
    printf("R37 ADC Digital Volume Right = 0x%04X\r\n", raw);
    TWI_DisableMaster(I2C0_control.pTwi);

    int16_t min =  32767;
    int16_t max = -32768;

    uint32_t k;
    for (k = 0; k < used; k++) {
        if (CODEC_Data[k] < min) min = CODEC_Data[k];
        if (CODEC_Data[k] > max) max = CODEC_Data[k];
    }

    printf("Captured samples: %lu\r\n", (unsigned long)used);
    printf("Signal min = %d, max = %d\r\n", min, max);

    AUDIO_FFT_ProcessCapturedAudio(CODEC_Data, used);

    CODEC_PrintAudioCaptured_MONO( CODEC_Data, DATA_BUFFER_SIZE );

    i = 0;
}

/**
 * @brief This function prints all the data audio captured via serial terminal.
 * @note Is useful to take this printed data in TeraTerm for ploting.
 * @note Can be used for the raw audio captured or the fft audio captured.
 * 
 * @param data Pointer to data buffer.
 * @param size Number of data elements of buffer.
 */
void CODEC_PrintAudioCaptured_MONO( int32_t *data, uint32_t size )
{
    uint32_t i;

    for (i = 0; i < size; i += 5)
    {
        printf( "AUDIO_DATA[%u] = %d\n\r", i, data[i] );
    }
}