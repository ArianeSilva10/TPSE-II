CC ?= g++
CFLAGS = -Wall -Wextra

all: pwm_led

pwm_led: pwm_led.cpp
	$(CC) $(CFLAGS) -o pwm_led pwm_led.cpp

clean:
	rm -f pwm_led