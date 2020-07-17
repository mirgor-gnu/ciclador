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

// Begin header file, lpf.h

#ifndef LPF_H_ // Include guards
#define LPF_H_

#define ARM_MATH_CM3  // Use ARM Cortex M3
#include <arm_math.h>    // Include CMSIS header

// Link with library: libarm_cortexM3l_math.a (or equivalent)
// Add CMSIS/Lib/GCC to the library search path
// Add CMSIS/Include to the include search path
extern q15_t lpf_coefficients[12];
static const int lpf_numStages = 2;
static const int lpf_postShift = 1;

typedef struct
{
   arm_biquad_casd_df1_inst_q15 instance;
   q15_t state[8];
   q15_t output;
} lpfType;


lpfType *lpf_create( void );
void lpf_destroy( lpfType *pObject );
void lpf_init( lpfType * pThis );
void lpf_reset( lpfType * pThis );
#define lpf_writeInput( pThis, input )  \
      arm_biquad_cascade_df1_fast_q15( &pThis->instance, &input, &pThis->output, 1 );

#define lpf_readOutput( pThis )  \
      pThis->output


int lpf_filterBlock( lpfType * pThis, short * pInput, short * pOutput, unsigned int count );
#define lpf_outputToFloat( output )  \
      (( (1.0f/16384) * (output) ))

#define lpf_inputFromFloat( input )  \
      ((short)(32768f * (input)))

#endif // LPF_H_

