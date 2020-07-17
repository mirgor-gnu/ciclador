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
//!   \file       adc_drv_api.h
//!
//!   \brief      ADC driver APIs header file
//!
//!   \author     Esteban Pupillo
//!
//!   \date       21 Apr 2020
//!
//********************************************************************

#ifndef  _ADC_DRV_API_H
#define  _ADC_DRV_API_H 1

//********************************************************************
//! @addtogroup adc_drv_api
//!   @{
//********************************************************************

#include "adc_drv_conf.h"

//********************************************************************
// Constant and Macro Definitions using #define
//********************************************************************

//********************************************************************
// Enumerations and Structures and Typedefs
//********************************************************************
/**
 * Structure containing all the statistics
 */
typedef struct adc_drv_ch_stat_tag
{
   int32_t min;         /**< min conversion */
   int32_t max;         /**< max conversion */
   int32_t avg;         /**< avg conversion */
   int32_t value;       /**< last filtered conversion */
   int32_t rawValue;    /**< last raw conversion */
   uint32_t tmin;       /**< min time between conversions */
   uint32_t tmax;       /**< max time between conversions */
} ADCDrvChStatType;

/* \cond DO_NOT_DOCUMENT */
#define X(a,b,c,d) a,
/* \endcond */
/** @brief Analog channels enumeration 
 * 
 */
typedef enum adc_drv_ch_tag
{
   ADC_DRV_INPUTS_CFG
   //do not remove this one
   AN_NUM_CHANNELS,
} ADCDrvChType;
#undef X

/**
 * Trigger types
 */
typedef enum adc_drv_trigger_type_tag
{
   ADC_DRV_TRIGGER_TYPE_LOWER_THAN,    /**< Value is lower than the trigger */
   ADC_DRV_TRIGGER_TYPE_HIGHER_THAN,   /**< Value is higher than the trigger */
}ADCDrvTriggerType;

/**
 * Trygger config type
 */
typedef struct adc_drv_trigger_conf_tag ADCDrvTriggerConfType;

/**
 * Pointer to function to execute when a trigger is active
 */
typedef void (*onTriggerEventFnt)(ADCDrvTriggerConfType *trigger, int32_t value);

/**
 * Trigger structure
 */
struct adc_drv_trigger_conf_tag
{
   ADCDrvChType channel;         /**< Channel to applied the trigger to */
   ADCDrvTriggerType type;       /**< Trigger type */
   int32_t threshold;            /**< Trigger threshold */
   onTriggerEventFnt triggerFnt; /**< Function to execute when the trigger is met */
   void *pUserData;              /**< To be used by the higher layers */
   uint32_t uuid;                /**< id of the trigger */

   //used internally
   //uint32_t id;
   //volatile Bool raised;
   //uint32_t rawThreshold;
   //volatile uint32_t value;
};

//********************************************************************
// Global Variable extern Declarations
//********************************************************************

//********************************************************************
// Function Prototypes
//********************************************************************
// Remember to use extern modifier

/**
 * Initializes the ADC driver module
 *
 * @param
 *
 * @return none
 * 
 */
extern StatusType ADCDrv_Init(void);

/**
 * Starts a conversion of the ADC
 *
 * @param
 *
 * @return none
 * 
 */
extern void ADCDrv_StartConversion(void);

/**
 * Checks if there is a trigger active and calls the respective callback
 * This function must be called periodically
 *
 * @param
 *
 * @return none
 * 
 */
extern void ADCDrv_Update(void);

/**
 * Returns the value of the last conversion of a channel.
 * The rerturned value can be scaled and or filtered.
 *
 * @param channel the channel to obtain the conversion from
 *        applyFilter true: return the filtered conversion
 *                    false: return the raw conversion
 *        scaled      true: return the scaled conversion
 *                    false: do not apply any scale to the conversion
 * @return The conversion result with the filter and scaling applied if selected
 * 
 */
extern int32_t ADCDrv_GetValue(ADCDrvChType channel, Bool applyFilter, Bool scaled);

/**
 * Get the conversion statistics since their last reset of a channel
 * 
 * The statistics returned are
 *   max_value
 *   min_value
 *   average_value
 *   last filtered value
 *   last raw value
 *   max time between conversions
 *   min time between conversions 
 *
 * @param channel the channel to obtain the statistics from
 *        stats the structure to be filled
 *        scaled true: return the scaled statistics
 *               false: do not apply any scale to the statistics
 *
 * @return #E_OK if the operation was successful\n
 *         #E_ERROR if an error occurred
 * 
 */
extern StatusType ADCDrv_GetStats(ADCDrvChType channel, ADCDrvChStatType *stats, Bool scaled);

/**
 * Reset the stats counters of a certain channel
 *
 * @param channel the channel to reset the statistics
 *
 * @return #E_OK if the operation was successful\n
 *         #E_ERROR if an error occurred
 * 
 */
extern StatusType ADCDrv_ResetStats(ADCDrvChType channel);

/**
 * Register a trigger to the system 
 *
 * @param trigger the trigger to register
 *
 * @return #E_OK if the operation was successful\n
 *         #E_ERROR if an error occurred
 * 
 */
extern StatusType ADCDrv_RegisterTrigger(ADCDrvTriggerConfType *trigger);

/**
 * Unregister a trigger to the system 
 *
 * @param trigger the trigger to deregister
 *
 * @return #E_OK if the operation was successful\n
 *         #E_ERROR if an error occurred
 * 
 */
extern StatusType ADCDrv_UnregisterTrigger(ADCDrvTriggerConfType *trigger);

/**
 * Reset a trigger state
 *
 * @param trigger the trigger to reset
 *
 * @return #E_OK if the operation was successful\n
 *         #E_ERROR if an error occurred
 * 
 */
extern StatusType ADCDrv_ResetTrigger(ADCDrvTriggerConfType *trigger);

/**
 * Handle of the DMA channel interrupt
 *
 * @param
 *
 * @return none
 * 
 */
extern void ADCDrv_DMAIRQHandler(void);

/**
 * Send adc information for debug purposes 
 *
 * @param
 *
 * @return none
 * 
 */
extern void ADCDrv_Dbg(void);

//********************************************************************
//
// Close the Doxygen group.
//! @}
//
//********************************************************************

#endif // _ADC_DRV_API_H
//********************************************************************
//
// Modification Record
//
//********************************************************************
//
// 
//
//********************************************************************

