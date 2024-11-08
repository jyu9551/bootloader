################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Source/3_EcuAbs/Can/CanIf.c \
../Source/3_EcuAbs/Can/CanIf_Cbk.c \
../Source/3_EcuAbs/Can/EcuAbsCan.c 

OBJS += \
./Source/3_EcuAbs/Can/CanIf.o \
./Source/3_EcuAbs/Can/CanIf_Cbk.o \
./Source/3_EcuAbs/Can/EcuAbsCan.o 

COMPILED_SRCS += \
./Source/3_EcuAbs/Can/CanIf.src \
./Source/3_EcuAbs/Can/CanIf_Cbk.src \
./Source/3_EcuAbs/Can/EcuAbsCan.src 

C_DEPS += \
./Source/3_EcuAbs/Can/CanIf.d \
./Source/3_EcuAbs/Can/CanIf_Cbk.d \
./Source/3_EcuAbs/Can/EcuAbsCan.d 


# Each subdirectory must supply rules for building sources it contributes
Source/3_EcuAbs/Can/%.src: ../Source/3_EcuAbs/Can/%.c Source/3_EcuAbs/Can/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING C/C++ Compiler'
	cctc -D_TASKING_C_TRICORE_ "-fD:/OTA/OTA_APP_3/Training/TASKING_C_C___Compiler-Include_paths.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc23x -o "$@"  "$<"  -cs --misrac-version=2012 -N0 -Z0 -Y0 2>&1;
	@echo 'Finished building: $<'
	@echo ' '

Source/3_EcuAbs/Can/%.o: ./Source/3_EcuAbs/Can/%.src Source/3_EcuAbs/Can/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<" --list-format=L1 --optimize=gs
	@echo 'Finished building: $<'
	@echo ' '


