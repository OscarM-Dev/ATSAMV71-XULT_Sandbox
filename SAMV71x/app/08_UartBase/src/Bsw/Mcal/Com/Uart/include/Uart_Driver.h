/**
 * @file Uart_Driver.h
 * @author Oscar Mercado omercadorico@gmail.com
 * @brief This header contains the declaration of the UART MCAL driver functions closely related to the peripheral registers.
 * 
 */
#ifndef UART_DRIVER_H
#define UART_DRIVER_H
/* ************************************************************************** */
/* Includes.  
/* ************************************************************************** */
#include "chip.h"

/* ************************************************************************** */
/* Public function prototypes  
/* ************************************************************************** */
void UART_Driver_Configure( Uart *uart, uint16_t mode, uint16_t clkSrc, uint16_t parity, uint32_t baudrate, uint32_t clkSrcFreq );
void UART_Driver_SetBaudrate( Uart *uart, uint32_t baudrate, uint32_t clkSrcFreq );
void UART_Driver_SetIt( Uart *uart, uint8_t enable, uint32_t mode );
void UART_Driver_SetTx( Uart *uart, uint8_t enable );
void UART_Driver_SendByte( Uart *uart, uint8_t data );
uint8_t UART_Driver_IsTxReady( Uart *uart );

#endif