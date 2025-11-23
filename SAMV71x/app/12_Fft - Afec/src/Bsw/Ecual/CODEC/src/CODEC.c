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

/* ************************************************************************** */
/* Private Macros.
/* ************************************************************************** */
//CLK related macros.
#define FREQ_SCL            400000      //Desired SCL frequency.
#define FREQ_PERIPH_CLK     150000000   //Peripheral CLK frequency.
#define PMC_PCK2_PRES_VALUE 0           //Prescaler value for the PCK2 input clk.

//Pin related macros.
#define PCK2_PIN            1
#define I2C_PINS            2


/* ************************************************************************** */
/* Global data.
/* ************************************************************************** */

/* ************************************************************************** */
/* Private data.
/* ************************************************************************** */
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

/* ************************************************************************** */
/* Public functions.
/* ************************************************************************** */
/**
 * @brief 
 * 
 */
void CODEC_Init( void )
{   

    //Initialize related MCU peripherals.
    PMC_EnablePeripheral( ID_PIOA );
    MCLK_Init();
    I2C0_Init();

    //Configuring CODEC via I2C.
    WM8904_Init( &I2C0_control, WM8904_SLAVE_ADDRESS, PMC_PCK_CSS_SLOW_CLK );
}