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
//!   @file       hmi_conf.h
//!
//!   @brief      HMI configuration header file
//!
//!   @author     Agustin Diaz Antuna
//!
//!   @date       29 Apr 2020
//!
//********************************************************************

#ifndef  _HMI_CONF_H
#define  _HMI_CONF_H 1

#include "hmi.h"

//********************************************************************
//! @addtogroup hmi_conf
//! @{
//********************************************************************

//********************************************************************
// Constant and Macro Definitions using #define
//********************************************************************
/**
 * Displayed parameters common to both control mode setup.
 * 
 * It will be used to initialize #HmiElementType
 * 
 * The input format is: X( [id], [line], [text], [text_x], 
 *                         [value_enable], [value_default], [value_x], [value_format], [value_min], [value_max], [value_step],
 *                         [cursor_enable], [cursor_x],
 *                         [unit_enable], [unit_text], [unit_x], [key])
 */
#define HMI_PARAMS_COMMON_CFG \
   X(PARAM_TI  , LINE1, "Ti:"      , 0 , true , 1000 , 3 , FMT_DOT_DIV1000   , 700, 1200, 100, true , 7, true , "s  " , 6 ) \
   X(PARAM_FR  , LINE3, "FR:"      , 0 , true , 18   , 3 , FMT_2_DIGITS      , 10 , 30  , 1  , true , 7, true , "/m  ", 5 ) \
   X(PARAM_MORE, LINE4, "MAS      ", 0 , false, 0    , 0 , FMT_NONE          , 0  , 0   , 0  , false, 0, false, ""    , 0 ) \

/**
 * Displayed parameters in volume control setup.
 * 
 * It will be used to initialize #HmiElementType
 * 
 * The input format is: X( [id], [line], [text], [text_x], 
 *                         [value_enable], [value_default], [value_x], [value_format], [value_min], [value_max], [value_step],
 *                         [cursor_enable], [cursor_x],
 *                         [unit_enable], [unit_text], [unit_x], [key])
 */
#define HMI_PARAMS_CTRL_VOLUME_CFG \
   X(PARAM_VT  , LINE2, "VT:"      , 0 , true , 300  , 3 , FMT_3_DIGITS      , 250, 650 , 25 , true , 8, true , "ml " , 6 ) \

/**
 * Displayed parameters in pressure control setup.
 * 
 * It will be used to initialize #HmiElementType
 * 
 * The input format is: X( [id], [line], [text], [text_x], 
 *                         [value_enable], [value_default], [value_x], [value_format], [value_min], [value_max], [value_step],
 *                         [cursor_enable], [cursor_x],
 *                         [unit_enable], [unit_text], [unit_x], [key])
 */
#define HMI_PARAMS_CTRL_PRESSURE_CFG \
   X(PARAM_PI  , LINE2, "PI:"      , 0 , true , 200  , 3 , FMT_2_DIGITS_DIV10, 50 , 400 , 10 , true , 8, true , "cmH ", 5 ) \

/**
 * Displayed sensors setup.
 * 
 * It will be used to initialize #HmiElementType
 * 
 * The input format is: X( [id], [line], [text], [text_x], 
 *                         [value_enable], [value_default], [value_x], [value_format], [value_min], [value_max], [value_step],
 *                         [cursor_enable], [cursor_x],
 *                         [unit_enable], [unit_text], [unit_x], [key])
 */
#define HMI_SENSORS_CFG \
   X(SENSOR_IE  , LINE1, "IE: " , 9 , true , 100               , 13, FMT_DOT_DIV10         , 0, 0  , 0 , false, 0, false , "", 0 ) \
   X(SENSOR_PIP , LINE2, "PIP: ", 9 , true , HMI_PARAM_VALUE_NA, 14, FMT_2_DIGITS_DIV10_INV, 0, 0  , 0 , false, 0, false , "", 0 ) \
   X(SENSOR_PEEP, LINE3, "PEEP:", 9 , true , HMI_PARAM_VALUE_NA, 14, FMT_2_DIGITS_DIV10_INV, 0, 0  , 0 , false, 0, false , "", 0 ) \
   X(SENSOR_VT  , LINE4, "Vt: " , 9 , true , HMI_PARAM_VALUE_NA, 13, FMT_3_DIGITS_INV      , 0, 0  , 0 , false, 0, false , "", 0 ) \

/**
 * Second screen screen setup.
 * 
 * It will be used to initialize #HmiElementType
 * 
 * The input format is: X( [id], [line], [text], [text_x], 
 *                         [value_enable], [value_default], [value_x], [value_format], [value_min], [value_max], [value_step],
 *                         [cursor_enable], [cursor_x],
 *                         [unit_enable], [unit_text], [unit_x], [key])
 */
#define HMI_SECOND_SCREEN_CFG \
   X(SS_ADJUST_ALARMS, LINE1, "AJUSTE ALARMAS  ", 0, false, 0  , 0 , FMT_NONE        , 0, 0, 0, false, 0, false, "", 0 ) \
   X(SS_SEE_ALARMS   , LINE2, "ALARMAS ACTIV: " , 0, true , 0  , 14, FMT_2_DIGITS    , 0, 0, 0, false, 0, false, "", 0 ) \
   X(SS_BACK         , LINE4, "VOLVER          ", 0, false, 0  , 0 , FMT_NONE        , 0, 0, 0, false, 0, false, "", 0 ) \

/**
 * Settings screen setup.
 * It is used to configure the displayed strings of the settings screen.
 * It will be used to initialize #HmiElementType
 * 
 * The input format is: X( [id], [line], [text], [text_x], [key] )
 */
#define HMI_SETTING_CFG \
   X(SET_DIVISOR, LINE2, "--------", 0 ) \
   X(SET_ACCEPT , LINE3, "ACEPTAR ", 0 ) \
   X(SET_CANCEL , LINE4, "CANCELAR", 0 ) \

/**
 * Set motor control mode screen setup.
 * It is used to configure the displayed strings of the settings screen.
 * It will be used to initialize #HmiElementType
 * 
 * The input format is: X( [id], [line], [text], [text_x], [key] )
 */
#define HMI_CONTROL_MODE_CFG \
   X(CM_TITLE   , LINE1, "  MODO CONTROL  ", 0 ) \
   X(CM_VOLUME  , LINE2, "CTRL VOLUMEN    ", 0 ) \
   X(CM_PRESSURE, LINE3, "CTRL PRESION    ", 0 ) \
   X(CM_CANCEL  , LINE4, HMI_STR_CANCEL    , 0 ) \

/**
 * Double check stop screen setup.
 * It is used to configure the displayed strings of the settings screen.
 * It will be used to initialize #HmiElementType
 * 
 * The input format is: X( [id], [line], [text], [text_x], [key] )
 */
#define HMI_CONFIRM_STOP_CFG \
   X(CS_TITLE  , LINE1, "PARAR VENTILADO?", 0 ) \
   X(CS_VOLUME , LINE3, "CONFIRMAR       ", 0 ) \
   X(CS_CANCEL , LINE4, HMI_STR_CANCEL    , 0 ) \




/**
 * Second screen screen setup.
 * 
 * It will be used to initialize #HmiElementType
 * 
 * The input format is: X( [id], [line], [text], [text_x], 
 *                         [value_enable], [value_default], [value_x], [value_format], [value_min], [value_max], [value_step],
 *                         [cursor_enable], [cursor_x],
 *                         [unit_enable], [unit_text], [unit_x], [key])
 */
#define HMI_ADJUST_ALARM_SCREEN_1_CFG \
   X(AAS1_PEEP_MIN , LINE1, "PEEP Min: <  "   , 0, true , 0, 13, FMT_2_DIGITS_DIV10,  10, 200, 10, true ,15, true , " ", 15) \
   X(AAS1_PIP_MIN  , LINE2, "PIP  Min: <  "   , 0, true , 0, 13, FMT_2_DIGITS_DIV10, 100, 400, 10, true ,15, true , " ", 15) \
   X(AAS1_PIP_MAX  , LINE3, "PIP  Max: >  "   , 0, true , 0, 13, FMT_2_DIGITS_DIV10, 100, 400, 10, true ,15, true , " ", 15) \
   X(AAS1_MORE     , LINE4, "MAS             ", 0, false, 0,  0, FMT_NONE          ,   0,   0,  0, false, 0, false, "" ,  0) \

/**
 * Second screen screen setup.
 * 
 * It will be used to initialize #HmiElementType
 * 
 * The input format is: X( [id], [line], [text], [text_x], 
 *                         [value_enable], [value_default], [value_x], [value_format], [value_min], [value_max], [value_step],
 *                         [cursor_enable], [cursor_x],
 *                         [unit_enable], [unit_text], [unit_x], [key])
 */
#define HMI_ADJUST_ALARM_SCREEN_2_CFG \
   X(AAS2_PIP_PEEP , LINE1, "PIP-PEEP: <  "   , 0, true , 0, 13, FMT_2_DIGITS_DIV10 ,40,200, 10, true ,15, true , " ", 15) \
   X(AAS2_TI_MAX   , LINE2, "TI ERROR:+/- "   , 0, true , 0, 13, FMT_2_DIGITS       , 5, 20,  5, true ,15, true , "%", 15) \
   X(AAS2_FR_MAX   , LINE3, "FR ERROR:+/- "   , 0, true , 0, 13, FMT_2_DIGITS       , 5, 20,  5, true ,15, true , "%", 15) \
   X(AAS2_MORE     , LINE4, "MAS             ", 0, false, 0,  0, FMT_NONE           , 0,  0,  0, false, 0, false, "" ,  0) \

/**
 * Second screen screen setup.
 * 
 * It will be used to initialize #HmiElementType
 * 
 * The input format is: X( [id], [line], [text], [text_x], 
 *                         [value_enable], [value_default], [value_x], [value_format], [value_min], [value_max], [value_step],
 *                         [cursor_enable], [cursor_x],
 *                         [unit_enable], [unit_text], [unit_x], [key])
 */
#define HMI_ADJUST_ALARM_SCREEN_3_CFG \
   X(AAS3_VT_MIN   , LINE1, "VT Min:  <  "    , 0, true , 300, 12, FMT_3_DIGITS, 200, 400, 10, true ,15, true , " ", 15) \
   X(AAS3_VT_MAX   , LINE2, "VT Max:  >  "    , 0, true , 600, 12, FMT_3_DIGITS, 450, 700, 10, true ,15, true , " ", 15) \
   X(AAS3_BACK     , LINE4, "VOLVER          ", 0, false,   0,  0, FMT_NONE    ,   0,   0,  0, false, 0, false, "" ,  0) \




/**
 * Alarm screen setup.
 * It is used to configure the alarm screen.
 * It will be used to initialize #HmiMessageType
 *
 * The input format is: X([id], [type], [text_line2], [text_line3], [has_value], [value_default], [value_x], [value_format])
 */
#define HMI_ALARM_CFG \
   X(AM_AIRFLOW_ERROR              , HMI_MSG_ALARM, "", "  DESCONEXION   ", "VERIF.TUBULADURA", false, LINE3, 0, FMT_NONE              )  \
   X(AM_BATTERY_LOW                , HMI_MSG_ALARM, "", "  BATERIA BAJA  ", "<5 MIN.OPERACION", false, LINE3, 0, FMT_NONE              )  \
   X(AM_BATTERY_MODE               , HMI_MSG_ALARM, "", "  MODO BATERIA  ", HMI_STR_EMPTY     , false, LINE3, 0, FMT_NONE              )  \
   X(AM_INTERNAL_FAILURE           , HMI_MSG_ALARM, "", "FALLO GENERAL XX", "CAMBIAR EQUIPO! ", true , LINE2,14, FMT_2_DIGITS          )  \
   X(AM_PIP_NOTREACHED             , HMI_MSG_ALARM, "", "PIP NO ALCANZADA", "    XX  cmH2O   ", true , LINE3, 4, FMT_2_DIGITS_DIV10_INV)  \
   X(AM_PIP_EXCEEDED               , HMI_MSG_ALARM, "", "  PIP EXCEDIDA  ", "    XX  cmH2O   ", true , LINE3, 4, FMT_2_DIGITS_DIV10_INV)  \
   X(AM_UNDER_PRESSURE             , HMI_MSG_ALARM, "", "   PEEP  BAJA   ", "    XX  cmH2O   ", true , LINE3, 4, FMT_2_DIGITS_DIV10_INV)  \
   X(AM_DRIVING_PRESSURE_LOW       , HMI_MSG_ALARM, "", " ERROR  PRESION ", "PIP - PEEP =  XX", true , LINE3,14, FMT_2_DIGITS_DIV10_INV)  \
   X(AM_TIDAL_VOLUME_LOW           , HMI_MSG_ALARM, "", "VOL. TIDAL BAJO ", "    XXX   ml    ", true , LINE3, 4, FMT_3_DIGITS          )  \
   X(AM_TIDAL_VOLUME_HIGH          , HMI_MSG_ALARM, "", "VOL. TIDAL ALTO ", "    XXX   ml    ", true , LINE3, 4, FMT_3_DIGITS          )  \
   X(AM_INSPIRATORY_TIME_UNREACHED , HMI_MSG_ALARM, "", "TI FUERA LIMITE ", "    XXXX  ms    ", true , LINE3, 4, FMT_4_DIGITS          )  \
   X(AM_RESP_FREQ_NOT_MET          , HMI_MSG_ALARM, "", "FR FUERA LIMITE ", "    XX   RPM    ", true , LINE3, 4, FMT_2_DIGITS          )  \
   X(AM_MAINTENANCE_REQUIRED       , HMI_MSG_ALARM, "", " MANTENIMIENTO  ", " VERIFICAR AMBU ", false, LINE3, 0, FMT_NONE              )  \
   X(AM_OVER_PRESSURE              , HMI_MSG_ALARM, "", "PRESION EXCEDIDA", "   > XX cmH2O   ", true , LINE3, 5, FMT_2_DIGITS_DIV10_INV)  \
   X(AM_BATTERY_VOLTAGE_LOW        , HMI_MSG_ALARM, "", "  BATERIA BAJA  ", HMI_STR_EMPTY     , false, LINE3, 0, FMT_NONE              )  \
   X(AM_DOOR_OPEN                  , HMI_MSG_ALARM, "", " PUERTA ABIERTA ", HMI_STR_EMPTY     , false, LINE3, 0, FMT_NONE              )  \

/**
 * Error screen setup.
 * It is used to configure the error screen.
 * It will be used to initialize #HmiMessageType
 * 
 * The input format is: X([id], [type], [text_line2], [text_line3], [has_value], [value_default], [value_x], [value_format])
 */
#define HMI_ERROR_CFG \
   X(ERROR_STOP_BEFORE_CHANGE_MODE , HMI_MSG_ERROR, "PARAR RESPIRADOR", "ANTES DE CAMBIAR", "MODO OPERATIVO  ", false, 0, 0, FMT_NONE)  \

//********************************************************************
// Enumerations and Structures and Typedefs
//********************************************************************

//********************************************************************
// Global Variable extern Declarations
//********************************************************************

//********************************************************************
// Function Prototypes
//********************************************************************

#endif // _HMI_CONF_H
//********************************************************************
//
// Modification Record
//
//********************************************************************
//
// 
//
//********************************************************************

