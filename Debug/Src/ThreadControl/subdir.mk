################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Src/ThreadControl/SystemFlag.cpp \
../Src/ThreadControl/SystemInit.cpp 

OBJS += \
./Src/ThreadControl/SystemFlag.o \
./Src/ThreadControl/SystemInit.o 

CPP_DEPS += \
./Src/ThreadControl/SystemFlag.d \
./Src/ThreadControl/SystemInit.d 


# Each subdirectory must supply rules for building sources it contributes
Src/ThreadControl/%.o: ../Src/ThreadControl/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -I/usr/include -I/usr/include/opencv -I/usr/include/opencv2 -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


