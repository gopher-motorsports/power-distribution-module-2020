/*
 * 		GO4_PDM_2020.c
 *
 *  	Created on: Aug 20, 2020
 *      Author: ian
 *      For use by UMN Gopher Motorsports only.
 */

// SYSTEM INCLUDES
#include "main.h"
#include "cmsis_os.h"

// Project Includes
#include "base_types.h"
#include "GO4_PDM_2020.h"

// Peripheral handles
extern ADC_HandleTypeDef hadc;
extern DMA_HandleTypeDef hdma_adc;
extern CAN_HandleTypeDef hcan;
extern TIM_HandleTypeDef htim16;
extern TIM_HandleTypeDef htim17;

/*
 static U16 channel_two_max_setpoint;
 static U16 channel_three_max_setpoint;
 static U16 channel_five_max_setpoint;
 static U16 channel_six_max_setpoint;
 static U16 channel_seven_max_setpoint;
 static U16 channel_eight_max_setpoint;
 static U16 channel_nine_max_setpoint;
 static U16 channel_ten_max_setpoint;
 static U16 channel_eleven_max_setpoint;
 static U16 channel_twelve_max_setpoint;
 static U16 channel_tempsensor_max_setpoint;
 */

// List of all adc channels with the micro defines
/*
 static U8 adc_channels[] = {ADC_CHANNEL_0, ADC_CHANNEL_1, ADC_CHANNEL_2,
 ADC_CHANNEL_3, ADC_CHANNEL_4, ADC_CHANNEL_5,
 ADC_CHANNEL_6, ADC_CHANNEL_7, ADC_CHANNEL_8,
 ADC_CHANNEL_9, ADC_CHANNEL_10, ADC_CHANNEL_11,
 ADC_CHANNEL_12, ADC_CHANNEL_TEMPSENSOR
 };
 */

/*

 static U16* channel_setpoints = {&channel_zero_max_setpoint, &channel_one_max_setpoint, &channel_two_max_setpoint,
 &channel_three_max_setpoint, &channel_four_max_setpoint, &channel_five_max_setpoint,
 &channel_six_max_setpoint, &channel_seven_max_setpoint, &channel_eight_max_setpoint,
 &channel_nine_max_setpoint, &channel_ten_max_setpoint, &channel_eleven_max_setpoint,
 &channel_twelve_max_setpoint, &channel_tempsensor_max_setpoint
 };
 */


// a current draw above this defines an overcurrent event
/*
static U16 channel_zero_max_setpoint = 3500;
static U16 channel_one_max_setpoint  = 3500;
static U16 channel_four_max_setpoint = 3500;

static U16 channel_zero_integral_max = 3500;
static U16 channel_one_integral_max  = 3500;
static U16 channel_four_integral_max = 3500;

static U16 channel_zero_fet_ratio = 13000;
static U16 channel_one_fet_ratio  = 13000;
static U16 channel_four_fet_ratio = 13000;

*/

//static PDM_Device_t devices[NUM_ADC_CHANNELS];

// Channel Information (consider putting in a struct)
//static U8                   adc_channels[]            = { ADC_CHANNEL_0, ADC_CHANNEL_1, ADC_CHANNEL_4 };
/*
static U8                   num_restart_attempts[]      = { 10, 10, 10 };
static U16                  gpio_control_pins[]         = { GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5 }; // Pay attention to ordering!!
static U16*                 channel_setpoints[]         = { &channel_zero_max_setpoint, &channel_one_max_setpoint, &channel_four_max_setpoint };
static U16*                 channel_integral_maxes[]    = { &channel_zero_integral_max, &channel_one_integral_max, &channel_four_integral_max };
static U16*                 device_fet_current_ratios[] = { &channel_zero_fet_ratio, &channel_one_fet_ratio, &channel_four_fet_ratio};
static U32                  channel_resistors[]         = { CHANNEL_ONE_RESISTOR_VALUE, CHANNEL_TWO_RESISTOR_VALUE, CHANNEL_FOUR_RESISTOR_VALUE };

static U16                  current_overage_integrals[NUM_ADC_CHANNELS]; // units of milliamps * seconds
static U16                  restart_timeout_ref[NUM_ADC_CHANNELS];


static CHANNEL_STATE channel_states[] = { NORMAL, NORMAL, NORMAL };
*/
//static CURRENT_DATA_STATE   channel_data_states[]   = { USED, USED, USED };	// Dont want to use inital data in calculation

static U8  restart_adc_flag;
static U32 avg_adc_interrupt_delta;
static U32 adc_interrupt_cnt;
static U64 adc_delta_sum;



// Define the device settings
static PDM_Device_t test_device_1 =
{
        .num_restart_attempts   = 10,
        .channel_integral       = 0,
        .restart_timeout_ref    = 0,
        .gpio_control_pin       = GPIO_PIN_3,
        .channel_setpoint       = 3500,
        .max_channel_integral   = 2000,
        .device_fet_IS_ratio    = 13000,
        .channel_resistor_val   = 10000,
        .state                  = NORMAL,
        .device_name            = CHANNEL_1_DEVICE
};

static PDM_Device_t test_device_2 =
{
        .num_restart_attempts   = 10,
        .channel_integral       = 0,
        .restart_timeout_ref    = 0,
        .gpio_control_pin       = GPIO_PIN_4,
        .channel_setpoint       = 3500,
        .max_channel_integral   = 2000,
        .device_fet_IS_ratio    = 13000,
        .channel_resistor_val   = 10000,
        .state                  = NORMAL,
        .device_name            = CHANNEL_2_DEVICE
};

static PDM_Device_t test_device_3 =
{
        .num_restart_attempts   = 10,
        .channel_integral       = 0,
        .restart_timeout_ref    = 0,
        .gpio_control_pin       = GPIO_PIN_5,
        .channel_setpoint       = 3500,
        .max_channel_integral   = 2000,
        .device_fet_IS_ratio    = 13000,
        .channel_resistor_val   = 10000,
        .state                  = NORMAL,
        .device_name            = CHANNEL_4_DEVICE
};


// MUST PUT DEVICES IN ORDER OF ADC CHANNEL NO. FROM LOWEST TO HIGHEST
// so the current buffer value contains the correct value for each device
static U16          current_buffer[NUM_ADC_CHANNELS];
static PDM_Device_t pdm_devices[] = { test_device_1, test_device_2, test_device_3 };


// Functions
void Log_CAN_Messages(void) {

}



void PDM_Init(void) {
    PDM_Device_t*   device;

    // Enable Channels
    for (device = pdm_devices; device < pdm_devices + NUM_ADC_CHANNELS; device++) {
        HAL_GPIO_WritePin(GPIO_CONTROL_PORT, device->gpio_control_pin, GPIO_PIN_SET);
    }

    // Unsure about this auto calibration
    HAL_ADCEx_Calibration_Start(&hadc);		// start cal before starting adc

    // start timers so interrupt gives accurate time
    htim17.Instance->CNT = 0;
    htim16.Instance->CNT = 0;
    HAL_TIM_Base_Start(&htim16);
    HAL_TIM_Base_Start(&htim17);

    HAL_ADC_Start_DMA(&hadc, (uint32_t*) current_buffer, NUM_ADC_CHANNELS); // start first round of conversions
}



void Schedule_ADC(void) {
    U8              i;
    U16*            adc_val;
    U16             temp_current_buffer[NUM_ADC_CHANNELS];
    U32             timer_val;                              // in microseconds
    double          load_current;                           // in milliamps
    double          voltage;                                // in Volts
    PDM_Device_t*   device;

    while (1) {
        if (restart_adc_flag) {
            restart_adc_flag = 0;       // reset flag before DMA start so sw wont clear flag after interrupt

            for (i = 0, adc_val = current_buffer; i < NUM_ADC_CHANNELS; adc_val++, i++) {
                // save current data so DMA doesnt overwrite it
                temp_current_buffer[i] = *adc_val;
            }

            HAL_ADC_Start_DMA(&hadc, (uint32_t*) current_buffer, NUM_ADC_CHANNELS);


            // do math while ADC goes brrrrrr
            timer_val = htim16.Instance->CNT + NUM_ADC_CHANNELS; // add 1us per channel, due to adc conversion time
            for (adc_val = temp_current_buffer, device = pdm_devices;
                        device < pdm_devices + NUM_ADC_CHANNELS; adc_val++, device++) {
                voltage = (ADC_REF_VOLTAGE * (*adc_val)) / MAX_12b_ADC_VAL;
                load_current = ((voltage * device->device_fet_IS_ratio) / device->channel_resistor_val) / MA_IN_A;

                // get rid of this if/else?
                if (load_current > device->channel_setpoint) {
                    channel_integral += (load_current - device->channel_setpoint) *  (timer_val / US_IN_S);
                } else {
                    channel_integral -= (device->channel_setpoint - load_current) *  (timer_val / US_IN_S);
                    if (channel_integral < 0) {
                        channel_integral = 0;
                    }
                }
                timer_val--; // subtract 1us because the next channel was converted after this one
            }

            // restart the timer
            htim16.Instance->CNT = 0;

            // calculate average interrupt delta
            /*adc_delta_sum += htim16.Instance->CNT;
            adc_interrupt_cnt++;
            avg_adc_interrupt_delta = adc_delta_sum / adc_interrupt_cnt;*/

        }
    }
}



void Current_Control_Loop(void) {
    U16           time_ref;
    U16           curr_time;
    U16           time_diff;
    PDM_Device_t* device;

    device = pdm_devices;
    // Infinite loop cycles through NUM_ADC_CHANNELS state machines, one for each channel
    while (1) {
        switch (device->state) {
            case PERMANENT_OFF:
                HAL_GPIO_WritePin(GPIO_CONTROL_PORT, device->gpio_control_pin, GPIO_PIN_RESET);
                break;

            case RESTART_OFF:
                curr_time = htim17.Instance->CNT;
                time_ref = device->restart_timeout_ref;
                if (curr_time < time_ref) {
                    time_diff = (MAX_16b_TIMER_VALUE - time_ref) + curr_time;
                } else {
                    time_diff = curr_time - time_ref;
                }

                if (time_diff >= DEVICE_RESTART_TIMEOUT_MS) {
                    // turn the channel back on
                    HAL_GPIO_WritePin(GPIO_CONTROL_PORT, device->gpio_control_pin, GPIO_PIN_SET);
                    device->state = NORMAL;
                }
                break;

            case NORMAL:
                    if (device->channel_integral >= device->max_channel_integral) {
                        // Shut off
                        // turn off control pin
                        HAL_GPIO_WritePin(GPIO_CONTROL_PORT, device->gpio_control_pin, GPIO_PIN_RESET);

                        if (device->num_restart_attempts > 0) {
                            device->num_restart_attempts--;
                            device->restart_timeout_ref = htim17.Instance->CNT;
                            device->state = RESTART_OFF;
                        } else {
                            device->state = PERMANENT_OFF;
                        }
                    }
                    break;

            default:
                Error_Handler();
                break;
        }

        //Log_CAN_Messages();

        device++;
        if (device > pdm_devices + NUM_ADC_CHANNELS - 1) {
            device = pdm_devices;
        }
    }
        // Unreachable
}


// Callbacks / ISRs
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *adc_handle) {
    // Let the OS loop know that the conversion is done
    restart_adc_flag = 1;
}

