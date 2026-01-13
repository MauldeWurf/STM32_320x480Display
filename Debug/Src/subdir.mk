################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/exti.c \
../Src/gpio.c \
../Src/main.c \
../Src/myGFX.c \
../Src/sbc_lcd01.c \
../Src/spi.c \
../Src/spi_dma.c \
../Src/syscalls.c \
../Src/sysmem.c \
../Src/systick.c 

OBJS += \
./Src/exti.o \
./Src/gpio.o \
./Src/main.o \
./Src/myGFX.o \
./Src/sbc_lcd01.o \
./Src/spi.o \
./Src/spi_dma.o \
./Src/syscalls.o \
./Src/sysmem.o \
./Src/systick.o 

C_DEPS += \
./Src/exti.d \
./Src/gpio.d \
./Src/main.d \
./Src/myGFX.d \
./Src/sbc_lcd01.d \
./Src/spi.d \
./Src/spi_dma.d \
./Src/syscalls.d \
./Src/sysmem.d \
./Src/systick.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DNUCLEO_F411RE -DSTM32 -DSTM32F4 -DSTM32F411RETx -DSTMF411_DISCO -DSTM32F411VETx -DSTM32F411xE -c -I../Inc -I"C:/Users/stevo/STM32CubeIDE/workspace_1.19.0/02012026_SBC_LCD01-F411/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/stevo/STM32CubeIDE/workspace_1.19.0/02012026_SBC_LCD01-F411/CMSIS/Include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/exti.cyclo ./Src/exti.d ./Src/exti.o ./Src/exti.su ./Src/gpio.cyclo ./Src/gpio.d ./Src/gpio.o ./Src/gpio.su ./Src/main.cyclo ./Src/main.d ./Src/main.o ./Src/main.su ./Src/myGFX.cyclo ./Src/myGFX.d ./Src/myGFX.o ./Src/myGFX.su ./Src/sbc_lcd01.cyclo ./Src/sbc_lcd01.d ./Src/sbc_lcd01.o ./Src/sbc_lcd01.su ./Src/spi.cyclo ./Src/spi.d ./Src/spi.o ./Src/spi.su ./Src/spi_dma.cyclo ./Src/spi_dma.d ./Src/spi_dma.o ./Src/spi_dma.su ./Src/syscalls.cyclo ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.cyclo ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su ./Src/systick.cyclo ./Src/systick.d ./Src/systick.o ./Src/systick.su

.PHONY: clean-Src

