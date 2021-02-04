//
//typedef enum {
//	DIRTY = 0,
//	CLEAN = 1,
//} RING_BUFFER_AVERAGE_STATE;
//
//typedef enum {
//	LOCKED = 0,
//	UNLOCKED = 1,
//} RING_BUFFER_SEMAPHORE_STATE;
//
//typedef struct
//{
//	U16 data_buffer[(2 << RING_BUFFER_POWER)];		//Stores the data TODO: Does the compiler optimize this? Check ASM output
//	U8 size;										//Stores the size from compile time
//	U8 head;										//Stores the current position
//	U8 average;										//Stores average for quick retrieval, can calc average later
//	RING_BUFFER_AVERAGE_STATE average_state;
//	RING_BUFFER_SEMAPHORE_STATE semaphore;
//} U16_ring_buffer;									//A circular buffer implemented like a queue. I just think ring buffer sounds cooler
//
//void ringBufferAdd(U16_ring_buffer * buffer, U16 value);
//boolean ringBufferCalcAverage(U16_ring_buffer * buffer);
//
//Hello Henry
