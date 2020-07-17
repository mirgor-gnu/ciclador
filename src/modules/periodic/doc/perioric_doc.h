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
 * @defgroup periodic
 * @brief Periodic module documentation.
 *
 * The periodic module executes the main loop of the program.
 * It is an endless loop that has triggers at different timeouts 
 * to execute the update functions of the rest of the modules.
 * 
 * It works with a base time slot to start processing the triggers,
 * and each trigger timeout must be a multiple of this base time slot.
 *
 * @startuml
 *
 * @enduml
 *
 * @{
 *
 * @defgroup periodic_conf Module Configuration
 * @brief periodic module configuration parameters
 *
 * @defgroup periodic_api Module API Interface
 * @brief periodic module API functions
 *
 * @defgroup periodic_callouts Module Callouts
 * @brief periodic callout functions
 *
 * @defgroup periodic_imp Module Implementation
 * @brief periodic implementation
 * @}
 */
