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

#include "lpf_butter_10hz_float.h"

#include <stdlib.h> // For malloc/free
#include <string.h> // For memset

float32_t lpf_butter_10hz_float_coefficients[10] =
{
// Scaled for floating point

    0.0009200498139105926, 0.0018400996278211852, 0.0009200498139105926, 1.8866095826215064, -0.8903397362840242,// b0, b1, b2, a1, a2
    0.0009765625, 0.001953125, 0.0009765625, 1.9492159580258417, -0.9530698953278909// b0, b1, b2, a1, a2

};


lpf_butter_10hz_floatType *lpf_butter_10hz_float_create( void )
{
   lpf_butter_10hz_floatType *result = (lpf_butter_10hz_floatType *)malloc( sizeof( lpf_butter_10hz_floatType ) );   // Allocate memory for the object
   lpf_butter_10hz_float_init( result );                                // Initialize it
   return result;                                              // Return the result
}

void lpf_butter_10hz_float_destroy( lpf_butter_10hz_floatType *pObject )
{
   free( pObject );
}

 void lpf_butter_10hz_float_init( lpf_butter_10hz_floatType * pThis )
{
   arm_biquad_cascade_df1_init_f32( &pThis->instance, lpf_butter_10hz_float_numStages, lpf_butter_10hz_float_coefficients, pThis->state );
   lpf_butter_10hz_float_reset( pThis );

}

 void lpf_butter_10hz_float_reset( lpf_butter_10hz_floatType * pThis )
{
   memset( &pThis->state, 0, sizeof( pThis->state ) ); // Reset state to 0
   pThis->output = 0;                           // Reset output

}

 int lpf_butter_10hz_float_filterBlock( lpf_butter_10hz_floatType * pThis, float * pInput, float * pOutput, unsigned int count )
{
   arm_biquad_cascade_df1_f32( &pThis->instance, pInput, pOutput, count );
   return count;

}


