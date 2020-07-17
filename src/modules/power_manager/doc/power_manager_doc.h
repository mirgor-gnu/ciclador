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
 * @defgroup power_manager
 * @brief Power Manager module documentation.
 *
 * The power manager module monitor the status of the system 
 * power source. 
 * The voltage is not measured directly since the 
 * power source has a dedicated pin to report if it is using 
 * the main power line or the backup battery. The system just
 * reads this pin with a GPIO and changes its own status
 * accordingly
 * 
 * @startuml
 *
 * @enduml
 *
 * @{
 *
 * @defgroup power_manager_conf Module Configuration
 * @brief Power Manager module configuration parameters
 *
 * @defgroup power_manager_api Module API Interface
 * @brief Power Manager module API functions
 *
 * @defgroup power_manager_callouts Module Callouts
 * @brief Power Manager callout functions
 *
 * @defgroup power_manager_imp Module Implementation
 * @brief Power Manager implementation
 * @}
 */
