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
//!   \file       ringbuf.c
//!
//!   \brief      This is the code file for ring buffer module.
//!
//!               This module is used for ring buffer managing.
//!               Was adapted from the Stellaris utility.
//!
//!   \author     Esteban Pupillo
//!
//!   \date       13 Aug 2011
//
//********************************************************************

//********************************************************************
// Include header files                                              
//********************************************************************
// include external headers here
#include "standard.h"

//********************************************************************
//! @addtogroup RingBuffer_imp
//!   @{
//********************************************************************

// include local module header here
#include "ringbuf.h"

//********************************************************************
// File level pragmas
//********************************************************************

//********************************************************************
// Constant and Macro Definitions using #define
//********************************************************************

//********************************************************************
// Enumerations and Structures and Typedefs
//********************************************************************

typedef enum idx_op_tag
{
   READ_IDX,
   WRITE_IDX,
} idx_op_type;

//********************************************************************
// Function Prototypes for Private Functions with File Level Scope
//********************************************************************

//********************************************************************
// ROM Const Variables With File Level Scope
//********************************************************************

//********************************************************************
// Static Variables and Const Variables With File Level Scope
//********************************************************************


//********************************************************************
// Function Definitions
//********************************************************************

//********************************************************************
//
//! Brief description
//!
//! \param param1 description
//!
//! Full description
//!
//! \return None.
//
//********************************************************************


//*****************************************************************************
//
// Change the value of a variable atomically.
//
// \param pulVal points to the index whose value is to be modified.
// \param ulDelta is the number of bytes to increment the index by.
// \param ulSize is the size of the buffer the index refers to.
//
// This function is used to increment a read or write buffer index that may be
// written in various different contexts. It ensures that the read/modify/write
// sequence is not interrupted and, hence, guards against corruption of the
// variable. The new value is adjusted for buffer wrap.
//
// \return None.
//
//*****************************************************************************
static void UpdateIndexAtomic(Ring_Buf_Type *ptRingBuf, uint16_t ulDelta, idx_op_type op)
{
   volatile uint16_t *idx;

   // Get the index pointer
   if (WRITE_IDX == op)
      idx = &ptRingBuf->write_index;
   else 
      idx = &ptRingBuf->read_index;

    // Callback handle
    if (NULL != *ptRingBuf->idx_callback)
    {
       (*ptRingBuf->idx_callback)(true);
    }

    // Update the variable value.
    *idx += ulDelta;

    // Correct for wrap. We use a loop here since we don't want to use a
    // modulus operation with interrupts off but we don't want to fail in
    // case ulDelta is greater than ulSize (which is extremely unlikely but...)
    while(*idx >= ptRingBuf->size)
    {
        *idx -= ptRingBuf->size;
    }

    // Restore the interrupt state
    if (NULL != *ptRingBuf->idx_callback)
    {
       (*ptRingBuf->idx_callback)(false);
    }

}

//*****************************************************************************
//
//! Determines whether the ring buffer whose pointers and size are provided
//! is full or not.
//!
//! \param ptRingBuf is the ring buffer object to empty.
//!
//! This function is used to determine whether or not a given ring buffer is
//! full.  The structure is specifically to ensure that we do not see
//! warnings from the compiler related to the order of volatile accesses
//! being undefined.
//!
//! \return Returns \b true if the buffer is full or \b false otherwise.
//
//*****************************************************************************
bool RingBufFull(Ring_Buf_Type *ptRingBuf)
{
    uint16_t ulWrite;
    uint16_t ulRead;

    //
    // Check the arguments.
    //
    assert_param(ptRingBuf != NULL);

    //
    // Copy the Read/Write indices for calculation.
    //
    ulWrite = ptRingBuf->write_index;
    ulRead = ptRingBuf->read_index;

    //
    // Return the full status of the buffer.
    //
    return((((ulWrite + 1) % ptRingBuf->size) == ulRead) ? true : false);
}

//*****************************************************************************
//
//! Determines whether the ring buffer whose pointers and size are provided
//! is empty or not.
//!
//! \param ptRingBuf is the ring buffer object to empty.
//!
//! This function is used to determine whether or not a given ring buffer is
//! empty.  The structure is specifically to ensure that we do not see
//! warnings from the compiler related to the order of volatile accesses
//! being undefined.
//!
//! \return Returns \b true if the buffer is empty or \b false otherwise.
//
//*****************************************************************************
bool RingBufEmpty(Ring_Buf_Type *ptRingBuf)
{
    uint16_t ulWrite;
    uint16_t ulRead;

    //
    // Check the arguments.
    //
    assert_param(ptRingBuf != NULL);

    //
    // Copy the Read/Write indices for calculation.
    //
    ulWrite = ptRingBuf->write_index;
    ulRead = ptRingBuf->read_index;

    //
    // Return the empty status of the buffer.
    //
    return((ulWrite == ulRead) ? true : false);
}

//*****************************************************************************
//
//! Empties the ring buffer.
//!
//! \param ptRingBuf is the ring buffer object to empty.
//!
//! Discards all data from the ring buffer.
//!
//! \return None.
//
//*****************************************************************************
void RingBufFlush(Ring_Buf_Type *ptRingBuf)
{
    // Check the arguments.
    assert_param(ptRingBuf != NULL);

    // Set the Read/Write pointers to be the same. Do this with interrupts
    // disabled to prevent the possibility of corruption of the read index.
    if (NULL != *ptRingBuf->idx_callback)
    {
       (*ptRingBuf->idx_callback)(true);
    }

    ptRingBuf->read_index = ptRingBuf->write_index;

    if (NULL != *ptRingBuf->idx_callback)
    {
       (*ptRingBuf->idx_callback)(false);
    }
}

//*****************************************************************************
//
//! Returns number of bytes stored in ring buffer.
//!
//! \param ptRingBuf is the ring buffer object to check.
//!
//! This function returns the number of bytes stored in the ring buffer.
//!
//! \return Returns the number of bytes stored in the ring buffer.
//
//*****************************************************************************
uint16_t RingBufUsed(Ring_Buf_Type *ptRingBuf)
{
    uint16_t ulWrite;
    uint16_t ulRead;

    //
    // Check the arguments.
    //
    assert_param(ptRingBuf != NULL);

    //
    // Copy the Read/Write indices for calculation.
    //
    ulWrite = ptRingBuf->write_index;
    ulRead = ptRingBuf->read_index;

    //
    // Return the number of bytes contained in the ring buffer.
    //
    return((ulWrite >= ulRead) ? (ulWrite - ulRead) :
           (ptRingBuf->size - (ulRead - ulWrite)));
}

//*****************************************************************************
//
//! Returns number of bytes available in a ring buffer.
//!
//! \param ptRingBuf is the ring buffer object to check.
//!
//! This function returns the number of bytes available in the ring buffer.
//!
//! \return Returns the number of bytes available in the ring buffer.
//
//*****************************************************************************
uint16_t RingBufFree(Ring_Buf_Type *ptRingBuf)
{
    //
    // Check the arguments.
    //
    assert_param(ptRingBuf != NULL);

    //
    // Return the number of bytes available in the ring buffer.
    //
    return((ptRingBuf->size - 1) - RingBufUsed(ptRingBuf));
}

//*****************************************************************************
//
//! Returns number of contiguous bytes of data stored in ring buffer ahead of
//! the current read pointer.
//!
//! \param ptRingBuf is the ring buffer object to check.
//!
//! This function returns the number of contiguous bytes of data available in
//! the ring buffer ahead of the current read pointer. This represents the
//! largest block of data which does not straddle the buffer wrap.
//!
//! \return Returns the number of contiguous bytes available.
//
//*****************************************************************************
uint16_t RingBufContigUsed(Ring_Buf_Type *ptRingBuf)
{
    uint16_t ulWrite;
    uint16_t ulRead;

    //
    // Check the arguments.
    //
    assert_param(ptRingBuf != NULL);

    //
    // Copy the Read/Write indices for calculation.
    //
    ulWrite = ptRingBuf->write_index;
    ulRead = ptRingBuf->read_index;

    //
    // Return the number of contiguous bytes available.
    //
    return((ulWrite >= ulRead) ? (ulWrite - ulRead) :
           (ptRingBuf->size - ulRead));
}

//*****************************************************************************
//
//! Returns number of contiguous free bytes available in a ring buffer.
//!
//! \param ptRingBuf is the ring buffer object to check.
//!
//! This function returns the number of contiguous free bytes ahead of the
//! current write pointer in the ring buffer.
//!
//! \return Returns the number of contiguous bytes available in the ring
//! buffer.
//
//*****************************************************************************
uint16_t RingBufContigFree(Ring_Buf_Type *ptRingBuf)
{
    uint16_t ulWrite;
    uint16_t ulRead;

    //
    // Check the arguments.
    //
    assert_param(ptRingBuf != NULL);

    //
    // Copy the Read/Write indices for calculation.
    //
    ulWrite = ptRingBuf->write_index;
    ulRead = ptRingBuf->read_index;

    //
    // Return the number of contiguous bytes available.
    //
    if(ulRead > ulWrite)
    {
        //
        // The read pointer is above the write pointer so the amount of free
        // space is the difference between the two indices minus 1 to account
        // for the buffer full condition (write index one behind read index).
        //
        return((ulRead - ulWrite) - 1);
    }
    else
    {
        //
        // If the write pointer is above the read pointer, the amount of free
        // space is the size of the buffer minus the write index. We need to
        // add a special-case adjustment if the read index is 0 since we need
        // to leave 1 byte empty to ensure we can tell the difference between
        // the buffer being full and empty.
        //
        return(ptRingBuf->size - ulWrite - ((ulRead == 0) ? 1 : 0));
    }
}

//*****************************************************************************
//
//! Return size in bytes of a ring buffer.
//!
//! \param ptRingBuf is the ring buffer object to check.
//!
//! This function returns the size of the ring buffer.
//!
//! \return Returns the size in bytes of the ring buffer.
//
//*****************************************************************************
uint16_t RingBufSize(Ring_Buf_Type *ptRingBuf)
{
    //
    // Check the arguments.
    //
    assert_param(ptRingBuf != NULL);

    //
    // Return the number of bytes available in the ring buffer.
    //
    return(ptRingBuf->size);
}

//*****************************************************************************
//
//! Reads a single byte of data from a ring buffer.
//!
//! \param ptRingBuf points to the ring buffer to be written to.
//!
//! This function reads a single byte of data from a ring buffer.
//!
//! \return The byte read from the ring buffer.
//
//*****************************************************************************
uint8_t RingBufReadOne(Ring_Buf_Type *ptRingBuf)
{
    uint8_t ucTemp;

    //
    // Check the arguments.
    //
    assert_param(ptRingBuf != NULL);

    //
    // Verify that space is available in the buffer.
    //
    assert_param(RingBufUsed(ptRingBuf) != 0);

    //
    // Write the data byte.
    //
    ucTemp = ptRingBuf->buffer[ptRingBuf->read_index];

    //
    // Increment the read index.
    //
    UpdateIndexAtomic(ptRingBuf, 1, READ_IDX);

    //
    // Return the character read.
    //
    return(ucTemp);
}

//*****************************************************************************
//
//! Reads data from a ring buffer.
//!
//! \param ptRingBuf points to the ring buffer to be read from.
//! \param pucData points to where the data should be stored.
//! \param ulLength is the number of bytes to be read.
//!
//! This function reads a sequence of bytes from a ring buffer.
//!
//! \return None.
//
//*****************************************************************************
void RingBufRead(Ring_Buf_Type *ptRingBuf, uint8_t *pucData, uint16_t ulLength)
{
    uint16_t ulTemp;

    //
    // Check the arguments.
    //
    assert_param(ptRingBuf != NULL);
    assert_param(pucData != NULL);
    assert_param(ulLength != 0);

    //
    // Verify that data is available in the buffer.
    //
    assert_param(ulLength <= RingBufUsed(ptRingBuf));

    //
    // Read the data from the ring buffer.
    //
    for(ulTemp = 0; ulTemp < ulLength; ulTemp++)
    {
        pucData[ulTemp] = RingBufReadOne(ptRingBuf);
    }
}

//*****************************************************************************
//
//! Remove bytes from the ring buffer by advancing the read index.
//!
//! \param ptRingBuf points to the ring buffer from which bytes are to be
//! removed.
//! \param ulNumBytes is the number of bytes to be removed from the buffer.
//!
//! This function advances the ring buffer read index by a given number of
//! bytes, removing that number of bytes of data from the buffer. If \e
//! ulNumBytes is larger than the number of bytes currently in the buffer, the
//! buffer is emptied.
//!
//! \return None.
//
//*****************************************************************************
void RingBufAdvanceRead(Ring_Buf_Type *ptRingBuf, uint16_t ulNumBytes)
{
    uint16_t ulCount;

    //
    // Check the arguments.
    //
    assert_param(ptRingBuf != NULL);

    //
    // Make sure that we are not being asked to remove more data than is
    // there to be removed.
    //
    ulCount = RingBufUsed(ptRingBuf);
    ulCount =  (ulCount < ulNumBytes) ? ulCount : ulNumBytes;

    //
    // Advance the buffer read index by the required number of bytes.
    //
    UpdateIndexAtomic(ptRingBuf, ulCount, READ_IDX);
}

//*****************************************************************************
//
//! Add bytes to the ring buffer by advancing the write index.
//!
//! \param ptRingBuf points to the ring buffer to which bytes have been added.
//! \param ulNumBytes is the number of bytes added to the buffer.
//!
//! This function should be used by clients who wish to add data to the buffer
//! directly rather than via calls to RingBufWrite() or RingBufWriteOne(). It
//! advances the write index by a given number of bytes.  If the \e ulNumBytes
//! parameter is larger than the amount of free space in the buffer, the
//! read pointer will be advanced to cater for the addition.  Note that this
//! will result in some of the oldest data in the buffer being discarded.
//!
//! \return None.
//
//*****************************************************************************
void RingBufAdvanceWrite(Ring_Buf_Type *ptRingBuf, uint16_t ulNumBytes)
{
    uint16_t ulCount;

    // Check the arguments.
    assert_param(ptRingBuf != NULL);

    // Make sure we were not asked to add a silly number of bytes.
    assert_param(ulNumBytes <= ptRingBuf->size);

    // Determine how much free space we currently think the buffer has.
    ulCount = RingBufFree(ptRingBuf);

    // Advance the buffer write index by the required number of bytes and
    // check that we have not run past the read index. Note that we must do
    // this within a critical section (interrupts disabled) to prevent
    // race conditions that could corrupt one or other of the indices.
    if (NULL != *ptRingBuf->idx_callback)
    {
       (*ptRingBuf->idx_callback)(true);
    }

    // Update the write pointer.
    ptRingBuf->write_index += ulNumBytes;

    // Check and correct for wrap.
    if(ptRingBuf->write_index >= ptRingBuf->size)
    {
        ptRingBuf->write_index -= ptRingBuf->size;
    }

    // Did the client add more bytes than the buffer had free space for?
    if(ulCount < ulNumBytes)
    {
        // Yes - we need to advance the read pointer to ahead of the write
        // pointer to discard some of the oldest data.
        ptRingBuf->read_index = ptRingBuf->write_index + 1;

        // Correct for buffer wrap if necessary.
        if(ptRingBuf->read_index >= ptRingBuf->size)
        {
            ptRingBuf->read_index -= ptRingBuf->size;
        }
    }

    // Restore interrupts if we turned them off earlier.
    if (NULL != *ptRingBuf->idx_callback)
    {
       (*ptRingBuf->idx_callback)(false);
    }
}

//*****************************************************************************
//
//! Writes a single byte of data to a ring buffer.
//!
//! \param ptRingBuf points to the ring buffer to be written to.
//! \param ucData is the byte to be written.
//!
//! This function writes a single byte of data into a ring buffer.
//!
//! \return None.
//
//*****************************************************************************
void RingBufWriteOne(Ring_Buf_Type *ptRingBuf, uint8_t ucData)
{
    //
    // Check the arguments.
    //
    assert_param(ptRingBuf != NULL);

    //
    // Verify that space is available in the buffer.
    //
    assert_param(RingBufFree(ptRingBuf) != 0);

    //
    // Write the data byte.
    //
    ptRingBuf->buffer[ptRingBuf->write_index] = ucData;

    //
    // Increment the write index.
    //
    UpdateIndexAtomic(ptRingBuf, 1, WRITE_IDX);
}

//*****************************************************************************
//
//! Writes data to a ring buffer.
//!
//! \param ptRingBuf points to the ring buffer to be written to.
//! \param pucData points to the data to be written.
//! \param ulLength is the number of bytes to be written.
//!
//! This function write a sequence of bytes into a ring buffer.
//!
//! \return None.
//
//*****************************************************************************
void RingBufWrite(Ring_Buf_Type *ptRingBuf, uint8_t *pucData, uint16_t ulLength)
{
    uint16_t ulTemp;

    //
    // Check the arguments.
    //
    assert_param(ptRingBuf != NULL);
    assert_param(pucData != NULL);
    assert_param(ulLength != 0);

    //
    // Verify that space is available in the buffer.
    //
    assert_param(ulLength <= RingBufFree(ptRingBuf));

    //
    // Write the data into the ring buffer.
    //
    for(ulTemp = 0; ulTemp < ulLength; ulTemp++)
    {
        RingBufWriteOne(ptRingBuf, pucData[ulTemp]);
    }
}

//*****************************************************************************
//
//! Initialize a ring buffer object.
//!
//! \param ptRingBuf points to the ring buffer to be initialized.
//! \param pucBuf points to the data buffer to be used for the ring buffer.
//! \param ulSize is the size of the buffer in bytes.
//!
//! This function initializes a ring buffer object, preparing it to store data.
//!
//! \return None.
//
//*****************************************************************************
void RingBufInit(Ring_Buf_Type *ptRingBuf, uint8_t *pucBuf, void_bool_fptr idxCallback, uint16_t ulSize)
{
    //
    // Check the arguments.
    //
    assert_param(ptRingBuf != NULL);
    assert_param(pucBuf != NULL);
    assert_param(ulSize != 0);

    //
    // Initialize the ring buffer object.
    //
    ptRingBuf->size = ulSize;
    ptRingBuf->buffer = pucBuf;
    ptRingBuf->idx_callback = idxCallback;
    ptRingBuf->write_index = ptRingBuf->read_index = 0;
}

//********************************************************************
//
// Close the Doxygen group.
//! @}
//
//********************************************************************

//********************************************************************
//
// Modification Record
//
//********************************************************************
//
// 
//
//********************************************************************

