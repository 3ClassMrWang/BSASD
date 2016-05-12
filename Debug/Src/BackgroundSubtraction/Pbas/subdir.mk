################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Src/BackgroundSubtraction/Pbas/PBAS.cpp \
../Src/BackgroundSubtraction/Pbas/PixelBasedAdaptiveSegmenter.cpp 

OBJS += \
./Src/BackgroundSubtraction/Pbas/PBAS.o \
./Src/BackgroundSubtraction/Pbas/PixelBasedAdaptiveSegmenter.o 

CPP_DEPS += \
./Src/BackgroundSubtraction/Pbas/PBAS.d \
./Src/BackgroundSubtraction/Pbas/PixelBasedAdaptiveSegmenter.d 


# Each subdirectory must supply rules for building sources it contributes
Src/BackgroundSubtraction/Pbas/%.o: ../Src/BackgroundSubtraction/Pbas/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -I/usr/include -I/usr/include/opencv -I/usr/include/opencv2 -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


