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
//!   \file       power_manager_api.h
//!
//!   \brief      power manager APIs header file
//!
//!   \author     Esteban Pupillo
//!
//!   \date       11 MAY 2020
//!
//********************************************************************

#ifndef  _POWER_MANAGER_API_H
#define  _POWER_MANAGER_API_H 1

//********************************************************************
//! @addtogroup power_manager_api
//!   @{
//********************************************************************

//********************************************************************
// Constant and Macro Definitions using #define
//********************************************************************

//********************************************************************
// Enumerations and Structures and Typedefs
//********************************************************************

/**
 * @brief Power manager valid states
 * 
 */
typedef enum power_manager_state_tag
{
   POWER_MGR_POWER_STATE_NORMAL,    /**< power manager working under normal conditions */
   POWER_MGR_POWER_STATE_BACKUP,    /**< power manager working with backup battery */
}PowerMgrStateType;
//********************************************************************
// Global Variable extern Declarations
//********************************************************************

//********************************************************************
// Function Prototypes
//********************************************************************
// Remember to use extern modifier

/**
 * Initialize the power manager module
 *    - Initialize internal variables
 *    
 *
 * @param none
 *
 * @return none
 * 
 */
extern StatusType PowerMgr_Init(void);

/**
 * Periodic function to measure the voltages and update the
 * power manager FSM
 *    
 * @param none
 *
 * @return none
 * 
 */
extern void PowerMgr_Update();

//********************************************************************
//
// Close the Doxygen group.
//! @}
//
//********************************************************************

#endif // _POWER_MANAGER_API_H
//********************************************************************
//
// Modification Record
//
//********************************************************************
//
// 
//
//********************************************************************

