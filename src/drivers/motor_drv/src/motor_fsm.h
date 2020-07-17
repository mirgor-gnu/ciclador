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
//!   \file       motor_fsm.h
//!
//!   \brief      motor state machine header file
//!
//!   \author     Esteban Pupillo
//!
//!   \date       21 Apr 2020
//
//********************************************************************

#ifndef  _MOTOR_FSM_H
#define  _MOTOR_FSM_H 1

//********************************************************************
//! @addtogroup motor_drv_imp
//!   @{
//********************************************************************

#include "fsm.h"
#include "motor_drv.h"

//********************************************************************
// Constant and Macro Definitions using #define
//********************************************************************
#define START_SIG       (USER_SIG)        /**< Motor FSM start event */
#define STOP_SIG        (USER_SIG+1)      /**< Motor FSM stop event */
#define HOME_SIG        (USER_SIG+2)      /**< Motor FSM home event */
#define UPDATE_POS_SIG  (USER_SIG+3)      /**< Motor FSM update position event */
#define GO_HOME_SIG     (USER_SIG+4)      /**< Motor FSM go home event */
#define UPDATE_DRV_SIG  (USER_SIG+5)      /**< Motor FSM update driver event */

/**
 * @brief  Motor FSM states X-macro
 * 
 *
 * 
 */
#define MOTOR_FSM_STATES \
   X(STATE_INITIAL) \
   X(STATE_HOME)    \
   X(STATE_RUN)     \
   X(STATE_STOP)    \
   X(STATE_ERROR)   \


//********************************************************************
// Enumerations and Structures and Typedefs
//********************************************************************

/**
 * @brief  Motor FSM states definition
 * 
 */
typedef enum motor_fsm_state_tag
{
#define X(a) a,
   MOTOR_FSM_STATES
#undef X
} MotorFsmStateType;

/**
 * @brief  Motor FSM data structure
 * 
 */
typedef struct motor_fsm_tag
{
   State state__;                /**< The current state */
   MotorDrvType *pDrvData;       /**< Pointer to the motor_drv data */
   uint32_t lastTimestamp;       /**< Timestamp of the last update received */
   uint32_t autoRestart;         /**< If true signal to start a new cycle after the current one is completed */
   MotorStopType stopType;       /**< Type of stop to be executed, normal or brake */
   int32_t lastPosition;         /**< Last position obtained from the hardware */
} MotorFsmType;

/**
 * @brief  Motor FSM event structure, used as an argument when entering the motor FSM.
 *         The final action derived from the event will depend on the current state the
 *         motor FSM is.
 * 
 */
typedef struct hmi_fsm_evt_tag
{
   Signal sig;          /**< Motor FSM event to be executed */
   uint32_t data;       /**< Data related to the event (the meaning depends on the event) */
   int32_t pos;         /**< Position to reach (if applicable, depends on the event) */
   uint32_t speed;      /**< Speed to reach (if applicable, depends on the event) */
} MotorEventType;

//********************************************************************
// Global Variable extern Declarations
//********************************************************************

static MotorEventType const startEvt = { START_SIG }; /**< Event to signal the motor should START moving */

//********************************************************************
// Function Prototypes
//********************************************************************

/**
 * @brief Initialize the motor FSM
 *        The motor driver must be initialized before this function
 *  
 * @param drvData pointer to the motor driver information
 *
 * @return #E_OK if the operation was successful\n
 *         #E_ERROR if an error occurred
 */
StatusType motor_fsm_init(MotorDrvType *drvData);

/**
 * @brief Send an event to the motor FSM
 *  
 * @param drvData pointer to the motor driver information (not changed)
 * @param e  event to send to the motor FSM
 *
 * @return none
 *
 */
void motor_fsm_dispatch(MotorDrvType *drvData, const MotorEventType *e);

/**
 * @brief Force a transition in the motor FSM - UNUSED
 *  
 * @param drvData pointer to the motor driver information (not changed)
 * @param newState state to transition to
 *
 * @return none
 *
 */
void motor_fsm_transition(MotorDrvType *drvData, State *newState);

/**
 * @brief Get the current state of the motor FSM
 *  
 * @param drvData pointer to the motor driver information (not changed)
 *
 * @return the current state of the motor FSM
 *
 */
MotorFsmStateType motor_fsm_get_state(MotorDrvType *pDrvData);


//********************************************************************
//
// Close the Doxygen group.
//! @}
//
//********************************************************************

#endif // _MOTOR_FSM_H
//********************************************************************
//
// Modification Record
//
//********************************************************************
//
//
//
//********************************************************************

