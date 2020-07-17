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
 * @defgroup adc_drv ADC driver
 * @brief ADC driver module documentation.
 *
 * The ADC driver interfaces with the ADC hardware. 
 * It adds the possibility add further processing to the raw data 
 * such as filtering and scaling. It also lets the upper layers set 
 * triggers by comparing the converted value to some threshold and 
 * calling a function callout.
 * 
 * @startuml
 *
 * @enduml
 *
 * @{
 *
 * @defgroup adc_drv_conf Module Configuration
 * @brief adc_drv module configuration parameters
 *
 * @defgroup adc_drv_api Module API Interface
 * @brief adc_drv module API functions
 *
 * @defgroup adc_drv_callouts Module Callouts
 * @brief adc_drv callout functions
 *
 * @defgroup adc_drv_imp Module Implementation
 * @brief adc_drv implementation
 * @}
 */
