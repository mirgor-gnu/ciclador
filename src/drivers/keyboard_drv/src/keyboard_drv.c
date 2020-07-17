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
//!   \file       keyboard_drv.c
//!
//!   \brief      This is the Keyboard Driver implementation
//!
//!   \author     Agustin Diaz Antuna
//!
//!   \date       29 Apr 2020
//
//********************************************************************

//********************************************************************
// Include header files                                              
//********************************************************************
#include "standard.h"
#include "stm32f1xx_hal.h"
#include "board_hw_io.h"

//********************************************************************
//! @addtogroup keyboard_drv_imp
//! @{
//********************************************************************

#include "keyboard_drv_conf.h"
#include "keyboard_drv_callouts.h"
#include "keyboard_drv_api.h"

//********************************************************************
// File level pragmas
//********************************************************************

//********************************************************************
// Constant and Macro Definitions using #define
//********************************************************************
#define KEYBOARD_LKP_TIME           (KEYBOARD_LKP_TIME_MS / KEYBOARD_UPDATE_TIME_MS)

//********************************************************************
// Enumerations and Structures and Typedefs
//********************************************************************
typedef struct keyboard_drv_data_tag
{
   uint32_t lastNewKey;
   uint32_t lastNewKeyTimelapse;
   bool     lkpDetected;
} KeyboardDrvDataType;

//********************************************************************
// Function Prototypes for Private Functions with File Level Scope
//********************************************************************
uint32_t KeyboardDrv_Scan(void);

//********************************************************************
// ROM Const Variables With File Level Scope
//********************************************************************
#undef X
#define X(a, b, c) a,
static uint8_t const keyboard_drv_keys[KEYBOARD_DRV_ROWSIZE][KEYBOARD_DRV_COLUMNSIZE] = 
   {
      KEYBOARD_DRV_KEYS_CFG
   };

#undef X
#define X(a,b) b,
static uint8_t const keyboard_drv_rows_cfg[] =
   {
      KEYBOARD_DRV_ROWS_CFG
   };
static uint8_t const keyboard_drv_columns_cfg[] =
   {
      KEYBOARD_DRV_COLUMNS_CFG
   };

//********************************************************************
// Static Variables and Const Variables With File Level Scope
//********************************************************************
static KeyboardDrvDataType keyboardData;

//********************************************************************
// Function Definitions
//********************************************************************
StatusType KeyboardDrv_Init(void)
{
   keyboardData.lastNewKey = NO_KEY;
   keyboardData.lastNewKeyTimelapse = 0;
   keyboardData.lkpDetected = 0;
   return E_OK;
}

void KeyboardDrv_Update(void)
{
   uint32_t NewKey = KeyboardDrv_Scan();

#if (KEYBOARD_HAS_LKP == true)
   if (KEYBOARD_LKP_TIMEOUT_MS < keyboardData.lastNewKeyTimelapse)
   {
      // TODO:
      // Generate an alarm -> here we reset the counter to avoid blocking keyboard
      keyboardData.lastNewKeyTimelapse = 0;
   }
   else if (KEYBOARD_LKP_TIME < keyboardData.lastNewKeyTimelapse)
   {
      if (false == keyboardData.lkpDetected)
      {
         KeyboardDrv_NewKeyMap(keyboardData.lastNewKey, LKP);
         keyboardData.lkpDetected = true;
      }
      else if (NO_KEY == NewKey)
      {
         keyboardData.lastNewKey = NewKey;
         keyboardData.lastNewKeyTimelapse = 0;
         keyboardData.lkpDetected = false;
      }
   }
   else if (NewKey != keyboardData.lastNewKey)
   {
      if (NO_KEY == NewKey)
      {
         KeyboardDrv_NewKeyMap(keyboardData.lastNewKey, SKP);
      }
      keyboardData.lastNewKey = NewKey;
      keyboardData.lastNewKeyTimelapse = 0;
   }
   else if ((NO_KEY != NewKey) && (NewKey == keyboardData.lastNewKey))
   {
      keyboardData.lastNewKeyTimelapse ++;
   }
#else
   if (NewKey != keyboardData.lastNewKey)
   {
      if (NO_KEY != NewKey)
      {
         KeyboardDrv_NewKeyMap(NewKey, SKP);
      }
      keyboardData.lastNewKey = NewKey;
   }
#endif
}

KeyIdType KeyboardDrv_ConvertKeyMapToKeyId(uint32_t keyMap)
{
   for (int keyId = NO_KEY ; keyId < MAX_KEYS ; keyId++) {
      if (keyMap == (1 << keyId)) {
         return keyId;
      }
   }
   return NO_KEY;
}

uint32_t KeyboardDrv_Scan(void)
{
   uint32_t ret = NO_KEY;
   uint8_t column = 0;
   uint8_t row = 0;
   for(column = 0 ; column < KEYBOARD_DRV_COLUMNSIZE ; column++)
   {
      IOWritePinID(keyboard_drv_columns_cfg[column], IO_OFF);
      for(row = 0 ; row < KEYBOARD_DRV_ROWSIZE ; row++)
      {
         if(IOReadPinID(keyboard_drv_rows_cfg[row]) == GPIO_PIN_RESET)
         {
            ret |= (1 << keyboard_drv_keys[row][column]);
         }
      }
      IOWritePinID(keyboard_drv_columns_cfg[column], IO_ON);
   }
   return ret;
}

//********************************************************************
//
// Close the Doxygen group.
//!   @}
//! @}
//
//********************************************************************

//********************************************************************
//
// Modification Record
//
//********************************************************************
//
// 
//
//********************************************************************

