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
//   Title                : logger_api.h
//
//   Description          : module APIs header file
//
//   Author               : Esteban Pupillo
//
//   Created              : 21 Apr 2020
//
//********************************************************************

#ifndef  _LOGGER_API_H
#define  _LOGGER_API_H 1

//********************************************************************
// Constant and Macro Definitions using #define
//********************************************************************
//#define LOG_CONF_NO_LOGS
#define LOG_CONF_VERBOSE

#define DEBUG_ADC_DRV      1
#define DEBUG_MOTOR        1
#define DEBUG_MOTOR_DRV    1
#define DEBUG_ENCODER      1
#define DEBUG_FMETER       1
#define DEBUG_SMON         1
#define DEBUG_CPU          1
#define DEBUG_VENT_MGR     1
#define DEBUG_ADCT         1
#define DEBUG_PM           1
#define DEBUG_VENT_P       1
#define DEBUG_VENT_E       1
#define DEBUG_AM           1
#define DEBUG_HMI          1


#define LOG_PRINT_ERR(enable, tag, msg...)   do                            \
                                             {                             \
                                                if (enable)                \
                                                {                          \
                                                   LOG_ERR(tag , ##msg);   \
                                                }                          \
                                             } while(0)

#define LOG_PRINT_INFO(enable, tag, msg...)  do                            \
                                             {                             \
                                                if (enable)                \
                                                {                          \
                                                   LOG_INFO(tag , ##msg);  \
                                                }                          \
                                             } while(0)

#define LOG_PRINT_VER(enable, tag, msg...)   do                            \
                                             {                             \
                                                if (enable)                \
                                                {                          \
                                                   LOG_VER(tag , ##msg);   \
                                                }                          \
                                             } while(0)


#if defined(LOG_CONF_NO_LOGS)
   #define LOG_ERR(tag, arg...)
   #define LOG_INFO(tag, arg...)
   #define LOG_VER(tag, arg...)
#else
   #define LOG_ERR(tag, arg...)     Logger_WriteLine(tag , ##arg)
   #define LOG_INFO(tag, arg...)    Logger_WriteLine(tag , ##arg)
   #if defined(LOG_CONF_VERBOSE)
      #define LOG_VER(tag, arg...)  Logger_WriteLine(tag , ##arg)
   #else
      #define LOG_VER(tag, arg...)
   #endif
#endif

//********************************************************************
// Enumerations and Structures and Typedefs
//********************************************************************

//********************************************************************
// Global Variable extern Declarations
//********************************************************************

//********************************************************************
// Function Prototypes
//********************************************************************
// Remember to use extern modifier
extern uint32_t Logger_Init();
extern uint32_t Logger_WriteLine(char *tag, char *msg, ...);
extern void Logger_DMACpltCallback(void);

#endif // _LOGGER_API_H
//********************************************************************
//
// Modification Record
//
//********************************************************************
//
// 
//
//********************************************************************

