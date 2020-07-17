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
//   Title                : board_hw_io.h
//
//   Description          : This is the header file for the board
//                          IO module.
//
//   Author               : Esteban Pupillo
//
//   Created              : 21 Apr 2020
//
//********************************************************************

#ifndef  _HW_IO_H
#define  _HW_IO_H 1

//********************************************************************
// Include header files                                              
//********************************************************************
#include "stm32f1xx_hal.h"
#include "board_hw_io_map.h"

//********************************************************************
// Constant and Macro Definitions using #define
//********************************************************************

// IO pin read macro
#define IOReadPin(gpio, pin)           (0 != (gpio->IDR & pin))
#define IOReadPinID(io_id)             (0 != ((hw_io_cfg[io_id].GPIOx->IDR) & hw_io_cfg[io_id].pin))

// IO pin write macro
#define IOWritePin(gpio, pin, value)   do       \
{                                               \
   if ((IO_ON) == value)                        \
      gpio->BSRR = pin;                         \
   else                                         \
      gpio->BRR = pin;                          \
} while (0)

#define IOWritePinID(io_id, value)   do                           \
{                                                                 \
   if ((IO_ON) == value)                                          \
      hw_io_cfg[io_id].GPIOx->BSRR = hw_io_cfg[io_id].pin;        \
   else                                                           \
      hw_io_cfg[io_id].GPIOx->BRR = hw_io_cfg[io_id].pin;         \
} while (0)

#define IOTogglePinID(io_id)   do                           \
{                                                           \
   if ((IO_ON) == (0 != (hw_io_cfg[io_id].GPIOx->IDR & hw_io_cfg[io_id].pin)))                 \
      hw_io_cfg[io_id].GPIOx->BRR = hw_io_cfg[io_id].pin;        \
   else                                                           \
      hw_io_cfg[io_id].GPIOx->BSRR = hw_io_cfg[io_id].pin;         \
} while (0)

#define IOGetPortFromPinID(io_id)         (hw_io_cfg[io_id].GPIOx)
#define IOGetPinNumberFromPinID(io_id)    (hw_io_cfg[io_id].pin)


//********************************************************************
// Enumerations and Structures and Typedefs
//********************************************************************
typedef struct hw_io_cfg_tag
{
   GPIO_TypeDef* GPIOx;
   uint16_t pin;
   uint32_t mode;
   uint32_t pull;
   uint32_t speed;
}hw_io_cfg_t;

typedef enum IO_pin_state_tag
{
   IO_OFF = 0u,
   IO_ON,
} IOPinStateType;

//********************************************************************
// Global Variable extern Declarations
//********************************************************************
extern hw_io_cfg_t const hw_io_cfg[];

//********************************************************************
// Function Prototypes
//********************************************************************
extern void IO_Config(GPIO_TypeDef* GPIOx, uint16_t pin, uint32_t mode, uint32_t pull, uint32_t speed);

#endif // _HW_IO_H
//********************************************************************
//
// Modification Record
//
//********************************************************************
//
// 
//
//********************************************************************

