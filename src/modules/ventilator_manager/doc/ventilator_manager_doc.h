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

/**
 * @defgroup ventilator_manager Ventilator Manager
 * @brief Ventilator manager module documentation.
 *
 * The Ventilator Manager is responsible of controlling the mechanical
 * finger based on the breathing parameters
 *
 * @startuml
 *
 * [*] --> IDLE
 * IDLE: entry / SetMotorToHome()
 * IDLE: tick/
 * IDLE: exit/
 * IDLE --> INHALE : START_SIG
 *
 * INHALE: entry / StartMotorCompression(InspDist, InspSpeed)
 * INHALE: entry / ResetPressureMeasurement()
 * INHALE: tick / CheckForPhaseCompletion()
 * INHALE: exit/
 * INHALE --> IDLE : ABORT_SIG
 * INHALE --> INHALE : MOVE_CPLT_SIG / phaseCompleted = true\nSTOP_SIG / needToStop = true
 * INHALE --> PLATEAU : InspTime [phaseCompleted == true]/onPressureMeasurement(PIP)\nInspTimeTimeout [phaseCompleted == false] / onError(INSP_TIME_EXCEEDED)
 *
 * PLATEAU: entry / Brakemotor()
 * PLATEAU: entry / ResetPressureMeasurement()
 * PLATEAU: tick / CheckForPhaseCompletion
 * PLATEAU --> IDLE : ABORT_SIG
 * PLATEAU --> PAUSE : PlateauTime / onPressureMeasurement(PLT)
 * PLATEAU --> PLATEAU : STOP_SIG / needToStop = true
 *
 * PAUSE: entry / Brakemotor()
 * PAUSE: tick / CheckForPhaseCompletion
 * PAUSE --> EXHALE : PauseTime
 * PAUSE --> PAUSE : STOP_SIG / needToStop = true
 * PAUSE --> IDLE : ABORT_SIG
 *
 * EXHALE: entry / StartMotorRelease(HomeDist, ExpSpeed)
 * EXHALE: entry / ResetPressureMeasurement()
 * EXHALE: tick / CheckForPhaseCompletion()
 * EXHALE --> EXHALE : MOVE_CPLT_SIG / phaseCompleted = true\nSTOP_SIG / needToStop = true
 * EXHALE --> INHALE : ExpTime [needToStop == false && phaseCompleted == true] / onPressureMeasurement(PEEP)\nExpTimeout [phaseCompleted == false] / onError(EXP_TIME_EXCEEDED)
 * EXHALE --> IDLE : ABORT_SIG\nExpTime [needToStop == true]
 *
 * @enduml
 *
 * @{
 *
 * @defgroup ventilator_manager_conf Module Configuration
 * @brief Ventilator Manager module configuration parameters
 *
 * @defgroup ventilator_manager_api Module API Interface
 * @brief Ventilator Manager module API functions
 *
 * @defgroup ventilator_manager_callouts Module Callouts
 * @brief Ventialtor Manager callout functions
 *
 * @defgroup ventilator_manager_imp Module Implementation
 * @brief Ventialtor Manager implementation
 * @}
 */
