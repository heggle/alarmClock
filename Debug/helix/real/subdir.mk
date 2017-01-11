################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../helix/real/bitstream.c \
../helix/real/buffers.c \
../helix/real/dct32.c \
../helix/real/dequant.c \
../helix/real/dqchan.c \
../helix/real/huffman.c \
../helix/real/hufftabs.c \
../helix/real/imdct.c \
../helix/real/polyphase.c \
../helix/real/scalfact.c \
../helix/real/stproc.c \
../helix/real/subband.c \
../helix/real/trigtabs_fixpt.c 

OBJS += \
./helix/real/bitstream.o \
./helix/real/buffers.o \
./helix/real/dct32.o \
./helix/real/dequant.o \
./helix/real/dqchan.o \
./helix/real/huffman.o \
./helix/real/hufftabs.o \
./helix/real/imdct.o \
./helix/real/polyphase.o \
./helix/real/scalfact.o \
./helix/real/stproc.o \
./helix/real/subband.o \
./helix/real/trigtabs_fixpt.o 

C_DEPS += \
./helix/real/bitstream.d \
./helix/real/buffers.d \
./helix/real/dct32.d \
./helix/real/dequant.d \
./helix/real/dqchan.d \
./helix/real/huffman.d \
./helix/real/hufftabs.d \
./helix/real/imdct.d \
./helix/real/polyphase.d \
./helix/real/scalfact.d \
./helix/real/stproc.d \
./helix/real/subband.d \
./helix/real/trigtabs_fixpt.d 


# Each subdirectory must supply rules for building sources it contributes
helix/real/%.o: ../helix/real/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-move-loop-invariants -Wall -Wextra  -g3 -DUSE_HAL_DRIVER -DHSE_VALUE=8000000 -DSTM32F407VG -DSTM32F4XX -DUSE_STDPERIPH_DRIVER -DUSE_USB_OTG_FS -DMEDIA_USB_KEY -I"/Users/hilarye/dev/alarmclock/helix/real" -I"/Users/hilarye/dev/alarmclock/STM32F4-Discovery" -I"/Users/hilarye/dev/alarmclock/STM32_USB_HOST_Library/Core/inc" -I"/Users/hilarye/dev/alarmclock/conf" -I"/Users/hilarye/dev/alarmclock/src" -I"/Users/hilarye/dev/alarmclock/helix" -I"/Users/hilarye/dev/alarmclock/cmsis/inc" -I"/Users/hilarye/dev/alarmclock/STM32F4xx_StdPeriph_Driver/inc" -I"/Users/hilarye/dev/alarmclock/helix/pub" -I"/Users/hilarye/dev/alarmclock/STM32_USB_OTG_Driver/inc" -I"/Users/hilarye/dev/alarmclock/STM32_USB_HOST_Library/Class/MSC/inc" -I"/Users/hilarye/dev/alarmclock/include" -I"/Users/hilarye/dev/alarmclock/fat_fs/inc" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


