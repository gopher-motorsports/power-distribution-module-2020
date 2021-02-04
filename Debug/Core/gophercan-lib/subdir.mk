################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/gophercan-lib/GopherCAN.c \
../Core/gophercan-lib/GopherCAN_example.c \
../Core/gophercan-lib/GopherCAN_ring_buffer.c \
../Core/gophercan-lib/GopherCAN_router_example.c 

OBJS += \
./Core/gophercan-lib/GopherCAN.o \
./Core/gophercan-lib/GopherCAN_example.o \
./Core/gophercan-lib/GopherCAN_ring_buffer.o \
./Core/gophercan-lib/GopherCAN_router_example.o 

C_DEPS += \
./Core/gophercan-lib/GopherCAN.d \
./Core/gophercan-lib/GopherCAN_example.d \
./Core/gophercan-lib/GopherCAN_ring_buffer.d \
./Core/gophercan-lib/GopherCAN_router_example.d 


# Each subdirectory must supply rules for building sources it contributes
Core/gophercan-lib/GopherCAN.o: ../Core/gophercan-lib/GopherCAN.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F072xB -DDEBUG -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/CMSIS/Include -I../Core/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM0 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/gophercan-lib/GopherCAN.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Core/gophercan-lib/GopherCAN_example.o: ../Core/gophercan-lib/GopherCAN_example.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F072xB -DDEBUG -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/CMSIS/Include -I../Core/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM0 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/gophercan-lib/GopherCAN_example.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Core/gophercan-lib/GopherCAN_ring_buffer.o: ../Core/gophercan-lib/GopherCAN_ring_buffer.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F072xB -DDEBUG -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/CMSIS/Include -I../Core/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM0 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/gophercan-lib/GopherCAN_ring_buffer.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Core/gophercan-lib/GopherCAN_router_example.o: ../Core/gophercan-lib/GopherCAN_router_example.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F072xB -DDEBUG -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/CMSIS/Include -I../Core/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM0 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/gophercan-lib/GopherCAN_router_example.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

