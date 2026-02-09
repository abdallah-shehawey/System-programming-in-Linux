################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/media/abdallah-shehawey/Local-Disk2/System_Programming_under_Linux/Phase_2/01_Code/RTOS/STM32CubeWBA/Drivers/BSP/STM32WBAxx_Nucleo/stm32wbaxx_nucleo.c 

OBJS += \
./Drivers/BSP/STM32WBAxx_Nucleo/stm32wbaxx_nucleo.o 

C_DEPS += \
./Drivers/BSP/STM32WBAxx_Nucleo/stm32wbaxx_nucleo.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/STM32WBAxx_Nucleo/stm32wbaxx_nucleo.o: /media/abdallah-shehawey/Local-Disk2/System_Programming_under_Linux/Phase_2/01_Code/RTOS/STM32CubeWBA/Drivers/BSP/STM32WBAxx_Nucleo/stm32wbaxx_nucleo.c Drivers/BSP/STM32WBAxx_Nucleo/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32WBA55xx -c -I../../Inc -I../../../../../../../Drivers/STM32WBAxx_HAL_Driver/Inc -I../../../../../../../Drivers/STM32WBAxx_HAL_Driver/Inc/Legacy -I../../../../../../../Drivers/CMSIS/Device/ST/STM32WBAxx/Include -I../../../../../../../Drivers/CMSIS/Include -I../../../../../../../Drivers/BSP/STM32WBAxx_Nucleo -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-BSP-2f-STM32WBAxx_Nucleo

clean-Drivers-2f-BSP-2f-STM32WBAxx_Nucleo:
	-$(RM) ./Drivers/BSP/STM32WBAxx_Nucleo/stm32wbaxx_nucleo.cyclo ./Drivers/BSP/STM32WBAxx_Nucleo/stm32wbaxx_nucleo.d ./Drivers/BSP/STM32WBAxx_Nucleo/stm32wbaxx_nucleo.o ./Drivers/BSP/STM32WBAxx_Nucleo/stm32wbaxx_nucleo.su

.PHONY: clean-Drivers-2f-BSP-2f-STM32WBAxx_Nucleo

