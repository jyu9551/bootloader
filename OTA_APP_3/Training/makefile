################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Source/5_Bsw/Service/SchM/subdir.mk
-include Source/5_Bsw/Service/Os/subdir.mk
-include Source/5_Bsw/Mcal/McalTraining/cfg/src/subdir.mk
-include Source/5_Bsw/Mcal/AURIX_MC-ISAR_AS4XX_AURIX_TC23X_AC_V201/wdg/subdir.mk
-include Source/5_Bsw/Mcal/AURIX_MC-ISAR_AS4XX_AURIX_TC23X_AC_V201/tricore_general/subdir.mk
-include Source/5_Bsw/Mcal/AURIX_MC-ISAR_AS4XX_AURIX_TC23X_AC_V201/spi/subdir.mk
-include Source/5_Bsw/Mcal/AURIX_MC-ISAR_AS4XX_AURIX_TC23X_AC_V201/pwm/subdir.mk
-include Source/5_Bsw/Mcal/AURIX_MC-ISAR_AS4XX_AURIX_TC23X_AC_V201/port/subdir.mk
-include Source/5_Bsw/Mcal/AURIX_MC-ISAR_AS4XX_AURIX_TC23X_AC_V201/mcu/subdir.mk
-include Source/5_Bsw/Mcal/AURIX_MC-ISAR_AS4XX_AURIX_TC23X_AC_V201/irq/subdir.mk
-include Source/5_Bsw/Mcal/AURIX_MC-ISAR_AS4XX_AURIX_TC23X_AC_V201/icu/subdir.mk
-include Source/5_Bsw/Mcal/AURIX_MC-ISAR_AS4XX_AURIX_TC23X_AC_V201/gpt/subdir.mk
-include Source/5_Bsw/Mcal/AURIX_MC-ISAR_AS4XX_AURIX_TC23X_AC_V201/flsloader_infineon_tricore/src/subdir.mk
-include Source/5_Bsw/Mcal/AURIX_MC-ISAR_AS4XX_AURIX_TC23X_AC_V201/fls_17_pmu_infineon_tricore/src/subdir.mk
-include Source/5_Bsw/Mcal/AURIX_MC-ISAR_AS4XX_AURIX_TC23X_AC_V201/fee_infineon_tricore/src/subdir.mk
-include Source/5_Bsw/Mcal/AURIX_MC-ISAR_AS4XX_AURIX_TC23X_AC_V201/ecum/subdir.mk
-include Source/5_Bsw/Mcal/AURIX_MC-ISAR_AS4XX_AURIX_TC23X_AC_V201/dio/subdir.mk
-include Source/5_Bsw/Mcal/AURIX_MC-ISAR_AS4XX_AURIX_TC23X_AC_V201/can/subdir.mk
-include Source/5_Bsw/Mcal/AURIX_MC-ISAR_AS4XX_AURIX_TC23X_AC_V201/adc/subdir.mk
-include Source/4_Cdd/CcpCustom/subdir.mk
-include Source/3_EcuAbs/Wdg/subdir.mk
-include Source/3_EcuAbs/Spi/subdir.mk
-include Source/3_EcuAbs/Nvm/subdir.mk
-include Source/3_EcuAbs/Mcu/subdir.mk
-include Source/3_EcuAbs/Gpt/subdir.mk
-include Source/3_EcuAbs/Dio/subdir.mk
-include Source/3_EcuAbs/Can/subdir.mk
-include Source/3_EcuAbs/Adc/subdir.mk
-include Source/3_EcuAbs/subdir.mk
-include Source/2_App/BswDcm/subdir.mk
-include Source/2_App/BswCom/subdir.mk
-include Source/1_Common/subdir.mk
-include Source/0_Rte/subdir.mk
-include Source/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
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


BUILD_ARTIFACT_NAME := SCU_Gen6
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_SIZE += \
SCU_Gen6.siz \


# All Target
all: main-build

# Main-build Target
main-build: SCU_Gen6.elf secondary-outputs

# Tool invocations
SCU_Gen6.elf: $(OBJS) makefile objects.mk $(OPTIONAL_TOOL_DEPS)
	@echo 'Building target: $@'
	@echo 'Invoking: TASKING Linker'
	cctc -lrt -lfp_fpu -lcs_fpu  -Wl-Oc -Wl-OL -Wl-Ot -Wl-Ox -Wl-Oy -Wl--map-file="SCU_Gen6.map" -Wl-mc -Wl-mf -Wl-mi -Wl-mk -Wl-ml -Wl-mm -Wl-md -Wl-mr -Wl-mu --no-warnings= -Wl--error-limit=42 --exceptions --strict --anachronisms --force-c++ -Ctc23x   -o"SCU_Gen6.elf" -Wl-o"SCU_Gen6.hex:IHEX" --lsl-core=vtc  --lsl-file=../SCU_Gen6_Training.lsl $(OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

SCU_Gen6.siz: SCU_Gen6.elf makefile objects.mk $(OPTIONAL_TOOL_DEPS)
	@echo 'Invoking: Print Size'
	elfsize  "SCU_Gen6.elf"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	find . ! -name "*.mk" ! -name "makefile" ! -name "." -type f -delete 
	-@echo ' '

secondary-outputs: $(SECONDARY_SIZE)

.PHONY: all clean dependents main-build

-include ../makefile.targets
