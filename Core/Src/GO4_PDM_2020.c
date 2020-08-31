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

static U8 restart_adc_flag;
static U32 avg_adc_interrupt_delta;
static U32 adc_interrupt_cnt;
static U64 adc_delta_sum;

// a current draw above this defines an overcurrent event
static U16 channel_zero_max_setpoint = 3500;
static U16 channel_one_max_setpoint  = 3500;
static U16 channel_four_max_setpoint = 3500;


// Channel Information
//static U8                   adc_channels[]          = { ADC_CHANNEL_0, ADC_CHANNEL_1, ADC_CHANNEL_4 };
static U8                   num_restart_attempts[]  = { 10, 10, 10 };
static U16                  gpio_control_pins[]     = { GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5 }; // Pay attention to ordering!!
static U16                  *channel_setpoints[]    = { &channel_zero_max_setpoint, &channel_one_max_setpoint, &channel_four_max_setpoint };

static U16                  current_buffer[NUM_ADC_CHANNELS];
static U16                  current_overage_integrals[NUM_ADC_CHANNELS]; // units of milliamps * seconds
static
static U16                  restart_timeout_ref[NUM_ADC_CHANNELS];

static ADC_CHANNEL_STATE    channel_states[]        = { NORMAL, NORMAL, NORMAL };
//static CURRENT_DATA_STATE   channel_data_states[]   = { USED, USED, USED };	// Dont want to use inital data in calculation


// Functions
void Log_CAN_Messages(void) {

}



void PDM_Init(void) {
    U8 i;

    // Set timers to 0 (may not need to do this)
    // Enable Channels
    for (i = 0; i < NUM_ADC_CHANNELS; i++) {
        HAL_GPIO_WritePin(GPIO_CONTROL_PORT, gpio_control_pins[i],
                GPIO_PIN_SET);
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
    U8  i;
    U16 temp_current_buffer[NUM_ADC_CHANNELS];

    while (1) {
        if (restart_adc_flag) {
            restart_adc_flag = 0;       // reset flag before DMA start so sw wont clear flag after interrupt

            for (i = 0; i < NUM_ADC_CHANNELS; i++) {
                // save current data so DMA doesnt overwrite it
                temp_current_buffer[i] = current_buffer[i];
            }

            HAL_ADC_Start_DMA(&hadc, (uint32_t*) current_buffer, NUM_ADC_CHANNELS);

            // calculate average interrupt delta
            adc_delta_sum += htim16.Instance->CNT;
            adc_interrupt_cnt++;
            avg_adc_interrupt_delta = adc_delta_sum / adc_interrupt_cnt;

            htim16.Instance->CNT = 0;
            HAL_TIM_Base_Start(&htim16);
        }
    }
}



void Current_Control_Loop(void) {
    U8 adc_channels_idx = 0;
    U16 time_ref, curr_time, time_diff;

    // Infinite loop cycles through NUM_ADC_CHANNELS state machines, one for each channel
    while (1) {

        switch (channel_states[adc_channels_idx]) {
            case PERMANENT_OFF:
                // Ensure current off??
                break;

            case RESTART_OFF:
                curr_time = htim17.Instance->CNT;
                time_ref = restart_timeout_ref[adc_channels_idx];
                if (curr_time < time_ref) {
                    time_diff = (MAX_TIMER_VALUE - time_ref) + curr_time;
                } else {
                    time_diff = curr_time - time_ref;
                }

                if (time_diff >= DEVICE_RESTART_TIMEOUT_MS) {
                    // turn the channel back on
                    HAL_GPIO_WritePin(GPIO_CONTROL_PORT, gpio_control_pins[adc_channels_idx],
                                                                                GPIO_PIN_SET);
                    channel_states[adc_channels_idx] = NORMAL;
                }
                break;

            case NORMAL:
                    if (current_overage_integrals[adc_channels_idx] >= *channel_setpoints[adc_channels_idx]) {
                        // Overcurrent event (level triggered)
                        // turn off control pin
                        HAL_GPIO_WritePin(GPIO_CONTROL_PORT, gpio_control_pins[adc_channels_idx],
                                                                                  GPIO_PIN_RESET);
                        if (num_restart_attempts[adc_channels_idx] > 0) {
                            num_restart_attempts[adc_channels_idx]--;
                            restart_timeout_ref[adc_channels_idx] =
                                    htim17.Instance->CNT;
                            channel_states[adc_channels_idx] = RESTART_OFF;
                        } else {
                            channel_states[adc_channels_idx] = PERMANENT_OFF;
                        }
                    }
                }
                break;

            default:
                Error_Handler();
                break;
        }

        //Log_CAN_Messages();

        adc_channels_idx++;
        if (adc_channels_idx >= NUM_ADC_CHANNELS) {
            adc_channels_idx = 0;
        }
        // Unreachable

}


// Callbacks / ISRs
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *adc_handle) {
    // Let the OS loop know that the conversion is done
    restart_adc_flag = 1;
}

