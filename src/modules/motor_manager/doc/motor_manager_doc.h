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
 * @defgroup motor_manager Motor manager 
 * @brief Motor manager module documentation.
 *
 * The motor manager is a high level Finite State Machine (FSM)
 * of the motor behavior.
 * It will receive the commands to go to one direction, the other or 
 * to stop and it will translate them to the lower levels whom in turn
 * will control the hardware.
 * 
 * @startuml
 *
 * @enduml
 *
 * @{
 *
 * @defgroup motor_manager_conf Module Configuration
 * @brief motor_manager module configuration parameters
 *
 * @defgroup motor_manager_api Module API Interface
 * @brief motor_manager module API functions
 *
 * @defgroup motor_manager_callouts Module Callouts
 * @brief motor_manager callout functions
 *
 * @defgroup motor_manager_imp Module Implementation
 * @brief motor_manager implementation
 * @}
 */
