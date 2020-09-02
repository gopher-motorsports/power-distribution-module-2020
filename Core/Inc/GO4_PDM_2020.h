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
#define DEVICE_RESTART_TIMEOUT_MS 1000 //Max of 65000 when at 48MHZ with prescalar of 48000 (16 bit timer, no overflow)
#define GPIO_CONTROL_PORT GPIOB
#define MAX_16b_TIMER_VALUE 65536
#define ADC_REF_VOLTAGE ((double) 3.3)
#define US_IN_S 1000000
#define MA_IN_A 1000
#define MAX_12b_ADC_VAL ((double) 4096)

// Resistor Values
#define CHANNEL_ONE_RESISTOR_VALUE  10000
#define CHANNEL_TWO_RESISTOR_VALUE  10000
#define CHANNEL_FOUR_RESISTOR_VALUE 10000

// Typedefs
typedef struct {
    U8             num_restart_attempts;    // How many time the channel will restart itself
    double         channel_integral;        // Current integral value (milliamps * seconds)
    U16            restart_timeout_ref;     // Timer value when overcurrent event was detected (ms)
    U16            gpio_control_pin;        // Channel control pin number
    U16            channel_setpoint;        // value which defines over or undercurrent (milliamps)
    U16            max_channel_integral;    // max current integral allowed before shutoff (milliamps * seconds)
    U16            device_fet_IS_ratio;     // IL:IS (no units)
    U32            channel_resistor_val;    // Value of resistor attached to IS (Ohms)
    CHANNEL_STATE  state;                   // channel state
    CHANNEL_DEVICE device_name;             // Name of this device
} PDM_Device_t;


typedef enum {
    CHANNEL_1_DEVICE = 0,
    CHANNEL_2_DEVICE = 1,
    CHANNEL_4_DEVCIE = 2
} CHANNEL_DEVICE;

typedef enum {
    NORMAL = 0,	        // Channel withing operating current
    RESTART_OFF = 1,	// Channel off, will attempt to restart
    PERMANENT_OFF = 2   // Channel off forever
} CHANNEL_STATE;

typedef enum {
    UNUSED = 0, // Current data has not been used in a calculation yet
    USED = 1,   // Current data HAS been used in a calculation - dont double count
} CURRENT_DATA_STATE;

// Functions
void PDM_Init(void);
void Current_Control_Loop(void);
void Log_CAN_Messages(void);
void Schedule_ADC(void);

#endif /* INC_GO4_PDM_2020_H_ */
