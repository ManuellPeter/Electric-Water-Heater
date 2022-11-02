################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../MCAL/EXTI/EXTI_config.c \
../MCAL/EXTI/EXTI_prog.c 

OBJS += \
./MCAL/EXTI/EXTI_config.o \
./MCAL/EXTI/EXTI_prog.o 

C_DEPS += \
./MCAL/EXTI/EXTI_config.d \
./MCAL/EXTI/EXTI_prog.d 


# Each subdirectory must supply rules for building sources it contributes
MCAL/EXTI/%.o: ../MCAL/EXTI/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"C:\Users\manue\Desktop\Github Repository\Electric-Water-Heater\NG\APP" -I"C:\Users\manue\Desktop\Github Repository\Electric-Water-Heater\NG" -I"C:\Users\manue\Desktop\Github Repository\Electric-Water-Heater\NG\HAL" -I"C:\Users\manue\Desktop\Github Repository\Electric-Water-Heater\NG\LIBRARY" -I"C:\Users\manue\Desktop\Github Repository\Electric-Water-Heater\NG\MCAL" -I"C:\Users\manue\Desktop\Github Repository\Electric-Water-Heater\NG\HAL\Temp_sensor" -I"C:\Users\manue\Desktop\Github Repository\Electric-Water-Heater\NG\HAL\two_7seg_decoder" -I"C:\Users\manue\Desktop\Github Repository\Electric-Water-Heater\NG\MCAL\ADC" -I"C:\Users\manue\Desktop\Github Repository\Electric-Water-Heater\NG\MCAL\DIO" -I"C:\Users\manue\Desktop\Github Repository\Electric-Water-Heater\NG\MCAL\EEPROM" -I"C:\Users\manue\Desktop\Github Repository\Electric-Water-Heater\NG\MCAL\EXTI" -I"C:\Users\manue\Desktop\Github Repository\Electric-Water-Heater\NG\MCAL\GIE" -I"C:\Users\manue\Desktop\Github Repository\Electric-Water-Heater\NG\MCAL\TIMER2" -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


