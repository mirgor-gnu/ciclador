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
//!   \file       adc_drv.c
//!
//!   \brief      This is the ADC driver implementation file
//!
//!   \author     Esteban Pupillo
//!
//!   \date       21 Apr 2020
//
//********************************************************************

//********************************************************************
// Include header files                                              
//********************************************************************
#include <stdio.h>
#include "standard.h"
#include "stm32f1xx_hal.h"
#include "logger_api.h"

//********************************************************************
//! @addtogroup adc_drv_imp
//!   @{
//********************************************************************

#include "adc_drv_api.h"
#include "adc_drv_conf.h"
#include "adc_drv_callouts.h"


//********************************************************************
// File level pragmas
//********************************************************************

//********************************************************************
// Constant and Macro Definitions using #define
//********************************************************************
#define LOG_TAG "ADCDrv"
#define ADC_INVALID_VALUE (0xFFFF)
#define ADC_VALUE_NO_OVERRIDE (-1)

//********************************************************************
// Enumerations and Structures and Typedefs
//********************************************************************
// Analog input configuration
typedef struct adc_dvr_ch_cfg_tag
{
   uint8_t     hw_channel;
   int32_t    limit_l;
   int32_t    limit_h;
} ADCDrvChCfgType;

typedef struct adc_drv_trigger_cdt_tag
{
   ADCDrvTriggerConfType config;

   //used internally
   uint32_t id;
   volatile Bool raised;
   uint32_t rawThreshold;
   volatile uint32_t value;
   volatile Bool pendingAttention;
} ADCDrvTriggerCDType;


typedef struct adc_drv_tag
{
   volatile uint16_t an_buffer_raw[AN_NUM_CHANNELS];
   volatile int16_t an_buffer_f[AN_NUM_CHANNELS];
   volatile uint16_t an_buffer_min[AN_NUM_CHANNELS];
   volatile uint16_t an_buffer_max[AN_NUM_CHANNELS];
   volatile uint16_t an_buffer_avg[AN_NUM_CHANNELS];
   volatile uint32_t an_buffer_min_time[AN_NUM_CHANNELS];
   volatile uint32_t an_buffer_max_time[AN_NUM_CHANNELS];
   volatile uint32_t an_buffer_avg_samples[AN_NUM_CHANNELS];
   volatile int32_t an_buffer_override[AN_NUM_CHANNELS];

   //lpf_butter_10hz_floatType *filter_lpf;

   Bool lockTriggers;
   ADCDrvTriggerCDType triggers[ADC_DRV_MAX_TRIGGERS];
   uint32_t triggersQtty;
   ADC_HandleTypeDef hadc;
   DMA_HandleTypeDef hdma_adc;
} ADCDrvType;

//********************************************************************
// Function Prototypes for Private Functions with File Level Scope
//********************************************************************
static int32_t Scale(int32_t value, int32_t min_from, int32_t max_from, int32_t min_to, int32_t max_to);
static void adc_drv_update_stats(void);
static void adc_drv_process_triggers(void);
static void adc_drv_process_overrides(void);
static void adc_drv_process_filters(void);
StatusType adc_drv_wait_for_lock(volatile Bool *pLock);

//********************************************************************
// ROM Const Variables With File Level Scope
//********************************************************************
#define X(a,b,c,d) { b, c, d },
static ADCDrvChCfgType const adc_drv_ch_cfg[] =
{
      ADC_DRV_INPUTS_CFG
};
#undef X

//********************************************************************
// Static Variables and Const Variables With File Level Scope
//********************************************************************

static ADCDrvType adc_drv_data;

//********************************************************************
// Function Definitions
//********************************************************************
StatusType ADCDrv_Init(void)
{
   uint32_t i;
   ADC_ChannelConfTypeDef sConfig = {0};

   //adc_drv_data.filter_lpf = lpf_butter_10hz_float_create();
   //lpf_butter_10hz_float_init(adc_drv_data.filter_lpf);

   adc_drv_data.triggersQtty = 0;
   adc_drv_data.lockTriggers = FALSE;

   adc_drv_data.hadc.Instance = ADC1;
   adc_drv_data.hadc.Init.ScanConvMode = ADC_SCAN_ENABLE;
   adc_drv_data.hadc.Init.ContinuousConvMode = DISABLE; //ENABLE;
   adc_drv_data.hadc.Init.DiscontinuousConvMode = DISABLE;
   adc_drv_data.hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
   adc_drv_data.hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
   adc_drv_data.hadc.Init.NbrOfConversion = 3;
   if (HAL_ADC_Init(&adc_drv_data.hadc) != HAL_OK)
   {
      return E_ERROR;
   }

   for(i = 0; i < AN_NUM_CHANNELS; i++)
   {
      // reset statistics
      ADCDrv_ResetStats(i);

      // reset override
      adc_drv_data.an_buffer_override[i] = ADC_VALUE_NO_OVERRIDE;

      // Configure Channel
      sConfig.Channel = adc_drv_ch_cfg[i].hw_channel;
      sConfig.Rank = ADC_REGULAR_RANK_1 + i;
      sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
      if (HAL_ADC_ConfigChannel(&adc_drv_data.hadc, &sConfig) != HAL_OK)
      {
         break;
      }
   }
   // check if there was an error during channel initialization
   if (AN_NUM_CHANNELS > i)
   {
      return E_ERROR;
   }

   /* ADC DMA Init */
   adc_drv_data.hdma_adc.Instance = ADC_DRV_DMA_CHANNEL;
   adc_drv_data.hdma_adc.Init.Direction = DMA_PERIPH_TO_MEMORY;
   adc_drv_data.hdma_adc.Init.PeriphInc = DMA_PINC_DISABLE;
   adc_drv_data.hdma_adc.Init.MemInc = DMA_MINC_ENABLE;
   adc_drv_data.hdma_adc.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
   adc_drv_data.hdma_adc.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
   adc_drv_data.hdma_adc.Init.Mode = DMA_CIRCULAR;
   adc_drv_data.hdma_adc.Init.Priority = DMA_PRIORITY_HIGH;
   if (HAL_DMA_Init(&adc_drv_data.hdma_adc) != HAL_OK)
   {
      return E_ERROR;
   }

   __HAL_LINKDMA(&adc_drv_data.hadc,DMA_Handle,adc_drv_data.hdma_adc);

   HAL_NVIC_SetPriority(ADC_DRV_DMA_IRQ_NAME, ADC_DRV_DMA_IRQ_PRIORITY, 0);
   HAL_NVIC_EnableIRQ(ADC_DRV_DMA_IRQ_NAME);

   ADCDrv_DriverInit();

   HAL_ADC_Start_DMA(&adc_drv_data.hadc, (uint32_t*)adc_drv_data.an_buffer_raw, AN_NUM_CHANNELS);

   return E_OK;
}

void ADCDrv_Update(void)
{
   uint32_t i;

   for(i=0; i < adc_drv_data.triggersQtty; i++)
   {
      ADCDrvTriggerCDType *trigger = &adc_drv_data.triggers[i];
      if ((!trigger->pendingAttention) && (trigger->raised))
      {
         ADCDrvChType ch = trigger->config.channel;
         int32_t value = Scale(trigger->value,
               0, ADC_DRV_MAX_OUTPUT,
               adc_drv_ch_cfg[ch].limit_l, adc_drv_ch_cfg[ch].limit_h);
         //Logger_WriteLine("ADCT", "ch=%ul;v=%ld", ch, value);
         trigger->pendingAttention = TRUE;
         (*trigger->config.triggerFnt)(&trigger->config, value);

         trigger->raised = FALSE;
      }
   }

}

void ADCDrv_StartConversion(void)
{
   HAL_ADC_Start_DMA(&adc_drv_data.hadc, (uint32_t*)adc_drv_data.an_buffer_raw, AN_NUM_CHANNELS);
}

int32_t ADCDrv_GetValue(ADCDrvChType channel, Bool applyFilter, Bool scaled)
{
   int32_t value = 0;

   // Check bounds
   if ((0 <= channel) && (AN_NUM_CHANNELS > channel))
   {
      int16_t adcValue = (applyFilter == TRUE)? adc_drv_data.an_buffer_f[channel] : adc_drv_data.an_buffer_raw[channel];
      // Check scale setting
      if (TRUE == scaled)
      {
         value = Scale (adcValue, 0,
               ADC_DRV_MAX_OUTPUT,
               adc_drv_ch_cfg[channel].limit_l,
               adc_drv_ch_cfg[channel].limit_h);
      }
      else
      {
         value = (int32_t) adcValue;
      }
   }

   return (value);
}

StatusType ADCDrv_ResetStats(ADCDrvChType channel)
{
   // Check bounds
   if ((0 <= channel) && (AN_NUM_CHANNELS > channel))
   {
      adc_drv_data.an_buffer_min[channel] = ADC_INVALID_VALUE;
      adc_drv_data.an_buffer_max[channel] = ADC_INVALID_VALUE;
      adc_drv_data.an_buffer_avg[channel] = ADC_INVALID_VALUE;
      adc_drv_data.an_buffer_avg_samples[channel] = 0;
      adc_drv_data.an_buffer_max_time[channel] = 0;
      adc_drv_data.an_buffer_min_time[channel] = 0;
      return E_OK;
   }
   else
   {
      return E_ERROR;
   }
}

StatusType ADCDrv_GetStats(ADCDrvChType channel, ADCDrvChStatType *stats, Bool scaled)
{
   if (NULL == stats)
   {
      return E_ERROR;
   }

   // Check bounds
   if ((0 <= channel) && (AN_NUM_CHANNELS > channel))
   {
      if (adc_drv_data.an_buffer_avg_samples == 0)
      {
         return E_ERROR;
      }
      // Check scale setting
      if (TRUE == scaled)
      {
         stats->min = Scale (adc_drv_data.an_buffer_min[channel], 0,
               ADC_DRV_MAX_OUTPUT,
               adc_drv_ch_cfg[channel].limit_l,
               adc_drv_ch_cfg[channel].limit_h);

         stats->max = Scale (adc_drv_data.an_buffer_max[channel], 0,
               ADC_DRV_MAX_OUTPUT,
               adc_drv_ch_cfg[channel].limit_l,
               adc_drv_ch_cfg[channel].limit_h);

         stats->avg = Scale (adc_drv_data.an_buffer_avg[channel], 0,
               ADC_DRV_MAX_OUTPUT,
               adc_drv_ch_cfg[channel].limit_l,
               adc_drv_ch_cfg[channel].limit_h);

         stats->value = Scale (adc_drv_data.an_buffer_f[channel], 0,
                        ADC_DRV_MAX_OUTPUT,
                        adc_drv_ch_cfg[channel].limit_l,
                        adc_drv_ch_cfg[channel].limit_h);

         stats->rawValue = Scale (adc_drv_data.an_buffer_raw[channel], 0,
                                 ADC_DRV_MAX_OUTPUT,
                                 adc_drv_ch_cfg[channel].limit_l,
                                 adc_drv_ch_cfg[channel].limit_h);

         stats->tmin = adc_drv_data.an_buffer_min_time[channel];
         stats->tmax = adc_drv_data.an_buffer_max_time[channel];
      }
      else
      {
         stats->min = (int32_t)adc_drv_data.an_buffer_min[channel];
         stats->max = (int32_t)adc_drv_data.an_buffer_max[channel];
         stats->avg = (int32_t)adc_drv_data.an_buffer_avg[channel];
         stats->value = (int32_t)adc_drv_data.an_buffer_f[channel];
         stats->rawValue = (int32_t)adc_drv_data.an_buffer_raw[channel];
         stats->tmin = adc_drv_data.an_buffer_min_time[channel];
         stats->tmax = adc_drv_data.an_buffer_max_time[channel];
      }
      return E_OK;
   }
   else
   {
      return E_ERROR;
   }
}


StatusType ADCDrv_RegisterTrigger(ADCDrvTriggerConfType *trigger)
{
   ADCDrvChType ch;
   ADCDrvTriggerCDType *t;
   if (adc_drv_data.triggersQtty >= ADC_DRV_MAX_TRIGGERS)
   {
      return E_ERROR;
   }

   if (NULL == trigger)
      return E_ERROR;

   if ((trigger->channel >= AN_NUM_CHANNELS) || (NULL == trigger->triggerFnt))
      return E_ERROR;

   if (E_OK != adc_drv_wait_for_lock(&adc_drv_data.lockTriggers))
      return E_ERROR;

   adc_drv_data.lockTriggers = TRUE;

   ch = trigger->channel;
   t = &adc_drv_data.triggers[adc_drv_data.triggersQtty];

   t->id = adc_drv_data.triggersQtty;
   t->rawThreshold = Scale (trigger->threshold,
         adc_drv_ch_cfg[ch].limit_l,adc_drv_ch_cfg[ch].limit_h,
         0, ADC_DRV_MAX_OUTPUT);
   t->raised = FALSE;

   adc_drv_data.triggersQtty++;

   trigger->uuid = t->id;
   t->config = *trigger;

   adc_drv_data.lockTriggers = FALSE;

   return E_OK;
}

StatusType ADCDrv_UnregisterTrigger(ADCDrvTriggerConfType *trigger)
{
   int32_t i, triggerIdx;

   if ((adc_drv_data.triggersQtty == 0) || (NULL == trigger))
   {
      return E_ERROR;
   }
   if (E_OK != adc_drv_wait_for_lock(&adc_drv_data.lockTriggers))
      return E_ERROR;

   adc_drv_data.lockTriggers = TRUE;

   triggerIdx = -1;
   for(i=0; i < adc_drv_data.triggersQtty; i++)
   {
      if (adc_drv_data.triggers[i].id == trigger->uuid)
      {
         triggerIdx = i;
         break;
      }
   }

   if (triggerIdx < 0)
   {
      adc_drv_data.lockTriggers = FALSE;
      return E_ERROR;
   }

   for(i=triggerIdx; i < (adc_drv_data.triggersQtty - 1); i++)
   {
      adc_drv_data.triggers[i] = adc_drv_data.triggers[i+1];
   }

   adc_drv_data.triggersQtty--;

   adc_drv_data.lockTriggers = FALSE;

   return E_OK;
}

StatusType ADCDrv_ResetTrigger(ADCDrvTriggerConfType *trigger)
{
   int32_t i, triggerIdx;

   if ((adc_drv_data.triggersQtty == 0) || (NULL == trigger))
   {
      return E_ERROR;
   }

   triggerIdx = -1;
   for(i=0; i < adc_drv_data.triggersQtty; i++)
   {
      if (adc_drv_data.triggers[i].id == trigger->uuid)
      {
         triggerIdx = i;
         break;
      }
   }

   if (triggerIdx < 0)
      return E_ERROR;

   adc_drv_data.triggers[triggerIdx].pendingAttention = FALSE;

   return E_OK;
}

void ADCDrv_Dbg(void)
{
   char tmp[64];
   uint32_t i, pos;

   pos = 0;
   for(i=0; i< AN_NUM_CHANNELS; i++)
   {
      pos += snprintf(tmp + pos, 64-pos, "%lu=%u;", i, adc_drv_data.an_buffer_f[i]);
   }

   tmp[pos] = '\0';

   LOG_PRINT_INFO(DEBUG_ADC_DRV, LOG_TAG, "%s", tmp);
}

void ADCDrv_DMAIRQHandler(void)
{
   DMA_HandleTypeDef *hdma = &(adc_drv_data.hdma_adc);
   uint32_t flag_it = hdma->DmaBaseAddress->ISR;
   uint32_t source_it = hdma->Instance->CCR;


   /* Half Transfer Complete Interrupt management ******************************/
   if (((flag_it & (DMA_FLAG_HT1 << hdma->ChannelIndex)) != RESET) && ((source_it & DMA_IT_HT) != RESET))
   {
      /* Disable the half transfer interrupt if the DMA mode is not CIRCULAR */
      if((hdma->Instance->CCR & DMA_CCR_CIRC) == 0U)
      {
         /* Disable the half transfer interrupt */
         __HAL_DMA_DISABLE_IT(hdma, DMA_IT_HT);
      }
      /* Clear the half transfer complete flag */
      __HAL_DMA_CLEAR_FLAG(hdma, __HAL_DMA_GET_HT_FLAG_INDEX(hdma));

      // if you want here you can notify someone about the half transfer event
   }

   /* Transfer Complete Interrupt management ***********************************/
   else if (((flag_it & (DMA_FLAG_TC1 << hdma->ChannelIndex)) != RESET) && ((source_it & DMA_IT_TC) != RESET))
   {
      if((hdma->Instance->CCR & DMA_CCR_CIRC) == 0U)
      {
         /* Disable the transfer complete and error interrupt */
         __HAL_DMA_DISABLE_IT(hdma, DMA_IT_TE | DMA_IT_TC);



         /* Change the DMA state */
         hdma->State = HAL_DMA_STATE_READY;
      }

      /* Clear the transfer complete flag */
      __HAL_DMA_CLEAR_FLAG(hdma, __HAL_DMA_GET_TC_FLAG_INDEX(hdma));

      /* Process Unlocked */
      __HAL_UNLOCK(hdma);

      // if you want here you can notify someone about the transfer complete event
      IOWritePinID(IO_DBG_LED, IO_ON);
      adc_drv_process_overrides();
      adc_drv_process_filters();

      //adc_drv_data.an_buffer_f[1] = adc_drv_data.an_buffer_raw[1];
      //adc_drv_data.an_buffer_f[2] = adc_drv_data.an_buffer_raw[0];
      //tmpIn = (1.0f * adc_drv_data.an_buffer_raw[0]) / 4096;
      //lpf_butter_10hz_float_writeInput(adc_drv_data.filter_lpf, tmpIn);
      //tmpOut = lpf_butter_10hz_float_readOutput(adc_drv_data.filter_lpf);
      //adc_drv_data.an_buffer_f[0] = (int16_t)(tmpOut * 4096);

      adc_drv_update_stats();
      adc_drv_process_triggers();
      IOWritePinID(IO_DBG_LED, IO_OFF);
      //HAL_ADC_Start_DMA(&adc_drv_data.hadc, (uint32_t*)adc_drv_data.an_buffer, AN_NUM_CHANNELS);
   }

   /* Transfer Error Interrupt management **************************************/
   else if (( RESET != (flag_it & (DMA_FLAG_TE1 << hdma->ChannelIndex))) && (RESET != (source_it & DMA_IT_TE)))
   {
      /* When a DMA transfer error occurs */
      /* A hardware clear of its EN bits is performed */
      /* Disable ALL DMA IT */
      __HAL_DMA_DISABLE_IT(hdma, (DMA_IT_TC | DMA_IT_HT | DMA_IT_TE));

      /* Clear all flags */
      hdma->DmaBaseAddress->IFCR = (DMA_ISR_GIF1 << hdma->ChannelIndex);

      /* Update error code */
      hdma->ErrorCode = HAL_DMA_ERROR_TE;

      /* Change the DMA state */
      hdma->State = HAL_DMA_STATE_READY;

      /* Process Unlocked */
      __HAL_UNLOCK(hdma);

      // if you want here you can notify someone about the error event
   }
   return;
}

StatusType adc_drv_wait_for_lock(volatile Bool *pLock)
{
   uint32_t count;
   count = ADC_DRV_WAIT_FOR_LOCK_TIMEOUT * (SystemCoreClock / 25U / 1000U);
   do
   {
      count--;
      if (count == 0U)
      {
         return E_ERROR;
      }
   }
   while (*pLock != FALSE);

   return E_OK;
}

static void adc_drv_process_filters(void)
{
   uint32_t i;
   for(i=0; i< AN_NUM_CHANNELS; i++)
   {
      ADCDrv_FilterSignal(i, adc_drv_data.an_buffer_raw[i], &adc_drv_data.an_buffer_f[i]);
   }
}

static void adc_drv_update_stats(void)
{
   uint32_t i;

   for(i=0; i< AN_NUM_CHANNELS; i++)
   {
      // calculate the minimum value
      if ((ADC_INVALID_VALUE == adc_drv_data.an_buffer_min[i]) || (adc_drv_data.an_buffer_f[i] < adc_drv_data.an_buffer_min[i]))
      {
         adc_drv_data.an_buffer_min[i] = adc_drv_data.an_buffer_f[i];
         adc_drv_data.an_buffer_min_time[i] = ADCDrv_GetHighResTimestamp();
      }

      // calculate the average value
      if (ADC_INVALID_VALUE == adc_drv_data.an_buffer_min[i])
      {
         adc_drv_data.an_buffer_avg[i] = adc_drv_data.an_buffer_f[i];
         adc_drv_data.an_buffer_avg_samples[i] = 1;
      }
      else
      {
         adc_drv_data.an_buffer_avg[i] = (adc_drv_data.an_buffer_avg[i] * adc_drv_data.an_buffer_avg_samples[i] + adc_drv_data.an_buffer_f[i])
                                 / (adc_drv_data.an_buffer_avg_samples[i] + 1);

         if (adc_drv_data.an_buffer_avg_samples[i] < ADC_DRV_STATS_AVG_MAX_WINDOW )
         {
            adc_drv_data.an_buffer_avg_samples[i]++;
         }
      }

      // calculate the max value
      if ((ADC_INVALID_VALUE == adc_drv_data.an_buffer_max[i]) || (adc_drv_data.an_buffer_f[i] > adc_drv_data.an_buffer_max[i]))
      {
         adc_drv_data.an_buffer_max[i] = adc_drv_data.an_buffer_f[i];
         adc_drv_data.an_buffer_max_time[i] = ADCDrv_GetHighResTimestamp();
      }
   }
}

static void adc_drv_process_triggers(void)
{
   uint32_t i;

   if (TRUE == adc_drv_data.lockTriggers)
   {
      //triggers are locked. we skip processing them
      return;
   }

   adc_drv_data.lockTriggers = TRUE;

   for(i=0; i< adc_drv_data.triggersQtty; i++)
   {
      ADCDrvTriggerCDType *trigger = &adc_drv_data.triggers[i];
      ADCDrvChType ch = trigger->config.channel;
      int32_t threshold = trigger->rawThreshold;
      int32_t adcValue = adc_drv_data.an_buffer_f[ch];

      switch (trigger->config.type)
      {
         case ADC_DRV_TRIGGER_TYPE_LOWER_THAN:
            if (adcValue < threshold)
            {
               trigger->raised = TRUE;
               trigger->value = adcValue;
            }
            break;
         case ADC_DRV_TRIGGER_TYPE_HIGHER_THAN:
            if (adcValue > threshold)
            {
               trigger->raised = TRUE;
               trigger->value = adcValue;
            }
            break;
         default:
            break;
      }
   }

   adc_drv_data.lockTriggers = FALSE;

}


//********************************************************************
//
//! Value scale
//!
//! \param value     The value to be scaled
//! \param min_from  Lower limit
//! \param max_from  Higher limit
//! \param min_to    Lower limit
//! \param max_to    Higher limit
//!
//! This function will scale the value using the specified limits.
//!
//! \return Scaled value.
//
//********************************************************************
static int32_t Scale(int32_t value, int32_t min_from, int32_t max_from, int32_t min_to, int32_t max_to)
{
   int32_t result;

   result = max_to - (((max_to - min_to) * (max_from - value)) / (max_from - min_from));

   return (result);
}

//***********************************
// functions for debugging purposses
//***********************************

static void adc_drv_process_overrides(void)
{
   uint32_t i;

   for(i=0; i< AN_NUM_CHANNELS; i++)
   {
      if (ADC_VALUE_NO_OVERRIDE != adc_drv_data.an_buffer_override[i])
      {
         adc_drv_data.an_buffer_raw[i] = adc_drv_data.an_buffer_override[i];
      }
   }
}

void ADCDrv_DbgOverrideChannel(ADCDrvChType ch, int32_t value)
{
   if (value < 0)
   {
      //disable override
      adc_drv_data.an_buffer_override[ch] = ADC_VALUE_NO_OVERRIDE;
   }
   else
   {
      adc_drv_data.an_buffer_override[ch] = value;
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

