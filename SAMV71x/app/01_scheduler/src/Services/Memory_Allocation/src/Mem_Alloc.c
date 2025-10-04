/**
 * @file Mem_Alloc.c
 * @author Oscar Mercado omercadorico@gmail.com
 * @brief This file contains the definitions of private data and functions related to the Mem Alloc handler.
 * 
 */
/* ************************************************************************** */
/* Includes.  
/* ************************************************************************** */
#include "Mem_Alloc.h"

/* ************************************************************************** */
/* Private macros  
/* ************************************************************************** */
#define ADDR_ALIGN_BYTES    4   //Number of bytes to allign addresses.
#define ALIGN_4BYTES( addr ) ( ( uint8_t * ) ( ( ( ( uint32_t ) addr + ( ADDR_ALIGN_BYTES - 1 ) ) / ADDR_ALIGN_BYTES ) * ADDR_ALIGN_BYTES ) )

/* ************************************************************************** */
/* Private data.  
/* ************************************************************************** */
static MemHandlerType MemControl;

/* ************************************************************************** */
/* Functions.  
/* ************************************************************************** */
/**
 * @brief This function allocates memory space in heap_memalloc.
 * @note All memory spaces are contiguous. 
 * @note Addresses are always alligned to 32bits.
 * @note If there is not available space NULL is returned.
 * 
 * @param Size size in bytes to be allocated.
 * @return MemReturnType Initial address of the new allocated memory space.
 */
MemReturnType Mem_Alloc( MemSizeType Size )
{
    uint8_t *InitialAddr = 0;

    //Check if there is available space in heap_memalloc to allocate memory space.
    if ( ( Size > 0 ) && ( Size <= MemControl.FreeBytes ) )
    {   //Available space.
        InitialAddr = MemControl.CurrAddr;  //Initial address of new memory space.

        //Update current address.
        MemControl.CurrAddr += Size;

        //Align new current address with 4 bytes.
        if ( ( uint32_t ) MemControl.CurrAddr % 4 != 0 )
        {   //Not a valid address, round to closer address aligned.
            MemControl.CurrAddr =  ALIGN_4BYTES( MemControl.CurrAddr );
        }

        //Checking if aligned address exceeds final address of heap_memalloc.
        if ( MemControl.CurrAddr > MemControl.MemEnd )
        {   //Exceeds.
            MemControl.CurrAddr = MemControl.MemEnd;
        }

        //Update Free bytes.
        MemControl.FreeBytes = MemControl.MemEnd - MemControl.CurrAddr;
    }

    return ( MemReturnType ) InitialAddr;
}