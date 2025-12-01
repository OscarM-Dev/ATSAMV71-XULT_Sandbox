/**
 * @file CODEC.h
 * @author Oscar Mercado omercadorico@gmail.com
 * @brief This header contains the declaration of public types, structs and functions for operating the onboard audio CODEC.
 * 
 */
#ifndef CODEC_H
#define CODEC_H
/* ************************************************************************** */
/* Includes.
/* ************************************************************************** */
#include "stdint.h"
#include "chip.h"
#include "board.h"

/* ************************************************************************** */
/* Public function prototypes.
/* ************************************************************************** */
void CODEC_Init( void );
void CODEC_StartAudioCapture_MONO( void );
void CODEC_StopAudioCapture_MONO( void );
void CODEC_PrintAudioCaptured_MONO( uint16_t *data, uint32_t size );
#endif