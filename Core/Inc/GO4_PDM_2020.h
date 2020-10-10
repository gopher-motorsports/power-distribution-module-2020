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
// CAN
#define PDM_CAN_ID 2

// Channel/buffer information
#define NUM_ADC_CHANNELS        10
#define NUM_SAMPLES_PER_CHANNEL 3
#define CURRENT_BUFFER_SIZE     (NUM_ADC_CHANNELS * NUM_SAMPLES_PER_CHANNEL)

// Conversion stuff
#define BTS50085_IL_IS_RATIO    13000
#define BTS443P_IL_IS_RATIO     8200            // For better precision, maintain a cal table https://www.mouser.com/datasheet/2/196/Infineon-BTS443P-DS-v01_00-EN-1226556.pdf, page 5
#define ADC_REF_VOLTAGE         ((double) 3.3)
#define MAX_16b_TIMER_VALUE     65536
#define MAX_12b_ADC_VAL         ((double) 4096) //
#define US_IN_S                 1000000
#define MA_IN_A                 1000
#define ADC_US_PER_SAMPLE       1

// Device control
#define GPIO_CONTROL_PORT                   GPIOB
#define DEFAULT_DEVICE_RESTART_TIMEOUT_MS   1000    // Max of 65000 when at 48MHZ with prescalar of 48000 (16 bit timer, no overflow)

// HW Device Defines
// HCM1
#define HCM1_RESRART_ATTEMPTS         10
#define HCM1_RESTART_TIMEOUT          DEFAULT_DEVICE_RESTART_TIMEOUT_MS
#define HCM1_GPIO_PIN                 GPIO_PIN_13
#define HCM1_SETPOINT                 10
#define HCM1_MAX_CHANNEL_INTEGRAL     50
#define HCM1_RESISTOR_VALUE           210
#define HCM1_INITIAL_STATE            NORMAL

// LCM2
#define LCM2_RESRART_ATTEMPTS         10
#define LCM2_RESTART_TIMEOUT          DEFAULT_DEVICE_RESTART_TIMEOUT_MS
#define LCM2_GPIO_PIN                 GPIO_PIN_6
#define LCM2_SETPOINT                 10
#define LCM2_MAX_CHANNEL_INTEGRAL     50
#define LCM2_RESISTOR_VALUE           210
#define LCM2_INITIAL_STATE            NORMAL

// LCM3
#define LCM3_RESRART_ATTEMPTS         10
#define LCM3_RESTART_TIMEOUT          DEFAULT_DEVICE_RESTART_TIMEOUT_MS
#define LCM3_GPIO_PIN                 GPIO_PIN_8
#define LCM3_SETPOINT                 10
#define LCM3_MAX_CHANNEL_INTEGRAL     50
#define LCM3_RESISTOR_VALUE           210
#define LCM3_INITIAL_STATE            NORMAL

// LCM1
#define LCM1_RESRART_ATTEMPTS         10
#define LCM1_RESTART_TIMEOUT          DEFAULT_DEVICE_RESTART_TIMEOUT_MS
#define LCM1_GPIO_PIN                 GPIO_PIN_7
#define LCM1_SETPOINT                 10
#define LCM1_MAX_CHANNEL_INTEGRAL     50
#define LCM1_RESISTOR_VALUE           210
#define LCM1_INITIAL_STATE            NORMAL


// HCM3
#define HCM3_RESRART_ATTEMPTS         10
#define HCM3_RESTART_TIMEOUT          DEFAULT_DEVICE_RESTART_TIMEOUT_MS
#define HCM3_GPIO_PIN                 GPIO_PIN_9
#define HCM3_SETPOINT                 10
#define HCM3_MAX_CHANNEL_INTEGRAL     50
#define HCM3_RESISTOR_VALUE           210
#define HCM3_INITIAL_STATE            NORMAL

// HCM4
#define HCM4_RESRART_ATTEMPTS         10
#define HCM4_RESTART_TIMEOUT          DEFAULT_DEVICE_RESTART_TIMEOUT_MS
#define HCM4_GPIO_PIN                 GPIO_PIN_10
#define HCM4_SETPOINT                 10
#define HCM4_MAX_CHANNEL_INTEGRAL     50
#define HCM4_RESISTOR_VALUE           210
#define HCM4_INITIAL_STATE            NORMAL

// HCM5
#define HCM5_RESRART_ATTEMPTS         10
#define HCM5_RESTART_TIMEOUT          DEFAULT_DEVICE_RESTART_TIMEOUT_MS
#define HCM5_GPIO_PIN                 GPIO_PIN_11
#define HCM5_SETPOINT                 10
#define HCM5_MAX_CHANNEL_INTEGRAL     50
#define HCM5_RESISTOR_VALUE           210
#define HCM5_INITIAL_STATE            NORMAL

// HCM2
#define HCM2_RESRART_ATTEMPTS         10
#define HCM2_RESTART_TIMEOUT          DEFAULT_DEVICE_RESTART_TIMEOUT_MS
#define HCM2_GPIO_PIN                 GPIO_PIN_12
#define HCM2_SETPOINT                 10
#define HCM2_MAX_CHANNEL_INTEGRAL     50
#define HCM2_RESISTOR_VALUE           210
#define HCM2_INITIAL_STATE            NORMAL



//********** Typedefs **********/
typedef enum {
    HCM1 = 0,
    LCM2 = 1,
    LCM1 = 2,
    LCM3 = 3,
    HCM3 = 4,
    HCM4 = 5,
    HCM5 = 6,
    HCM2 = 7,
    VBAT = 8,
    TEMP = 9
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
void CAN_tx(void);
void CAN_rx(void);
void PDM_Init(void);
void Current_Control_Loop(void);
void Log_CAN_Messages(void);
void Schedule_ADC(void);

#endif /* INC_GO4_PDM_2020_H_ */
