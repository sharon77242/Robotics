################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../ConfigurationManager.cpp \
../LocalizationManager.cpp \
../Map.cpp \
../PathPlanner.cpp \
../RobotController.cpp \
../RobotMovement.cpp \
../WaypointManager.cpp \
../main.cpp 

OBJS += \
./ConfigurationManager.o \
./LocalizationManager.o \
./Map.o \
./PathPlanner.o \
./RobotController.o \
./RobotMovement.o \
./WaypointManager.o \
./main.o 

CPP_DEPS += \
./ConfigurationManager.d \
./LocalizationManager.d \
./Map.d \
./PathPlanner.d \
./RobotController.d \
./RobotMovement.d \
./WaypointManager.d \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D__GXX_EXPERIMENTAL_CXX0X__ -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


