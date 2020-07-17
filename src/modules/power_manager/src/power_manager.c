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
//!   \file       power_manager.c
//!
//!   \brief      This is the power manager module implementation file
//!
//!   \author     Esteban Pupillo
//!
//!   \date       05 MAY 2020
//
//********************************************************************

//********************************************************************
// Include header files                                              
//********************************************************************
#include "standard.h"

//********************************************************************
//! @addtogroup power_manager_imp
//!   @{
//********************************************************************

#include "power_manager_conf.h"
#include "power_manager_api.h"
#include "power_manager_callouts.h"


//********************************************************************
// File level pragmas
//********************************************************************

//********************************************************************
// Constant and Macro Definitions using #define
//********************************************************************

//********************************************************************
// Enumerations and Structures and Typedefs
//********************************************************************
typedef struct power_mgr_data_tag
{
   PowerMgrStateType state;
} PowerMgrDataType;

//********************************************************************
// Function Prototypes for Private Functions with File Level Scope
//********************************************************************


//********************************************************************
// ROM Const Variables With File Level Scope
//********************************************************************

//********************************************************************
// Static Variables and Const Variables With File Level Scope
//********************************************************************
static PowerMgrDataType power_mgr_data;

//********************************************************************
// Function Definitions
//********************************************************************

StatusType PowerMgr_Init(void)
{
   power_mgr_data.state = POWER_MGR_POWER_STATE_NORMAL;

   return E_OK;
}

void PowerMgr_Update()
{
   if (POWER_MGR_MAIN_POWER_SENSE_PIN_ACTIVE_STATE == IOReadPinID(POWER_MGR_MAIN_POWER_SENSE_PIN))
   {
      //main power active
      //check if we need to change our internal state
      if (POWER_MGR_POWER_STATE_NORMAL != power_mgr_data.state)
      {
         //inform a new state change
         PowerMgr_OnStateChange(POWER_MGR_POWER_STATE_NORMAL);
         power_mgr_data.state = POWER_MGR_POWER_STATE_NORMAL;
      }
   }
   else
   {
      //we are running with the backup power supply
      //check if we need to change our internal state
      if (POWER_MGR_POWER_STATE_BACKUP != power_mgr_data.state)
      {
         //inform a new state change
         PowerMgr_OnStateChange(POWER_MGR_POWER_STATE_BACKUP);
         power_mgr_data.state = POWER_MGR_POWER_STATE_BACKUP;
      }
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

