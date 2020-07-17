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

//*****************************************************************************/
//
//!   \file       interrupt_handlers.c
//!
//!   \brief      This is the implementation of the interrupt handlers
//!
//!   \author     Esteban G. Pupillo
//!
//!   \date       21 Apr 2020
//
//*****************************************************************************/

//*****************************************************************************/
// Include header files
//*****************************************************************************/
#include "standard.h"
#include "clock_drv_api.h"
#include "usart_drv_api.h"
#include "motor_drv_api.h"
#include "rotary_enc_drv_api.h"
#include "periodic_api.h"
#include "logger_api.h"
#include "adc_drv_api.h"
#include "dflow_meter_drv_api.h"

//*****************************************************************************/
//! \addtogroup
//! @{
//!   \addtogroup
//!   @{
//*****************************************************************************/

//*****************************************************************************/
// File level pragmas
//*****************************************************************************/

//*****************************************************************************/
// Constant and Macro Definitions using #define
//*****************************************************************************/

//*****************************************************************************/
// Enumerations and Structures and Typedefs
//*****************************************************************************/

//*****************************************************************************/
// Function Prototypes for Private Functions with File Level Scope
//*****************************************************************************/

//*****************************************************************************/
// ROM Const Variables With File Level Scope
//*****************************************************************************/

//*****************************************************************************/
// Static Variables and Const Variables With File Level Scope
//*****************************************************************************/


//*****************************************************************************/
// Function Definitions
//*****************************************************************************/

/******************************************************************************/
/*           Cortex-M3 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{

}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
   static volatile uint32_t cont;

   cont = 0;

   //for debuggin we loop here.
   //make sure that for the release version you restart the system
  while (cont == 0)
     ;
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
   //for debuggin we loop here.
      //make sure that for the release version you restart the system
  while (1)
     ;
}

/**
  * @brief This function handles Prefetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
   //for debuggin we loop here.
      //make sure that for the release version you restart the system
  while (1)
     ;
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
   //for debuggin we loop here.
      //make sure that for the release version you restart the system
  while (1)
  ;
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{

}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{

}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{

}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  HAL_IncTick();
}

/******************************************************************************/
/* STM32F1xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f1xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles DMA1 channel1 global interrupt.
  */
void DMA1_Channel1_IRQHandler(void)
{
  ADCDrv_DMAIRQHandler();
}

/**IOWritePinID(IO_DBG_LED, IO_OFF);
  * @brief This function handles DMA1 channel4 global interrupt.
  */
void DMA1_Channel4_IRQHandler(void)
{
  USARTDrv_DMATxIRQHandler();
}

/**
  * @brief This function handles DMA1 channel5 global interrupt.
  */
void DMA1_Channel5_IRQHandler(void)
{
  USARTDrv_DMARxIRQHandler();
}

/**
  * @brief This function handles TIM1 update interrupt.
  */
void TIM1_UP_IRQHandler(void)
{
   //IOWritePinID(IO_DBG_LED, IO_ON);
   ClockDrv_IRQHandler();
   //IOWritePinID(IO_DBG_LED, IO_OFF);
}

/**
  * @brief This function handles TIM1 CC interrupt.
  */
void TIM1_CC_IRQHandler(void)
{
   ClockDrv_CCIRQHandler();
}

/**
  * @brief This function handles TIM2 global interrupt.
  */
void TIM2_IRQHandler(void)
{

   MotorDrv_IRQHandler();

}

/**
  * @brief This function handles USART1 global interrupt.
  */
void USART1_IRQHandler(void)
{
   USARTDrv_IRQHandler();
}

/**
  * @brief This function handles EXTI line[15:10] interrupts.
  */
void EXTI15_10_IRQHandler(void)
{
   RotaryEncDrv_IRQHandler();
   //FlowMeterDrv_IRQHandler();
   MotorDrv_HomeIRQHandler();
}

/**
  * @brief This function handles EXTI line[15:10] interrupts.
  */
void EXTI9_5_IRQHandler(void)
{
   //MotorDrv_HomeIRQHandler();
}

void I2C2_EV_IRQHandler(void)
{
   DFlowMeterDrv_IRQEvHandler();
}

void I2C2_ER_IRQHandler(void)
{
   DFlowMeterDrv_IRQErHandler();
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
