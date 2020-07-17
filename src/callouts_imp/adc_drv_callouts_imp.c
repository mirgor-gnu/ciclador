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
//!   \file       adc_drv_callouts_imp.c
//!
//!   \brief      This is the adc driver callouts implementation.
//!
//!   \author     Esteban G. Pupillo
//!
//!   \date       04 May 2020
//
//********************************************************************

//********************************************************************
// Include header files
//********************************************************************
#include "standard.h"
#include "stm32f1xx_hal.h"
#include "clock_drv_api.h"
#include "lpf_butter_10hz_float.h"


//********************************************************************
//! @addtogroup adc_drv_callouts
//!   @{
//********************************************************************

#include "adc_drv_conf.h"
#include "adc_drv_api.h"
#include "adc_drv_callouts.h"

//********************************************************************
// File level pragmas
//********************************************************************

//********************************************************************
// Constant and Macro Definitions using #define
//********************************************************************

//********************************************************************
// Enumerations and Structures and Typedefs
//********************************************************************

//********************************************************************
// Function Prototypes for Private Functions with File Level Scope
//********************************************************************

//********************************************************************
// ROM Const Variables With File Level Scope
//********************************************************************

//********************************************************************
// Static Variables and Const Variables With File Level Scope
//********************************************************************
static lpf_butter_10hz_floatType *filter_lpf;
//********************************************************************
// Function Definitions
//********************************************************************

inline StatusType ADCDrv_DriverInit(void)
{
   filter_lpf = lpf_butter_10hz_float_create();
   lpf_butter_10hz_float_init(filter_lpf);

   return E_OK;
}

inline uint32_t ADCDrv_GetHighResTimestamp()
{
   return ClockDrv_GetHighResTimestamp();
}

inline void ADCDrv_FilterSignal(ADCDrvChType ch, int16_t in, int16_t *out)
{
   float tmpIn, tmpOut;

   if (AIN_PRESSURE == ch)
   {
      tmpIn = (1.0f * in) / 4096;
      lpf_butter_10hz_float_writeInput(filter_lpf, tmpIn);
      tmpOut = lpf_butter_10hz_float_readOutput(filter_lpf);
      *out = (int16_t)(tmpOut * 4096);
   }
   else
   {
      *out = in;
   }
}

//********************************************************************
//
// Close the Doxygen group.
//! @}
//
//********************************************************************

//********************************************************************
//
// Modification Record
//
//********************************************************************
//
//
//
//********************************************************************
