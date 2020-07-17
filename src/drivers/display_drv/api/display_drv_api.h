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
//!   \file       display_drv_api.h
//!
//!   \brief      Display Driver APIs header file
//!
//!   \author     Agustin Diaz Antuna
//!
//!   \date       29 Apr 2020
//!
//********************************************************************

#ifndef  _DISPLAY_DRV_API_H
#define  _DISPLAY_DRV_API_H 1

//********************************************************************
//! @addtogroup display_drv_api
//!   @{
//********************************************************************

//********************************************************************
// Constant and Macro Definitions using #define
//********************************************************************

//********************************************************************
// Enumerations and Structures and Typedefs
//********************************************************************

//********************************************************************
// Global Variable extern Declarations
//********************************************************************

//********************************************************************
// Function Prototypes
//********************************************************************
// Remember to use extern modifier
/**
 * Initializes the Display Driver.
 * This function shall be called before call any other API function
 *
 * @return #E_OK if initialization is successful\n
 *         #E_ERROR is initialization fails
 */
extern StatusType DisplayDrv_Init(void);

/**
 * Driver periodic update.
 * This function shall be called with a micro seconds interval period
 * to read the display buffer and write each byte to the display in
 * 4-bit-mode generating the respective E pulse.
 */
extern void DisplayDrv_UpdateData(void);

/**
 * Clear all display.
 * Warning: This is the slowest command. Must have at least a 1 milli
 * seconds delay after a new command.
 */
extern void DisplayDrv_ClearDisplay(void);

/**
 * Write a string into the display.
 * It writes from the cursor position.
 *
 * @param str string to write. Must end with '\0' character.
 */
extern void DisplayDrv_WriteString(char *str);

/**
 * Write a char on the display.
 * It writes from the cursor position.
 * 
 * @param character char to write.
 */
extern void DisplayDrv_WriteChar(char character);

/**
 * Move the cursor in a specific position.
 *
 * @param x desire position inside a line. Starts at 0.
 * @param y desire line. Look at LINE defines.
 *
 * @return #E_OK if is a valid position\n
 *         #E_ERROR is a non valid position
 */
extern uint8_t DisplayDrv_PositionXY(char x, char y);

/**
 * Enable the cursor.
 */
extern void DisplayDrv_CursorOn(void);

/**
 * Disable the cursor.
 */
extern void DisplayDrv_CursorOff(void);

#endif // _DISPLAY_DRV_API_H
//********************************************************************
//
// Modification Record
//
//********************************************************************
//
// 
//
//********************************************************************

