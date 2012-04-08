################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../cache.c \
../http.c \
../list.c \
../server.c \
../socketio.c \
../use_threads.c 

OBJS += \
./cache.o \
./http.o \
./list.o \
./server.o \
./socketio.o \
./use_threads.o 

C_DEPS += \
./cache.d \
./http.d \
./list.d \
./server.d \
./socketio.d \
./use_threads.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


