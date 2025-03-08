CC ?= gcc
CFLAGS = -Wall -Wextra

all: pwm_buzzer

pwm_buzzer: pwm_buzzer.c
	$(CC) $(CFLAGS) -o pwm_buzzer pwm_buzzer.c

clean:
	rm -f pwm_buzzer