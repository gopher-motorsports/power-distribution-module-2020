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
#include "GopherCAN.h"



//********** Peripheral Handles **********/
extern ADC_HandleTypeDef hadc;
extern DMA_HandleTypeDef hdma_adc;
extern TIM_HandleTypeDef htim16;
extern TIM_HandleTypeDef htim17;



//********** Globals **********/
// Define the device settings

static PDM_Device_t HCM1_d =
{
        .num_restart_attempts    = HCM1_RESRART_ATTEMPTS,
        .channel_restart_timeout = HCM1_RESTART_TIMEOUT,
        .channel_integral        = 0,
        .restart_timeout_ref     = 0,
        .gpio_control_pin        = HCM1_GPIO_PIN,
        .channel_setpoint        = HCM1_SETPOINT,
        .max_channel_integral    = HCM1_MAX_CHANNEL_INTEGRAL,
        .device_fet_IL_IS_ratio  = BTS50085_IL_IS_RATIO,
        .channel_resistor_val    = HCM1_RESISTOR_VALUE,
        .state                   = HCM1_INITIAL_STATE,
        .device_name             = HCM1
};

static PDM_Device_t LCM2_d =
{
        .num_restart_attempts    = LCM2_RESRART_ATTEMPTS,
        .channel_restart_timeout = LCM2_RESTART_TIMEOUT,
        .channel_integral        = 0,
        .restart_timeout_ref     = 0,
        .gpio_control_pin        = LCM2_GPIO_PIN,
        .channel_setpoint        = LCM2_SETPOINT,
        .max_channel_integral    = LCM2_MAX_CHANNEL_INTEGRAL,
        .device_fet_IL_IS_ratio  = BTS443P_IL_IS_RATIO,
        .channel_resistor_val    = LCM2_RESISTOR_VALUE,
        .state                   = LCM2_INITIAL_STATE,
        .device_name             = LCM2
};

static PDM_Device_t LCM3_d =
{
        .num_restart_attempts    = LCM3_RESRART_ATTEMPTS,
        .channel_restart_timeout = LCM3_RESTART_TIMEOUT,
        .channel_integral        = 0,
        .restart_timeout_ref     = 0,
        .gpio_control_pin        = LCM3_GPIO_PIN,
        .channel_setpoint        = LCM3_SETPOINT,
        .max_channel_integral    = LCM3_MAX_CHANNEL_INTEGRAL,
        .device_fet_IL_IS_ratio  = BTS443P_IL_IS_RATIO,
        .channel_resistor_val    = LCM3_RESISTOR_VALUE,
        .state                   = LCM3_INITIAL_STATE,
        .device_name             = LCM3
};

static PDM_Device_t LCM1_d =
{
        .num_restart_attempts    = LCM1_RESRART_ATTEMPTS,
        .channel_restart_timeout = LCM1_RESTART_TIMEOUT,
        .channel_integral        = 0,
        .restart_timeout_ref     = 0,
        .gpio_control_pin        = LCM1_GPIO_PIN,
        .channel_setpoint        = LCM1_SETPOINT,
        .max_channel_integral    = LCM1_MAX_CHANNEL_INTEGRAL,
        .device_fet_IL_IS_ratio  = BTS443P_IL_IS_RATIO,
        .channel_resistor_val    = LCM1_RESISTOR_VALUE,
        .state                   = LCM1_INITIAL_STATE,
        .device_name             = LCM1
};
static PDM_Device_t HCM3_d =
{
        .num_restart_attempts    = HCM3_RESRART_ATTEMPTS,
        .channel_restart_timeout = HCM3_RESTART_TIMEOUT,
        .channel_integral        = 0,
        .restart_timeout_ref     = 0,
        .gpio_control_pin        = HCM3_GPIO_PIN,
        .channel_setpoint        = HCM3_SETPOINT,
        .max_channel_integral    = HCM3_MAX_CHANNEL_INTEGRAL,
        .device_fet_IL_IS_ratio  = BTS50085_IL_IS_RATIO,
        .channel_resistor_val    = HCM3_RESISTOR_VALUE,
        .state                   = HCM3_INITIAL_STATE,
        .device_name             = HCM3
};

static PDM_Device_t HCM4_d =
{
        .num_restart_attempts    = HCM4_RESRART_ATTEMPTS,
        .channel_restart_timeout = HCM4_RESTART_TIMEOUT,
        .channel_integral        = 0,
        .restart_timeout_ref     = 0,
        .gpio_control_pin        = HCM4_GPIO_PIN,
        .channel_setpoint        = HCM4_SETPOINT,
        .max_channel_integral    = HCM4_MAX_CHANNEL_INTEGRAL,
        .device_fet_IL_IS_ratio  = BTS50085_IL_IS_RATIO,
        .channel_resistor_val    = HCM4_RESISTOR_VALUE,
        .state                   = HCM4_INITIAL_STATE,
        .device_name             = HCM4
};


static PDM_Device_t HCM5_d =
{
        .num_restart_attempts    = HCM5_RESRART_ATTEMPTS,
        .channel_restart_timeout = HCM5_RESTART_TIMEOUT,
        .channel_integral        = 0,
        .restart_timeout_ref     = 0,
        .gpio_control_pin        = HCM5_GPIO_PIN,
        .channel_setpoint        = HCM5_SETPOINT,
        .max_channel_integral    = HCM5_MAX_CHANNEL_INTEGRAL,
        .device_fet_IL_IS_ratio  = BTS50085_IL_IS_RATIO,
        .channel_resistor_val    = HCM5_RESISTOR_VALUE,
        .state                   = HCM5_INITIAL_STATE,
        .device_name             = HCM5
};


static PDM_Device_t HCM2_d =
{
        .num_restart_attempts    = HCM2_RESRART_ATTEMPTS,
        .channel_restart_timeout = HCM2_RESTART_TIMEOUT,
        .channel_integral        = 0,
        .restart_timeout_ref     = 0,
        .gpio_control_pin        = HCM2_GPIO_PIN,
        .channel_setpoint        = HCM2_SETPOINT,
        .max_channel_integral    = HCM2_MAX_CHANNEL_INTEGRAL,
        .device_fet_IL_IS_ratio  = BTS50085_IL_IS_RATIO,
        .channel_resistor_val    = HCM2_RESISTOR_VALUE,
        .state                   = HCM2_INITIAL_STATE,
        .device_name             = HCM2
};

static PDM_Device_t TEMP_d;
static PDM_Device_t VBAT_d;


static U8           conv_cplt_flag;
static U16          current_buffer[CURRENT_BUFFER_SIZE];
static U16          averaged_buffer[NUM_ADC_CHANNELS];
static PDM_Device_t pdm_devices[NUM_ADC_CHANNELS];


//********** CAN Parameters **********/
extern FLOAT_CAN_STRUCT HCM1_current;
extern FLOAT_CAN_STRUCT LCM2_current;
extern FLOAT_CAN_STRUCT LCM3_current;
extern FLOAT_CAN_STRUCT LCM1_current;
extern FLOAT_CAN_STRUCT HCM3_current;
extern FLOAT_CAN_STRUCT HCM4_current;
extern FLOAT_CAN_STRUCT HCM5_current;
extern FLOAT_CAN_STRUCT HCM2_current;
extern FLOAT_CAN_STRUCT BAT_voltage;
extern FLOAT_CAN_STRUCT TEMP_sensor;

//********** Function Definitions **********/

/*
 * Args:
 * PDM_Device_t* - which device param to update
 * double val - value to update into the param
 *
 * Info:
 * Updates the CAN parameter
 */
void update_can_param(PDM_Device_t* device, double val) {
    switch(device->device_name) {
        case HCM1:
            HCM1_current.data = val;
            break;
        case LCM2:
            LCM2_current.data = val;
            break;
        case LCM1:
            LCM1_current.data = val;
            break;
        case LCM3:
            LCM3_current.data = val;
            break;
        case HCM3:
            HCM3_current.data = val;
            break;
        case HCM4:
            HCM4_current.data = val;
            break;
        case HCM5:
            HCM5_current.data = val;
            break;
        case HCM2:
            HCM2_current.data = val;
            break;
    }
}



/*
 * Args:
 * none
 *
 * Info:
 * Runs before OS kernal starts to initialize
 * peripherals and other general setup
 */
void PDM_Init(void) {
    PDM_Device_t*   device;

    // MUST PUT DEVICES IN ORDER OF ADC CHANNEL NO. FROM LOWEST TO HIGHEST
    // so the current buffer value contains the correct value for each device
    pdm_devices[0] = HCM1_d;
    pdm_devices[1] = LCM2_d;
    pdm_devices[2] = LCM3_d;
    pdm_devices[3] = LCM1_d;
    pdm_devices[4] = HCM3_d;
    pdm_devices[5] = HCM4_d;
    pdm_devices[6] = HCM5_d;
    pdm_devices[7] = HCM2_d;
    pdm_devices[8] = TEMP_d;
    pdm_devices[9] = VBAT_d;


    //CAN init
    init_can(PDM_CAN_ID);
    HCM1_current.update_enabled = TRUE;
    LCM2_current.update_enabled = TRUE;
    LCM3_current.update_enabled = TRUE;
    LCM1_current.update_enabled = TRUE;
    HCM3_current.update_enabled = TRUE;
    HCM4_current.update_enabled = TRUE;
    HCM5_current.update_enabled = TRUE;
    HCM2_current.update_enabled = TRUE;
    BAT_voltage.update_enabled = TRUE;
    TEMP_sensor.update_enabled = TRUE;




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



/*
 * Args:
 * none
 *
 * Info:
 * OS task that calculates the current overage
 * integral. Only runs when a new set of averages
 * has been calcuclated by the ADC callback,
 * 'HAL_ADC_ConvCpltCallback'
 */
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

            // current buffer caseted to U32 to please the compiler gods, DMA request size set to half word (U16)
            HAL_ADC_Start_DMA(&hadc, (uint32_t*) current_buffer, CURRENT_BUFFER_SIZE);

            // do math while ADC goes brrrrrr

            for (adc_val = temp_current_buffer, device = pdm_devices;
                                                device < pdm_devices + NUM_ADC_CHANNELS; adc_val++, device++) {
                // Skip calculation of integral for special devices
                if (!(device->device_name == TEMP || device->device_name == VBAT)) {
                    voltage = (ADC_REF_VOLTAGE * (*adc_val)) / MAX_12b_ADC_VAL; // V = IR
                    // TODO: load current macro?
                    load_current = ((voltage * device->device_fet_IL_IS_ratio) / device->channel_resistor_val) * MA_IN_A;
                    update_can_param(device, load_current);

                    device->channel_integral += (load_current - device->channel_setpoint) *  (timer_val / US_IN_S);

                    if (device->channel_integral < 0) {
                        device->channel_integral = 0;
                    }
                }
            }
        }
    }
}



/*
 * Args:
 * none
 *
 * Info:
 * OS task that monitors the state of each channel
 * and is responsible for stopping and restarting
 * each channel.
 */
void Current_Control_Loop(void) {
    U16           time_ref;
    U16           curr_time;
    U16           time_diff;
    PDM_Device_t* device;

    device = pdm_devices;
    // Infinite loop cycles through NUM_ADC_CHANNELS state machines, one for each channel
    while (1) {
        // special device handling code
        if (device->device_name == TEMP) {
            // TODO: use voltage divider conversion
            TEMP_sensor.data =
        }
        else if (device->device_name == VBAT) {
            // TODO: xuse voltage divider conversion
            BAT_voltage =
        }
        // Regular overcurrent handling
        else {
            switch (device->state) {
                case PERMANENT_OFF:
                    break;

                case RESTART_OFF:
                    curr_time = htim17.Instance->CNT;
                    time_ref = device->restart_timeout_ref;
                    if (curr_time < time_ref) {
                        time_diff = (MAX_16b_TIMER_VALUE - time_ref) + curr_time;
                    } else {
                        time_diff = curr_time - time_ref;
                    }

                    if (time_diff >= device->channel_restart_timeout) {
                        // turn the channel back on
                        HAL_GPIO_WritePin(GPIO_CONTROL_PORT, device->gpio_control_pin, GPIO_PIN_SET);
                        device->channel_integral = 0;
                        device->state = NORMAL;
                    }
                    break;

                case NORMAL:
                        if (device->channel_integral >= device->max_channel_integral ) {
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

        device++;
        if (device >= pdm_devices + NUM_ADC_CHANNELS) {
            device = pdm_devices;
        }
    }
        // Unreachable
}

/*
 * Args:
 * none
 *
 * Info:
 * OS task that handles CAN tx
 */
void CAN_tx(void) {
    while (1) {
        service_can_tx_hardware();
    }
}

/*
 * Args:
 * none
 *
 * Info:
 * OS task that handles incoming CAN messages
 */
void CAN_rx(void) {
    while (1) {
        service_can_rx_buffer();
    }
}


//********** Callbacks/ISRs **********/
/* Args:
 * ADC_HandleTypeDef: the handle to the ADC that interrupted
 *
 * Info:
 * This function is called by HW when the DMA buffer becomes full.
 * This function resets timer 16, Stops the DMA, and calculates
 * the channel average adc value over 'NUM_SAMPLES_PER_CHANNEL' samples
 * it places the averages into the global 'averaged_buffer'
 *
 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *adc_handle) {
    U8   i;
    U16* current_ptr;

    // reset timer
    htim16.Instance->CNT = 0;

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

    conv_cplt_flag = 1;
}
