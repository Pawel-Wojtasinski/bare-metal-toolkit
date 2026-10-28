DEBUGFLAGS:=

CC:=avr-gcc
LD:=avr-gcc
CPU:=atmega328p
TARGET:=base

CFLAGS:=-mmcu=$(CPU) -c -O3 -Wfatal-errors -Werror -Wall -Wshadow -fmessage-length=0
//CFLAGS:=-mmcu=$(CPU) -c -O3 -Wfatal-errors -Werror -Wall -Wshadow -fmessage-length=0 -E

#LFLAGS:=-mmcu=$(CPU) -lc -Wl,-Map=map.txt
LFLAGS:=-mmcu=$(CPU) -lc

.PHONY: all clean cleanup install test

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

C_SOURCES:=$(call rwildcard, sources, *.c)
C_DIRS:=$(subst objs/sources/, objs/, $(addprefix objs/, $(sort $(dir $(C_SOURCES)))))
C_OBJS:=$(subst objs/sources/, objs/, $(addprefix objs/,$(patsubst %.c, %.o, $(C_SOURCES))))

all: $(TARGET).hex

$(TARGET).hex: $(TARGET).bin
	avr-objcopy -j .text -j .data -O ihex $< $@

$(TARGET).bin: $(C_OBJS)
	$(LD) $^ $(DEBUGFLAGS) $(LFLAGS) -o $@

$(C_OBJS): objs/%.o : sources/%.c | $(C_DIRS)
	$(CC) $(DEBUGFLAGS) $(CFLAGS) -Isources -c $< -o $@

$(sort $(C_DIRS)):
	mkdir -p $@

clean:
	rm -f -R objs

cleanall: clean
	rm -f $(TARGET).bin
	rm -f $(TARGET).hex

upload: $(TARGET).hex
	avrdude -p atmega328p -P /dev/ttyUSB0 -c arduino -D -Uflash:w:$<:i

flash: $(TARGET).hex
	avrdude -c usbasp -p m328p -e -Uflash:w:$<:i

upload_slow: $(TARGET).hex
	avrdude -p atmega328p -P /dev/ttyUSB0 -c arduino -b 57600 -D -Uflash:w:$<:i

com:
	picocom --baud 115200 --parity none --databits 8 --stopbits 1 /dev/ttyUSB0

com_slow:
	picocom --baud 57600 --parity none --databits 8 --stopbits 1 /dev/ttyUSB0

commit:
	git add .
	git commit -a --allow-empty-message -m ''

gui:
	gitk --all
