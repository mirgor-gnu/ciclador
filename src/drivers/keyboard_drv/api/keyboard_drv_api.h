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
//!   \file       keyboard_drv_api.h
//!
//!   \brief      Keyboard Driver APIs header file
//!
//!   \author     Agustin Diaz Antuna
//!
//!   \date       29 Apr 2020
//!
//********************************************************************

#ifndef  _KEYBOARD_DRV_API_H
#define  _KEYBOARD_DRV_API_H 1

#include "keyboard_drv_conf.h"

//********************************************************************
//! @addtogroup keyboard_drv_api
//!   @{
//********************************************************************

//********************************************************************
// Constant and Macro Definitions using #define
//********************************************************************

//********************************************************************
// Enumerations and Structures and Typedefs
//********************************************************************
#undef X
 #define X(a, b, c) a,
 /**
  * Key Id.
  * All the available keys.
  */
 typedef enum {
    NO_KEY,
    KEYBOARD_DRV_KEYS_CFG
    MAX_KEYS,
 } KeyIdType;

/**
 * Key type.
 * Used to identify if the detected key was pressed for a long or a
 * short period of time.
 */
typedef enum key_press_tag {
   SKP,
   LKP,
} KeyPressType;

//********************************************************************
// Global Variable extern Declarations
//********************************************************************

//********************************************************************
// Function Prototypes
//********************************************************************
// Remember to use extern modifier
/**
 * Initializes the Keyboard Driver.
 * This function shall be called before call any other API function
 *
 * @return #E_OK if initialization is successful\n
 *         #E_ERROR is initialization fails
 */
extern StatusType KeyboardDrv_Init(void);

/**
 * Driver periodic update.
 * This function shall be called at fixed time intervals. This time
 * must be configured wisely in #KEYBOARD_UPDATE_TIME_MS so the key
 * debounce work as expected.
 */
extern void KeyboardDrv_Update(void);

/**
 * Convert a key map to a KeyId.
 *
 * @param keyMap each active bit represent a button.
 *
 * @return KeyIdType a valid KeyId
 */
extern KeyIdType KeyboardDrv_ConvertKeyMapToKeyId(uint32_t keyMap);

#endif // _KEYBOARD_DRV_API_H
//********************************************************************
//
// Modification Record
//
//********************************************************************
//
// 
//
//********************************************************************

