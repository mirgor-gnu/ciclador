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
//!   \file       hmi.h
//!
//!   \brief      HMI header file
//!
//!   \author     Agustin Diaz Antuna
//!
//!   \date       06 May 2020
//
//********************************************************************

#ifndef  _HMI_H
#define  _HMI_H 1

//********************************************************************
//! @addtogroup hmi_imp
//! @{
//********************************************************************

//********************************************************************
// Constant and Macro Definitions using #define
//********************************************************************
#define HMI_PARAM_VALUE_NA (1UL<<31)

// Shared strings used by different screens
#define HMI_STR_ALARM_TITLE      "<<<< ALARMA >>>>"
#define HMI_STR_ACCEPT           "ACEPTAR         "
#define HMI_STR_CANCEL           "CANCELAR        "
#define HMI_STR_SILENCE          "SILENCIAR       "
#define HMI_STR_NEXT             "SIGUIENTE       "
#define HMI_STR_GO_TO_MENU       "VOLVER AL MENU  "
#define HMI_STR_EMPTY            "                "
#define HMI_STR_DIVISOR          "----------------"

//********************************************************************
// Enumerations and Structures and Typedefs
//********************************************************************
/**
 * HMI number format.
 * These values are used to identify each available number format to
 * display different kind of values.
 */
typedef enum hmi_value_format_tag {
   FMT_DOT_DIV10,          /**< Example: Value 123 - Displayed 1.2 */
   FMT_DOT_DIV1000,        /**< Example: Value 1234 - Displayed 1.2 */
   FMT_1_DIGIT,            /**< Example: Value 1 - Displayed 1 */
   FMT_2_DIGITS,           /**< Example: Value 12 - Displayed 12 */
   FMT_2_DIGITS_DIV10,     /**< Example: Value 123 - Displayed 12 */
   FMT_2_DIGITS_DIV10_INV, /**< Example: Value 123 - Displayed 12 and could display '--' if it has an invalid value */
   FMT_3_DIGITS,           /**< Example: Value 123 - Displayed 123 */
   FMT_3_DIGITS_INV,       /**< Example: Value 123 - Displayed 123 */
   FMT_4_DIGITS,           /**< Example: Value 1234 - Displayed 1234 */
   FMT_CONTROL_MODE,       /**< Special case for select an #HmiMotorControlType it could be 'V' or 'P' */
   FMT_NONE                /**< No format */
} HmiValueFormatType;

/**
 * HMI message type.
 * This module has defined two possible messages: alarm or error.
 * Both screens are very similar:
 * - #LINE1 is the title.
 * - #LINE2 and #LINE3 has a message.
 * - #LINE4 has a way to leave this screen (accept, next or silence)
 */
typedef enum hmi_message_type_tag {
   HMI_MSG_ALARM,
   HMI_MSG_ERROR,
} HmiMessageTypeId;

enum {
   LINE1,
   LINE2,
   LINE3,
   LINE4,
   LINE_MAX,
};

//********************************************************************
// Global Variable extern Declarations
//********************************************************************

//********************************************************************
// Function Prototypes
//********************************************************************

#endif // _HMI_H
//********************************************************************
//
// Modification Record
//
//********************************************************************
//
// 
//
//********************************************************************

