################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../STM32_USB_HOST_Library/Class/HID/src/usbh_hid_keybd.c \
../STM32_USB_HOST_Library/Class/HID/src/usbh_hid_mouse.c 

OBJS += \
./STM32_USB_HOST_Library/Class/HID/src/usbh_hid_keybd.o \
./STM32_USB_HOST_Library/Class/HID/src/usbh_hid_mouse.o 

C_DEPS += \
./STM32_USB_HOST_Library/Class/HID/src/usbh_hid_keybd.d \
./STM32_USB_HOST_Library/Class/HID/src/usbh_hid_mouse.d 


# Each subdirectory must supply rules for building sources it contributes
STM32_USB_HOST_Library/Class/HID/src/%.o: ../STM32_USB_HOST_Library/Class/HID/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DSTM32F407xx -DUSE_HAL_DRIVER -DHSE_VALUE=8000000 -DSTM32F407VG -DSTM32F4XX -DUSE_STDPERIPH_DRIVER -DUSE_USB_OTG_FS -DMEDIA_USB_KEY -I"/home/brent/workspace/alarmclock/STM32_USB_HOST_Library/Class/MSC/inc" -I"/home/brent/workspace/alarmclock/STM32_USB_OTG_Driver/inc" -I/home/brent/workspace/alarmclock/include -I"/home/brent/workspace/alarmclock/helix/pub" -I"/home/brent/workspace/alarmclock/STM32F4xx_StdPeriph_Driver/inc" -I"/home/brent/workspace/alarmclock/cmsis/inc" -I"/home/brent/workspace/alarmclock/helix" -I"/home/brent/workspace/alarmclock/src" -I"/home/brent/workspace/alarmclock/conf" -I"/home/brent/workspace/alarmclock/STM32_USB_HOST_Library/Core/inc" -I"/home/brent/workspace/alarmclock/STM32F4-Discovery" -I"/home/brent/workspace/alarmclock/fat_fs/inc" -I"/home/brent/workspace/alarmclock/helix/real" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


