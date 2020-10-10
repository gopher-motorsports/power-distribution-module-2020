################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/ian/SAECode/STM32_CAN/GopherCAN.c \
C:/Users/ian/SAECode/STM32_CAN/GopherCAN_example.c 

OBJS += \
./Core/STM32_CAN/GopherCAN.o \
./Core/STM32_CAN/GopherCAN_example.o 

C_DEPS += \
./Core/STM32_CAN/GopherCAN.d \
./Core/STM32_CAN/GopherCAN_example.d 


# Each subdirectory must supply rules for building sources it contributes
Core/STM32_CAN/GopherCAN.o: C:/Users/ian/SAECode/STM32_CAN/GopherCAN.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F072xB -DDEBUG -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I"C:/Users/ian/SAECode/STM32_CAN" -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/CMSIS/Include -I../Core/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I"C:/Users/ian/SAECode/c-libraries/C-Utils" -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM0 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/STM32_CAN/GopherCAN.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Core/STM32_CAN/GopherCAN_example.o: C:/Users/ian/SAECode/STM32_CAN/GopherCAN_example.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F072xB -DDEBUG -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I"C:/Users/ian/SAECode/STM32_CAN" -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/CMSIS/Include -I../Core/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I"C:/Users/ian/SAECode/c-libraries/C-Utils" -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM0 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/STM32_CAN/GopherCAN_example.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

