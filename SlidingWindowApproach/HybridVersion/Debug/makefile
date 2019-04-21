################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include vlfeat-0.9.20/subdir.mk
-include image/subdir.mk
-include gaussian/subdir.mk
-include distance/subdir.mk
-include cluster/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: HybridChangeDetection

# Tool invocations
HybridChangeDetection: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C Linker'
	mpicc -L/usr/include/lib -o "HybridChangeDetection" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) HybridChangeDetection
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
