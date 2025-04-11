CC ?= gcc
CFLAGS = -Wall -Wextra

all: led_blink_sequence

led_blink_sequence: led_blink_sequence.c
	$(CC) $(CFLAGS) -o led_blink_sequence led_blink_sequence.c

clean:
	rm -f led_blink_sequence