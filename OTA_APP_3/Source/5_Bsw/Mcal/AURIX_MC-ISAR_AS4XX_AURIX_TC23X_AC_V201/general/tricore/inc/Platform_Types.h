/******************************************************************************
**                                                                           **
** Copyright (C) Infineon Technologies (2013)                                **
**                                                                           **
** All rights reserved.                                                      **
**                                                                           **
** This document contains proprietary information belonging to Infineon      **
** Technologies. Passing on and copying of this document, and communication  **
** of its contents is not permitted without prior written authorization.     **
**                                                                           **
*******************************************************************************
**                                                                           **
**  $FILENAME   : Platform_Types.h $                                         **
**                                                                           **
**  $CC VERSION : \main\7 $                                                  **
**                                                                           **
**  $DATE       : 2014-08-12 $                                               **
**                                                                           **
**  AUTHOR      : DL-AUTOSAR-Engineering                                     **
**                                                                           **
**  VENDOR      : Infineon Technologies                                      **
**                                                                           **
**  DESCRIPTION : File holding the definition of all the Platform types      **
**                as defined by Autosar                                      **                      
**                                                                           **
**  MAY BE CHANGED BY USER [yes/no]: Yes                                     **
**                                                                           **
******************************************************************************/

#ifndef _pPLATFORM_TYPES_H_
#define _pPLATFORM_TYPES_H_

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/

/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
/*
  File version information
*/
#define PLATFORM_MAJOR_VERSION 5
#define PLATFORM_MINOR_VERSION 0
#define PLATFORM_PATCH_VERSION 1

/* 
  CPU register type width
*/
#define CPU_TYPE_8 8
#define CPU_TYPE_16 16
#define CPU_TYPE_32 32

/* 
  Bit order definition
*/
#define MSB_FIRST 0 /* Big endian bit ordering        */
#define LSB_FIRST 1 /* Little endian bit ordering     */

/* 
  Byte order definition
*/
#define HIGH_BYTE_FIRST 0 /* Big endian byte ordering       */
#define LOW_BYTE_FIRST 1  /* Little endian byte ordering    */

/* 
  Word order definition
*/
#define HIGH_WORD_FIRST 0 /* Big endian word ordering       */
#define LOW_WORD_FIRST 1  /* Little endian word ordering    */

/*
  Platform type and endianess definitions for TC1796
*/
#define CPU_TYPE (CPU_TYPE_32)
#define CPU_BIT_ORDER (LSB_FIRST)
#define CPU_BYTE_ORDER (LOW_BYTE_FIRST)
#define CPU_WORD_ORDER (LOW_WORD_FIRST)
#define EE_OK 0x0
#define EE_NOT_OK 0x1
/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/

/*
  AUTOSAR integer data types
*/
typedef signed char sint8;	   /*        -128 .. +127            */
typedef unsigned char uint8;   /*           0 .. 255             */
typedef signed short sint16;   /*      -32768 .. +32767          */
typedef unsigned short uint16; /*           0 .. 65535           */
typedef signed long sint32;	   /* -2147483648 .. +2147483647     */
typedef unsigned long uint32;  /*           0 .. 4294967295      */
typedef float float32;
typedef double float64;

typedef unsigned long uint8_least;	/* At least 8 bit                 */
typedef unsigned long uint16_least; /* At least 16 bit                */
typedef unsigned long uint32_least; /* At least 32 bit                */
typedef signed long sint8_least;	/* At least 7 bit + 1 bit sign    */
typedef signed long sint16_least;	/* At least 15 bit + 1 bit sign   */
typedef signed long sint32_least;	/* At least 31 bit + 1 bit sign   */

typedef unsigned char boolean; /* for use with TRUE/FALSE        */

#ifndef TRUE /* conditional check */
#define TRUE 1U
#endif

#ifndef FALSE /* conditional check */
#define FALSE 0U
#endif

/* posix conform: osdSuppressPosixTypes of Operating system */
#ifdef osdSuppressPosixTypes
typedef char int8;	 /*        -128 .. +127            */
typedef short int16; /*      -32768 .. +32767          */
typedef long int32;	 /* -2147483648 .. +2147483647     */
#endif

/*******************************************************************************
**                      Global Data                                           **
*******************************************************************************/

/*******************************************************************************
**                      Global Function Prototypes                            **
*******************************************************************************/
/* For mando AB */
typedef unsigned int uBitType;
typedef struct TYPE_BITA_Byte
{
	unsigned BIT7 : 1;
	unsigned BIT6 : 1;
	unsigned BIT5 : 1;
	unsigned BIT4 : 1;
	unsigned BIT3 : 1;
	unsigned BIT2 : 1;
	unsigned BIT1 : 1;
	unsigned BIT0 : 1;
} BITA_Byte;

typedef struct TYPE_BIT_Byte
{
	unsigned BIT0 : 1;
	unsigned BIT1 : 1;
	unsigned BIT2 : 1;
	unsigned BIT3 : 1;
	unsigned BIT4 : 1;
	unsigned BIT5 : 1;
	unsigned BIT6 : 1;
	unsigned BIT7 : 1;
} BIT_Byte;

typedef struct TYPE_BITA_WORD
{
	unsigned BIT15 : 1;
	unsigned BIT14 : 1;
	unsigned BIT13 : 1;
	unsigned BIT12 : 1;
	unsigned BIT11 : 1;
	unsigned BIT10 : 1;
	unsigned BIT9 : 1;
	unsigned BIT8 : 1;
	unsigned BIT7 : 1;
	unsigned BIT6 : 1;
	unsigned BIT5 : 1;
	unsigned BIT4 : 1;
	unsigned BIT3 : 1;
	unsigned BIT2 : 1;
	unsigned BIT1 : 1;
	unsigned BIT0 : 1;
} BITA_Word;

typedef signed char int8_t;
typedef short int int16_t;
typedef long int int32_t;

typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;
typedef unsigned long int uint32_t;
typedef unsigned long long int uint64_t;

typedef char char8_t;
typedef volatile int8_t vint8_t;
typedef volatile uint8_t vuint8_t;

typedef volatile int16_t vint16_t;
typedef volatile uint16_t vuint16_t;

typedef volatile int32_t vint32_t;
typedef volatile uint32_t vuint32_t;

/* Data types from eetypes.h
*
* */

typedef unsigned char BOOL;

typedef signed char INT8;
typedef unsigned char UINT8;
typedef volatile signed char VINT8;
typedef volatile unsigned char VUINT8;

typedef signed short INT16;
typedef unsigned short UINT16;
typedef volatile signed short VINT16;
typedef volatile unsigned short VUINT16;

typedef signed long INT32;
typedef unsigned long UINT32;
typedef volatile signed long VINT32;
typedef volatile unsigned long VUINT32;

typedef float float32_t;

#define SIZE_OF_INT8_T 1u
#define SIZE_OF_UINT8_T 1u
#define SIZE_OF_INT16_T 2u
#define SIZE_OF_UINT16_T 2u

#include "Std_Types.h"

#endif /* PLATFORM_TYPES_H */
