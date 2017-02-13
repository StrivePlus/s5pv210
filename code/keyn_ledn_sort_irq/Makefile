CC = arm-linux-gcc 
LD = arm-linux-ld 
OBJCOPY = arm-linux-objcopy 

LIB :=-lgcc -lgcc_eh -L/usr/local/arm/lib/gcc/arm-none-linux-gnueabi/4.5.1
INCLUDEDIR := $(shell pwd)/include/
CPPFLAGS := -nostdinc -nostdlib -I$(INCLUDEDIR)
CFLAGS := -fno-builtin -Wall -O2 

export CC LD OBJCOPY CPPFLAGS CFLAGS 

OBJS := cpu/start.o cpu/main.o  lib/uart.o lib/libc.a
		

all:start.bin clean

start.bin:$(OBJS)
	$(LD) -Ttext=0x40000000 $(OBJS) -o start.elf $(LIB)
	$(OBJCOPY) -O binary -S start.elf $@

%.o:%.S
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $< -o $@
%.o:%.c
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $< -o $@
clean:
	#rm -rf ./start.elf
	make clean -C cpu 

