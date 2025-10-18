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
void UART_Driver_Configure( Uart *uart, uint8_t mode, uint8_t clkSrc, uint8_t parity, uint32_t baudrate, uint32_t clkSrcFreq )
{

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

}

/**
 * @brief This function enables or disables the transmitter of the given UART peripheral.
 * 
 * @param uart Pointer to the base addr of the UART to configure.
 * @param enable Enable flag.
 */
void UART_Driver_SetTx( Uart *uart, uint8_t enable )
{

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

}

/**
 * @brief This function returns the actual value of TxReady flag.
 * 
 * @param uart Pointer to the base addr of the UART to configure.
 * @retval TxReady flag value.
 */
uint8_t UART_Driver_IsTxReady( Uart *uart )
{

}