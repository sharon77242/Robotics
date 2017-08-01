################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/AStarPredicate.cpp \
../src/Brain.cpp \
../src/Colors.cpp \
../src/ConfigurationManager.cpp \
../src/Converter.cpp \
../src/Grid.cpp \
../src/LocalizationManager.cpp \
../src/Location.cpp \
../src/Map.cpp \
../src/MoveForward.cpp \
../src/MoveToPoint.cpp \
../src/NewRobot.cpp \
../src/Node.cpp \
../src/Particle.cpp \
../src/PathPlanner.cpp \
../src/Robot.cpp \
../src/TurnInPlace.cpp \
../src/Vector2d.cpp \
../src/WaypointsManager.cpp \
../src/Wheels.cpp \
../src/lodepng.cpp \
../src/pngUtil.cpp 

OBJS += \
./src/AStarPredicate.o \
./src/Brain.o \
./src/Colors.o \
./src/ConfigurationManager.o \
./src/Converter.o \
./src/Grid.o \
./src/LocalizationManager.o \
./src/Location.o \
./src/Map.o \
./src/MoveForward.o \
./src/MoveToPoint.o \
./src/NewRobot.o \
./src/Node.o \
./src/Particle.o \
./src/PathPlanner.o \
./src/Robot.o \
./src/TurnInPlace.o \
./src/Vector2d.o \
./src/WaypointsManager.o \
./src/Wheels.o \
./src/lodepng.o \
./src/pngUtil.o 

CPP_DEPS += \
./src/AStarPredicate.d \
./src/Brain.d \
./src/Colors.d \
./src/ConfigurationManager.d \
./src/Converter.d \
./src/Grid.d \
./src/LocalizationManager.d \
./src/Location.d \
./src/Map.d \
./src/MoveForward.d \
./src/MoveToPoint.d \
./src/NewRobot.d \
./src/Node.d \
./src/Particle.d \
./src/PathPlanner.d \
./src/Robot.d \
./src/TurnInPlace.d \
./src/Vector2d.d \
./src/WaypointsManager.d \
./src/Wheels.d \
./src/lodepng.d \
./src/pngUtil.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/player-2.0 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


