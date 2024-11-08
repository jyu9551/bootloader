################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Source/5_Bsw/Service/SchM/SchM.c 

OBJS += \
./Source/5_Bsw/Service/SchM/SchM.o 

COMPILED_SRCS += \
./Source/5_Bsw/Service/SchM/SchM.src 

C_DEPS += \
./Source/5_Bsw/Service/SchM/SchM.d 


# Each subdirectory must supply rules for building sources it contributes
Source/5_Bsw/Service/SchM/%.src: ../Source/5_Bsw/Service/SchM/%.c Source/5_Bsw/Service/SchM/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING C/C++ Compiler'
	cctc -D_TASKING_C_TRICORE_ "-fD:/OTA/OTA_BOOT_8/Training/TASKING_C_C___Compiler-Include_paths.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc23x -o "$@"  "$<"  -cs --misrac-version=2012 -N0 -Z0 -Y0 2>&1;
	@echo 'Finished building: $<'
	@echo ' '

Source/5_Bsw/Service/SchM/%.o: ./Source/5_Bsw/Service/SchM/%.src Source/5_Bsw/Service/SchM/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<" --list-format=L1 --optimize=gs
	@echo 'Finished building: $<'
	@echo ' '


