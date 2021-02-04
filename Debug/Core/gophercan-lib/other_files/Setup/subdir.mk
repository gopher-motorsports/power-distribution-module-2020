################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/gophercan-lib/other_files/Setup/F0_example_main.c \
../Core/gophercan-lib/other_files/Setup/F7_example_main.c 

OBJS += \
./Core/gophercan-lib/other_files/Setup/F0_example_main.o \
./Core/gophercan-lib/other_files/Setup/F7_example_main.o 

C_DEPS += \
./Core/gophercan-lib/other_files/Setup/F0_example_main.d \
./Core/gophercan-lib/other_files/Setup/F7_example_main.d 


# Each subdirectory must supply rules for building sources it contributes
Core/gophercan-lib/other_files/Setup/F0_example_main.o: ../Core/gophercan-lib/other_files/Setup/F0_example_main.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F072xB -DDEBUG -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/CMSIS/Include -I../Core/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM0 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/gophercan-lib/other_files/Setup/F0_example_main.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Core/gophercan-lib/other_files/Setup/F7_example_main.o: ../Core/gophercan-lib/other_files/Setup/F7_example_main.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F072xB -DDEBUG -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/CMSIS/Include -I../Core/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM0 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/gophercan-lib/other_files/Setup/F7_example_main.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

