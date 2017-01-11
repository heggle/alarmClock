################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../STM32F4xx_StdPeriph_Driver/src/misc.c \
../STM32F4xx_StdPeriph_Driver/src/stm32f4xx_adc.c \
../STM32F4xx_StdPeriph_Driver/src/stm32f4xx_dac.c \
../STM32F4xx_StdPeriph_Driver/src/stm32f4xx_dma.c \
../STM32F4xx_StdPeriph_Driver/src/stm32f4xx_exti.c \
../STM32F4xx_StdPeriph_Driver/src/stm32f4xx_gpio.c \
../STM32F4xx_StdPeriph_Driver/src/stm32f4xx_i2c.c \
../STM32F4xx_StdPeriph_Driver/src/stm32f4xx_pwr.c \
../STM32F4xx_StdPeriph_Driver/src/stm32f4xx_rcc.c \
../STM32F4xx_StdPeriph_Driver/src/stm32f4xx_rtc.c \
../STM32F4xx_StdPeriph_Driver/src/stm32f4xx_spi.c \
../STM32F4xx_StdPeriph_Driver/src/stm32f4xx_syscfg.c \
../STM32F4xx_StdPeriph_Driver/src/stm32f4xx_tim.c \
../STM32F4xx_StdPeriph_Driver/src/stm32f4xx_usart.c 

OBJS += \
./STM32F4xx_StdPeriph_Driver/src/misc.o \
./STM32F4xx_StdPeriph_Driver/src/stm32f4xx_adc.o \
./STM32F4xx_StdPeriph_Driver/src/stm32f4xx_dac.o \
./STM32F4xx_StdPeriph_Driver/src/stm32f4xx_dma.o \
./STM32F4xx_StdPeriph_Driver/src/stm32f4xx_exti.o \
./STM32F4xx_StdPeriph_Driver/src/stm32f4xx_gpio.o \
./STM32F4xx_StdPeriph_Driver/src/stm32f4xx_i2c.o \
./STM32F4xx_StdPeriph_Driver/src/stm32f4xx_pwr.o \
./STM32F4xx_StdPeriph_Driver/src/stm32f4xx_rcc.o \
./STM32F4xx_StdPeriph_Driver/src/stm32f4xx_rtc.o \
./STM32F4xx_StdPeriph_Driver/src/stm32f4xx_spi.o \
./STM32F4xx_StdPeriph_Driver/src/stm32f4xx_syscfg.o \
./STM32F4xx_StdPeriph_Driver/src/stm32f4xx_tim.o \
./STM32F4xx_StdPeriph_Driver/src/stm32f4xx_usart.o 

C_DEPS += \
./STM32F4xx_StdPeriph_Driver/src/misc.d \
./STM32F4xx_StdPeriph_Driver/src/stm32f4xx_adc.d \
./STM32F4xx_StdPeriph_Driver/src/stm32f4xx_dac.d \
./STM32F4xx_StdPeriph_Driver/src/stm32f4xx_dma.d \
./STM32F4xx_StdPeriph_Driver/src/stm32f4xx_exti.d \
./STM32F4xx_StdPeriph_Driver/src/stm32f4xx_gpio.d \
./STM32F4xx_StdPeriph_Driver/src/stm32f4xx_i2c.d \
./STM32F4xx_StdPeriph_Driver/src/stm32f4xx_pwr.d \
./STM32F4xx_StdPeriph_Driver/src/stm32f4xx_rcc.d \
./STM32F4xx_StdPeriph_Driver/src/stm32f4xx_rtc.d \
./STM32F4xx_StdPeriph_Driver/src/stm32f4xx_spi.d \
./STM32F4xx_StdPeriph_Driver/src/stm32f4xx_syscfg.d \
./STM32F4xx_StdPeriph_Driver/src/stm32f4xx_tim.d \
./STM32F4xx_StdPeriph_Driver/src/stm32f4xx_usart.d 


# Each subdirectory must supply rules for building sources it contributes
STM32F4xx_StdPeriph_Driver/src/%.o: ../STM32F4xx_StdPeriph_Driver/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-move-loop-invariants -Wall -Wextra  -g3 -DUSE_HAL_DRIVER -DHSE_VALUE=8000000 -DSTM32F407VG -DSTM32F4XX -DUSE_STDPERIPH_DRIVER -DUSE_USB_OTG_FS -DMEDIA_USB_KEY -I"/Users/hilarye/dev/alarmclock/helix/real" -I"/Users/hilarye/dev/alarmclock/STM32F4-Discovery" -I"/Users/hilarye/dev/alarmclock/STM32_USB_HOST_Library/Core/inc" -I"/Users/hilarye/dev/alarmclock/conf" -I"/Users/hilarye/dev/alarmclock/src" -I"/Users/hilarye/dev/alarmclock/helix" -I"/Users/hilarye/dev/alarmclock/cmsis/inc" -I"/Users/hilarye/dev/alarmclock/STM32F4xx_StdPeriph_Driver/inc" -I"/Users/hilarye/dev/alarmclock/helix/pub" -I"/Users/hilarye/dev/alarmclock/STM32_USB_OTG_Driver/inc" -I"/Users/hilarye/dev/alarmclock/STM32_USB_HOST_Library/Class/MSC/inc" -I"/Users/hilarye/dev/alarmclock/include" -I"/Users/hilarye/dev/alarmclock/fat_fs/inc" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


