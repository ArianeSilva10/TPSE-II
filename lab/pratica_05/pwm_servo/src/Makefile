CC ?= g++
CFLAGS = -Wall -Wextra

all: pwm_servo

pwm_servo: pwm_servo.cpp
	$(CC) $(CFLAGS) -o pwm_servo pwm_servo.cpp

clean:
	rm -f pwm_servo