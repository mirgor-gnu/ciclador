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
//!   \file       motor_drv_conf.h
//!
//!   \brief      motor driver configuration header file
//!
//!   \author     Esteban Pupillo
//!
//!   \date       21 Apr 2020
//!
//********************************************************************

#ifndef  _MOTOR_DRV_CONF_H
#define  _MOTOR_DRV_CONF_H 1

//********************************************************************
//! @addtogroup motor_drv_conf
//!   @{
//********************************************************************

//********************************************************************
// Constant and Macro Definitions using #define
//********************************************************************
#define MOTOR_DRV_TIMER_BASE_CLK_FREQ_HZ  (72000000)                /**< Base frequency for the motor clock timer */
#define MOTOR_DRV_TIMER                   (TIM2)                    /**< Timer used to generate the motor PWM */
#define MOTOR_DRV_TIMER_PRESCALER         (144)                     /**< Prescaler for the motor timer */
#define MOTOR_DRV_PWM_FREQ_HZ             (10000)                   /**< Motor PWM frequency */
#define MOTOR_DRV_PWM_CHAN_A              (TIM_CHANNEL_1)           /**< Motor PWM channel A hardware definition */
#define MOTOR_DRV_PWM_CHAN_B              (TIM_CHANNEL_2)           /**< Motor PWM channel B hardware definition */

#define MOTOR_DRV_TIMER_IRQ_NAME          (TIM2_IRQn)               /**< Motor timer interrupt handler */
#define MOTOR_DRV_TIMER_IRQ_PRIORITY      (2)                       /**< Motor timer interrupt priority */

#define MOTOR_DRV_STARTUP_TIME_MS         (1)                       /**< Motor startup in ms */
#define MOTOR_DRV_MIN_STOP_TIME_MS        (500)                     /**< Motor stop time in ms */

#define MOTOR_DRV_HOME_SWITCH_PIN          (IO_MOTOR_HOME)          /**< Motor home switch GPIO */
#define MOTOR_DRV_HOME_SWITCH_IRQ          (EXTI15_10_IRQn)         /**< Motor home switch interrupt handler (GPIO interrupt) */
#define MOTOR_DRV_HOME_SWITCH_IRQ_PRIORITY (0)                      /**< Motor home switch interrupt priority */

#define MOTOR_DRV_HOMING_DRIVE_LEVEL      (20)                      /**< PWM (drive level) used when going to HOME */
#define MOTOR_DRV_HOMING_TIMEOUT_MILLIS   (5000)                    /**< Timeout to reach home in ms. If home is not reached in this time an error es generated */

#define MOTOR_DRV_MAX_DISTANCE            (45)                      /**< Motor max distance in degrees */

//********************************************************************
// Enumerations and Structures and Typedefs
//********************************************************************

//********************************************************************
// Global Variable extern Declarations
//********************************************************************

//********************************************************************
// Function Prototypes
//********************************************************************

//********************************************************************
//
// Close the Doxygen group.
//! @}
//
//********************************************************************

#endif // _MOTOR_DRV_CONF_H
//********************************************************************
//
// Modification Record
//
//********************************************************************
//
// 
//
//********************************************************************

