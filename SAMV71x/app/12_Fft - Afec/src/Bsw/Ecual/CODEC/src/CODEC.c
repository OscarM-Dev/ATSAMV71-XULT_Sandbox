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

//Pin related macros.
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
 * @brief I2C0 control structure.
 * 
 */
static Twid I2C0_control;

/* ************************************************************************** */
/* Private functions.
/* ************************************************************************** */
/**
 * @brief This function initialices the I2C0 MCU peripheral.
 * 
 */
static I2C0_Init( void )
{
    //Enable required peripherals.
    PMC_EnablePeripheral( ID_PIOA );
    PMC_EnablePeripheral( ID_TWIHS0 );

    //Configure pins.
    PIO_Configure( I2C0_PinConf, I2C_PINS );

    //Initialize control structure.
    TWID_Initialize( &I2C0_control, TWIHS0 );

    //Configure I2C0 as master and desired bit rate -->400Kbps, aprox 403.225Kbps, data hold time = 20ns.
    TWI_ConfigureMaster( I2C0_control.pTwi, FREQ_SCL, FREQ_PERIPH_CLK );
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
    I2C0_Init();
}