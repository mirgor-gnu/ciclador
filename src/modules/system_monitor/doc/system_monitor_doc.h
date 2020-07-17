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
 * @defgroup system_monitor System Monitor
 * @brief System Monitor module documentation.
 *
 * The System Monitor is responsible for doing the overall system health
 * evaluation.
 * Current implementation provides two major functionalities:
 * - Stack usage
 * - CPU usage statistics
 *
 * # Usage
 * In order to use this module you have to know in advance the memory region of
 * the stack and currently there is only support for the system stack (no
 * process stack). This means that this module is targeted for bare-metal
 * implementations.
 *
 * First configure the module defining these parameters:
 * - #SYSTEM_MONITOR_BASE_STACK_ADDRESS
 * - #SYSTEM_MONITOR_STACK_REGION_SIZE
 * - #SYSTEM_MONITOR_MIN_FREE_STACK_SIZE
 * - #SYSTEM_MONITOR_CPU_USAGE_REPORT_PERIOD
 *
 * Call the function #SystemMonitor_Init as soon as possible in your software
 * entry point. Since this module monitors the stack usage is desirable that you
 * call to the initialization function before calling any other function.
 *
 * After initialization, the function #SystemMonitor_Update should be called at
 * fixed time intervals. The time interval should be shorter than the
 * #SYSTEM_MONITOR_CPU_USAGE_REPORT_PERIOD value.
 *
 * The module will update the CPU usage statistics based difference between the
 * system wall clock time and the user busy time. In order to compute the user
 * busy time, the functions #SystemMonitor_StartUserTime and
 * #SystemMonitor_StopUserTime are used.
 *
 * Therefore, the platform shall decide what is going to computed as user time
 * and is responsible of calling to the above mentioned functions
 *
 * # Callouts
 * This module needs the user to implement the function
 * #SystemMonitor_GetHighResTimestamp.
 *
 * The module reports the corresponding information by calling to the functions:
 * - #SystemMonitor_OnFreeStackSizeLow
 * - #SystemMonitor_OnCPUUsageReport
 *
 * The user shall implement those functions to get the information out of this
 * module
 *
 * @{
 *
 * @defgroup system_monitor_conf Module Configuration
 * @brief System Monitor module configuration parameters
 *
 * @defgroup system_monitor_api Module API Interface
 * @brief System Monitor module API functions
 *
 * @defgroup system_monitor_callouts Module Callouts
 * @brief System Monitor callout functions
 *
 * @defgroup system_monitor_imp Module Implementation
 * @brief System Monitor implementation
 * @}
 */
