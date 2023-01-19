COMPILER  = arm-none-eabi-gcc
PROCESSOR = cortex-m0
CFLAGS    = -c -mcpu=$(PROCESSOR) -mthumb -std=gnu11 -Wall -o0
LDFLAGS   = -nostdlib -T linker.ld -Wl,-Map=final.map

all:main.o stm32_startup.o final.elf

main.o:main.c
	$(COMPILER) $(CFLAGS) $^ -o $@

stm32_startup.o:stm32_startup.c
	$(COMPILER) $(CFLAGS) $^ -o $@

final.elf:main.o stm32_startup.o
	$(COMPILER) $(LDFLAGS) $^ -o $@

load:
	openocd -f board/st_nucleo_f0.cfg

clean:
	rm -rf *.o *.elf