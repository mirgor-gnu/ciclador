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
//!   @file       display_drv_conf.h
//!
//!   @brief      Display Driver configuration header file
//!
//!   @author     Agustin Diaz Antuna
//!
//!   @date       29 Apr 2020
//!
//********************************************************************

#ifndef  _DISPLAY_DRV_CONF_H
#define  _DISPLAY_DRV_CONF_H 1

//********************************************************************
//! @addtogroup display_drv_conf
//! @{
//********************************************************************

//********************************************************************
// Constant and Macro Definitions using #define
//********************************************************************
/**
 * Display buffer size.
 * The display driver will write in this buffer and a function called
 * on the interrupt read each word and send it to the display.
 */
#define DISPLAY_DRV_BUFFER_SIZE         512

//********************************************************************
// Enumerations and Structures and Typedefs
//********************************************************************

//********************************************************************
// Global Variable extern Declarations
//********************************************************************

//********************************************************************
// Function Prototypes
//********************************************************************

#endif // _DISPLAY_DRV_CONF_H
//********************************************************************
//
// Modification Record
//
//********************************************************************
//
// 
//
//********************************************************************


