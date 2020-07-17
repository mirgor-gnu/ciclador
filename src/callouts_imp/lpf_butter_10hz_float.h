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

/******************************* SOURCE LICENSE *********************************
Copyright (c) 2019 MicroModeler.

A non-exclusive, nontransferable, perpetual, royalty-free license is granted to the Licensee to
use the following Information for academic, non-profit, or government-sponsored research purposes.
Use of the following Information under this License is restricted to NON-COMMERCIAL PURPOSES ONLY.
Commercial use of the following Information requires a separately executed written license agreement.

This Information is distributed WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

******************************* END OF LICENSE *********************************/

// A commercial license for MicroModeler DSP can be obtained at http://www.micromodeler.com/launch.jsp

// Begin header file, lpf_butter_10hz_float.h

#ifndef LPF_BUTTER_10HZ_FLOAT_H_ // Include guards
#define LPF_BUTTER_10HZ_FLOAT_H_

#define ARM_MATH_CM3  // Use ARM Cortex M3
#include <arm_math.h>    // Include CMSIS header

// Link with library: libarm_cortexM3l_math.a (or equivalent)
// Add CMSIS/Lib/GCC to the library search path
// Add CMSIS/Include to the include search path
extern float32_t lpf_butter_10hz_float_coefficients[10];
static const int lpf_butter_10hz_float_numStages = 2;

typedef struct
{
   arm_biquad_casd_df1_inst_f32 instance;
   float32_t state[8];
   float32_t output;
} lpf_butter_10hz_floatType;


lpf_butter_10hz_floatType *lpf_butter_10hz_float_create( void );
void lpf_butter_10hz_float_destroy( lpf_butter_10hz_floatType *pObject );
 void lpf_butter_10hz_float_init( lpf_butter_10hz_floatType * pThis );
 void lpf_butter_10hz_float_reset( lpf_butter_10hz_floatType * pThis );
#define lpf_butter_10hz_float_writeInput( pThis, input )  \
   arm_biquad_cascade_df1_f32( &pThis->instance, &input, &pThis->output, 1 );

#define lpf_butter_10hz_float_readOutput( pThis )  \
   pThis->output


 int lpf_butter_10hz_float_filterBlock( lpf_butter_10hz_floatType * pThis, float * pInput, float * pOutput, unsigned int count );
#define lpf_butter_10hz_float_outputToFloat( output )  \
   (output)

#define lpf_butter_10hz_float_inputFromFloat( input )  \
   (input)

#endif // LPF_BUTTER_10HZ_FLOAT_H_

