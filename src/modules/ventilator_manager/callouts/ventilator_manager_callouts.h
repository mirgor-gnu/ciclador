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
//!   @file                 ventilator_manager_callouts.h
//!
//!   Description           ventilator manager callouts header file
//!
//!   Author                Esteban Pupillo
//!
//!   Created               29 Apr 2020
//!
//********************************************************************

#ifndef  _VENTILATOR_MANAGER_CALLOUTS_H
#define  _VENTILATOR_MANAGER_CALLOUTS_H 1

//********************************************************************
//! @addtogroup ventilator_manager_callouts
//!   @{
//********************************************************************

#include "ventilator_manager_api.h"

//********************************************************************
// Constant and Macro Definitions using #define
//********************************************************************

/**
 * Move to home distance.
 * This constant is used to inform to the motor driver that it has
 * to move to the home position regardless of the current position.
 */
#define VENTILATOR_MGR_MOTOR_HOME_DISTANCE    (-1)

/**
 * Invalid pressure value.
 * This constant is used to represent an invalid pressure
 */
#define VENTILATOR_MGR_PRESSURE_INVALID_VALUE (INT32_MIN)

//********************************************************************
// Enumerations and Structures and Typedefs
//********************************************************************

/**
 * Finger motor states.
 * These states are used to inform the motor driver the action that
 * has to be done
 */
typedef enum ventilator_mgr_motor_state{
   VENTILATOR_MGR_MOTOR_STOP,          /**< Stop motor without braking */
   VENTILATOR_MGR_MOTOR_HOME,          /**< Go to home position */
   VENTILATOR_MGR_MOTOR_COMPRESS_SPEED, /**< Compress AMBU bag by speed*/
   VENTILATOR_MGR_MOTOR_COMPRESS_DRIVE,/**< Compress AMBU bag by drive level */
   VENTILATOR_MGR_MOTOR_UPDATE_DRIVE,  /**< Update motor drive level */
   VENTILATOR_MGR_MOTOR_RELEASE,       /**< Release AMBU bag */
   VENTILATOR_MGR_MOTOR_BRAKE,         /**< Brake motor */
}VentilatorMotorStateType;

/**
 * Ventilator Manager States.
 * These values are used to represent the internal state of the ventilator
 */
typedef enum ventilator_mgr_state_tag {
   VENTILATOR_MGR_STATE_IDLE,   /**< IDLE: the ventilator is in standby. No breathing cycles are performed */
   VENTILATOR_MGR_STATE_INHALE, /**< INHALE: the ventilator is in the inhale phase */
   VENTILATOR_MGR_STATE_PLATEAU,/**< PLATEAU: the ventilator is in the plateau phase */
   VENTILATOR_MGR_STATE_PAUSE,  /**< PAUSE: the ventilator is in pause phase */
   VENTILATOR_MGR_STATE_EXHALE, /**< EXHALE: the ventilator is in the exhale phase */
   VENTILATOR_MGR_STATE_ERROR,  /**< ERROR: the ventilator is in error state */
}VentilatorStateType;

/**
 * Ventilator Manager Error States.
 * These values are used to inform all the errors that are being handled by this module
 */
typedef enum ventilator_mgr_error_tag {
   VENTILATOR_MGR_ERROR_NOERROR,        /**< No error occurred */
   VENTILATOR_MGR_ERROR_HIGHPRESSURE,   /**< Pressure higher than allowed maximum */
   VENTILATOR_MGR_ERROR_LOWPRESSURE,    /**< Pressure lower than allowed minimum */
   VENTILATOR_MGR_ERROR_INSTIMEEXCEEDED,/**< Inspiratory time exceeded */
   VENTILATOR_MGR_ERROR_EXPTIMEEXCEEDED,/**< Expiration time exceeded */
   VENTILATOR_MGR_ERROR_RESPRATENOTMET, /**< Respiratory rate not met */
   VENTILATOR_MGR_ERROR_GENERALFAILURE, /**< General Failure */
   VENTILATOR_MGR_ERROR_DOOROPEN,       /**< Ventilator door open */
   VENTILATOR_MGR_ERROR_AIRFLOWDISCONN, /**< Airflow disconnection */
   VENTILATOR_MGR_ERROR_MAXTVEXCEEDED,  /**< Maximum Tidal volume exceeded */
   VENTILATOR_MGR_ERROR_MINTVNOTREACHED,/**< Minimum Tidal volume not reached */
   VENTILATOR_MGR_ERROR_PIPEXCEEDED,    /**< Maximum PIP exceeded */
   VENTILATOR_MGR_ERROR_PIPNOTREACHED,  /**< Minimum PIP not reached */
   VENTILATOR_MGR_ERROR_PEEPMINNOTREACHED, /**< Minimum PEEP not reached */
   VENTILATOR_MGR_ERROR_DRIVINGPRESSURE,/**< PIP - PEEP lower than allowed minimum */
   VENTILATOR_MGR_ERROR_MAINTENANCEREQ, /**< Maintenance required */

   VENTILATOR_MGR_ERROR_NUM,   //do not delete this one
}VentilatorMgrErrorType;

/**
 * Pressure Measurement data.
 * This represents the pressure statistics since the call to
 * #VentilatorMgr_StartPressureMeasurement
 */
typedef struct ventilator_mgr_pressure_meas_value_tag
{
   int32_t min; /**< Minimum value */
   int32_t avg; /**< Average value */
   int32_t max; /**< Maximum value */
   int32_t value; /**< Current value */

   uint32_t tmin; /**< Timestamp for minimum */
   uint32_t tmax; /**< Timestamp for maximum */
}VentilatorMgrPressureMeasStatsType;

/**
 * Type of pressure measurements.
 * These values are used to inform the type of pressure that are measured in this module
 */
typedef enum ventialtor_mgr_pressure_meas_tag
{
   VENTILATOR_MGR_PRESSURE_TYPE_PEEP,/**< Positive end-expiratory pressure */
   VENTILATOR_MGR_PRESSURE_TYPE_PIP, /**< Peak Inspiratory pressure */
   VENTILATOR_MGR_PRESSURE_TYPE_PLT  /**< Plateau pressure */
}VentilatorMgrPressureMeasType;

/**
 * Pressure trigger type.
 * This values are used when requesting to set a pressure trigger
 */
typedef enum ventilator_mgr_pressure_trig_type_tag
{
   VENTILATOR_MGR_PRESSURE_TRIGGER_LOWER_THAN, /**< The event is triggered when the measured value is lower than threshold */
   VENTILATOR_MGR_PRESSURE_TRIGGER_HIGHER_THAN /**< The event is triggered when the measured value is higher than threshold */
}VentilatorMgrPressureTrigTType;

/**
 * Pressure Trigger Type
 */
typedef struct ventilator_mgr_pressure_trig_tag VentilatorMgrPressureTrigType;

/**
 * Pressure Trigger callback.
 * This function shall be called when the trigger condition is met
 * @param trigger pointer to trigger object that was triggered
 * @param value value that generated the trigger event
 */
typedef void (*onPressureTriggerEventFnt)(VentilatorMgrPressureTrigType *trigger, int32_t value);

/**
 * Pressure Trigger Type.
 *
 */
struct ventilator_mgr_pressure_trig_tag
{
   VentilatorMgrPressureTrigTType type; /**< Type of trigger */
   int32_t threshold;                   /**< Trigger threshold value */
   onPressureTriggerEventFnt callback;  /**< Trigger callback function */
   uint32_t id;                         /**< Trigger unique id */
};

//********************************************************************
// Global Variable extern Declarations
//********************************************************************

//********************************************************************
// Function Prototypes
//********************************************************************

/**
 * Module low level initialization callout.
 * This function is called during the module initialization and shall
 * be used to initialize external module that interacts with the #VentilatorManager
 *
 * If an error is returned, the module initialization will not continue.
 *
 * @return #E_OK if no error occurred\n
 *         #E_ERROR if an error occurred
 */
extern StatusType VentilatorMgr_ModuleInit(void);

/**
 * Set mechanical finger motor state.
 * This function is called whenever the #VentilatorManager needs to actuate over the mechanical finger
 *
 * @param state New motor state
 * @param distance distance to travel expressed in deg
 * @param speed required speed expressed in deg/s
 * @return #E_OK if no errors occurred\n
 *         #E_ERROR if an error occurred
 */
extern StatusType VentilatorMgr_SetMotorState(VentilatorMotorStateType state, int32_t distance, int32_t speed);

/**
 * Informs when there is a change in the #VentilatorManager state.
 * This function is called every time there is a change in the internal state
 * @param state new #VentilatorManager state
 */
extern void VentilatorMgr_OnStateChange(VentilatorStateType state);

/**
 * Initializes the Pressure Measurement system.
 * This functions is called when the module needs the pressure measurement subsystem to be initialized
 *
 * @return #E_OK if no errors occurred\n
 *         #E_ERROR if an error occurred
 */
extern StatusType VentilatorMgr_InitPressureMeasurement(void);

/**
 * Creates a new pressure trigger.
 * This function is called when a new pressure trigger needs to be created
 *
 * @param trigger
 * @return #E_OK if no errors occurred\n
 *         #E_ERROR if an error occurred
 */
extern StatusType VentilatorMgr_SetPressureTrigger(VentilatorMgrPressureTrigType *trigger);

/**
 * Starts a new pressure measurement.
 * This function is called when a new measurement cycle is needed. This call shall be used
 * to clear all the measurement statistics
 *
 * @return #E_OK if no errors occurred\n
 *         #E_ERROR if an error occurred
 */
extern StatusType VentilatorMgr_StartPressureMeasurement(void);

/**
 * Stops the pressure measurement.
 * This function is called to stop the pressure measurement. This call shall be used to
 * free any resources allocated in the #VentilatorMgr_StartPressureMeasurement
 *
 * @return #E_OK if no errors occurred\n
 *         #E_ERROR if an error occurred
 */
extern StatusType VentilatorMgr_StopPressureMeasurement(void);

/**
 * Gets a new pressure measurement statistics.
 *
 * @param value pointer to return the new statistics
 *
 * @return #E_OK if no errors occurred\n
 *         #E_ERROR if an error occurred
 */
extern StatusType VentilatorMgr_GetPressureMeasurementStats(VentilatorMgrPressureMeasStatsType *value);

/**
 * Gets a new pressure measurement.
 *
 * @param pressure pointer to return the measured pressure expressed in mmH2O
 * @return #E_OK if no errors occurred\n
 *         #E_ERROR if an error occurred
 */
extern StatusType VentilatorMgr_GetPressureMeasurement(int32_t *pressure);

/**
 * Gets a new air volume measurement.
 *
 * @param pointer to return the measured air volume expressed in ml
 * @return #E_OK if no errors occurred\n
 *         #E_ERROR if an error occurred
 */
extern StatusType VentilatorMgr_GetVolumeMeasurement(int32_t *volume);


/**
 * Informs when an error condition is detected.
 *
 * @param error Error type
 * @param pdata Error specific information
 */
extern void VentilatorMgr_OnError(VentilatorMgrErrorType error, void *pdata);

/**
 * Informs when a new pressure measurement is available.
 * This function is called every time a new pressure measurement is available
 *
 * @param type Type of pressure measured
 * @param value Pressure value expressed in mmH2O
 * @param timestamp Timestamp when the informed pressure value was detected
 */
extern void VentilatorMgr_onPressureMeasurement(VentilatorMgrPressureMeasType type, int32_t value, uint32_t timestamp);


//********************************************************************
//
// Close the Doxygen group.
//! @}
//
//********************************************************************

#endif // _VENTILATOR_MANAGER_CALLOUTS_H
//********************************************************************
//
// Modification Record
//
//********************************************************************
//
// 
//
//********************************************************************

