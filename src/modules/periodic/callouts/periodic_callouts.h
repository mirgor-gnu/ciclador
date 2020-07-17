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
//!   \file       periodic_callouts.h
//!
//!   \brief      Periodic callouts header file
//!
//!   \author     Esteban Pupillo
//!
//!   \date       21 Apr 2020
//!
//********************************************************************
#ifndef PERIODIC_CALLOUTS_H_
#define PERIODIC_CALLOUTS_H_

//********************************************************************
//! @addtogroup periodic_callouts
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
 * Callout called whenever the base slot time has elapsed and 
 * the periodic module will start cheking for the trigger timeouts
 *    
 * @param none
 *
 * @return none
 * 
 */
void Periodic_OnProcessingStart(void);

/**
 * Callout called when the periodic module has finished checking all
 * the triggers timeouts.
 * 
 *    
 * @param none
 *
 * @return none
 * 
 */
void Periodic_OnProcessingStop(void);

/**
 * Callout called when the base slot timeout has elapsed 1 time
 * The count is reset after this callout is called
 *    
 * @param none
 *
 * @return none
 * 
 */
void Periodic_handler_1x(void);

/**
 * Callout called when the base slot timeout has elapsed 2 times
 * The count is reset after this callout is called
 *    
 * @param none
 *
 * @return none
 * 
 */
void Periodic_handler_2x(void);

/**
 * Callout called when the base slot timeout has elapsed 4 times
 * The count is reset after this callout is called
 *    
 * @param none
 *
 * @return none
 * 
 */
void Periodic_handler_4x(void);

/**
 * Callout called when the base slot timeout has elapsed 8 times
 * The count is reset after this callout is called
 *    
 * @param none
 *
 * @return none
 * 
 */
void Periodic_handler_8x(void);

/**
 * Callout called when the base slot timeout has elapsed 16 times
 * The count is reset after this callout is called
 *    
 * @param none
 *
 * @return none
 * 
 */
void Periodic_handler_16x(void);

/**
 * Callout called when the base slot timeout has elapsed 32 times
 * The count is reset after this callout is called
 *    
 * @param none
 *
 * @return none
 * 
 */
void Periodic_handler_32x(void);

/**
 * Callout called when the base slot timeout has elapsed 64 times
 * The count is reset after this callout is called
 *    
 * @param none
 *
 * @return none
 * 
 */
void Periodic_handler_64x(void);

/**
 * Callout called when the base slot timeout has elapsed 128 times
 * The count is reset after this callout is called
 *    
 * @param none
 *
 * @return none
 * 
 */
void Periodic_handler_128x(void);

//********************************************************************
//
// Close the Doxygen group.
//! @}
//
//********************************************************************

#endif /* PERIODIC_CALLOUTS_H_ */
