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
//!   \file                 ventilator_manager_api.h
//!
//!   \brief                ventilator manager APIs header file
//!
//!   \author               Esteban Pupillo
//!
//!   \date                 29 Apr 2020
//!
//********************************************************************

#ifndef  _VENTILATOR_MANAGER_API_H
#define  _VENTILATOR_MANAGER_API_H 1

#define ARM_MATH_CM3  // Use ARM Cortex M3
#include "arm_math.h"

//********************************************************************
//! @addtogroup ventilator_manager_api
//!   @{
//********************************************************************
//********************************************************************
// Constant and Macro Definitions using #define
//********************************************************************

//********************************************************************
// Enumerations and Structures and Typedefs
//********************************************************************

/**
 * Ventilator stop modes.
 * All the stop modes make the pressing finger to go to the home
 * position
 */
typedef enum ventilator_mgr_stop_type_tag
{
   VENTILATOR_MGR_STOP_TYPE_NORMAL,   /**< VENTILATOR_MGR_STOP_TYPE_NORMAL */
   /**< Normal stop will make the ventilator to stop after the completion
    * of the current cycle */
   VENTILATOR_MGR_STOP_TYPE_EMERGENCY,/**< VENTILATOR_MGR_STOP_TYPE_EMERGENCY */
   /**< Emergency stop will make the ventilator to stop immediately */
} VentilatorMgrStopType;

/**
 * Ventilator control modes.
 */
typedef enum ventilator_mgr_control_mode_tag
{
   VENTILATOR_MGR_VOLUME_CONTROL,  /**< Ventilator runs using tidal volume control */
   VENTILATOR_MGR_PRESSURE_CONTROL,/**< Ventilator runs using pressue control */
} VentilatorMgrModeControlType;

//********************************************************************
// Global Variable extern Declarations
//********************************************************************

//********************************************************************
// Function Prototypes
//********************************************************************
// Remember to use extern modifier
/**
 * Initializes the Ventilator manager module.
 * This function shall be called before any other call API functions
 *
 * @return #E_OK if initialization is successful\n
 *         #E_ERROR is initialization fails
 */
extern StatusType VentilatorMgr_Init(void);

/**
 * Starts the ventilation process.
 *
 * @return #E_OK if ventilation process started successfully\n
 *         #E_ERROR if an error occurred
 */
extern StatusType VentilatorMgr_Start(void);

/**
 * Stops the ventilation process.
 * Depending on the stop type the ventilation process will stop immediately
 * or will be stopped before starting a new inspiration phase
 *
 * @param type #VentilatorMgrStopType type of stop required
 *
 * @return #E_OK if the stop command was processed successfully\n
 *         #E_ERROR if an error occurred
 */
extern StatusType VentilatorMgr_Stop(VentilatorMgrStopType type);

/**
 * Sets the control mode for the ventilator.
 * This function shall be called while the ventilator manager is in stop state
 *
 * @param mode #VentilatorMgrModeControlType type of control mode
 * @return #E_OK if the stop command was processed successfully\n
 *         #E_ERROR if an error occurred
 */
extern StatusType VentialtorMgr_SetControlMode(VentilatorMgrModeControlType mode);

/**
 * Gets the control mode for the ventilator.
 *
 * @param mode #VentilatorMgrModeControlType pointer to return current mode
 * @return #E_OK if the stop command was processed successfully\n
 *         #E_ERROR if an error occurred
 */
extern StatusType VentilatorMgr_GetControlMode(VentilatorMgrModeControlType *mode);

/**
 * Sets the required Respiratory rate.
 *
 * @param bpm Respiratory rate expressed in BPM (Breath per minute)
 *
 * @return #E_OK if the parameter is valid\n
 *         #E_ERROR if the parameter is invalid
 */
extern StatusType VentilatorMgr_SetRespiratoryRate(uint32_t bpm);

/**
 * Gets the current Respiratory rate.
 *
 * @param bpm Pointer to return the current rate in BPM
 *
 * @return #E_OK if no error occurred\n
 *         #E_ERROR if an error occurred
 */
extern StatusType VentilatorMgr_GetRespiratoryRate(uint32_t* bpm);

/**
 * Sets the tidal volume.
 *
 * @param volInML tidal volume expressed in ml (milliliters)
 * @return #E_OK if the parameter is valid\n
 *         #E_ERROR if the parameter is invalid
 */
extern StatusType VentilatorMgr_SetTidalVolume(uint32_t volInML);

/**
 * Gets current tidal volume.
 *
 * @param volInML pointer where to return the tidal volume expressed in ml
 * @return #E_OK if no error occurred\n
 *         #E_ERROR if an error occurred
 */
extern StatusType VentilatorMgr_GetTidalVolume(uint32_t *volInML);

/**
 * Sets inspiratory-expiratory ratio
 *
 * @param ieRatio Ratio expressed as @p 1:[ieRatio/100] @p .\n
 * For example, @p ieRatio = 100 @p means a 1:1 ratio
 *
 * @return #E_OK if the parameter is valid\n
 *         #E_ERROR if the parameter is invalid
 */
extern StatusType VentilatorMgr_SetIERatio(uint32_t ieRatio);

/**
 * Gets current IE ratio
 *
 * @param ieRatio pointer to return the current ratio expressed as @p 1:[ieRatio/100] @p
 *
 * @return #E_OK if no error occurred\n
 *         #E_ERROR if an error occurred
 */
extern StatusType VentilatorMgr_GetIERatio(uint32_t *ieRatio);

/**
 * Sets the inspiratory time
 *
 * @param inspTimeMillis Inspiratory time expressed in ms
 *
 * @return #E_OK if the parameter is valid\n
 *         #E_ERROR if the parameter is invalid
 */
extern StatusType VentilatorMgr_SetInspiratoryTime(uint32_t inspTimeMillis);

/**
 * Gets current inspiratory time
 *
 * @param insTimeMillis Pointer to return the inspiratory time expressed in ms
 *
 * @return #E_OK if no error occurred\n
 *         #E_ERROR if an error occurred
 */
extern StatusType VentilatorMgr_GetInspiratoryTime(uint32_t *insTimeMillis);

/**
 * Sets Plateau time
 *
 * @param pltTimeMillis Plateau time expressed in ms
 *
 * @return #E_OK if the parameter is valid\n
 *         #E_ERROR if the parameter is invalid
 */
extern StatusType VentilatorMgr_SetPlateauTime(uint32_t pltTimeMillis);

/**
 * Gets current plateau time.
 *
 * @param pltTimeMillis Pointer to store the Plateau time expressed in ms
 *
 * @return #E_OK if no error occurred\n
 *         #E_ERROR if an error occurred
 */
extern StatusType VentilatorMgr_GetPlateauTime(uint32_t *pltTimeMillis);

/**
 * Sets inspiratory pressure setpoint.
 * This function is only applicable for pressure control mode
 *
 * @param inspPressure Inspiratory pressure expressed in mmH2O
 *
 * @return #E_OK if the parameter is valid\n
 *         #E_ERROR if the parameter is invalid
 */
extern StatusType VentilatorMgr_SetInspiratoryPressure(uint32_t inspPressure);

/**
 * Gets current inspiratory pressure setpoint.
 *
 * @param inspPressure Pointer to store the inspiratory pressure expressed in
 *        mmH2O
 *
 * @return #E_OK if no error occurred\n
 *         #E_ERROR if an error occurred
 */
extern StatusType VentilatorMgr_GetInspiratoryPressure(uint32_t *inspPressure);

/**
 * Sets minimum tidal volume threshold.
 * if the delivered volume is lower than the value provided an error condition
 * will be raised
 *
 * @param minTidalVolume Minimum Tidal volume expressed in ml
 *
 * @return #E_OK if the parameter is valid\n
 *         #E_ERROR if the parameter is invalid
 */
extern StatusType VentilatorMgr_SetMinTidalVolume(uint32_t minTidalVolume);

/**
 * Gets current minimum tidal volume threshold.
 *
 * @param minTidalVolume Pointer to store the minimum tidal volume expressed
 *        in ml
 *
 * @return #E_OK if no error occurred\n
 *         #E_ERROR if an error occurred
 */
extern StatusType VentilatorMgr_GetMinTidalVolume(uint32_t *minTidalVolume);

/**
 * Sets maximum tidal volume threshold.
 * if the delivered volume is higher than the value provided an error condition
 * will be raised
 *
 * @param maxTidalVolume Maximum Tidal volume expressed in ml
 *
 * @return #E_OK if the parameter is valid\n
 *         #E_ERROR if the parameter is invalid
 */
extern StatusType VentilatorMgr_SetMaxTidalVolume(uint32_t maxTidalVolume);

/**
 * Gets current maximum tidal volume threshold.
 *
 * @param maxTidalVolume Pointer to store the maximum tidal volume expressed
 *        in ml
 *
 * @return #E_OK if no error occurred\n
 *         #E_ERROR if an error occurred
 */
extern StatusType VentilatorMgr_GetMaxTidalVolume(uint32_t *maxTidalVolume);

/**
 * Sets maximum PIP threshold.
 * if the measured pressure is higher than the value provided an error condition
 * will be raised
 *
 * @param maxPIP Maximum PIP expressed in mmH2O
 *
 * @return #E_OK if the parameter is valid\n
 *         #E_ERROR if the parameter is invalid
 */
extern StatusType VentilatorMgr_SetMaxPIP(uint32_t maxPIP);

/**
 * Gets current maximum PIP threshold.
 *
 * @param maxPIP Pointer to store the maximum PIP expressed
 *        in mmH2O
 *
 * @return #E_OK if no error occurred\n
 *         #E_ERROR if an error occurred
 */
extern StatusType VentilatorMgr_GetMaxPIP(uint32_t *maxPIP);

/**
 * Sets minimum PIP threshold.
 * if the measured pressure is lower than the value provided an error condition
 * will be raised
 *
 * @param minPIP Minimum PIP expressed in mmH2O
 *
 * @return #E_OK if the parameter is valid\n
 *         #E_ERROR if the parameter is invalid
 */
extern StatusType VentilatorMgr_SetMinPIP(uint32_t minPIP);

/**
 * Gets current minimum PIP threshold.
 *
 * @param minPIP Pointer to store the minimum PIP expressed
 *        in mmH2O
 *
 * @return #E_OK if no error occurred\n
 *         #E_ERROR if an error occurred
 */
extern StatusType VentilatorMgr_GetMinPIP(uint32_t *minPIP);


/**
 * Sets maximum PEEPP threshold.
 * if the measured pressure is higher than the value provided an error condition
 * will be raised
 *
 * @param maxPEEP Maximum PEEP expressed in mmH2O
 *
 * @return #E_OK if the parameter is valid\n
 *         #E_ERROR if the parameter is invalid
 */
extern StatusType VentilatorMgr_SetMaxPEEP(uint32_t maxPEEP);

/**
 * Gets current maximum PEEP threshold.
 *
 * @param maxPEEP Pointer to store the maximum PEEP expressed
 *        in mmH2O
 *
 * @return #E_OK if no error occurred\n
 *         #E_ERROR if an error occurred
 */
extern StatusType VentilatorMgr_GetMaxPEEP(uint32_t *maxPEEP);

/**
 * Sets minimum PEEP threshold.
 * if the measured pressure is lower than the value provided an error condition
 * will be raised
 *
 * @param minPEEP Minimum PEEP expressed in mmH2O
 *
 * @return #E_OK if the parameter is valid\n
 *         #E_ERROR if the parameter is invalid
 */
extern StatusType VentilatorMgr_SetMinPEEP(uint32_t minPEEP);

/**
 * Gets current minimum PEEP threshold.
 *
 * @param minPEEP Pointer to store the minimum PEEP expressed
 *        in mmH2O
 *
 * @return #E_OK if no error occurred\n
 *         #E_ERROR if an error occurred
 */
extern StatusType VentilatorMgr_GetMinPEEP(uint32_t *minPEEP);

/**
 * Sets maximum inspiratory time error threshold.
 * if the calculated time is is higher than the value provided an error condition
 * will be raised
 *
 * @param tiError Maximum TI error expressed in %
 *
 * @return #E_OK if the parameter is valid\n
 *         #E_ERROR if the parameter is invalid
 */
extern StatusType VentilatorMgr_SetMaxTIError(uint32_t tiError);

/**
 * Gets current maximum TI error threshold.
 *
 * @param tiError Pointer to store the maximum TI error expressed
 *        in %
 *
 * @return #E_OK if no error occurred\n
 *         #E_ERROR if an error occurred
 */
extern StatusType VentilatorMgr_GetMaxTIError(uint32_t *tiError);

/**
 * Sets maximum breathing frequency error threshold.
 * if the calculated frequency is is higher or lower than the value provided an error condition
 * will be raised
 *
 * @param bpmError Maximum BPM error expressed in %
 *
 * @return #E_OK if the parameter is valid\n
 *         #E_ERROR if the parameter is invalid
 */
extern StatusType VentilatorMgr_SetMaxBPMError(uint32_t bpmError);

/**
 * Gets current maximum breathing frequency error threshold.
 *
 * @param bpmError Pointer to store the maximum BPM error expressed
 *        in %
 *
 * @return #E_OK if no error occurred\n
 *         #E_ERROR if an error occurred
 */
extern StatusType VentilatorMgr_GetMaxBPMError(uint32_t *bpmError);


/**
 * Sets minimum PIP - PEEP difference threshold.
 * if the measured pressure difference is lower than the value provided an error condition
 * will be raised
 *
 * @param minPIPPEEPDif Minimum PIP - PEEP expressed in mmH2O
 *
 * @return #E_OK if the parameter is valid\n
 *         #E_ERROR if the parameter is invalid
 */
extern StatusType VentilatorMgr_SetMinPIPPEEPDif(uint32_t minPIPPEEPDif);

/**
 * Gets current minimum PIP - PEEP difference threshold.
 *
 * @param minPIPPEEPDif Pointer to store the minimum PIP - PEEP expressed
 *        in mmH2O
 *
 * @return #E_OK if no error occurred\n
 *         #E_ERROR if an error occurred
 */
extern StatusType VentilatorMgr_GetMinPIPPEEPDif(uint32_t *minPIPPEEPDif);

/**
 * Sets ventilator parameters using the Inspiratory-Expiration ratio.
 *
 * @param bpm Respiratory rate expressed in BPM
 * @param volInML Tidal volume expressed in ml
 * @param ieRatio Inspiratory-Expiration ratio expressed as @p 1:(ieRatio/100)@p .\n
 * for example: @p ieRatio = 100 @p meas a ratio of 1:1
 *
 * @return #E_OK if the parameter is valid\n
 *         #E_ERROR if the parameter is invalid
 */
extern StatusType VentilatorMgr_SetParametersWithIERatio(uint32_t bpm, uint32_t volInML, uint32_t ieRatio);

/**
 * Sets ventilator parameters using the inspiratory time.
 *
 * @param bpm Respiratory rate expressed in BPM
 * @param volInML Tidal volume expressed in ml
 * @param inspTimeMillis Inspiratory Time in ms
 * @return #E_OK if the parameter is valid\n
 *         #E_ERROR if the parameter is invalid
 */
extern StatusType VentilatorMgr_SetParametersWithInspTime(uint32_t bpm, uint32_t volInML, uint32_t inspTimeMillis);

/**
 * Module periodic update.
 * This function shall be called at fixed time intervals. The ventilator manager
 * internal state is updated with this function call.
 *
 * @return #E_OK if the update was successful\n
 *         #E_ERROR if an error occurred
 */
extern StatusType VentilatorMgr_Update(void);

/**
 * Notifies that the motor has completed the requested movement.
 * This function shall be called when the motor reached the final position
 *
 * How to determine if the motor is at final position is not within the responsibility
 * of this module.
 */
extern void VentilatorMgr_onMoveComplete(void);

/**
 * Sets the PID parameters of the control system.
 * These parameters are only applicable when using the pressure control
 *
 * @param kp proportional constant. Range [-1.0, 1.0)
 * @param ki integral constant. Range [-1.0, 1.0)
 * @param kd derivative constant. Range [-1.0, 1.0)
 *
 * @return #E_OK if the update was successful\n
 *         #E_ERROR if an error occurred
 */
extern StatusType VentilatorMgr_SetPIDParameters(float32_t kp, float32_t ki, float32_t kd);

/**
 * Gets the PID parameters of the control system.
 * These parameters are only applicable when using the pressure control
 *
 * @param Kp pointer to return proportional constant. Range [-1.0, 1.0)
 * @param ki pointer to return integral constant. Range [-1.0, 1.0)
 * @param kd pointer to derivative constant. Range [-1.0, 1.0)
 *
 * @return #E_OK if the update was successful\n
 *         #E_ERROR if an error occurred
 */
extern StatusType VentilatorMgr_GetPIDParameters(float32_t *kp, float32_t *ki, float32_t *kd);

//********************************************************************
//
// Close the Doxygen group.
//! @}
//
//********************************************************************

#endif // _VENTILATOR_MANAGER_API_H
//********************************************************************
//
// Modification Record
//
//********************************************************************
//
// 
//
//********************************************************************

