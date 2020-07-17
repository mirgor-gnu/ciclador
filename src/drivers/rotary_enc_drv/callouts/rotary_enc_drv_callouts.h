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
//!   \file       rotary_enc_drv_callouts.h
//!
//!   \brief      rotary encoder driver callouts header file
//!
//!   \author     Esteban Pupillo
//!
//!   \date       21 Apr 2020
//
//********************************************************************

#ifndef  _ROTARY_ENC_DRV_CALLOUTS_H
#define  _ROTARY_ENC_DRV_CALLOUTS_H 1

//********************************************************************
//! @addtogroup rotary_enc_drv_callouts
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
 * @brief Obtain the high resolution timestamp used by this module
 *  
 * @param none
 *
 * @return The time resolution timestamp
 */
uint32_t RotaryEncDrv_GetHighResTimestamp(void);

/**
 * @brief This callout signals the encoder updated the step counter
 *  
 * @param pos the new position after the update
 * @param deltaPos the delta position from the last update
 * @param speed the new speed after the update 
 *
 * @return none
 */
void RotaryEncDrv_OnNewStep(int32_t pos, int32_t deltaPos, uint32_t speed);

//********************************************************************
//
// Close the Doxygen group.
//! @}
//
//********************************************************************

#endif // _ROTARY_ENC_DRV_CALLOUTS_H
//********************************************************************
//
// Modification Record
//
//********************************************************************
//
// 
//
//********************************************************************

