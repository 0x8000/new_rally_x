################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/battle_city.c \
../src/bitmap.c \
../src/main.c \
../src/platform.c \
../src/tools.c 

LD_SRCS += \
../src/lscript.ld 

OBJS += \
./src/battle_city.o \
./src/bitmap.o \
./src/main.o \
./src/platform.o \
./src/tools.o 

C_DEPS += \
./src/battle_city.d \
./src/bitmap.d \
./src/main.d \
./src/platform.d \
./src/tools.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MicroBlaze gcc compiler'
	mb-gcc -Wall -O0 -g3 -c -fmessage-length=0 -I../../battle_city_soft_bsp/microblaze_0/include -mlittle-endian -mxl-barrel-shift -mxl-pattern-compare -mcpu=v8.50.b -mno-xl-soft-mul -Wl,--no-relax -ffunction-sections -fdata-sections -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


