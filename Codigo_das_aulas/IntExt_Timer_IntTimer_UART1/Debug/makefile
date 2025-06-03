################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Drivers/STM32F1xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := IntExt_Timer_IntTimer_UART1
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
IntExt_Timer_IntTimer_UART1.elf \

MAP_FILES += \
IntExt_Timer_IntTimer_UART1.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
IntExt_Timer_IntTimer_UART1.list \

OBJCOPY_HEX += \
IntExt_Timer_IntTimer_UART1.hex \


# All Target
all: main-build

# Main-build Target
main-build: IntExt_Timer_IntTimer_UART1.elf secondary-outputs

# Tool invocations
IntExt_Timer_IntTimer_UART1.elf IntExt_Timer_IntTimer_UART1.map: $(OBJS) $(USER_OBJS) C:\BACKUP_LOCAL\AULAS\ FATEC\STM32\Codigos\IntExt_Timer_IntTimer_UART1\STM32F103C8TX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "IntExt_Timer_IntTimer_UART1.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m3 -T"C:\BACKUP_LOCAL\AULAS FATEC\STM32\Codigos\IntExt_Timer_IntTimer_UART1\STM32F103C8TX_FLASH.ld" --specs=nosys.specs -Wl,-Map="IntExt_Timer_IntTimer_UART1.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

IntExt_Timer_IntTimer_UART1.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "IntExt_Timer_IntTimer_UART1.list"
	@echo 'Finished building: $@'
	@echo ' '

IntExt_Timer_IntTimer_UART1.hex: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O ihex $(EXECUTABLES) "IntExt_Timer_IntTimer_UART1.hex"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) IntExt_Timer_IntTimer_UART1.elf IntExt_Timer_IntTimer_UART1.hex IntExt_Timer_IntTimer_UART1.list IntExt_Timer_IntTimer_UART1.map default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_HEX)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
