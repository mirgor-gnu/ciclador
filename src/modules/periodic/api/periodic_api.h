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
//!   \file       periodic_api.h
//!
//!   \brief      Periodic APIs header file
//!
//!   \author     Esteban Pupillo
//!
//!   \date       21 Apr 2020
//!
//********************************************************************

#ifndef PERIODIC_API_H_
#define PERIODIC_API_H_

//********************************************************************
//! @addtogroup periodic_api
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
 * UNUSED - Initialize the periodic module
 *    
 *
 * @param none
 *
 * @return none
 * 
 */
extern void Periodic_Init(void);

/**
 * Endless loop, uses the system tick to measure the passing of time
 * Whenever a fixed amount of time has passed a callout is called. 
 * This function serves as the operating system of the mcu. All periodic
 * functions of the rest of the modules are called from here.
 *  
 * The function is always running but it only starts processing anc checking
 * for timeouts after a time slot has passed. All the timeouts must be 
 * multiple of this slot.
 * 
 * This function should never exit
 *    
 *
 * @param none
 *
 * @return none
 * 
 */
extern void Periodic_Start(void);

//********************************************************************
//
// Close the Doxygen group.
//! @}
//
//********************************************************************

#endif /* PERIODIC_API_H_ */
