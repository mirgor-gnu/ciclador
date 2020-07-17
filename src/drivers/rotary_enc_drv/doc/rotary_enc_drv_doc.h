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
 * @defgroup rotary_enc_drv Encoder driver
 * @brief Rotary encoder driver module documentation.
 *
 * The Rotary encoder module counts the steps travelled 
 * by the system by a GPIO input used as interrupt.
 * The module keeps track of the steps and time elapsed
 * between those steps.
 * To the upper layers it provides the position and speed
 * of the movement.
 * It is possible to set a point as the 0 and start 
 * counting from there on.
 *
 *
 * @startuml
 *
 * @enduml
 *
 * @{
 *
 * @defgroup rotary_enc_drv_conf Module Configuration
 * @brief rotary_enc_drv module configuration parameters
 *
 * @defgroup rotary_enc_drv_api Module API Interface
 * @brief rotary_enc_drv module API functions
 *
 * @defgroup rotary_enc_drv_callouts Module Callouts
 * @brief rotary_enc_drv callout functions
 *
 * @defgroup rotary_enc_drv_imp Module Implementation
 * @brief rotary_enc_drv implementation
 * @}
 */
