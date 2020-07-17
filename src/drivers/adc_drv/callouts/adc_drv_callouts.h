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
//!   \file       adc_drv_callouts.h
//!
//!   \brief      ADC driver callouts header file
//!
//!   \author     Esteban Pupillo
//!
//!   \date       21 Apr 2020
//!
//********************************************************************

#ifndef  _ADC_DRV_CALLOUTS_H
#define  _ADC_DRV_CALLOUTS_H 1

//********************************************************************
//! @addtogroup adc_drv_callouts
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
/**
 * Callout to get the a high resolution timestamp
 *
 * @param none
 *        
 *
 * @return the high resolution timestamp
 * 
 */
extern uint32_t ADCDrv_GetHighResTimestamp(void);

/**
 * Callout to applied a filtering algorithm
 *
 * @param in the input data
 *        out the data after the filtering is applied
 *
 * @return none
 * 
 */
extern void ADCDrv_FilterSignal(ADCDrvChType ch, int16_t in, int16_t *out);

/**
 * Driver low level initialization callout.
 * This function is called during the driver initialization and shall
 * be used to initialize external modules/drivers that interact with the
 * #ADCDriver
 *
 * If an error is returned, the module initialization will not continue.
 *
 * @return #E_OK if no error occurred\n
 *         #E_ERROR if an error occurred
 */
extern StatusType ADCDrv_DriverInit(void);

//********************************************************************
//
// Close the Doxygen group.
//! @}
//
//********************************************************************

#endif // _ADC_DRV_CALLOUTS_H
//********************************************************************
//
// Modification Record
//
//********************************************************************
//
// 
//
//********************************************************************

