################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../DisplayManager.cpp \
../LocalizationManager.cpp \
../Map.cpp \
../PathPlanner.cpp \
../Robot.cpp \
../WaypointsManger.cpp \
../main.cpp 

OBJS += \
./DisplayManager.o \
./LocalizationManager.o \
./Map.o \
./PathPlanner.o \
./Robot.o \
./WaypointsManger.o \
./main.o 

CPP_DEPS += \
./DisplayManager.d \
./LocalizationManager.d \
./Map.d \
./PathPlanner.d \
./Robot.d \
./WaypointsManger.d \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


