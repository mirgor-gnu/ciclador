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
//!   \file       motor_drv.h
//!
//!   \brief      motor driver header file
//!
//!   \author     Esteban Pupillo
//!
//!   \date       21 Apr 2020
//!
//********************************************************************

#ifndef  _MOTOR_DRV_H
#define  _MOTOR_DRV_H 1
#define ARM_MATH_CM3  // Use ARM Cortex M3
#include <arm_math.h>    // Include CMSIS header

//********************************************************************
//! @addtogroup motor_drv_imp
//!   @{
//********************************************************************

//********************************************************************
// Constant and Macro Definitions using #define
//********************************************************************
#define MOTOR_DRV_FREQ_TO_COUNTS(x)       (MOTOR_DRV_TIMER_BASE_CLK_FREQ_HZ / (MOTOR_DRV_TIMER_PRESCALER * (x)))

#define MOTOR_DRV_SET_PWM(pdrv_data, ch, value) do { \
   *(pdrv_data->pwmReg[ch]) = ((value) * pdrv_data->timerPeriod) / 100; \
} while(0)


//********************************************************************
// Enumerations and Structures and Typedefs
//********************************************************************
typedef struct MotorFsmType* fsmType;  /**< Define a pointer for the FSM */

/**
 * @brief Channels for the motor PWM
 * 
 */
typedef enum motor_ch_tag
{
   MOTOR_DRV_CHANNEL_A,    /**< Channel A*/
   MOTOR_DRV_CHANNEL_B,    /**< Channel B*/

   // do not delete this
   MOTOR_DRV_CHANNEL_NUM   /**< Max number of channels*/
} MotorDrvChannelType;

/**
 * @brief Struct with all the information for the motor_drv
 * 
 */
typedef struct motor_drv_data_tag
{
   TIM_HandleTypeDef htim;                               /**< information for the hardware timer for the PWM */
   uint16_t timerPeriod;                                 /**< period for the PWM timer */
   volatile uint32_t *pwmReg[MOTOR_DRV_CHANNEL_NUM];     /**< internal register for the pwm register */

   Bool isInitialized;                                   /**< variable to check if the module is initialized */

   fsmType pFsm;                                         /**< Pointer to the motor FSM */

   MotorDirType newDir;                                  /**< new direction to set to the motor*/
   MotorDirType curDir;                                  /**< current direction of the motor */

   int32_t newDriveLvl;                                  /**< new driver level to set to the motor (drive level equals the PWM) */
   int32_t curDriveLvl;                                  /**< current driver level to set to the motor (drive level equals the PWM) */

   int32_t newDistance;                                  /**< new distance to travel */
   int32_t curDistance;                                  /**< current set distance to travel */

   int32_t newSpeed;                                     /**< new speed */
   int32_t curSpeed;                                     /**< current speed set */

   volatile int32_t homeEvent;                           /**< current state of the home switch, -1 unkown, otherwise the value read from the GPIO  */

   //lpfType *lpf;

   arm_pid_instance_q15 pid;                             /**< information and variables of the PID */

} MotorDrvType;



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

#endif // _MOTOR_DRV_H
//********************************************************************
//
// Modification Record
//
//********************************************************************
//
//
//
//********************************************************************

