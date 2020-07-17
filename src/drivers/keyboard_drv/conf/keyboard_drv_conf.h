/*
 *
 * MIT License
 *
 * Copyright (c) 2020 Mirgor
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

//********************************************************************
//!
//!   @file       keyboard_drv_conf.h
//!
//!   @brief      Keyboard Driver configuration header file
//!
//!   @author     Agustin Diaz Antuna
//!
//!   @date       29 Apr 2020
//!
//********************************************************************

#ifndef  _KEYBOARD_DRV_CONF_H
#define  _KEYBOARD_DRV_CONF_H 1

//********************************************************************
//! @addtogroup keyboard_drv_conf
//! @{
//********************************************************************

//********************************************************************
// Constant and Macro Definitions using #define
//********************************************************************
#define KEYBOARD_HAS_LKP            false
/**
 * Update period in milliseconds
 */
#define KEYBOARD_UPDATE_TIME_MS     (5*16)

/**
 * LKP detection time in milliseconds
 */
#define KEYBOARD_LKP_TIME_MS        (2000)

/**
 * LKP threshold time in milliseconds.
 * If the button is pressed longer than this time, it will be reported
 * as a malfunction and an alarm will be shown.
 */
#define KEYBOARD_LKP_TIMEOUT_MS     (30000)


/**
 * Keyboard column size
 */
#define KEYBOARD_DRV_COLUMNSIZE 3

/**
 * Keyboard row size
 */
#define KEYBOARD_DRV_ROWSIZE    3

/**
 * Keyboard row setup.
 * It associate each row to their respective pinout.
 * 
 * The input format is: X([row_id], [io_pin_id])
 */
#define KEYBOARD_DRV_ROWS_CFG \
   X(ROW0 ,   IO_KEY_R0   )\
   X(ROW1 ,   IO_KEY_R1   )\
   X(ROW2 ,   IO_KEY_R2   )\

/**
 * Keyboard column setup.
 * It associate each column to their respective pinout.
 * 
 * The input format is: X([column_id], [io_pin_id])
 */
#define KEYBOARD_DRV_COLUMNS_CFG \
   X(COLUMN0 ,   IO_KEY_C0   )  \
   X(COLUMN1 ,   IO_KEY_C1   )  \
   X(COLUMN2 ,   IO_KEY_C2   )  \

/**
 * Key mapping setup.
 * It associate each key with a combination of one column and one row.
 * 
 * The input format is: X([id], [row_id], [column_id])
 */
#define KEYBOARD_DRV_KEYS_CFG \
   X(KEY_EMERGENCY  , IO_KEY_R0 , IO_KEY_C0 )  \
   X(KEY_STOP       , IO_KEY_R0 , IO_KEY_C1 )  \
   X(KEY_START      , IO_KEY_R0 , IO_KEY_C2 )  \
   X(KEY_PARAMETER4 , IO_KEY_R1 , IO_KEY_C0 )  \
   X(KEY_UP         , IO_KEY_R1 , IO_KEY_C1 )  \
   X(KEY_DOWN       , IO_KEY_R1 , IO_KEY_C2 )  \
   X(KEY_PARAMETER1 , IO_KEY_R2 , IO_KEY_C0 )  \
   X(KEY_PARAMETER2 , IO_KEY_R2 , IO_KEY_C1 )  \
   X(KEY_PARAMETER3 , IO_KEY_R2 , IO_KEY_C2 )  \

//********************************************************************
// Enumerations and Structures and Typedefs
//********************************************************************

//********************************************************************
// Global Variable extern Declarations
//********************************************************************

//********************************************************************
// Function Prototypes
//********************************************************************

#endif // _KEYBOARD_DRV_CONF_H
//********************************************************************
//
// Modification Record
//
//********************************************************************
//
// 
//
//********************************************************************

