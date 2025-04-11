// led_blink_sequence.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define LED_PATH_FORMAT "/sys/class/leds/beaglebone:green:usr%d/brightness"
#define NUM_LEDS 4
#define BLINK_COUNT 10  // numero de vezes que cada led deve piscar

int open_led(int led_num) {
    char led_path[100];
    snprintf(led_path, sizeof(led_path), LED_PATH_FORMAT, led_num);
    
    int fd = open(led_path, O_WRONLY);
    if (fd < 0) {
        printf("Erro ao abrir LED%d: %s\n", led_num, led_path);
        perror("Erro");
    }
    return fd;
}

void write_led(int fd, char* value) {
    if (fd > 0) {
        write(fd, value, strlen(value));
    }
}

int main() {
    int led_fds[NUM_LEDS];
    
    // abrir todos os leds
    for (int i = 0; i < NUM_LEDS; i++) {
        led_fds[i] = open_led(i);
        if (led_fds[i] < 0) {
            printf("Falha ao abrir LED%d\n", i);
            // fechar leds ja abertos
            for (int j = 0; j < i; j++) {
                close(led_fds[j]);
            }
            return 1;
        }
    }
    
    printf("Piscando LEDs em sequência...\n");

    // cada led pisca BLINK_COUNT(10) vezes
    for (int count = 0; count < BLINK_COUNT; count++) {
        for (int i = 0; i < NUM_LEDS; i++) {
            // acende led atual
            write_led(led_fds[i], "1");
            usleep(200000);  // espera 200ms
            write_led(led_fds[i], "0");
            
            // delay
            usleep(100000);  // espera 100ms
        }
    }

    // fechar todos os leds
    for (int i = 0; i < NUM_LEDS; i++) {
        close(led_fds[i]);
    }

    printf("Fim do programa.\n");
    return 0;
}
