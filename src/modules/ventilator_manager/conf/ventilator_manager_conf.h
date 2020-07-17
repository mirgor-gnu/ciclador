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
//!   @file                 ventilator_manager_conf.h
//!
//!   @brief                ventilator manager configuration header file
//!
//!   @author               Esteban G. Pupillo
//!
//!   @date                 29 Apr 2020
//!
//********************************************************************

#ifndef  _VENTILATOR_MANAGER_CONF_H
#define  _VENTILATOR_MANAGER_CONF_H 1

//********************************************************************
//! @addtogroup ventilator_manager_conf
//!   @{
//********************************************************************

//********************************************************************
// Constant and Macro Definitions using #define
//********************************************************************

/**
 * Minimum Respiratory rate allowed expressed in BPM.
 */
#define VENTILATOR_MGR_BPM_MIN              (10)

/**
 * Maximum Respiratory rate allowed expressed in BPM.
 */
#define VENTILATOR_MGR_BPM_MAX              (30)

/**
 * Minium Tidal volume allowed expressed in milliliters.
 */
#define VENTILATOR_MGR_TIDAL_VOLUME_MIN     (200)  //ml

/**
 * Maximum Tidal volume allowed expressed in milliliters.
 */
#define VENTILATOR_MGR_TIDAL_VOLUME_MAX     (650)  //ml

/**
 * Minimum inspiratory pressure expressed in mmH2O.
 * This is applicable when using pressure-control
 */
#define VENTILATOR_MGR_INSP_PRESSURE_MIN    (100) //mmH2O

/**
 * Maximum inspiratory pressure expressed in mmH2O.
 * This is applicable when using pressure-control
 */
#define VENTILATOR_MGR_INSP_PRESSURE_MAX    (600) //mmH2O

/**
 * Minimum allowed IE ratio expressed as @p 1:(IERatio/100) @p .
 * For example, a value of 100 means a IE ratio 1:1
 */
#define VENTILATOR_MGR_IE_RATIO_MIN         (100) // 1:1

/**
 * Maximum allowed IE ratio expressed as @p 1:(IERatio/100) @p .
 * For example, a value of 100 means a IE ratio 1:1
 */
#define VENTILATOR_MGR_IE_RATIO_MAX         (400) // 1:4

/**
 * Minimum allowed inspiratory time expressed in milliseconds
 */
#define VENTILATOR_MGR_INSP_TIME_MIN_MILLIS (700)

/**
 * Maximum allowed inspiratory time expressed in milliseconds
 */
#define VENTILATOR_MGR_INSP_TIME_MAX_MILLIS (1800)

/**
 * Minimum allowed Plateau time expressed in milliseconds.
 * The Plateau time is defined as the wait time after
 * the end of the inspiratory time required for the
 * airflow pressure to stabilize
 */
#define VENTILATOR_MGR_PLT_TIME_MIN_MILLIS  (150)

/**
 * Maximum allowed Plateau time expressed in milliseconds.
 * The Plateau time is defined as the wait time after
 * the end of the inspiratory time required for the
 * airflow pressure to stabilize
 */
#define VENTILATOR_MGR_PLT_TIME_MAX_MILLIS  (450)

/**
 * Initial Respiratory rate expressed in BPM.
 * This value is used when the system powered up
 */
#define VENTILATOR_MGR_DEFAULT_BPM          (15)

/**
 * Initial Tidal Volume expressed in milliliters.
 * This value is used when the system powered up
 */
#define VENTILATOR_MGR_DEFAULT_TVOLUME      (300)

/**
 * Default Plateau measurement time expressed in milliseconds
 * This value define when the pressure is sampled when looking
 * for the Plateau pressure.
 */
#define VENTILATOR_MGR_PLATEAU_MEAS_TIME    (150)

/**
 * Initial hold time expressed in milliseconds.
 * This value is used when the system powered up
 */
#define VENTILATOR_MGR_HOLD_TIME_MILLIS     (300)

/**
 * Initial IE ratio.
 * This value is used when the system powered up
 */
#define VENTILATOR_MGR_DEFAULT_IE_RATIO     (300)


/**
 * Maximum allowed pressure at the airflow circuit expressed in mmH2O.
 * If the measured pressure is above this limit an error
 * will be generated
 */
#define VENTILATOR_MGR_MAX_PIP_THRESHOLD_MAX          (400) //mmH2O
#define VENTILATOR_MGR_MAX_PIP_THRESHOLD_MIN          (100) //mmH2O

#define VENTILATOR_MGR_MIN_PIP_THRESHOLD_MAX          (400) //mmH2O
#define VENTILATOR_MGR_MIN_PIP_THRESHOLD_MIN          (100) //mmH2O

/**
 * Minimum allowed pressure at the airflow circuit expressed in mmH2O.
 * If the measured pressure is below this limit an error
 * will be generated
 */
#define VENTILATOR_MGR_MIN_PEEP_THRESHOLD_MAX         (200) //mmH2O
#define VENTILATOR_MGR_MIN_PEEP_THRESHOLD_MIN         (10)  //mmH2O
#define VENTILATOR_MGR_MIN_PEEP_DEFAULT		          (100) //mmH2O


#define VENTILATOR_MGR_MAX_TIERROR_THRESHOLD_MAX      (20) //%
#define VENTILATOR_MGR_MAX_TIERROR_THRESHOLD_MIN      (5) //%

/**
 * Maximum allowed error on the respiratory rate.
 * The error is calculated as the difference between the
 * setpoint and the real rate expressed as percentage
 */
#define VENTILATOR_MGR_MAX_BPMERROR_THRESHOLD_MAX     (20)  //%
#define VENTILATOR_MGR_MAX_BPMERROR_THRESHOLD_MIN     (5)  //%

/**
 * Pressure threshold used to detect an airflow disconnection
 * In order to generate the airflow disconnection error the PIP, PEEP and Pplt
 * need to be within this value.
 */
#define VENTILATOR_MGR_MIN_PIP_VS_PEEP_THRESHOLD_MAX  (200) //mmH2O
#define VENTILATOR_MGR_MIN_PIP_VS_PEEP_THRESHOLD_MIN  (10)  //mmH2O

/**
 * Maximum Tidal volume that can be delivered to the patient
 * If this value is exceeded an error condition will be raised
 */
#define VENTILATOR_MGR_MAX_TIDAL_VOL_THRESHOLD_MAX    (700)  //ml

/**
 * Maximum Tidal volume that can be delivered to the patient
 * If this value is exceeded an error condition will be raised
 */
#define VENTILATOR_MGR_MAX_TIDAL_VOL_THRESHOLD_MIN    (450)  //ml

/**
 * Minimum Tidal volume that can be delivered to the patient
 * If this value is not reached an error condition will be raised
 */
#define VENTILATOR_MGR_MIN_TIDAL_VOL_THRESHOLD_MAX    (400)  //ml

/**
 * Minimum Tidal volume that can be delivered to the patient
 * If this value is not reached an error condition will be raised
 */
#define VENTILATOR_MGR_MIN_TIDAL_VOL_THRESHOLD_MIN    (200)  //ml

#define VENTILATOR_MGR_AIRFLOWDISCONN_THRESHOLD       (40) //cmH2O

#define VENTILATOR_MGR_MAINTENANCE_PERIOD_MILLIS      (14400000) //ms
/**
 * Numbers of used pressure triggers.
 * Triggers are used to monitor the airflow pressure
 * during the breath cycle
 */
#define VENTILATOR_MGR_PRESSURE_MAX_TRIGGERS (2)

/**
 * AMBU bag release (uncompress) speed expressed in deg/s
 */
#define VENTILATOR_MGR_RELEASE_SPEED        (40)  //deg/s

/**
 * Ventilator door sensor input pin.
 * This pin is used to monitor the state of the ventilator door.
 * If it's open an error will be generated
 */
#define VENTILATOR_MGR_DOOR_INPUT_PIN       (IO_DOOR_SENSOR)



//********************************************************************
// Enumerations and Structures and Typedefs
//********************************************************************
/**
 * Tidal Volume to finger angle conversion table.
 * The volume is expressed in milliliters and the angle is expressed
 * in Degrees
 *
 * This table will be used to interpolate the required tidal volume
 * to finger degrees
 *
 * The input format is: X([vol_in_ml], [angle_in_deg])
 */
#define VENTILATOR_MGR_VOL_DEG_TABLE \
      X(225 , 23) \
      X(290 , 24) \
      X(309 , 26) \
      X(357 , 28) \
      X(403 , 30) \
      X(449 , 32) \
      X(500 , 34) \
      X(543 , 36) \
      X(599 , 38) \
      X(630 , 40) \
      X(665 , 42)

//********************************************************************
// Global Variable extern Declarations
//********************************************************************

//********************************************************************
// Function Prototypes
//********************************************************************

//********************************************************************
//
// Close the Doxygen group.
//! @}
//
//********************************************************************

#endif // _VENTILATOR_MANAGER_CONF_H
//********************************************************************
//
// Modification Record
//
//********************************************************************
//
// 
//
//********************************************************************

