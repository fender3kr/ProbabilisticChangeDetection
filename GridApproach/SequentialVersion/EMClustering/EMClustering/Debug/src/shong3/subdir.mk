################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/shong3/KLD.c \
../src/shong3/app.c \
../src/shong3/gaussian.c \
../src/shong3/loader.c 

OBJS += \
./src/shong3/KLD.o \
./src/shong3/app.o \
./src/shong3/gaussian.o \
./src/shong3/loader.o 

C_DEPS += \
./src/shong3/KLD.d \
./src/shong3/app.d \
./src/shong3/gaussian.d \
./src/shong3/loader.d 


# Each subdirectory must supply rules for building sources it contributes
src/shong3/%.o: ../src/shong3/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/phantom/Desktop/Team Project/Grid-based/Sequential Version/EMClustering/EMClustering/Include" -I/usr/include/opencv -O0 -mavx -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


