################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
LD_SRCS += \
../src/lscript.ld 

C_SRCS += \
../src/main.c \
../src/rtos.c \
../src/scheduler.c \
../src/task.c 

OBJS += \
./src/main.o \
./src/rtos.o \
./src/scheduler.o \
./src/task.o 

C_DEPS += \
./src/main.d \
./src/rtos.d \
./src/scheduler.d \
./src/task.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM v7 gcc compiler'
	arm-none-eabi-gcc -Wall -O0 -g3 -I"D:\RTES\2025H1400069G\Expensive_RTOS\include" -c -fmessage-length=0 -MT"$@" -mcpu=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard -ID:/RTES/2025H1400069G/Expensive_RTOS/design_1_wrapper/export/design_1_wrapper/sw/design_1_wrapper/domain_ps7_cortexa9_0/bspinclude/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


