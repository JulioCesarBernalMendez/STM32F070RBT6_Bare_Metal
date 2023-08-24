TOOLCHAIN = arm-none-eabi
PROCESSOR = cortex-m0
AFLAGS    = -mcpu=cortex-m0 -mthumb -mfloat-abi=soft
CFLAGS    = -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -ggdb -Wall -O0 -std=c99 -ffunction-sections -fdata-sections # -MMD -MP
LDFLAGS   = -Wl,--gc-sections --specs=rdimon.specs --specs=nano.specs -Wl,--no-warn-rwx-segment -Wl,-Map=final.map

INCLUDES  = -I CMSIS/Device -I CMSIS/Include

all:final

final:final.elf
	$(TOOLCHAIN)-objdump -S $< > final.list
	$(TOOLCHAIN)-size --format=berkeley $<

final.elf:startup_stm32f070xb.o system_stm32f0xx.o main.o gpio.o interrupts.o timer.o i2c.o lcd.o systick.o spi.o accelerometer.o print_lcd.o
	$(TOOLCHAIN)-gcc $(AFLAGS) $(LDFLAGS) -T linker.ld -o $@ $^

print_lcd.o:print_lcd.c
	$(TOOLCHAIN)-gcc $(CFLAGS) $(INCLUDES) -o $@ -c $<

accelerometer.o:accelerometer.c
	$(TOOLCHAIN)-gcc $(CFLAGS) $(INCLUDES) -o $@ -c $<

spi.o:spi.c
	$(TOOLCHAIN)-gcc $(CFLAGS) $(INCLUDES) -o $@ -c $<
	
systick.o:systick.c
	$(TOOLCHAIN)-gcc $(CFLAGS) $(INCLUDES) -o $@ -c $<

lcd.o:lcd.c
	$(TOOLCHAIN)-gcc $(CFLAGS) $(INCLUDES) -o $@ -c $<

i2c.o:i2c.c
	$(TOOLCHAIN)-gcc $(CFLAGS) $(INCLUDES) -o $@ -c $<

timer.o:timer.c
	$(TOOLCHAIN)-gcc $(CFLAGS) $(INCLUDES) -o $@ -c $<

interrupts.o:interrupts.c
	$(TOOLCHAIN)-gcc $(CFLAGS) $(INCLUDES) -o $@ -c $<

gpio.o:gpio.c
	$(TOOLCHAIN)-gcc $(CFLAGS) $(INCLUDES) -o $@ -c $<

main.o:main.c
	$(TOOLCHAIN)-gcc $(CFLAGS) $(INCLUDES) -o $@ -c $<

system_stm32f0xx.o:CMSIS/system_stm32f0xx.c
	$(TOOLCHAIN)-gcc $(CFLAGS) $(INCLUDES) -o $@ -c $<

startup_stm32f070xb.o:CMSIS/Startup/startup_stm32f070xb.s
	$(TOOLCHAIN)-as $(AFLAGS) -o $@ -c $<

load:
	openocd -f board/st_nucleo_f0.cfg

clean:
	rm -rf *.o *.map *.list *.elf