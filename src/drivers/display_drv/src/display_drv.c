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
//!   \file       display_drv.c
//!
//!   \brief      This is the Display Driver implementation
//!
//!               It is based on HD44780 dispĺay driver
//!
//!   \author     Agustin Diaz Antuna
//!
//!   \date       24 Apr 2020
//
//********************************************************************

//********************************************************************
// Include header files                                              
//********************************************************************
#include "standard.h"
#include "stm32f1xx_hal.h"
#include "board_hw_io.h"
#include "ringbuf.h"
#include "clock_drv_api.h"

//********************************************************************
//! @addtogroup display_drv_imp
//! @{
//********************************************************************

#include "display_drv.h"
#include "display_drv_api.h"
#include "display_drv_conf.h"
#include "display_drv_callouts.h"

//********************************************************************
// File level pragmas
//********************************************************************

//********************************************************************
// Constant and Macro Definitions using #define
//********************************************************************

//********************************************************************
// Enumerations and Structures and Typedefs
//********************************************************************
typedef struct display_drv_data_tag
{
   volatile Bool isInitialized;
   uint8_t buffer[DISPLAY_DRV_BUFFER_SIZE];
   Ring_Buf_Type ring_buffer;
} DisplayDrvDataType;

//********************************************************************
// Function Prototypes for Private Functions with File Level Scope
//********************************************************************
static void DisplayDrv_WriteByte(uint8_t data, uint8_t type);
static void DisplayDrv_WriteNibble(uint8_t data);
static void DisplayDrv_Set4bitMode(void);
static void DisplayDrv_Set8bitMode(void);

//********************************************************************
// ROM Const Variables With File Level Scope
//********************************************************************

//********************************************************************
// Static Variables and Const Variables With File Level Scope
//********************************************************************
static DisplayDrvDataType display_drv_data;

//********************************************************************
// Function Definitions
//********************************************************************
StatusType DisplayDrv_Init(void)
{
   display_drv_data.isInitialized = FALSE;

   //add a dummy E pulse to start from a know state after the reset transient
   //if we don't do this, this initialization may fail leaving the LCD in its default state
   //IOWritePinID(IO_DISPLAY_E, IO_ON);
   //HAL_Delay(10);
   //IOWritePinID(IO_DISPLAY_E, IO_OFF);

   RingBufInit(&display_drv_data.ring_buffer, display_drv_data.buffer, NULL, DISPLAY_DRV_BUFFER_SIZE);
   display_drv_data.isInitialized = TRUE;

   HAL_Delay(10);
   DisplayDrv_Set8bitMode();
   HAL_Delay(5);
   DisplayDrv_Set8bitMode();
   HAL_Delay(1);
   DisplayDrv_Set8bitMode();
   HAL_Delay(1);
   DisplayDrv_Set4bitMode();
   HAL_Delay(1);

   //DisplayDrv_WriteByte(0x02, DISPLAY_DRV_COMMAND);
   //HAL_Delay(100);
   DisplayDrv_WriteByte(HD44780_FUNCTIONSET | HD44780_2LINE | 
                        HD44780_5x8DOTS, DISPLAY_DRV_COMMAND);
   HAL_Delay(1);
   DisplayDrv_WriteByte(HD44780_DISPLAYCONTROL | HD44780_DISPLAYON | 
                        HD44780_CURSOROFF | HD44780_BLINKOFF, DISPLAY_DRV_COMMAND);
   HAL_Delay(1);
   DisplayDrv_WriteByte(HD44780_ENTRYMODESET | HD44780_ENTRYLEFT, DISPLAY_DRV_COMMAND);
   HAL_Delay(1);

   DisplayDrv_WriteByte(HD44780_CLEARDISPLAY, DISPLAY_DRV_COMMAND);
   HAL_Delay(5);

   IOWritePinID(IO_DISPLAY_BL, IO_ON);

   return E_OK;
}

void DisplayDrv_ClearDisplay(void)
{
   DisplayDrv_WriteByte(HD44780_CLEARDISPLAY, DISPLAY_DRV_COMMAND);
}

void DisplayDrv_WriteString(char *str)
{
   while (*str != '\0')
   {
      DisplayDrv_WriteByte(*str++, DISPLAY_DRV_DATA);
   }
}

void DisplayDrv_WriteChar(char character)
{
   DisplayDrv_WriteByte(character, DISPLAY_DRV_DATA);
}

uint8_t DisplayDrv_PositionXY(char x, char y)
{
   if (x > HD44780_COLS || y > HD44780_ROWS)
   {
      return 1;
   }

   switch (y)
   {
      case 0:
         DisplayDrv_WriteByte(HD44780_ROW1_START + x,
                              DISPLAY_DRV_COMMAND);
         break;
      case 1:
         DisplayDrv_WriteByte(HD44780_ROW2_START + x,
                              DISPLAY_DRV_COMMAND);
         break;
      case 2:
         DisplayDrv_WriteByte(HD44780_ROW3_START + x,
                              DISPLAY_DRV_COMMAND);
         break;
      case 3:
         DisplayDrv_WriteByte(HD44780_ROW4_START + x,
                              DISPLAY_DRV_COMMAND);
         break;      
      default:
         break;
   }
   return 0;
}

void DisplayDrv_CursorOn(void)
{
   DisplayDrv_WriteByte(HD44780_DISPLAYCONTROL | HD44780_DISPLAYON |
                        HD44780_CURSORON | HD44780_BLINKON,
                        DISPLAY_DRV_COMMAND);
}

void DisplayDrv_CursorOff(void)
{
   DisplayDrv_WriteByte(HD44780_DISPLAYCONTROL | HD44780_DISPLAYON |
                        HD44780_CURSOROFF | HD44780_BLINKOFF,
                        DISPLAY_DRV_COMMAND);
}

//********************************************************************
// Private Functions
//********************************************************************
void DisplayDrv_UpdateData(void)
{
   static uint8_t status = 0;
   uint8_t data;

   //if we are not initialized we return immediately
   if (!display_drv_data.isInitialized)
       return;

   if (status == 0) {
      if(!RingBufEmpty(&display_drv_data.ring_buffer))
      {
         data = RingBufReadOne(&display_drv_data.ring_buffer);
         if (data & DISPLAY_DRV_COMMAND)
         {
            IOWritePinID(IO_DISPLAY_RS, IO_OFF);
         }
         else
         {
            IOWritePinID(IO_DISPLAY_RS, IO_ON);
         }
         DisplayDrv_WriteNibble(data);
         IOWritePinID(IO_DISPLAY_E, IO_ON);
         status = 1;
      }
   } else if (status == 1) {
      IOWritePinID(IO_DISPLAY_E, IO_OFF);
      status = 2;
   } else if (status == 2) {
      status = 0;
   }
}

void DisplayDrv_WriteByte(uint8_t data, uint8_t type)
{
   RingBufWriteOne(&display_drv_data.ring_buffer, ((data & 0xF0) >> 4) | type);
   RingBufWriteOne(&display_drv_data.ring_buffer, ((data & 0x0F) >> 0) | type);
}

void DisplayDrv_Set4bitMode(void)
{
   RingBufWriteOne(&display_drv_data.ring_buffer, (HD44780_FUNCTIONSET >> 4) | DISPLAY_DRV_COMMAND);
}

void DisplayDrv_Set8bitMode(void)
{
   RingBufWriteOne(&display_drv_data.ring_buffer, ((HD44780_FUNCTIONSET | HD44780_8BITMODE) >> 4) | DISPLAY_DRV_COMMAND);
}
void DisplayDrv_WriteNibble(uint8_t data)
{
   HAL_GPIO_WritePin(GPIOB, IOGetPinNumberFromPinID(IO_DISPLAY_DB7), (data & 0x08) >> 3);
   HAL_GPIO_WritePin(GPIOB, IOGetPinNumberFromPinID(IO_DISPLAY_DB6), (data & 0x04) >> 2);
   HAL_GPIO_WritePin(GPIOB, IOGetPinNumberFromPinID(IO_DISPLAY_DB5), (data & 0x02) >> 1);
   HAL_GPIO_WritePin(GPIOB, IOGetPinNumberFromPinID(IO_DISPLAY_DB4), (data & 0x01) >> 0);
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

