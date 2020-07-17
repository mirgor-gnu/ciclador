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
//   Title                : board_hw_io_map.h
//
//   Description          : This file defines the IO mapping macros
//
//   Author               : Esteban G. Pupillo
//
//   Created              : 21 Apr 20220
//
//********************************************************************

#ifndef  _BOARD_HW_IO_MAP_H
#define  _BOARD_HW_IO_MAP_H 1

//********************************************************************
// Constant and Macro Definitions using #define
//********************************************************************

//********************************************************************
// Misc
//********************************************************************

//********************************************************************
// SPI
//********************************************************************

//********************************************************************
// Internal DAC
//********************************************************************

//********************************************************************
// ADC
//********************************************************************

//********************************************************************
// Mifare Interface
//********************************************************************

//#define motorEncA_EXTI_IRQn EXTI15_10_IRQn
//#define motorEncB_EXTI_IRQn EXTI15_10_IRQn

#undef X
#define IO_CFG_TABLE \
   X(IO_MOTOR_DIRA   ,IO_OFF, GPIOC, GPIO_PIN_14, GPIO_SPEED_FREQ_LOW , GPIO_MODE_OUTPUT_PP        , GPIO_NOPULL) \
   X(IO_MOTOR_DIRB   ,IO_OFF, GPIOC, GPIO_PIN_15, GPIO_SPEED_FREQ_LOW , GPIO_MODE_OUTPUT_PP        , GPIO_NOPULL) \
   X(IO_MOTOR_PWMA   ,IO_OFF, GPIOA, GPIO_PIN_0 , GPIO_SPEED_FREQ_LOW , GPIO_MODE_AF_PP            , GPIO_NOPULL) \
   X(IO_MOTOR_PWMB   ,IO_OFF, GPIOA, GPIO_PIN_1 , GPIO_SPEED_FREQ_LOW , GPIO_MODE_AF_PP            , GPIO_NOPULL) \
   X(IO_MOTOR_ENCA   ,IO_OFF, GPIOB, GPIO_PIN_13, GPIO_SPEED_FREQ_LOW , GPIO_MODE_IT_RISING_FALLING, GPIO_PULLUP) \
   X(IO_MOTOR_ENCB   ,IO_OFF, GPIOB, GPIO_PIN_14, GPIO_SPEED_FREQ_LOW , GPIO_MODE_IT_RISING_FALLING, GPIO_PULLUP) \
   X(IO_DBG_TX       ,IO_OFF, GPIOA, GPIO_PIN_9 , GPIO_SPEED_FREQ_HIGH, GPIO_MODE_AF_PP            , GPIO_NOPULL) \
   X(IO_DBG_RX       ,IO_OFF, GPIOA, GPIO_PIN_10, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_INPUT            , GPIO_NOPULL) \
   X(IO_AN_PRESSURE  ,IO_OFF, GPIOA, GPIO_PIN_4 , GPIO_SPEED_FREQ_LOW , GPIO_MODE_ANALOG           , GPIO_NOPULL) \
   X(IO_MOTOR_HOME   ,IO_OFF, GPIOB, GPIO_PIN_15, GPIO_SPEED_FREQ_LOW , GPIO_MODE_IT_RISING_FALLING, GPIO_PULLDOWN) \
   X(IO_DISPLAY_RS   ,IO_OFF, GPIOB, GPIO_PIN_4 , GPIO_SPEED_FREQ_HIGH, GPIO_MODE_OUTPUT_PP    	   , GPIO_PULLDOWN) \
   X(IO_DISPLAY_E    ,IO_OFF, GPIOB, GPIO_PIN_3 , GPIO_SPEED_FREQ_HIGH, GPIO_MODE_OUTPUT_PP    	   , GPIO_PULLDOWN) \
   X(IO_DISPLAY_DB4  ,IO_OFF, GPIOB, GPIO_PIN_9 , GPIO_SPEED_FREQ_HIGH, GPIO_MODE_OUTPUT_PP        , GPIO_NOPULL) \
   X(IO_DISPLAY_DB5  ,IO_OFF, GPIOB, GPIO_PIN_8 , GPIO_SPEED_FREQ_HIGH, GPIO_MODE_OUTPUT_PP        , GPIO_NOPULL) \
   X(IO_DISPLAY_DB6  ,IO_OFF, GPIOB, GPIO_PIN_7 , GPIO_SPEED_FREQ_HIGH, GPIO_MODE_OUTPUT_PP        , GPIO_NOPULL) \
   X(IO_DISPLAY_DB7  ,IO_OFF, GPIOB, GPIO_PIN_6 , GPIO_SPEED_FREQ_HIGH, GPIO_MODE_OUTPUT_PP        , GPIO_NOPULL) \
   X(IO_DISPLAY_BL   ,IO_OFF, GPIOA, GPIO_PIN_15, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_OUTPUT_PP        , GPIO_NOPULL) \
   X(IO_KEY_C0       ,IO_ON , GPIOA, GPIO_PIN_5 , GPIO_SPEED_FREQ_LOW , GPIO_MODE_OUTPUT_PP        , GPIO_NOPULL) \
   X(IO_KEY_C1       ,IO_ON , GPIOA, GPIO_PIN_6 , GPIO_SPEED_FREQ_LOW , GPIO_MODE_OUTPUT_PP        , GPIO_NOPULL) \
   X(IO_KEY_C2       ,IO_ON , GPIOA, GPIO_PIN_7 , GPIO_SPEED_FREQ_LOW , GPIO_MODE_OUTPUT_PP        , GPIO_NOPULL) \
   X(IO_KEY_R0       ,IO_OFF, GPIOC, GPIO_PIN_10 ,GPIO_SPEED_FREQ_LOW , GPIO_MODE_INPUT            , GPIO_NOPULL) \
   X(IO_KEY_R1       ,IO_OFF, GPIOC, GPIO_PIN_12, GPIO_SPEED_FREQ_LOW , GPIO_MODE_INPUT            , GPIO_NOPULL) \
   X(IO_KEY_R2       ,IO_OFF, GPIOC, GPIO_PIN_11, GPIO_SPEED_FREQ_LOW , GPIO_MODE_INPUT            , GPIO_NOPULL) \
   X(IO_BUZZ_L       ,IO_OFF, GPIOA, GPIO_PIN_8 , GPIO_SPEED_FREQ_LOW , GPIO_MODE_OUTPUT_PP        , GPIO_NOPULL) \
   X(IO_LED_ALARM_EN ,IO_OFF, GPIOC, GPIO_PIN_9 , GPIO_SPEED_FREQ_LOW , GPIO_MODE_OUTPUT_PP        , GPIO_NOPULL) \
   X(IO_LED_BUB_EN   ,IO_OFF, GPIOC, GPIO_PIN_8 , GPIO_SPEED_FREQ_LOW , GPIO_MODE_OUTPUT_PP        , GPIO_NOPULL) \
   X(IO_LED_RUN_EN   ,IO_OFF, GPIOB, GPIO_PIN_2 , GPIO_SPEED_FREQ_LOW , GPIO_MODE_OUTPUT_PP        , GPIO_NOPULL) \
   X(IO_DBG_LED      ,IO_ON,  GPIOA, GPIO_PIN_11, GPIO_SPEED_FREQ_LOW , GPIO_MODE_OUTPUT_PP        , GPIO_NOPULL) \
   X(IO_FLOW_METER   ,IO_OFF, GPIOA, GPIO_PIN_12, GPIO_SPEED_FREQ_LOW , GPIO_MODE_INPUT            , GPIO_PULLUP) \
   X(IO_RELAY1       ,IO_OFF, GPIOD, GPIO_PIN_2,  GPIO_SPEED_FREQ_LOW , GPIO_MODE_OUTPUT_PP        , GPIO_NOPULL) \
   X(IO_RELAY2       ,IO_OFF, GPIOB, GPIO_PIN_5,  GPIO_SPEED_FREQ_LOW , GPIO_MODE_OUTPUT_PP        , GPIO_NOPULL) \
   X(IO_RELAY3       ,IO_OFF, GPIOB, GPIO_PIN_12, GPIO_SPEED_FREQ_LOW , GPIO_MODE_OUTPUT_PP        , GPIO_NOPULL) \
   X(IO_FLOW_SENS_SCL,IO_OFF, GPIOB, GPIO_PIN_10, GPIO_SPEED_FREQ_LOW , GPIO_MODE_AF_OD            , GPIO_PULLUP) \
   X(IO_FLOW_SENS_SDA,IO_OFF, GPIOB, GPIO_PIN_11, GPIO_SPEED_FREQ_LOW , GPIO_MODE_AF_OD            , GPIO_PULLUP) \
   X(IO_MAIN_POWER   ,IO_OFF, GPIOB, GPIO_PIN_1 , GPIO_SPEED_FREQ_LOW , GPIO_MODE_INPUT            , GPIO_NOPULL) \
   X(IO_AN_CURR_M1   ,IO_OFF, GPIOC, GPIO_PIN_0 , GPIO_SPEED_FREQ_LOW , GPIO_MODE_ANALOG           , GPIO_NOPULL) \

//********************************************************************
// Enumerations and Structures and Typedefs
//********************************************************************
#define X(a,f,b,c,d,e,g) a,
typedef enum {
   IO_CFG_TABLE
}IO_GPIO_t;
#undef X

#endif //_BOARD_HW_IO_MAP

//********************************************************************
// Global Variable extern Declarations
//********************************************************************

//********************************************************************
// Function Prototypes
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
