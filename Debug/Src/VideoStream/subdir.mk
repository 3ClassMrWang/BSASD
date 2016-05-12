################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Src/VideoStream/ReadVideo.cpp 

OBJS += \
./Src/VideoStream/ReadVideo.o 

CPP_DEPS += \
./Src/VideoStream/ReadVideo.d 


# Each subdirectory must supply rules for building sources it contributes
Src/VideoStream/%.o: ../Src/VideoStream/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -I/usr/include -I/usr/include/opencv -I/usr/include/opencv2 -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


