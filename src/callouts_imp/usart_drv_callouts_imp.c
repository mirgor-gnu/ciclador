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
//!   \file       usart_drv_callouts_imp.c
//!
//!   \brief      This is the usart driver callouts implementation.
//!
//!   \author     Esteban G. Pupillo
//!
//!   \date       21 Apr 2020
//
//********************************************************************

//********************************************************************
// Include header files
//********************************************************************
#include <string.h>
#include <stdlib.h>
#include "standard.h"
#include "stm32f1xx_hal.h"
#include "logger_api.h"
#include "motor_drv_api.h"
#include "motor_manager_api.h"
#include "rotary_enc_drv_api.h"
#include "ventilator_manager_api.h"
#include "adc_drv_api.h"
#include "alarm_manager_api.h"
#include "hmi_api.h"

//********************************************************************
//! \addtogroup
//! @{
//!   \addtogroup
//!   @{
//********************************************************************
#include "usart_drv_conf.h"
#include "usart_drv_api.h"
#include "usart_drv_callouts.h"

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
extern void ADCDrv_DbgOverrideChannel(ADCDrvChType ch, int32_t value);
//********************************************************************
// ROM Const Variables With File Level Scope
//********************************************************************

//********************************************************************
// Static Variables and Const Variables With File Level Scope
//********************************************************************

//********************************************************************
// Function Definitions
//********************************************************************
void USARTDrv_OnTransmitComplete(UART_HandleTypeDef *huart)
{
   Logger_DMACpltCallback();
}

static char buf[64];
void USARTDrv_OnReceiveComplete(UART_HandleTypeDef *huart, uint32_t size)
{
   char *token;
   uint32_t motorDir, motorLvl;
   uint32_t speed, dist, timeout;
   float kp, ki, kd;

   USARTDrv_Receive(buf, (size > 63)? 63: size);


   buf[size+1] = '\0';
   token = strtok(buf, ",;");
   if (0 == strncmp(token,"M",2))
   {
      /* MotorDrv Commands
       * M,1,[PWM];  -> Motor start CW with PWM
       * M,2,[PWM];  -> Motor start CCW with PWM
       * M,3;        -> Motor stop
       * M,4;        -> Motor brake
       * M,5,[dir],[dist],[speed]; -> Motor move distance at speed
       * M,6,kp,ki,kd; -> Motor PID parameters
       * M,7;        -> Motor goHome
       */
      token = strtok(NULL, ",;");
      switch (*token)
      {
         case '1':
         case '2':
            motorDir = (*token == '1')? MOTOR_DIR_CW : MOTOR_DIR_CCW;
            token = strtok(NULL, ",;");
            motorLvl = atoi(token);
            MotorDrv_Start(motorDir, motorLvl);
            break;
         case '3':
            MotorDrv_Stop(MOTOR_STOP_NORMAL);
            break;
         case '4':
            MotorDrv_Stop(MOTOR_STOP_BRAKE);
            break;
         case '5':
            token = strtok(NULL, ",;");
            motorDir = atoi(token);
            token = strtok(NULL, ",;");
            dist = atoi(token);
            token = strtok(NULL, ",;");
            speed = atoi(token);
            MotorDrv_MoveDistanceAtSpeed(motorDir, dist, speed);
            break;
         case '6':
            token = strtok(NULL, ",;");
            kp = atof(token);
            token = strtok(NULL, ",;");
            ki = atof(token);
            token = strtok(NULL, ",;");
            kd = atof(token);
            MotorDrv_SetPIDParameters(kp, ki, kd);
            break;
         case '7':
            MotorDrv_GoHome();
            break;
         default:
            break;
      }
   }
   else if (0 == strncmp(token, "MM", 2))
   {
      /* Motor Manager Commands
       * MM,1; -> MotorManager Start
       * MM,2; -> MotorManager Stop
       * MM,3,pos,speed; -> MotorManager Update
       * MM,4,kp,ki,kd; -> MotorManager SetPIDParameters
       * MM,5,setpoint; -> MotorManager SetSetpoint
       */
      int32_t pos;
      static int32_t lastPos;

      lastPos = 0;

      token = strtok(NULL, ",;");
      switch (*token)
      {
         case '1':
            RotaryEncDrv_SetPosition(0);
            MotorManager_Start();
            break;
         case '2':
            MotorManager_Stop();
            break;
         case '3':
            token = strtok(NULL, ",;");
            pos = atoi(token);
            token = strtok(NULL, ",;");
            speed = atoi(token);
            MotorManager_UpdatePosAndSpeed(pos, pos-lastPos, speed);
            lastPos = pos;
            break;
         case '4':
            token = strtok(NULL, ",;");
            kp = atof(token);
            token = strtok(NULL, ",;");
            ki = atof(token);
            token = strtok(NULL, ",;");
            kd = atof(token);
            MotorManager_SetPIDParameters(kp, ki, kd);
            break;
         case '5':
            token = strtok(NULL, ",;");
            speed = atoi(token);
            token = strtok(NULL, ",;");
            dist = atoi(token);
            token = strtok(NULL, ",;");
            timeout = atoi(token);
            MotorManager_SetSetpoint(speed, dist, timeout);
            break;

         default:
            break;
      }
   }
   else if (0 == strncmp(token, "VM", 2))
   {
      /* Ventilator Manager Commands
       * VM,1; -> VentialtorMgr Start
       * VM,2; -> VentialtorMgr Stop
       * VM,3,bpm,vol,ieratio; -> VentilatorMgr SetParameter
       * VM,4,bpm,vol,tinsp; -> VentilatorMgr SetParameter
       * VM,5,mode; -> VentilatorMgr SetControlMode
       * VM,6,pressure -> VentilatorMgr_SetInspiratoryPressure
       * VM,7,kp,ki,kd; -> VentilatorMgr_SetPIDParameters
       * VM,8,minTV; -> VentilatorMgr_SetMinTidalVolume
       * VM,9,maxTv; -> VentilatorMgr_SetMaxTidalVolume
       */
      uint32_t bpm, vol, ieratio, inspTime, mode, minTv, maxTv;
      token = strtok(NULL, ",;");
      switch (*token)
      {
         case '1':
            VentilatorMgr_Start();
            break;
         case '2':
            VentilatorMgr_Stop(VENTILATOR_MGR_STOP_TYPE_EMERGENCY);
            break;
         case '3':
            token = strtok(NULL, ",;");
            bpm = atoi(token);
            token = strtok(NULL, ",;");
            vol = atoi(token);
            token = strtok(NULL, ",;");
            ieratio = atoi(token);
            VentilatorMgr_SetParametersWithIERatio(bpm, vol, ieratio);
            break;
         case '4':
            token = strtok(NULL, ",;");
            bpm = atoi(token);
            token = strtok(NULL, ",;");
            vol = atoi(token);
            token = strtok(NULL, ",;");
            inspTime = atoi(token);
            VentilatorMgr_SetParametersWithInspTime(bpm, vol, inspTime);
            break;
         case '5':
            token = strtok(NULL, ",;");
            mode = atoi(token);
            VentialtorMgr_SetControlMode(mode);
            break;
         case '6':
            token = strtok(NULL, ",;");
            mode = atoi(token);
            VentilatorMgr_SetInspiratoryPressure(mode);
            break;
         case '7':
            token = strtok(NULL, ",;");
            kp = atof(token);
            token = strtok(NULL, ",;");
            ki = atof(token);
            token = strtok(NULL, ",;");
            kd = atof(token);
            VentilatorMgr_SetPIDParameters(kp, ki, kd);
            break;
         case '8':
            token = strtok(NULL, ",;");
            minTv = atoi(token);
            VentilatorMgr_SetMinTidalVolume(minTv);
            break;
         case '9':
            token = strtok(NULL, ",;");
            maxTv = atoi(token);
            VentilatorMgr_SetMaxTidalVolume(maxTv);
            break;
         default:
            break;
      }
   } else if (0 == strncmp(token, "AM", 2))
   {
      /* Alarm Manager Commands
       * AM,1,id,status,errorValue; -> AlarmMgr SetAlarm
       * AM,2; -> AlarmMgr AlarmStatus
       */
      uint32_t id, status, errorValue;
      token = strtok(NULL, ",;");
      switch (*token)
      {
         case '1':
            token = strtok(NULL, ",;");
            id = atoi(token);
            token = strtok(NULL, ",;");
            status = atoi(token);
            token = strtok(NULL, ",;");
            errorValue = atoi(token);
            AlarmMgr_SetAlarm(id, status, (void *) errorValue);
            break;
         case '2':
            AlarmMgr_AlarmStatus();
            break;
         default:
            break;
      }
   } else if (0 == strncmp(token, "HMI", 3))
   {
      /* HMI Commands
       * HMI,1,keyMap,type; -> HMI NewKeyMap
       */
      uint32_t keyMap, type;
      token = strtok(NULL, ",;");
      switch (*token)
      {
         case '1':
            token = strtok(NULL, ",;");
            keyMap = atoi(token);
            token = strtok(NULL, ",;");
            type = atoi(token);
            Hmi_NewKeyMap(keyMap, type);
            break;
         default:
            break;
      }
   }
   else if (0 == strncmp(token, "ADC", 2))
   {
      /* ADC driver Commands
       * ADC,1,[ch],[value]; -> ADC override channel
       */
      uint32_t ch;
      int32_t value;
      token = strtok(NULL, ",;");
      switch (*token)
      {
         case '1':
            token = strtok(NULL, ",;");
            ch = atoi(token);
            token = strtok(NULL, ",;");
            value = atoi(token);
            ADCDrv_DbgOverrideChannel(ch, value);
            break;
         default:
            break;
      }
   }
}

//********************************************************************
//
// Close the Doxygen group.
//!   @}
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
