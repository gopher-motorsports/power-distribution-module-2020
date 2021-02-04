//#ifndef RINGBUFFER_H
//#define RINGBUFFER_H
//
//#include "baseTypes.h"
//#include "ringBuffer.h"
//
////******* ringBufferAdd *******************************************************
////
//// This function is used to add a new value to the ring buffer. It first checks
//// the semaphore to see if the buffer is in use anywhere. If it not, it adds
//// sets the index at head to the new value, then increments head. Lastly, it
//// sets the average state to DIRTY and sets the semaphore to UNLOCKED
////
//// Inputs:
//// Returns: A boolean indicating if the buffer was locked or not
////
//// CONTEXT:
//// TIMING: 33 instructions longest path
////
//
//void ringBufferAdd(U16_ring_buffer * buffer, U16 value) {
//	if(buffer->semaphore == UNLOCKED) {
//		buffer->semaphore = LOCKED;
//		buffer->data_buffer[buffer->head] = value;
//		if(buffer->head < buffer->size) {
//			buffer->head++;
//		} else {
//			buffer->head = 0;
//		}
//		buffer->average_state = DIRTY;
//		buffer->semaphore = UNLOCKED;
//	}
//}
//
//// Overall note about the semaphore usage: Might be overkill, but allows the
//// caller to not worry at all about concurrency, a defensive coding tactic
//// Could probably make performance a increase if this bad boy was a
//// #pragma inline boy?
//
////******* ringBufferCalcAverage ***********************************************
////
//// This function is used to update the average of a ring buffer. It checks to
//// see if it is locked, and if not it enables the lock and checks to see if it
//// needs to recompute the average. If so, it does it, sets average_state CLEAN
//// and unlocks the buffer before returning
////
//// Inputs:
//// Returns: A boolean indicating if the buffer was locked or not
////
//// CONTEXT:
//// TIMING:
////
//
//boolean ringBufferCalcAverage(U16_ring_buffer * buffer) {
//	// Locals
//	static U8 i;
//	static U32 sum; //TODO is this big enough?
//
//	if(buffer->semaphore == UNLOCKED) {
//		buffer->semaphore = LOCKED;
//
//		// From here on data is guaranteed to be concurrent regardless of ISRs
//
//		if(buffer->average_state == CLEAN) {
//			buffer->semaphore = UNLOCKED;
//			return TRUE; // If the buffer_average is clean, we're good
//		} else {
//			// If not, calculate the average
//			for(i = 0; i < buffer->size; i++) {
//				sum += buffer->data_buffer[i];
//			}
//			buffer->average = sum >> RING_BUFFER_POWER;
//			buffer->average_state = CLEAN;
//			buffer->semaphore = UNLOCKED;
//			return TRUE;
//		}
//	} else {
//		return FALSE;
//	}
//}
//
//#endif
