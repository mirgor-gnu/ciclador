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
//   Title                : board_hw_config.h
//
//   Description          : This is the header file for the board
//                          initialization module.
//
//   Author               : Esteban Pupillo
//
//   Created              : 21 Apr 2020
//
//********************************************************************

//********************************************************************
// Constant and Macro Definitions using #define
//********************************************************************
#define BOARD_PERIPHERALS_CLK_TABLE \
   X(DMA1)  \
   X(GPIOA) \
   X(GPIOB) \
   X(GPIOC) \
   X(GPIOD) \
   X(ADC1)  \
   X(AFIO)  \
   X(PWR)   \
   X(TIM1)  \
   X(TIM2)  \
   X(USART1)\
   X(I2C2)

//********************************************************************
// Enumerations and Structures and Typedefs
//********************************************************************

//********************************************************************
// Global Variable extern Declarations
//********************************************************************

//********************************************************************
// Function Prototypes
//********************************************************************
extern void Board_Init(void);

//********************************************************************
//
// Modification Record
//
//********************************************************************
//
// 
//
//********************************************************************

