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
//!   \file       board_hw_config.c
//!
//!   \brief      This is the code file for board_hw_config module.
//!
//!               This module configures the hardware used for the
//!               application.
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
#include "stm32f1xx_hal.h"

//********************************************************************
//! \addtogroup high
//! @{
//!   \addtogroup sub
//!   @{
//********************************************************************
// include local module header here
#include "board_hw_config.h"
#include "board_hw_io.h"

//********************************************************************
// File level pragmas
//********************************************************************

//********************************************************************
// Constant and Macro Definitions using #define
//********************************************************************
#define HW_VectTab_FLASH         ((uint32_t)0x08000000)

//********************************************************************
// Enumerations and Structures and Typedefs
//********************************************************************

//********************************************************************
// Function Prototypes for Private Functions with File Level Scope
//********************************************************************
static StatusType hw_rcc_initialization(void);
static void hw_nvic_initialization(void);
static void hw_gpio_initialization(void);
static void hw_dma_initialization(void);

//********************************************************************
// ROM Const Variables With File Level Scope
//********************************************************************

//********************************************************************
// Static Variables and Const Variables With File Level Scope
//********************************************************************


//********************************************************************
// Function Definitions
//********************************************************************

//********************************************************************
//
//! Hardware startup initialization
//!
//! \param None.
//!
//! System hardware initialization after reset.
//!
//! \return None.
//
//********************************************************************
void Board_Init(void)
{
   // Init HAL
   HAL_Init();

   // Setup STM32 clock, PLL and Flash configuration
   //SystemInit();

   // Setup clocks
   hw_rcc_initialization();

   //NOJTAG: JTAG-DP Disabled and SW-DP Enabled
   __HAL_AFIO_REMAP_SWJ_NOJTAG();

   // Setup NVIC
   hw_nvic_initialization();

   // Setup GPIO
   hw_gpio_initialization();

   // Setup DMA controller
   hw_dma_initialization();

}

//********************************************************************
//
//! System clocks initialization
//!
//! \param None.
//!
//! Initializes all the clocking routing
//!
//! \return None.
//
//********************************************************************
static StatusType hw_rcc_initialization(void)
{

   RCC_OscInitTypeDef RCC_OscInitStruct = {0};
   RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
   RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

   /** Initializes the CPU, AHB and APB busses clocks
    */
   RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
   RCC_OscInitStruct.HSEState = RCC_HSE_ON;
   RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
   RCC_OscInitStruct.HSIState = RCC_HSI_ON;
   RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
   RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
   RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
   if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
   {
      return E_ERROR;
   }
   /** Initializes the CPU, AHB and APB busses clocks
    */
   RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
         |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
   RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
   RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
   RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
   RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

   if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
   {
      return E_ERROR;
   }
   PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
   PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
   if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
   {
      return E_ERROR;
   }

   // enable peripherals clocks
#undef X
#define X(a) __HAL_RCC_##a##_CLK_ENABLE();
   BOARD_PERIPHERALS_CLK_TABLE
#undef X

   return E_OK;
}




//********************************************************************
//
//! NVIC initialization
//!
//! \param None.
//!
//! Nested vectored interrupt controller
//!
//! \return None.
//
//********************************************************************
static void hw_nvic_initialization(void)
{
   // Set the Vector Table base address at 0x08000000
   //NVIC_SetVectorTable( HW_VectTab_FLASH, 0x0 );

   // Set priority group
   //NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );

}

//********************************************************************
//
//! GPIO initialization
//!
//! \param None.
//!
//! Initializes the GPIO's being used in this project
//!
//! \return None.
//
//********************************************************************
static void hw_gpio_initialization(void)
{
   // Configure microprocessor pins
#undef X
 #define X(a,b,c,d,e,f,g) IO_Config(c,d,f,g,e);
   IO_CFG_TABLE
#undef X

   // set default state
#define X(a,f,b,c,d,e,g) IOWritePinID(a, f);
   IO_CFG_TABLE
#undef X


}

//********************************************************************
//
//! Enable DMA controller clock
//!
//! \param None.
//!
//! Initialize the DMA controller
//!
//! \return None.
//
//********************************************************************
static void hw_dma_initialization(void)
{

   /* DMA controller clock enable */
   __HAL_RCC_DMA1_CLK_ENABLE();

   /* DMA interrupt init */
   /* DMA1_Channel1_IRQn interrupt configuration */
   HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
   HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
   /* DMA1_Channel4_IRQn interrupt configuration */
   HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 0, 0);
   HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);

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

