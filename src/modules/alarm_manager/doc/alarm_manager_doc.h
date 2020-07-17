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
 * @defgroup alarm_manager Alarm Manager
 * @brief Alarm Manager module documentation.
 *
 * This module is responsible of handling the system alarms based on their
 * status and priority.
 * 
 * # Alarm states
 * 
 * ### #AM_STATUS_OFF
 * The alarm is disable until the error situation is detected and informed to
 * the Alarm Manager.
 * 
 * ### #AM_STATUS_LATCHED
 * This is the state of the alarm that it is using all resources (display, buzzer,
 * led, panel light) to alert the user. Only one alarm could be latched.
 * 
 * There are 3 ways to leave this status:
 * - Reported as solved. It will change to #AM_STATUS_OFF.
 * - Silenced. It will change to #AM_STATUS_SILENCED.
 * - When a more priority alarm is reported. It will queue and change 
 * to #AM_STATUS_QUEUED.
 * 
 * ### #AM_STATUS_QUEUED
 * All the active alarms that could not be set as latched shall remain in this
 * status.
 * 
 * There are 2 ways to be queued:
 * - Being *latched* when another more priority alarm is reported. The new alarm
 * will be latched and the previous one marked as queued.
 * - When the alarm is reported there is a previous alarm latched that has the
 * same or a highest priority level. The new one will be queued.
 * 
 * There are 2 ways to leave this status:
 * - If the alarm is reported as solved, it will be marked as #AM_STATUS_OFF and
 * taken of this list.
 * - If the latched alarm is solved or silenced by the user, the most priority
 * queued alarm will take this place.
 * 
 * ### #AM_STATUS_SILENCED
 * All alarms that has been silenced by the user. It still have control of the
 * led and panel light to alert that the situation persist.
 * The alarm only could change to #AM_STATUS_OFF when the problem dissapear.
 *
 * @{
 *
 * @defgroup alarm_manager_conf Module Configuration
 * @brief Alarm Manager module configuration parameters
 *
 * @defgroup alarm_manager_api Module API Interface
 * @brief Alarm Manager module API functions
 *
 * @defgroup alarm_manager_callouts Module Callouts
 * @brief Alarm Manager callout functions
 *
 * @defgroup alarm_manager_imp Module Implementation
 * @brief Alarm Manager implementation
 * @}
 */
