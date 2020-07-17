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
//!   \file       clock_drv_api.h
//!
//!   \brief      Clock driver APIs header file
//!
//!   \author     Esteban Pupillo
//!
//!   \date       21 Apr 2020
//!
//********************************************************************

#ifndef  _CLOCK_DRV_API_H
#define  _CLOCK_DRV_API_H 1

//********************************************************************
//! @addtogroup clock_drv_api
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
 * @brief Initialize the clock driver module.
 *        There are one hardware timers used, with two different interrupts:
 *        - One as a normal timer that will be extended to generate delays and waits, 
 *        - The other is used as a high resolution timer for precise timestamps 
 *          for the control modules.
 * 
 * @param none
 * 
 * @return StatusType #E_OK if no error occurred\n
 *                    #E_ERROR if an error occurred
 */
extern StatusType ClockDrv_Init(void);

/**
 * @brief Get the number of ticks from the system
 * 
 * @param
 * 
 * @return the number of ticks
 * 
 */
extern uint32_t ClockDrv_GetTicks(void);

/**
 * @brief Get the timestamp from the hardware timer used as a
 *        high resolution timer
 * 
 * @param
 * 
 * @return the timestamp
 * 
 */
extern uint32_t ClockDrv_GetHighResTimestamp(void);

//extern void ClockDrv_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

/**
 * @brief Interruption of the hardware timers, 
 *        used by the high resolution timer of the system.
 * 
 * @param
 * 
 * @return none
 * 
 */
extern void ClockDrv_IRQHandler(void);

/**
 * @brief Interruption of the capture compare hardware timers used by the 
 *        delay timers. It calls a callout that is used to extend the timer.
 * 
 * @param
 * 
 * @return none
 * 
 */
extern void ClockDrv_CCIRQHandler(void);

//extern StatusType ClockDrv_ChangePeriod(uint32_t period);

//********************************************************************
//
// Close the Doxygen group.
//! @}
//
//********************************************************************

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

