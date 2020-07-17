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
//!   \file       logger.c
//!
//!   \brief      This is the logger module.
//!
//!               This module implements a logging to.
//!
//!   \author     Esteban G. Pupillo
//!
//!   \date       21 Apr 2020
//
//********************************************************************

//********************************************************************
// Include header files                                              
//********************************************************************
#include "standard.h"
#include "string.h"
#include <stdarg.h>
#include <stdio.h>

//********************************************************************
//! \addtogroup 
//! @{
//!   \addtogroup
//!   @{
//********************************************************************
#include "logger_conf.h"
#include "logger_api.h"
#include "logger_callouts.h"

//********************************************************************
// File level pragmas
//********************************************************************

//********************************************************************
// Constant and Macro Definitions using #define
//********************************************************************

//********************************************************************
// Enumerations and Structures and Typedefs
//********************************************************************
typedef struct log_data_tag {
   volatile uint8_t buffer[LOGGER_BUFFER_SIZE];
   volatile uint8_t *writePtr;
   volatile uint8_t *readPtr;
   volatile uint32_t bytesToWrite;
   volatile uint32_t bytesAvailable;
   volatile uint32_t bytesDMA;

   volatile uint32_t status;
   Bool isInitialized;
} LogType;

//********************************************************************
// Function Prototypes for Private Functions with File Level Scope
//********************************************************************
static uint32_t logGetAvailableBytes(void);
static uint32_t logGetAvailableLinearBytes(void);
static uint32_t logStartDMATransaction(void);
static uint32_t logGetFreeBytes(void);

//********************************************************************
// ROM Const Variables With File Level Scope
//********************************************************************

//********************************************************************
// Static Variables and Const Variables With File Level Scope
//********************************************************************
static LogType loggerData;

//********************************************************************
// Function Definitions
//********************************************************************

uint32_t Logger_Init(void)
{
   uint32_t i;
   loggerData.isInitialized = FALSE;
   for (i=0; i<LOGGER_BUFFER_SIZE; i++)
   {
      loggerData.buffer[i] = 0;
   }
   loggerData.readPtr = &loggerData.buffer[0];
   loggerData.writePtr = &loggerData.buffer[0];
   loggerData.bytesToWrite = 0;
   loggerData.bytesDMA = 0;
   loggerData.bytesAvailable = LOGGER_BUFFER_SIZE;
   loggerData.status = 0;

   loggerData.isInitialized = TRUE;
   return 0;
}

void Logger_Update(void)
{

}

uint32_t Logger_WriteLine(char *tag, char *msg, ...)
{
   va_list args;
   va_start(args, msg);
   volatile LogType *this = &loggerData;
   size_t totalLen;
   uint32_t freeBytes, bytesToWrite, bytesToCopy;
   char debug[128];

   if (!loggerData.isInitialized)
      return 0;

   freeBytes = logGetFreeBytes();
#ifdef LOGGER_DEBUG
   totalLen = snprintf(debug, 128, "[bF=%ld, bA=%ld, wP=%p, rP=%p]%s: ",freeBytes,
         logGetAvailableBytes(), this->writePtr, this->readPtr, tag);
#else
   totalLen = snprintf(debug, 128, "[%010lu]%s: ",Logger_GetTimestamp(), tag);
#endif

   totalLen += vsnprintf(debug + totalLen, 128 - totalLen, msg, args);
   if (totalLen < 128)
   {
      debug[totalLen++] = '\n';
      debug[totalLen++] = '\r';
   }

   bytesToWrite = (freeBytes < totalLen)? freeBytes : totalLen;

   if ((bytesToWrite == 0) || (bytesToWrite < totalLen))
   {
      snprintf(debug, 128, "no space");
      return 0;
   }

   bytesToCopy = ((LOGGER_BUFFER_SIZE - (uint32_t)(this->writePtr-this->buffer)) < bytesToWrite)?
         (LOGGER_BUFFER_SIZE - (uint32_t)(this->writePtr-this->buffer)): bytesToWrite;
   memcpy((uint8_t*)this->writePtr, debug, bytesToCopy);
   this->writePtr += bytesToCopy;
   bytesToWrite -= bytesToCopy;

   if (bytesToWrite > 0)
   {
      memcpy((uint8_t*)this->buffer, &debug[bytesToCopy], bytesToWrite);
      this->writePtr = this->buffer + bytesToWrite;
   }

   if (this->writePtr >= (this->buffer+LOGGER_BUFFER_SIZE))
   {
      this->writePtr = this->buffer;
   }

   logStartDMATransaction();

   return bytesToCopy + bytesToWrite;
}

void Logger_DMACpltCallback(void)
{
   volatile LogType *this = &loggerData;
   uint8_t *r;
   uint32_t bytesToSkip, bytesUsed;

   r = (uint8_t*)this->readPtr;
   bytesUsed = logGetAvailableBytes();
   bytesToSkip = this->bytesDMA;

   r += (bytesUsed < bytesToSkip)? bytesUsed:bytesToSkip;
   if (r >= (this->buffer + LOGGER_BUFFER_SIZE))
   {
      r -= LOGGER_BUFFER_SIZE;
   }
   this->readPtr = r;

   //this->readPtr += this->bytesDMA;
   //this->bytesToWrite -= this->bytesDMA;
   this->status = 0;

   logStartDMATransaction();
}

static uint32_t logGetAvailableBytes()
{
   volatile LogType *this = &loggerData;
   uint8_t *w, *r;
   uint32_t size = 0;

   w = (uint8_t*)this->writePtr;
   r = (uint8_t*)this->readPtr;

   if (w == r)
   {
      size = 0;
   } else if (w > r)
   {
      size = w - r;
   } else
   {
      size = LOGGER_BUFFER_SIZE - (r - w);
   }
   return size;
}

static uint32_t logGetAvailableLinearBytes()
{
   volatile LogType *this = &loggerData;
   uint8_t *w, *r;
   uint32_t size = 0;

   w = (uint8_t*)this->writePtr;
   r = (uint8_t*)this->readPtr;

   if (w > r)
   {
      size = w - r;
   } else if (r > w)
   {
      size = LOGGER_BUFFER_SIZE - (uint32_t)(r-this->buffer);
   } else
   {
      size = 0;
   }
   return size;
}

static uint32_t logStartDMATransaction(void)
{
   volatile LogType *this = &loggerData;
   int ret;
   uint32_t size;
   size = logGetAvailableLinearBytes();
   ret = Logger_StartDMATransaction((void*)this->readPtr, size);
   if (ret == 0)
   {
      this->bytesDMA = size;
      this->status = 1;
   }
   return ret;
}

static uint32_t logGetFreeBytes(void)
{
   volatile LogType *this = &loggerData;
   uint8_t *w, *r;
   uint32_t size = 0;

   w = (uint8_t*)this->writePtr;
   r = (uint8_t*)this->readPtr;

   if (w == r)
   {
      size = LOGGER_BUFFER_SIZE;
   } else if ( r > w)
   {
      size = r - w;
   } else
   {
      size = LOGGER_BUFFER_SIZE - (w - r);
   }
   return size;
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

