/*
 * 		GO4_PDM_2020.c
 *
 *  	Created on: Aug 20, 2020
 *      Author: ian
 *      For use by UMN Gopher Motorsports only.
 */

//********** System Includes **********/
#include "main.h"
#include "cmsis_os.h"



//********** Project Includes **********/
#include "base_types.h"
#include "GO4_PDM_2020.h"



//********** Peripheral Handles **********/
extern ADC_HandleTypeDef hadc;
extern DMA_HandleTypeDef hdma_adc;
extern CAN_HandleTypeDef hcan;
extern TIM_HandleTypeDef htim16;
extern TIM_HandleTypeDef htim17;



//********** Globals **********/
// Define the device settings
static PDM_Device_t test_device_1 =
{
        .num_restart_attempts    = DEVICE_ONE_RESRART_ATTEMPTS,
        .channel_restart_timeout = DEVICE_ONE_RESTART_TIMEOUT,
        .channel_integral        = 0,
        .restart_timeout_ref     = 0,
        .gpio_control_pin        = DEVICE_ONE_GPIO_PIN,
        .channel_setpoint        = DEVICE_ONE_SETPOINT,
        .max_channel_integral    = DEVICE_ONE_MAX_CHANNEL_INTEGRAL,
        .device_fet_IL_IS_ratio  = BTS50085_IL_IS_RATIO,
        .channel_resistor_val    = DEVICE_ONE_RESISTOR_VALUE,
        .state                   = DEVICE_ONE_INITIAL_STATE,
        .device_name             = CHANNEL_1_DEVICE
};

static PDM_Device_t test_device_2 =
{
        .num_restart_attempts    = 10,
        .channel_restart_timeout = DEFAULT_DEVICE_RESTART_TIMEOUT_MS,
        .channel_integral        = 0,
        .restart_timeout_ref     = 0,
        .gpio_control_pin        = GPIO_PIN_4,
        .channel_setpoint        = 3500,
        .max_channel_integral    = 2000,
        .device_fet_IL_IS_ratio  = BTS50085_IL_IS_RATIO,
        .channel_resistor_val    = DEVICE_TWO_RESISTOR_VALUE,
        .state                   = NORMAL,
        .device_name             = CHANNEL_2_DEVICE
};

static PDM_Device_t test_device_3 =
{
        .num_restart_attempts    = 10,
        .channel_restart_timeout = DEFAULT_DEVICE_RESTART_TIMEOUT_MS,
        .channel_integral        = 0,
        .restart_timeout_ref     = 0,
        .gpio_control_pin        = GPIO_PIN_5,
        .channel_setpoint        = 3500,
        .max_channel_integral    = 2000,
        .device_fet_IL_IS_ratio  = BTS50085_IL_IS_RATIO,
        .channel_resistor_val    = DEVICE_FOUR_RESISTOR_VALUE,
        .state                   = NORMAL,
        .device_name             = CHANNEL_4_DEVCIE
};


// MUST PUT DEVICES IN ORDER OF ADC CHANNEL NO. FROM LOWEST TO HIGHEST
// so the current buffer value contains the correct value for each device
static U8           conv_cplt_flag;
static U16          current_buffer[CURRENT_BUFFER_SIZE];
static U16          averaged_buffer[NUM_ADC_CHANNELS];
static PDM_Device_t pdm_devices[] = { test_device_1, test_device_2, test_device_3 };


//********** Function Definitions **********/
void Log_CAN_Messages(void) {
    // TODO: Get and use CAN lib
}



void PDM_Init(void) {
    PDM_Device_t*   device;

    // Unsure about this auto calibration
    HAL_ADCEx_Calibration_Start(&hadc);		// start cal before starting adc

    // start timers so interrupt gives accurate time
    htim17.Instance->CNT = 0;
    htim16.Instance->CNT = 0;
    HAL_TIM_Base_Start(&htim16);
    HAL_TIM_Base_Start(&htim17);

    HAL_ADC_Start_DMA(&hadc, (uint32_t*) current_buffer, CURRENT_BUFFER_SIZE); // start first round of conversions

    // Enable Channels
    for (device = pdm_devices; device < pdm_devices + NUM_ADC_CHANNELS; device++) {
        if (device->state == NORMAL) {
            HAL_GPIO_WritePin(GPIO_CONTROL_PORT, device->gpio_control_pin, GPIO_PIN_SET);
        }

    }

}



void Schedule_ADC(void) {
    U8              i;
    U16*            adc_val;
    U16             temp_current_buffer[NUM_ADC_CHANNELS];
    double          timer_val;                              // in microseconds
    double          load_current;                           // in milliamps
    double          voltage;                                // in Volts
    PDM_Device_t*   device;

    while (1) {
        if (conv_cplt_flag) {
            conv_cplt_flag = 0; // reset flag before DMA start so sw wont clear flag after interrupt

            // save averaged (filtered) current data so DMA doesnt overwrite it
            for (i = 0, adc_val = averaged_buffer; i < NUM_ADC_CHANNELS; adc_val++, i++) {
                temp_current_buffer[i] = *adc_val;
            }

            // timestamp now so DMA doesnt restart timer on interrupt
            timer_val = htim16.Instance->CNT + (CURRENT_BUFFER_SIZE * ADC_US_PER_SAMPLE); // adjust for ADC conversion time
            HAL_ADC_Start_DMA(&hadc, (uint32_t*) current_buffer, NUM_ADC_CHANNELS);

            // do math while ADC goes brrrrrr

            for (adc_val = temp_current_buffer, device = pdm_devices;
                                                device < pdm_devices + NUM_ADC_CHANNELS; adc_val++, device++) {
                // Skip calculation of integral for special devices
                if (!(device->device_name == TEMP_SENSOR || device->device_name == SPECIAL_DEVICE)) {
                    voltage = (ADC_REF_VOLTAGE * (*adc_val)) / MAX_12b_ADC_VAL;
                    load_current = ((voltage * device->device_fet_IL_IS_ratio) / device->channel_resistor_val) / MA_IN_A;
                    device->channel_integral += (load_current - device->channel_setpoint) *  (timer_val / US_IN_S);

                    if (device->channel_integral < 0) {
                        device->channel_integral = 0;
                    }
                }
            }
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
        // special device handling code
        if (device->device_name == TEMP_SENSOR) {
            ;
        }
        else if (device->device_name == SPECIAL_DEVICE)  {
            ;
        }
        // Regular overcurrent handling
        else {
            switch (device->state) {
                case PERMANENT_OFF:
                    break;

                case RESTART_OFF:
                    curr_time = htim17.Instance->CNT;
                    time_ref = device->restart_timeout_ref; // assign to local so not looking up multiple times
                    if (curr_time < time_ref) {
                        time_diff = (MAX_16b_TIMER_VALUE - time_ref) + curr_time;
                    } else {
                        time_diff = curr_time - time_ref;
                    }

                    if (time_diff >= device->channel_restart_timeout) {
                        // turn the channel back on
                        HAL_GPIO_WritePin(GPIO_CONTROL_PORT, device->gpio_control_pin, GPIO_PIN_SET);
                        device->state = NORMAL;
                    }
                    break;

                case NORMAL:
                        if (device->channel_integral >= device->max_channel_integral ) {
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
                    // Should never get here
                    Error_Handler();
                    break;
            }
        }

        //Log_CAN_Messages();

        device++;
        if (device >= pdm_devices + NUM_ADC_CHANNELS) {
            device = pdm_devices;
        }
    }
        // Unreachable
}


//********** Callbacks/ISRs **********/
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *adc_handle) {
    U8   i;
    U16* current_ptr;

    // Suspend tasks so OS doesnt interrupt
    // Need to find out if the OS interrupt has higher priority than this ISR, dont think so though
    // If this interrupt priorty is higher dont worry. if not, then some wacky stuff might happen
    // vTaskSuspendAll();

    // reset timer
    &htim16.Instance->CNT = 0;

    // Stop DMA to stop interrupts
    HAL_ADC_Stop_DMA(&hadc);

    // 0 the average buffer
    for (current_ptr = averaged_buffer; current_ptr < averaged_buffer + NUM_ADC_CHANNELS; current_ptr++) {
        *current_ptr = 0;
    }

    // sum all samples into correct spot
    for (i = 0, current_ptr = current_buffer; current_ptr < current_buffer + CURRENT_BUFFER_SIZE; current_ptr++) {
        averaged_buffer[i] += *current_ptr;
        i++;
        if (i % NUM_SAMPLES_PER_CHANNEL == 0) {
            i = 0;
        }
    }

    // reuse current_ptr to spin through and divide the summations
    for (current_ptr = averaged_buffer; current_ptr < averaged_buffer + NUM_ADC_CHANNELS; current_ptr++) {
        *current_ptr /= NUM_SAMPLES_PER_CHANNEL;
    }

    // average buffer?
    conv_cplt_flag = 1;

    //xTaskResumeAll(); only call if vTaskSuspendAll() is called
}
