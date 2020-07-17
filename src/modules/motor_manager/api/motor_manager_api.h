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
//!   \file       motor_manager_api.h
//!
//!   \brief      motor manager APIs header file
//!
//!   \author     Esteban Pupillo
//!
//!   \date       23 Apr 2020
//!
//********************************************************************

#ifndef  _MOTOR_MANAGER_API_H
#define  _MOTOR_MANAGER_API_H 1


#define ARM_MATH_CM3  // Use ARM Cortex M3
#include "arm_math.h"

//********************************************************************
//! @addtogroup motor_manager_api
//!   @{
//********************************************************************

//********************************************************************
// Constant and Macro Definitions using #define
//********************************************************************

//********************************************************************
// Enumerations and Structures and Typedefs
//********************************************************************
/**
 * @brief Motor manager FSM states
 * 
 */
typedef enum motor_mgr_motor_state{
   MOTOR_MGR_MOTOR_STOP,            /**< motor is stopped */
   MOTOR_MGR_MOTOR_RUN_UP,          /**< motor is running in the up direction */
   MOTOR_MGR_MOTOR_RUN_DOWN,        /**< motor is running in the down direction */
   MOTOR_MGR_MOTOR_BRAKE,           /**< motor is explicitely brake */
}MotorStateType;
//********************************************************************
// Global Variable extern Declarations
//********************************************************************

//********************************************************************
// Function Prototypes
//********************************************************************
// Remember to use extern modifier

/**
 * Initialize the motor manager module
 *    - Initialize the PID parameters (for the motor driver)
 *    - Initialize the motor manager FSM
 *
 * @param none
 *
 * @return #E_OK if the operation was successful\n
 *         #E_ERROR if an error occurred
 * 
 */
extern StatusType MotorManager_Init(void);

/**
 * Periodic function to advance the motor manager FSM
 *
 * @param none
 *
 * @return #E_OK if the operation was successful\n
 *         #E_ERROR if an error occurred
 * 
 */
extern StatusType MotorManager_Update(void);

/**
 * Start the motor in the configured direction
 *
 * @param none
 *
 * @return #E_OK if the operation was successful\n
 *         #E_ERROR if an error occurred
 * 
 */
extern StatusType MotorManager_Start(void);

/**
 * Stop the motor
 *
 * @param none
 *
 * @return #E_OK if the operation was successful\n
 *         #E_ERROR if an error occurred
 * 
 */
extern StatusType MotorManager_Stop(void);

/**
 * UNUSED (this goes to the motor driver FSM now) - Update the position and speed of the motor
 *
 * @param pos current position
 * @param deltaPos delta position from the last update
 * @param speed current speed
 *
 * @return #E_OK if the operation was successful\n
 *         #E_ERROR if an error occurred
 * 
 */
extern void MotorManager_UpdatePosAndSpeed(int32_t pos, int32_t deltaPos, uint32_t speed);

/**
 * @brief UNUSED (this goes to the motor driver FSM now) - Set the parameters of the motor PID
 *  
 * @param kp proportional term of the pid
 * @param ki integral term of the pid
 * @param kd derivative term of the pid
 *
 * @return #E_OK if the operation was successful
 *         #E_ERROR if an error occurred
 */
extern StatusType MotorManager_SetPIDParameters(float32_t kp, float32_t ki, float32_t kd);

/**
 * @brief UNUSED (this goes to the motor driver FSM now) - Set the parameters of the motor PID
 *  
 * @param speed speed to set
 * @param distance distance to travel
 * @param pauseTimeout time to wait before executing the action to go to the setpoint
 *
 * @return #E_OK if the operation was successful
 *         #E_ERROR if an error occurred
 */
extern StatusType MotorManager_SetSetpoint(uint32_t speed, uint32_t distance, uint32_t pauseTimeout);

//********************************************************************
//
// Close the Doxygen group.
//! @}
//
//********************************************************************

#endif // _MOTOR_MANAGER_API_H
//********************************************************************
//
// Modification Record
//
//********************************************************************
//
// 
//
//********************************************************************

