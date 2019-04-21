################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../image/image.c 

OBJS += \
./image/image.o 

C_DEPS += \
./image/image.d 


# Each subdirectory must supply rules for building sources it contributes
image/%.o: ../image/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	mpicc -I/usr/include/mpi -I"/home/phantom/Desktop/MPITest/HybridChangeDetection" -O0 -mavx -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


