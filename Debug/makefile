################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM0/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/subdir.mk
-include Drivers/STM32F0xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include Core/STM32_CAN/subdir.mk
-include Core/C-Utils/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
PDM_2021_devboard_version.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
PDM_2021_devboard_version.list \

OBJCOPY_BIN += \
PDM_2021_devboard_version.bin \


# All Target
all: PDM_2021_devboard_version.elf secondary-outputs

# Tool invocations
PDM_2021_devboard_version.elf: $(OBJS) $(USER_OBJS) C:\Users\ian\STM32CubeIDE\workspace_1.3.0\ADCTest\STM32F072RBTX_FLASH.ld
	arm-none-eabi-gcc -o "PDM_2021_devboard_version.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m0 -T"C:\Users\ian\STM32CubeIDE\workspace_1.3.0\ADCTest\STM32F072RBTX_FLASH.ld" --specs=nosys.specs -Wl,-Map="PDM_2021_devboard_version.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

PDM_2021_devboard_version.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "PDM_2021_devboard_version.list"
	@echo 'Finished building: $@'
	@echo ' '

PDM_2021_devboard_version.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "PDM_2021_devboard_version.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
