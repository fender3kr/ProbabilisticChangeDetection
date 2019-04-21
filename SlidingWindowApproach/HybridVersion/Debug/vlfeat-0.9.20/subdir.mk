################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../vlfeat-0.9.20/aib.c \
../vlfeat-0.9.20/array.c \
../vlfeat-0.9.20/covdet.c \
../vlfeat-0.9.20/dsift.c \
../vlfeat-0.9.20/fisher.c \
../vlfeat-0.9.20/generic.c \
../vlfeat-0.9.20/getopt_long.c \
../vlfeat-0.9.20/gmm.c \
../vlfeat-0.9.20/hikmeans.c \
../vlfeat-0.9.20/hog.c \
../vlfeat-0.9.20/homkermap.c \
../vlfeat-0.9.20/host.c \
../vlfeat-0.9.20/ikmeans.c \
../vlfeat-0.9.20/imopv.c \
../vlfeat-0.9.20/imopv_sse2.c \
../vlfeat-0.9.20/kdtree.c \
../vlfeat-0.9.20/kmeans.c \
../vlfeat-0.9.20/lbp.c \
../vlfeat-0.9.20/liop.c \
../vlfeat-0.9.20/mathop.c \
../vlfeat-0.9.20/mathop_avx.c \
../vlfeat-0.9.20/mathop_sse2.c \
../vlfeat-0.9.20/mser.c \
../vlfeat-0.9.20/pgm.c \
../vlfeat-0.9.20/quickshift.c \
../vlfeat-0.9.20/random.c \
../vlfeat-0.9.20/rodrigues.c \
../vlfeat-0.9.20/scalespace.c \
../vlfeat-0.9.20/sift.c \
../vlfeat-0.9.20/slic.c \
../vlfeat-0.9.20/stringop.c \
../vlfeat-0.9.20/svm.c \
../vlfeat-0.9.20/svmdataset.c \
../vlfeat-0.9.20/vlad.c 

OBJS += \
./vlfeat-0.9.20/aib.o \
./vlfeat-0.9.20/array.o \
./vlfeat-0.9.20/covdet.o \
./vlfeat-0.9.20/dsift.o \
./vlfeat-0.9.20/fisher.o \
./vlfeat-0.9.20/generic.o \
./vlfeat-0.9.20/getopt_long.o \
./vlfeat-0.9.20/gmm.o \
./vlfeat-0.9.20/hikmeans.o \
./vlfeat-0.9.20/hog.o \
./vlfeat-0.9.20/homkermap.o \
./vlfeat-0.9.20/host.o \
./vlfeat-0.9.20/ikmeans.o \
./vlfeat-0.9.20/imopv.o \
./vlfeat-0.9.20/imopv_sse2.o \
./vlfeat-0.9.20/kdtree.o \
./vlfeat-0.9.20/kmeans.o \
./vlfeat-0.9.20/lbp.o \
./vlfeat-0.9.20/liop.o \
./vlfeat-0.9.20/mathop.o \
./vlfeat-0.9.20/mathop_avx.o \
./vlfeat-0.9.20/mathop_sse2.o \
./vlfeat-0.9.20/mser.o \
./vlfeat-0.9.20/pgm.o \
./vlfeat-0.9.20/quickshift.o \
./vlfeat-0.9.20/random.o \
./vlfeat-0.9.20/rodrigues.o \
./vlfeat-0.9.20/scalespace.o \
./vlfeat-0.9.20/sift.o \
./vlfeat-0.9.20/slic.o \
./vlfeat-0.9.20/stringop.o \
./vlfeat-0.9.20/svm.o \
./vlfeat-0.9.20/svmdataset.o \
./vlfeat-0.9.20/vlad.o 

C_DEPS += \
./vlfeat-0.9.20/aib.d \
./vlfeat-0.9.20/array.d \
./vlfeat-0.9.20/covdet.d \
./vlfeat-0.9.20/dsift.d \
./vlfeat-0.9.20/fisher.d \
./vlfeat-0.9.20/generic.d \
./vlfeat-0.9.20/getopt_long.d \
./vlfeat-0.9.20/gmm.d \
./vlfeat-0.9.20/hikmeans.d \
./vlfeat-0.9.20/hog.d \
./vlfeat-0.9.20/homkermap.d \
./vlfeat-0.9.20/host.d \
./vlfeat-0.9.20/ikmeans.d \
./vlfeat-0.9.20/imopv.d \
./vlfeat-0.9.20/imopv_sse2.d \
./vlfeat-0.9.20/kdtree.d \
./vlfeat-0.9.20/kmeans.d \
./vlfeat-0.9.20/lbp.d \
./vlfeat-0.9.20/liop.d \
./vlfeat-0.9.20/mathop.d \
./vlfeat-0.9.20/mathop_avx.d \
./vlfeat-0.9.20/mathop_sse2.d \
./vlfeat-0.9.20/mser.d \
./vlfeat-0.9.20/pgm.d \
./vlfeat-0.9.20/quickshift.d \
./vlfeat-0.9.20/random.d \
./vlfeat-0.9.20/rodrigues.d \
./vlfeat-0.9.20/scalespace.d \
./vlfeat-0.9.20/sift.d \
./vlfeat-0.9.20/slic.d \
./vlfeat-0.9.20/stringop.d \
./vlfeat-0.9.20/svm.d \
./vlfeat-0.9.20/svmdataset.d \
./vlfeat-0.9.20/vlad.d 


# Each subdirectory must supply rules for building sources it contributes
vlfeat-0.9.20/%.o: ../vlfeat-0.9.20/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	mpicc -I/usr/include/mpi -I"/home/phantom/Desktop/MPITest/HybridChangeDetection" -O0 -mavx -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


