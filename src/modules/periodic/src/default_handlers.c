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
//!   \file       default_handlers.c
//!
//!   \brief      This is the Default handlers module implementation file
//!
//!   \author     Esteban Pupillo
//!
//!   \date       21 Apr 2020
//
//********************************************************************

//********************************************************************
// Include header files
//********************************************************************
#include "standard.h"

//********************************************************************
//! @addtogroup periodic_imp
//!   @{
//********************************************************************
#include "periodic_conf.h"
#include "periodic_api.h"
#include "periodic_callouts.h"

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

/**
 * @brief  Default hander implementation prototype, declared as weak so
 *         it can be override if the handler is implemented
 * 
 * @param none
 * 
 * @return none
 * 
 */
void __attribute__((weak)) periodic_default_handler(void);

/**
 * @brief  Forward declaration of the specific periodic handlers. These are aliased
 *         to the periodic_default_handler, which is do nothing. When the application
 *         defines a handler (with the same name), this will automatically take
 *         precedence over these weak definitions
 * 
 * @param none
 * 
 * @return none
 * 
 */
void __attribute__ ((weak, alias ("periodic_default_handler"))) Periodic_handler_1x(void);
void __attribute__ ((weak, alias ("periodic_default_handler"))) Periodic_handler_2x(void);
void __attribute__ ((weak, alias ("periodic_default_handler"))) Periodic_handler_4x(void);
void __attribute__ ((weak, alias ("periodic_default_handler"))) Periodic_handler_8x(void);
void __attribute__ ((weak, alias ("periodic_default_handler"))) Periodic_handler_16x(void);
void __attribute__ ((weak, alias ("periodic_default_handler"))) Periodic_handler_32x(void);
void __attribute__ ((weak, alias ("periodic_default_handler"))) Periodic_handler_64x(void);
void __attribute__ ((weak, alias ("periodic_default_handler"))) Periodic_handler_128x(void);

//********************************************************************
// ROM Const Variables With File Level Scope
//********************************************************************

//********************************************************************
// Static Variables and Const Variables With File Level Scope
//********************************************************************

//********************************************************************
// Function Definitions
//********************************************************************


/**
 * @brief  Default hander implementation
 * 
 * @param none
 * 
 * @return none
 * 
 */
void periodic_default_handler(void)
{

}

//********************************************************************
//
// Close the Doxygen group.
//! @}
//
//********************************************************************



