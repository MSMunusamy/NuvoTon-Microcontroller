################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include User/subdir.mk
-include Library/subdir.mk
-include CMSIS/GCC/subdir.mk
-include CMSIS/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
ADC_READ_BLOCKING_CALL.hex \

SECONDARY_SIZE += \
ADC_READ_BLOCKING_CALL.siz \


# All Target
all: ADC_READ_BLOCKING_CALL.elf secondary-outputs

# Tool invocations
ADC_READ_BLOCKING_CALL.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross ARM GNU C Linker'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g -T gcc_arm.ld -Xlinker --gc-sections -L"../CMSIS/GCC" -Wl,-Map,"ADC_READ_BLOCKING_CALL.map" --specs=nano.specs -o "ADC_READ_BLOCKING_CALL.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

ADC_READ_BLOCKING_CALL.hex: ADC_READ_BLOCKING_CALL.elf
	@echo 'Invoking: Cross ARM GNU Create Flash Image'
	arm-none-eabi-objcopy -O ihex "ADC_READ_BLOCKING_CALL.elf"  "ADC_READ_BLOCKING_CALL.hex"
	@echo 'Finished building: $@'
	@echo ' '

ADC_READ_BLOCKING_CALL.siz: ADC_READ_BLOCKING_CALL.elf
	@echo 'Invoking: Cross ARM GNU Print Size'
	arm-none-eabi-size --format=berkeley "ADC_READ_BLOCKING_CALL.elf"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(SECONDARY_FLASH)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_UPPER_DEPS)$(C_DEPS) ADC_READ_BLOCKING_CALL.elf
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_SIZE)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
