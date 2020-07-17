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
//!   \file       periodic.c
//!
//!   \brief      This is the periodic module implementation file
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
#include "logger_api.h"

//********************************************************************
//! @addtogroup periodic_imp
//!   @{
//********************************************************************

//********************************************************************
// File level pragmas
//********************************************************************
#include "periodic_conf.h"
#include "periodic_api.h"
#include "periodic_callouts.h"

//********************************************************************
// Constant and Macro Definitions using #define
//********************************************************************

//********************************************************************
// Enumerations and Structures and Typedefs
//********************************************************************

//********************************************************************
// Function Prototypes for Private Functions with File Level Scope
//********************************************************************
static void periodic_task(void);

//********************************************************************
// ROM Const Variables With File Level Scope
//********************************************************************

//********************************************************************
// Static Variables and Const Variables With File Level Scope
//********************************************************************
static void periodic_init(void);
static void periodic_1x(void);
static void periodic_2x(void);
static void periodic_4x(void);
static void periodic_8x(void);
static void periodic_16x(void);
static void periodic_32x(void);
static void periodic_64x(void);
static void periodic_128x(void);

//********************************************************************
// Function Definitions
//********************************************************************

//********************************************************************
//
//! Periodic module initialization
//!
//! \param None.
//!
//! This initialization should be performed prior to starting the
//! RTOS scheduling i.e. called from main() initialization.
//!
//! \return None.
//
//********************************************************************
void Periodic_Init(void)
{

}

void Periodic_Start(void)
{
   //start periodic_task
   periodic_task();
}

//********************************************************************
//
//! Periodic module task
//!
//! \param None.
//!
//! This is the RTOS task for periodic call handling.
//!
//! \return None.
//
//********************************************************************
static void periodic_task(void)
{
   uint8_t slice = 0;
   uint32_t lastTicks = 0;

   // Initialize required data
   periodic_init();

   // Task loop
   for (;;)
   {
      // Delay task
	   if ((HAL_GetTick() - lastTicks) >= PERIODIC_MIN_TIMESLOT)
	   {
	      lastTicks = HAL_GetTick();
		   slice++;

		   Periodic_OnProcessingStart();

		   periodic_1x();

		   if ((0x01 & slice) == 0x01)
			   periodic_2x();

		   if ((0x03 & slice) == 0x03)
			   periodic_4x();

		   if ((0x07 & slice) == 0x07)
			   periodic_8x();

		   if ((0x0F & slice) == 0x0F)
			   periodic_16x();

		   if ((0x1F & slice) == 0x1F)
			   periodic_32x();

		   if ((0x3F & slice) == 0x3F)
			   periodic_64x();

		   if ((0x7F & slice) == 0x7F)
			   periodic_128x();

		   Periodic_OnProcessingStop();
	   }
   }
   //we should never get here!
}

// Periodic call init
static void periodic_init(void)
{

}

// 1X periodic task (16ms)
static void periodic_1x(void)
{
   // call application specific handler
   Periodic_handler_1x();
}

// 2X periodic task
static void periodic_2x(void)
{
  // call application specific handler
  Periodic_handler_2x();
}

// 4X periodic task
static void periodic_4x(void)
{
   // call application specific handler
   Periodic_handler_4x();
}

// 8X periodic task
static void periodic_8x(void)
{
   // call application specific handler
   Periodic_handler_8x();
}

// 16X periodic task
static void periodic_16x(void)
{
   // call application specific handler
   Periodic_handler_16x();
}

// 32X periodic task
static void periodic_32x(void)
{
   // call application specific handler
   Periodic_handler_32x();
}

// 64X periodic task
static void periodic_64x(void)
{
   // call application specific handler
   Periodic_handler_64x();
}

// 128X periodic task
static void periodic_128x(void)
{
   // call application specific handler
   Periodic_handler_128x();
}

//********************************************************************
//
// Close the Doxygen group.
//! @}
//
//********************************************************************



