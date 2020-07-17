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

#include "mavg4.h"

#include <stdlib.h> // For malloc/free
#include <string.h> // For memset

q15_t mavg4_coefficients[4] =
{
   8192, 8192, 8192, 8192
};


mavg4Type *mavg4_create( void )
{
   mavg4Type *result = (mavg4Type *)malloc( sizeof( mavg4Type ) );   // Allocate memory for the object
   mavg4_init( result );                                 // Initialize it
   return result;                                              // Return the result
}

void mavg4_destroy( mavg4Type *pObject )
{
   free( pObject );
}

 void mavg4_init( mavg4Type * pThis )
{
   arm_fir_init_q15( &pThis->instance, mavg4_numTaps, mavg4_coefficients, pThis->state, mavg4_blockSize );
   mavg4_reset( pThis );

}

 void mavg4_reset( mavg4Type * pThis )
{
   memset( &pThis->state, 0, sizeof( pThis->state ) ); // Reset state to 0
   pThis->output = 0;                           // Reset output

}

 int mavg4_filterBlock( mavg4Type * pThis, short * pInput, short * pOutput, unsigned int count )
{
   arm_fir_fast_q15( &pThis->instance, (q15_t *)pInput, (q15_t *)pOutput, count );
   return count;

}


