/**
 * @file Mem_AllocTypes.h
 * @author Oscar Mercado omercadorico@gmail.com
 * @brief This header contains the declaration of all the internal data types related to the Mem Alloc handler.
 * 
 */
#ifndef MEM_ALLOCTYPES_H
#define MEM_ALLOCTYPES_H
/* ************************************************************************** */
/* Includes.  
/* ************************************************************************** */
#include <stdint.h>

/* ************************************************************************** */
/* Memory allocation data types.  
/* ************************************************************************** */
/**
 * @brief Return type for Mem Alloc function. 
 * 
 */
typedef void * MemReturnType;

/**
 * @brief Type for Memory Size in bytes.
 * 
 */
typedef uint16_t MemSizeType;

/**
 * @brief Type for memory handler control structure.
 * 
 */
typedef struct 
{
    uint8_t *MemStart;  //heap_memalloc start address.
    uint8_t *MemEnd;    //heap_memalloc end address.
    uint8_t *CurrAddr;  //Current address available in heap_memalloc.
    uint32_t FreeBytes; //Amount of bytes available in heap memalloc.
} MemHandlerType;

#endif