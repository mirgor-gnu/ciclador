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

#include "lpf.h"

#include <stdlib.h> // For malloc/free
#include <string.h> // For memset

q15_t lpf_coefficients[12] =
{
// Scaled for a 16x16:32 Direct form 1 IIR filter with:
// Feedback shift = 14
// Output shift = 14
// Input  has a maximum value of 1, scaled by 2^15
// Output has a maximum value of 1.2493048565317384, scaled by 2^14

    632, 0, 1265, 632, 17180, -4852,// b0 Q13(0.0772), 0, b1 Q13(0.154), b2 Q13(0.0772), a1 Q14(1.05), a2 Q14(-0.296)
    1024, 0, 2048, 1024, 21642, -10367// b0 Q14(0.0625), 0, b1 Q14(0.125), b2 Q14(0.0625), a1 Q14(1.32), a2 Q14(-0.633)

};
//q15_t lpf_coefficients[12] =
//{
//// Scaled for a 16x16:32 Direct form 1 IIR filter with:
//// Feedback shift = 14
//// Output shift = 14
//// Input  has a maximum value of 1, scaled by 2^15
//// Output has a maximum value of 1.402162957567006, scaled by 2^14
//
//    218, 0, 437, 218, 24243, -9107,// b0 Q13(0.0267), 0, b1 Q13(0.0533), b2 Q13(0.0267), a1 Q14(1.48), a2 Q14(-0.556)
//    256, 0, 512, 256, 27869, -12919// b0 Q14(0.0156), 0, b1 Q14(0.0313), b2 Q14(0.0156), a1 Q14(1.70), a2 Q14(-0.788)
//
//};


lpfType *lpf_create( void )
{
   lpfType *result = (lpfType *)malloc( sizeof( lpfType ) );   // Allocate memory for the object
   lpf_init( result );                                // Initialize it
   return result;                                              // Return the result
}

void lpf_destroy( lpfType *pObject )
{
   free( pObject );
}

 void lpf_init( lpfType * pThis )
{
   arm_biquad_cascade_df1_init_q15( &pThis->instance, lpf_numStages, lpf_coefficients, pThis->state, lpf_postShift );
   lpf_reset( pThis );

}

 void lpf_reset( lpfType * pThis )
{
   memset( &pThis->state, 0, sizeof( pThis->state ) ); // Reset state to 0
   pThis->output = 0;                           // Reset output

}

 int lpf_filterBlock( lpfType * pThis, short * pInput, short * pOutput, unsigned int count )
{
   arm_biquad_cascade_df1_fast_q15( &pThis->instance, pInput, pOutput, count );
   return count;

}


