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

// Begin header file, mavg4.h

#ifndef MAVG4_H_ // Include guards
#define MAVG4_H_

#define ARM_MATH_CM3  // Use ARM Cortex M3
#include <arm_math.h>    // Include CMSIS header

// Link with library: libarm_cortexM3l_math.a (or equivalent)
// Add CMSIS/Lib/GCC to the library search path
// Add CMSIS/Include to the include search path
extern q15_t mavg4_coefficients[4];
static const int mavg4_numTaps = 4;
static const int mavg4_blockSize = 1;

typedef struct
{
   arm_fir_instance_q15 instance;
   q15_t state[5];
   q15_t output;
} mavg4Type;


mavg4Type *mavg4_create( void );
void mavg4_destroy( mavg4Type *pObject );
 void mavg4_init( mavg4Type * pThis );
 void mavg4_reset( mavg4Type * pThis );
#define mavg4_writeInput( pThis, input )  \
   arm_fir_fast_q15( &pThis->instance, &input, &pThis->output, 1 );

#define mavg4_readOutput( pThis )  \
   pThis->output

 int mavg4_filterBlock( mavg4Type * pThis, short * pInput, short * pOutput, unsigned int count );
#define mavg4_outputToFloat( output )  \
   (( (1.0f/32768) * (output) ))

#define mavg4_inputFromFloat( input )  \
   ((short)(32768f * (input)))

#endif // MAVG4_H_

