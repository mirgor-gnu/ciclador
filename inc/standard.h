//********************************************************************
//!
//!   @file                standard.h
//!
//!   @brief               standard project header file
//!
//!   @author              Esteban Pupillo
//!
//!   @date                21 Apr 2020
//!
//********************************************************************

#ifndef STANDARD_H_
#define STANDARD_H_

#include "board_hw_config.h"
#include "board_hw_io.h"

//********************************************************************
// Constant and Macro Definitions using #define
//********************************************************************
#define false (0)
#define true  (!false)
#define IS_BOOL_STATE(STATE) (((STATE) == FALSE) || ((STATE) == TRUE))

#define Num_Elems(x)       (sizeof(x)/sizeof(x[0]))

//********************************************************************
// Enumerations and Structures and Typedefs
//********************************************************************

/**
 * Represents an standard return type for functions and APIS.
 */
typedef enum status_tag
{
   E_OK       = 0x00U,/**< No error */
   E_ERROR    = 0x01U,/**< Error while executing the corresponding function */
   E_BUSY     = 0x02U,/**< The system is busy and can't process the request. You should try again later */
   E_TIMEOUT  = 0x03U /**< A timeout has occurred during the execution.*/
}StatusType;

typedef enum
{
  FALSE = 0,
  TRUE = !FALSE
}Bool;

typedef Bool bool;

typedef void (*void_fptr) (void);               // void void Function pointer typedef
typedef uint8_t (*uint8_fptr) (void);           // uint8_t void function typedef
typedef bool (*bool_fptr) (void);               // boolean void function typedef
typedef void (*void_int16_fptr) (int16_t data); // void function taking 16 bit integer
typedef void (*void_bool_fptr) (bool data);     // Void function taking bool
typedef StatusType (*status_fptr) (void);       // Status_Type function pointer

//********************************************************************
// Global Variable extern Declarations
//********************************************************************

//********************************************************************
// Function Prototypes
//********************************************************************
// Remember to use extern modifier

#endif // _CLOCK_DRV_API_H
//********************************************************************
//
// Modification Record
//
//********************************************************************
//
//
//
//********************************************************************
