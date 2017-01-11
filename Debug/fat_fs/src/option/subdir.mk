################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../fat_fs/src/option/syncobj.c 

OBJS += \
./fat_fs/src/option/syncobj.o 

C_DEPS += \
./fat_fs/src/option/syncobj.d 


# Each subdirectory must supply rules for building sources it contributes
fat_fs/src/option/%.o: ../fat_fs/src/option/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DSTM32F407xx -DUSE_HAL_DRIVER -DHSE_VALUE=8000000 -DSTM32F407VG -DSTM32F4XX -DUSE_STDPERIPH_DRIVER -DUSE_USB_OTG_FS -DMEDIA_USB_KEY -I"/home/brent/workspace/alarmclock/STM32_USB_HOST_Library/Class/MSC/inc" -I"/home/brent/workspace/alarmclock/STM32_USB_OTG_Driver/inc" -I/home/brent/workspace/alarmclock/include -I"/home/brent/workspace/alarmclock/helix/pub" -I"/home/brent/workspace/alarmclock/STM32F4xx_StdPeriph_Driver/inc" -I"/home/brent/workspace/alarmclock/cmsis/inc" -I"/home/brent/workspace/alarmclock/helix" -I"/home/brent/workspace/alarmclock/src" -I"/home/brent/workspace/alarmclock/conf" -I"/home/brent/workspace/alarmclock/STM32_USB_HOST_Library/Core/inc" -I"/home/brent/workspace/alarmclock/STM32F4-Discovery" -I"/home/brent/workspace/alarmclock/fat_fs/inc" -I"/home/brent/workspace/alarmclock/helix/real" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


