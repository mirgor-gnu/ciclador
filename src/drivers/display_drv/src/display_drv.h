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
//
//!   \file       display_drv.h
//!
//!   \brief      Display driver header file
//!
//!   \author     Agustin Diaz Antuna
//!
//!   \date       29 Apr 2020
//
//********************************************************************

#ifndef  _DISPLAY_DRV_H
#define  _DISPLAY_DRV_H 1

//********************************************************************
//! @addtogroup display_drv_imp
//! @{
//********************************************************************

//********************************************************************
// Constant and Macro Definitions using #define
//********************************************************************
#define DISPLAY_DRV_DATA               0x10
#define DISPLAY_DRV_COMMAND            0x20

#define HD44780_ROWS                   4-1
#define HD44780_COLS                   20-1
#define HD44780_ROW1_START             0x80
#define HD44780_ROW1_END               HD44780_COLS
#define HD44780_ROW2_START             0xC0
#define HD44780_ROW2_END               HD44780_COLS
#define HD44780_ROW3_START             0x90//0x94
#define HD44780_ROW3_END               HD44780_COLS
#define HD44780_ROW4_START             0xD0//0xD4
#define HD44780_ROW4_END               HD44780_COLS

// HD44780 registers
#define HD44780_CLEARDISPLAY           0x01
#define HD44780_RETURNHOME             0x02
#define HD44780_ENTRYMODESET           0x04
   #define HD44780_ENTRYRIGHT          0x00
   #define HD44780_ENTRYLEFT           0x02
   #define HD44780_ENTRYSHIFTDECREMENT 0x00
   #define HD44780_ENTRYSHIFTINCREMENT 0x01
#define HD44780_DISPLAYCONTROL         0x08
   #define HD44780_DISPLAYON           0x04
   #define HD44780_DISPLAYOFF          0x00
   #define HD44780_CURSORON            0x02
   #define HD44780_CURSOROFF           0x00
   #define HD44780_BLINKON             0x01
   #define HD44780_BLINKOFF            0x00
#define HD44780_CURDISPSHIFT           0x10
   #define HD44780_DISPLAYMOVE         0x08
   #define HD44780_CURSORMOVE          0x00
   #define HD44780_MOVERIGHT           0x04
   #define HD44780_MOVELEFT            0x00
#define HD44780_FUNCTIONSET            0x20
   #define HD44780_8BITMODE            0x10
   #define HD44780_4BITMODE            0x00
   #define HD44780_2LINE               0x08
   #define HD44780_1LINE               0x00
   #define HD44780_5x10DOTS            0x04
   #define HD44780_5x8DOTS             0x00
#define HD44780_SETCGRAMADDR           0x40
#define HD44780_SETDDRAMADDR           0x80

//********************************************************************
// Enumerations and Structures and Typedefs
//********************************************************************

//********************************************************************
// Global Variable extern Declarations
//********************************************************************

//********************************************************************
// Function Prototypes
//********************************************************************

#endif // _DISPLAY_DRV_H
//********************************************************************
//
// Modification Record
//
//********************************************************************
//
//
//
//********************************************************************

