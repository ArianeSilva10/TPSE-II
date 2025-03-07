
#!/bin/sh
for i in 0 1 2 3; do
    echo 1 > /sys/devices/platform/leds/leds/beaglebone:green:usr$i/brightness
    sleep 1
    echo 0 > /sys/devices/platform/leds/leds/beaglebone:green:usr$i/brightness

   sleep 1
done
for i in 3 2 1 0; do
    echo 1 > /sys/devices/platform/leds/leds/beaglebone:green:usr$i/brightness
    sleep 1
    echo 0 > /sys/devices/platform/leds/leds/beaglebone:green:usr$i/brightness
done
