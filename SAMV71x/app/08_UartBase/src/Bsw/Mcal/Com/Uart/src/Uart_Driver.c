/**
 * @file Uart_Driver.c
 * @author Oscar Mercado omercadorico@gmail.com
 * @brief This file contains the definition of the UART MCAL driver functions closely related to the peripheral registers.
 * 
 */
/* ************************************************************************** */
/* Includes.  
/* ************************************************************************** */
#include "Uart_Driver.h"


/* ************************************************************************** */
/* Public functions.  
/* ************************************************************************** */
/**
 * @brief This function configures the given UART peripheral with the specified parameters. 
 * 
 * @param uart Pointer to the base addr of the UART to configure.
 * @param mode CHMODE to configure.
 * @param clkSrc Peripheral clock source.
 * @param parity Parity type.
 * @param baudrate Desired baudrate in hz.
 * @param clkSrcFreq Peripheral clock source frequency in hz.
 */
void UART_Driver_Configure( Uart *uart, uint16_t mode, uint16_t clkSrc, uint16_t parity, uint32_t baudrate, uint32_t clkSrcFreq )
{
	//Reset and disable receiver & transmitter
	uart->UART_CR = UART_CR_RSTRX | UART_CR_RSTTX | UART_CR_RXDIS | UART_CR_TXDIS | UART_CR_RSTSTA;

    //Disabling all interrupts.
	uart->UART_IDR = UART_IDR_RXRDY | UART_IDR_TXRDY | UART_IDR_OVRE | UART_IDR_FRAME | UART_IDR_PARE | UART_IDR_TXEMPTY | UART_IDR_CMP;

	//Configure mode register.
	uart->UART_MR = ( mode << 14 ) | ( clkSrc << 12 ) | ( parity << 9 ) | UART_MR_FILTER_DISABLED;

	//Configure default baudrate.
    UART_Driver_SetBaudrate( uart, baudrate, clkSrcFreq );
}

/**
 * @brief This function configures the baudrate of the given UART peripheral.
 * 
 * @param uart Pointer to the base addr of the UART to configure. 
 * @param baudrate Desired baudrate in hz.
 * @param clkSrcFreq Peripheral clock source frequency in hz.
 */
void UART_Driver_SetBaudrate( Uart *uart, uint32_t baudrate, uint32_t clkSrcFreq )
{   
    uart->UART_CR = UART_CR_RSTTX;
    uart->UART_BRGR = clkSrcFreq / ( baudrate * 16 );
}

/**
 * @brief This function enables or disables the local interrupts of the given UART peripheral. 
 * 
 * @param uart Pointer to the base addr of the UART to configure. 
 * @param enable Enable flag.
 * @param mode Interrupt mode.
 */
void UART_Driver_SetIt( Uart *uart, uint8_t enable, uint32_t mode )
{
    if ( enable )
    {
        uart->UART_IER = mode;
    }

    else
    {
        uart->UART_IDR = mode;
    }
}

/**
 * @brief This function enables or disables the transmitter of the given UART peripheral.
 * 
 * @param uart Pointer to the base addr of the UART to configure.
 * @param enable Enable flag.
 */
void UART_Driver_SetTx( Uart *uart, uint8_t enable )
{
    if ( enable )
    {
        uart->UART_CR = UART_CR_TXEN;
    }

    else
    {
        uart->UART_CR = UART_CR_TXDIS;
    }
}

/**
 * @brief This function transmits a single data byte with the given UART peripheral.
 * @note This function is based in interrupts.
 * 
 * @param uart Pointer to the base addr of the UART to configure.
 * @param data Data byte.
 */
void UART_Driver_SendByte( Uart *uart, uint8_t data )
{
    uart->UART_THR = data;
}

/**
 * @brief This function returns the actual value of TxReady flag.
 * 
 * @param uart Pointer to the base addr of the UART to configure.
 * @retval TxReady flag value.
 */
uint8_t UART_Driver_IsTxReady( Uart *uart )
{   
    uint8_t result = 0;

    if ( uart->UART_SR & UART_SR_TXRDY )
    {
        result = 1;
    }

    else
    {
        result = 0;
    }

    return result;
}

/**
 * @brief This function returns the actual Status register value.
 * 
 * @param uart Pointer to the base addr of the UART to configure.
 * @retval Status register value. 
 */
uint32_t UART_Driver_GetStatus( Uart *uart )
{
	return uart->UART_SR;
}