################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/ian/SAECode/c-libraries/C-Utils/base_types.c \
C:/Users/ian/SAECode/c-libraries/C-Utils/ring_buffer.c 

OBJS += \
./Core/C-Utils/base_types.o \
./Core/C-Utils/ring_buffer.o 

C_DEPS += \
./Core/C-Utils/base_types.d \
./Core/C-Utils/ring_buffer.d 


# Each subdirectory must supply rules for building sources it contributes
Core/C-Utils/base_types.o: C:/Users/ian/SAECode/c-libraries/C-Utils/base_types.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F072xB -DDEBUG -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I"C:/Users/ian/SAECode/STM32_CAN" -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/CMSIS/Include -I../Core/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I"C:/Users/ian/SAECode/c-libraries/C-Utils" -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM0 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/C-Utils/base_types.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Core/C-Utils/ring_buffer.o: C:/Users/ian/SAECode/c-libraries/C-Utils/ring_buffer.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F072xB -DDEBUG -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I"C:/Users/ian/SAECode/STM32_CAN" -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/CMSIS/Include -I../Core/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I"C:/Users/ian/SAECode/c-libraries/C-Utils" -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM0 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/C-Utils/ring_buffer.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

