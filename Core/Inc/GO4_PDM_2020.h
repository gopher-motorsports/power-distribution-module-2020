/*
 * GO4_PDM_2020.h
 *
 *  Created on: Aug 20, 2020
 *      Author: ian
 */

#ifndef INC_GO4_PDM_2020_H_
#define INC_GO4_PDM_2020_H_

// Includes
#include "base_types.h"

// Defines
#define NUM_ADC_CHANNELS 3
#define DEVICE_RESTART_TIMEOUT_MS 1000 //Max of 65000 when at 48MHZ with prescalar of 48 (16 bit timer, no overflow)
#define GPIO_CONTROL_PORT GPIOB
#define MAX_TIMER_VALUE 65536

// Typedefs
typedef enum {
	CHANNEL_1_DEVICE = 0,
	CHANNEL_2_DEVICE = 1,
	CHANNEL_4_DEVCIE = 2
} CHANNEL_DEVICE;

typedef enum {
	NORMAL 			= 0,	// Channel withing operating current
	RESTART_OFF 	= 1,	// Channel off, will attempt to restart
	PERMANENT_OFF	= 2		// Channel off forever
} ADC_CHANNEL_STATE;

typedef enum {
	UNUSED = 0,		// Current data has not been used in a calculation yet
	USED   = 1,		// Current data HAS been used in a calculation - dont double count
} CURRENT_DATA_STATE;

// Functions
void PDM_Init(void);
void Current_Control_Loop(void);


#endif /* INC_GO4_PDM_2020_H_ */
