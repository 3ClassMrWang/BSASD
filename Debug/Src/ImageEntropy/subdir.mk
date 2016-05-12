################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Src/ImageEntropy/EntropyOfEntropyArea.cpp \
../Src/ImageEntropy/ImageCombine.cpp \
../Src/ImageEntropy/ImageResize.cpp 

OBJS += \
./Src/ImageEntropy/EntropyOfEntropyArea.o \
./Src/ImageEntropy/ImageCombine.o \
./Src/ImageEntropy/ImageResize.o 

CPP_DEPS += \
./Src/ImageEntropy/EntropyOfEntropyArea.d \
./Src/ImageEntropy/ImageCombine.d \
./Src/ImageEntropy/ImageResize.d 


# Each subdirectory must supply rules for building sources it contributes
Src/ImageEntropy/%.o: ../Src/ImageEntropy/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -I/usr/include -I/usr/include/opencv -I/usr/include/opencv2 -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


