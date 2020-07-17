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
//!   \file       motor_drv_api.h
//!
//!   \brief      motor driver APIs header file
//!
//!   \author     Esteban Pupillo
//!
//!   \date       21 Apr 2020
//!
//********************************************************************

#ifndef  _MOTOR_DRV_API_H
#define  _MOTOR_DRV_API_H 1

#define ARM_MATH_CM3  // Use ARM Cortex M3
#include "arm_math.h"

//********************************************************************
//! @addtogroup motor_drv_api
//!   @{
//********************************************************************

//********************************************************************
// Constant and Macro Definitions using #define
//********************************************************************
#define MOTOR_DRV_HOME_DISTANCE  (-1)           /**< Initial distance to home UNUSED */

//********************************************************************
// Enumerations and Structures and Typedefs
//********************************************************************

/**
 * @brief Posible directions of the motor
 * 
 */
typedef enum motor_stop_tag
{
   MOTOR_DIR_CW,     /**< Motor movement clockwise */
   MOTOR_DIR_CCW     /**< Motor movement counter-clockwise */
} MotorDirType;

/**
 * @brief Posible states of the motor
 * 
 */
typedef enum motor_dir_tag
{
   MOTOR_STOP_NORMAL,   /**< Normal stop of the motors (both PWM to 0%) */
   MOTOR_STOP_BRAKE,    /**< Normal stop of the motors (both PWM to 100%) (faster stop than normal) */
} MotorStopType;

//********************************************************************
// Global Variable extern Declarations
//********************************************************************

//********************************************************************
// Function Prototypes
//********************************************************************
// Remember to use extern modifier

/**
 * @brief Initializes the motor driver module
 * Initialize
 *    - Local variables
 *    - Timer variables for the PWM
 *    - GPIOs
 *    - PID algorithm
 *    - Motor PWM configuration (within the timer)
 *    - Motor FSM
 *  
 * @param none
 *
 * @return none
 * 
 */
extern StatusType MotorDrv_Init(void);

/**
 * @brief Signal the motor FSM to start the motor 
 * The drive levels equal the PWM value, setting the drive level is like 
 * setting the PWM of the motor
 *  
 * @param dir direction to start the motor
 * @param drivelevel driveLevel equals PWM level
 *
 * @return #E_OK if the operation was successful\n
 *         #E_ERROR if an error occurred
 */
extern StatusType MotorDrv_Start(MotorDirType dir, uint32_t driveLevel);

/**
 * @brief Signal the motor FSM to stop the motor 
 *  
 * @param stopType the stop type selected
 *
 * @return #E_OK if the operation was successful
 *         #E_ERROR if an error occurred
 */
extern StatusType MotorDrv_Stop(MotorStopType stopType);

/**
 * @brief Signal the motor FSM to move certain distance at a certain speed 
 *        (we either control the speed or the driveLevel)
 *  
 * @param dir direction to start the motor
 * @param distance distance to move
 * @param speed speed of the movement
 *
 * @return #E_OK if the operation was successful
 *         #E_ERROR if an error occurred
 */
extern StatusType MotorDrv_MoveDistanceAtSpeed(MotorDirType, int32_t distance, int32_t speed);

/**
 * @brief Signal the motor FSM to move certain distance at a certain driveLevel. The drive
 *        levels equal the PWM value, setting the drive level is like setting the PWM of
 *        the motor
 *        (we either control the speed or the driveLevel)
 *  
 * @param dir direction to start the motor
 * @param distance distance to move
 * @param driveLevel driveLevel equals PWM level
 *
 * @return #E_OK if the operation was successful
 *         #E_ERROR if an error occurred
 */
extern StatusType MotorDrv_MoveDistanceAtDrive(MotorDirType dir, int32_t distance, uint32_t driveLevel);

/**
 * @brief Signal the motor FSM to move certain distance at a certain driveLevel. The drive
 *        levels equal the PWM value, setting the drive level is like setting the PWM of
 *        the motor
 *        (we either control the speed or the driveLevel)
 *  
 * @param driveLevel driveLevel equals PWM level
 *
 * @return #E_OK if the operation was successful
 *         #E_ERROR if an error occurred
 */
extern StatusType MotorDrv_ChangeDriveLevel(int32_t driveLevel);

/**
 * @brief Signal the motor FSM to move to the HOME position
 *  
 * @param driveLevel none
 *
 * @return #E_OK if the operation was successful
 *         #E_ERROR if an error occurred
 */
extern StatusType MotorDrv_GoHome(void);

/**
 * @brief Periodic function to advance the motor FSM
 *  
 * @param driveLevel none
 *
 * @return #E_OK if the operation was successful
 *         #E_ERROR if an error occurred
 */
extern StatusType MotorDrv_Update(void);

/**
 * @brief Set the parameters of the motor PID
 *  
 * @param kp proportional term of the pid
 * @param ki integral term of the pid
 * @param kd derivative term of the pid
 *
 * @return #E_OK if the operation was successful
 *         #E_ERROR if an error occurred
 */
extern StatusType MotorDrv_SetPIDParameters(float32_t kp, float32_t ki, float32_t kd);

/**
 * @brief Get the parameters of the motor PID
 *  
 * @param kp pointer to the proportional term of the pid
 * @param ki pointer to the integral term of the pid
 * @param kd pointer to the derivative term of the pid
 *
 * @return #E_OK if the operation was successful
 *         #E_ERROR if an error occurred
 */
extern StatusType MotorDrv_GetPIDParameters(float32_t *kp, float32_t *ki, float32_t *kd);

/**
 * @brief Signal the motor FSM to update the target position and speed
 *  
 * @param pos new position to go
 * @param deltaPos UNUSED
 * @param speed speed to set
 *
 * @return none
 */
extern void MotorDrv_UpdatePosAndSpeed(int32_t pos, int32_t deltaPos, uint32_t speed);

/**
 * @brief PWM interrupt handler
 *  
 *
 * @return none
 */
extern void MotorDrv_IRQHandler(void);

/**
 * @brief Home sensor interrupt handler (GPIO interrupt)
 *  
 * @return none
 */
extern void MotorDrv_HomeIRQHandler(void);

//********************************************************************
//
// Close the Doxygen group.
//! @}
//
//********************************************************************

#endif // _MOTOR_DRV_API_H
//********************************************************************
//
// Modification Record
//
//********************************************************************
//
// 
//
//********************************************************************

