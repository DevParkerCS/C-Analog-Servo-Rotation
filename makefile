MCU=atmega328p
F_CPU=16000000UL
BAUD=115200
PROGRAMMER=arduino
PORT=/dev/ttyACM0
CFLAGS=-Os -DF_CPU=$(F_CPU) -mmcu=$(MCU)

SRCS = src/main.c src/adc.c src/uart.c src/servo.c
OBJS = $(SRCS:.c=.o)

all: servoControl.hex

# Rule for compiling .c files into .o files
%.o: %.c
	avr-gcc $(CFLAGS) -c -o $@ $<

# Rule for linking object files into the final executable
servoControl: $(OBJS)
	avr-gcc $(CFLAGS) -o servoControl $(OBJS)

# Rule for creating the .hex file
servoControl.hex: servoControl
	avr-objcopy -O ihex -R .eeprom servoControl servoControl.hex

# Upload rule
upload: servoControl.hex
	avrdude -F -V -c $(PROGRAMMER) -p $(MCU) -P $(PORT) -b $(BAUD) -U flash:w:servoControl.hex

# Clean rule
clean:
	rm -f $(OBJS) servoControl servoControl.hex