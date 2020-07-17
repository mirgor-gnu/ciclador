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

//********************************************************************
//
//   Title                : ringbuf.h
//
//   Description          : This is the header file for the ringbuf
//                          module.
//
//   Author               : Esteban Pupillo
//
//   Created              : 13 Aug 2011
//
//********************************************************************

#ifndef _RINGBUF_H_
#define _RINGBUF_H_ 1

//********************************************************************
//! @addtogroup RingBuffer_api
//!   @{
//********************************************************************

//********************************************************************
// Constant and Macro Definitions using #define
//********************************************************************

//********************************************************************
// Enumerations and Structures and Typedefs
//********************************************************************

/**
 * @brief Ring buffer structure
 * 
 */
typedef struct Ring_Buf_Tag
{
   uint16_t          size;          /**< Ring buffer size */
   volatile uint16_t write_index;   /**< Buffer write index */
   volatile uint16_t read_index;    /**< Buffer read index */
   uint8_t           *buffer;       /**< Pointer to buffer */
   void_bool_fptr    idx_callback;  /**< Index update callback */
} Ring_Buf_Type;

//********************************************************************
// Global Variable extern Declarations
//********************************************************************

//********************************************************************
// Function Prototypes
//********************************************************************
extern bool       RingBufFull(Ring_Buf_Type *ring_buf);
extern bool       RingBufEmpty(Ring_Buf_Type *ring_buf);
extern void       RingBufFlush(Ring_Buf_Type *ring_buf);
extern uint16_t   RingBufUsed(Ring_Buf_Type *ring_buf);
extern uint16_t   RingBufFree(Ring_Buf_Type *ring_buf);
extern uint16_t   RingBufContigUsed(Ring_Buf_Type *ring_buf);
extern uint16_t   RingBufContigFree(Ring_Buf_Type *ring_buf);
extern uint16_t   RingBufSize(Ring_Buf_Type *ring_buf);
extern uint8_t    RingBufReadOne(Ring_Buf_Type *ring_buf);
extern void       RingBufRead(Ring_Buf_Type *ring_buf, uint8_t *data, uint16_t length);
extern void       RingBufWriteOne(Ring_Buf_Type *ring_buf, uint8_t data);
extern void       RingBufWrite(Ring_Buf_Type *ring_buf, uint8_t *data, uint16_t length);
extern void       RingBufAdvanceWrite(Ring_Buf_Type *ring_buf, uint16_t num_bytes);
extern void       RingBufAdvanceRead(Ring_Buf_Type *ring_buf, uint16_t num_bytes);
extern void       RingBufInit(Ring_Buf_Type *ptRingBuf, uint8_t *pucBuf, void_bool_fptr idxCallback, uint16_t ulSize);


//********************************************************************
//
// Close the Doxygen group.
//! @}
//
//********************************************************************

#endif // _RINGBUF_H_
//********************************************************************
//
// Modification Record
//
//********************************************************************
//
// 
//
//********************************************************************

