################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../STM32_USB_HOST_Library/Core/src/usbh_core.c \
../STM32_USB_HOST_Library/Core/src/usbh_hcs.c \
../STM32_USB_HOST_Library/Core/src/usbh_ioreq.c \
../STM32_USB_HOST_Library/Core/src/usbh_stdreq.c 

OBJS += \
./STM32_USB_HOST_Library/Core/src/usbh_core.o \
./STM32_USB_HOST_Library/Core/src/usbh_hcs.o \
./STM32_USB_HOST_Library/Core/src/usbh_ioreq.o \
./STM32_USB_HOST_Library/Core/src/usbh_stdreq.o 

C_DEPS += \
./STM32_USB_HOST_Library/Core/src/usbh_core.d \
./STM32_USB_HOST_Library/Core/src/usbh_hcs.d \
./STM32_USB_HOST_Library/Core/src/usbh_ioreq.d \
./STM32_USB_HOST_Library/Core/src/usbh_stdreq.d 


# Each subdirectory must supply rules for building sources it contributes
STM32_USB_HOST_Library/Core/src/%.o: ../STM32_USB_HOST_Library/Core/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-move-loop-invariants -Wall -Wextra  -g3 -DUSE_HAL_DRIVER -DHSE_VALUE=8000000 -DSTM32F407VG -DSTM32F4XX -DUSE_STDPERIPH_DRIVER -DUSE_USB_OTG_FS -DMEDIA_USB_KEY -I"/Users/hilarye/dev/alarmclock/helix/real" -I"/Users/hilarye/dev/alarmclock/STM32F4-Discovery" -I"/Users/hilarye/dev/alarmclock/STM32_USB_HOST_Library/Core/inc" -I"/Users/hilarye/dev/alarmclock/conf" -I"/Users/hilarye/dev/alarmclock/src" -I"/Users/hilarye/dev/alarmclock/helix" -I"/Users/hilarye/dev/alarmclock/cmsis/inc" -I"/Users/hilarye/dev/alarmclock/STM32F4xx_StdPeriph_Driver/inc" -I"/Users/hilarye/dev/alarmclock/helix/pub" -I"/Users/hilarye/dev/alarmclock/STM32_USB_OTG_Driver/inc" -I"/Users/hilarye/dev/alarmclock/STM32_USB_HOST_Library/Class/MSC/inc" -I"/Users/hilarye/dev/alarmclock/include" -I"/Users/hilarye/dev/alarmclock/fat_fs/inc" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


