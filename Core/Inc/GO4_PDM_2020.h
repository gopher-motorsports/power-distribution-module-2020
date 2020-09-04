/*
 * GO4_PDM_2020.h
 *
 *  Created on: Aug 20, 2020
 *      Author: ian
 */

#ifndef INC_GO4_PDM_2020_H_
#define INC_GO4_PDM_2020_H_

//********** Includes **********/
#include "base_types.h"



//********** Defines **********/
// Channel/buffer information
#define NUM_ADC_CHANNELS 3
#define NUM_SAMPLES_PER_CHANNEL 3   //
#define CURRENT_BUFFER_SIZE 9       // NUM_ADC_CHANNELS * NUM_SAMPLES_PER_CHANNEL

// Conversion stuff
#define BTS50085_IL_IS_RATIO 13000
#define BTS443P_IL_IS_RATIO 8200        // For better precision, maintain a cal table https://www.mouser.com/datasheet/2/196/Infineon-BTS443P-DS-v01_00-EN-1226556.pdf, page 5
#define ADC_REF_VOLTAGE ((double) 3.3)
#define MAX_16b_TIMER_VALUE 65536
#define MAX_12b_ADC_VAL ((double) 4096) //
#define US_IN_S 1000000
#define MA_IN_A 1000
#define ADC_US_PER_SAMPLE 1

// Device control
#define GPIO_CONTROL_PORT GPIOB
#define DEFAULT_DEVICE_RESTART_TIMEOUT_MS 1000 // Max of 65000 when at 48MHZ with prescalar of 48000 (16 bit timer, no overflow)

// HW Device Defines
// DEVICE_1
#define DEVICE_ONE_RESRART_ATTEMPTS 10
#define DEVICE_ONE_RESTART_TIMEOUT DEFAULT_DEVICE_RESTART_TIMEOUT_MS
#define DEVICE_ONE_GPIO_PIN GPIO_PIN_3
#define DEVICE_ONE_SETPOINT 10
#define DEVICE_ONE_MAX_CHANNEL_INTEGRAL 50
#define DEVICE_ONE_RESISTOR_VALUE  210
#define DEVICE_ONE_INITIAL_STATE NORMAL


// DEVICE_2
#define DEVICE_TWO_RESISTOR_VALUE  210



// DEVICE_4
#define DEVICE_FOUR_RESISTOR_VALUE 210



//********** Typedefs **********/
typedef enum {
    CHANNEL_1_DEVICE = 0,
    CHANNEL_2_DEVICE = 1,
    CHANNEL_4_DEVCIE = 2,
    TEMP_SENSOR      = 3,
    SPECIAL_DEVICE   = 4
} DEVICE_NAME;



typedef enum {
    NORMAL        = 0,	// Channel withing operating current
    RESTART_OFF   = 1,	// Channel off, will attempt to restart
    PERMANENT_OFF = 2   // Channel off forever
} CHANNEL_STATE;



typedef struct {
    U8             num_restart_attempts;    // How many time the channel will restart itself
    U16            channel_restart_timeout; // How long before the channel attempts to restart itself
    double         channel_integral;        // Current integral value (milliamps * seconds)
    U16            restart_timeout_ref;     // Timer value when overcurrent event was detected (ms)
    U16            gpio_control_pin;        // Channel control pin number
    U16            channel_setpoint;        // value which defines over or undercurrent (milliamps)
    U16            max_channel_integral;    // max current integral allowed before shutoff (milliamps * seconds)
    U16            device_fet_IL_IS_ratio;  // IL:IS (no units)
    U32            channel_resistor_val;    // Value of resistor attached to IS (Ohms)
    CHANNEL_STATE  state;                   // channel state
    DEVICE_NAME    device_name;             // Name of this device
} PDM_Device_t;



/*typedef enum {
    UNUSED = 0,   // Current data has not been used in a calculation yet
    USED   = 1,   // Current data HAS been used in a calculation - dont double count
} CURRENT_DATA_STATE;*/



//********** Functions **********/
void PDM_Init(void);
void Current_Control_Loop(void);
void Log_CAN_Messages(void);
void Schedule_ADC(void);

#endif /* INC_GO4_PDM_2020_H_ */
