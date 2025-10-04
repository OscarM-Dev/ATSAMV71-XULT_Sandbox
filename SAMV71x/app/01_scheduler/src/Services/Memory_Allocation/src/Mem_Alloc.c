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
/* Private data.  
/* ************************************************************************** */


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

}