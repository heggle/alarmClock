################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Audio.c \
../src/audioMP3.c \
../src/main.c \
../src/stm32f4xx_it.c \
../src/syscalls.c \
../src/system_stm32f4xx.c \
../src/timeKeeping.c \
../src/usb_bsp.c \
../src/usbh_usr.c 

OBJS += \
./src/Audio.o \
./src/audioMP3.o \
./src/main.o \
./src/stm32f4xx_it.o \
./src/syscalls.o \
./src/system_stm32f4xx.o \
./src/timeKeeping.o \
./src/usb_bsp.o \
./src/usbh_usr.o 

C_DEPS += \
./src/Audio.d \
./src/audioMP3.d \
./src/main.d \
./src/stm32f4xx_it.d \
./src/syscalls.d \
./src/system_stm32f4xx.d \
./src/timeKeeping.d \
./src/usb_bsp.d \
./src/usbh_usr.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-move-loop-invariants -Wall -Wextra  -g3 -DUSE_HAL_DRIVER -DHSE_VALUE=8000000 -DSTM32F407VG -DSTM32F4XX -DUSE_STDPERIPH_DRIVER -DUSE_USB_OTG_FS -DMEDIA_USB_KEY -I"/Users/hilarye/dev/alarmclock/helix/real" -I"/Users/hilarye/dev/alarmclock/STM32F4-Discovery" -I"/Users/hilarye/dev/alarmclock/STM32_USB_HOST_Library/Core/inc" -I"/Users/hilarye/dev/alarmclock/conf" -I"/Users/hilarye/dev/alarmclock/src" -I"/Users/hilarye/dev/alarmclock/helix" -I"/Users/hilarye/dev/alarmclock/cmsis/inc" -I"/Users/hilarye/dev/alarmclock/STM32F4xx_StdPeriph_Driver/inc" -I"/Users/hilarye/dev/alarmclock/helix/pub" -I"/Users/hilarye/dev/alarmclock/STM32_USB_OTG_Driver/inc" -I"/Users/hilarye/dev/alarmclock/STM32_USB_HOST_Library/Class/MSC/inc" -I"/Users/hilarye/dev/alarmclock/include" -I"/Users/hilarye/dev/alarmclock/fat_fs/inc" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


