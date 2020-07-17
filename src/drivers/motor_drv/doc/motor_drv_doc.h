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
 * @defgroup motor_drv Motor driver
 * @brief Motor driver module documentation.
 *
 * The Motor module has two different blocks:
 * 
 * 1. The proper motor driver which controls the hardware by the PWM
 * and manages the PID.
 * 2. The motor Finite State Machine (FSM) which keep tracks on what the 
 * motor driver actions should be depending on the desired state.
 * 
 * As a whole the motor module presents to the upper layers an interface to
 * simplify the control of the motor with easy commands.
 * 
 * @startuml
 *
 * @enduml
 *
 * @{
 *
 * @defgroup motor_drv_conf Module Configuration
 * @brief motor_drv module configuration parameters
 *
 * @defgroup motor_drv_api Module API Interface
 * @brief motor_drv module API functions
 *
 * @defgroup motor_drv_callouts Module Callouts
 * @brief motor_drv module callout functions
 *
 * @defgroup motor_drv_imp Module Implementation
 * @brief motor_drv module implementation
 * @}
 */
