#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>

#define SERVO_GPIO "28"  // P9_12 -> GPIO 28 (servo motor)
#define SENSOR_GPIO "16" // P9_15 -> GPIO 16 (sensor infravermelho)

// Função para exportar GPIOs
void export_gpio(const char *gpio) {
    int fd = open("/sys/class/gpio/export", O_WRONLY);
    if (fd == -1) {
        perror("Erro ao exportar GPIO");
        exit(1);
    }
    write(fd, gpio, sizeof(gpio));
    close(fd);
}

// Função para definir direção do GPIO
void set_gpio_direction(const char *gpio, const char *direction) {
    char path[50];
    snprintf(path, sizeof(path), "/sys/class/gpio/gpio%s/direction", gpio);
    int fd = open(path, O_WRONLY);
    if (fd == -1) {
        perror("Erro ao configurar direção do GPIO");
        exit(1);
    }
    write(fd, direction, sizeof(direction));
    close(fd);
}

// Função para escrever em GPIO (0 ou 1)
void write_gpio(const char *gpio, int value) {
    char path[50];
    snprintf(path, sizeof(path), "/sys/class/gpio/gpio%s/value", gpio);
    int fd = open(path, O_WRONLY);
    if (fd == -1) {
        perror("Erro ao escrever no GPIO");
        exit(1);
    }
    dprintf(fd, "%d", value);
    close(fd);
}

// Função para ler GPIO
int read_gpio(const char *gpio) {
    char path[50], value;
    snprintf(path, sizeof(path), "/sys/class/gpio/gpio%s/value", gpio);
    int fd = open(path, O_RDONLY);
    if (fd == -1) {
        perror("Erro ao ler GPIO");
        exit(1);
    }
    read(fd, &value, 1);
    close(fd);
    return (value == '1') ? 1 : 0;
}

// Função para movimentar o servo motor (simulação de PWM simples)
void move_servo(int position) {
    if (position == 1) {
        printf("Abrindo portão...\n");
        write_gpio(SERVO_GPIO, 1);
        usleep(1500000); // Pulso de 1.5ms (posição aberta)
    } else {
        printf("Fechando portão...\n");
        write_gpio(SERVO_GPIO, 0);
        usleep(1500000); // Pulso de 1.5ms (posição fechada)
    }
}

int main() {
    // Configurar GPIOs
    export_gpio(SERVO_GPIO);
    export_gpio(SENSOR_GPIO);
    usleep(100000); // Espera para o sistema criar arquivos GPIO

    set_gpio_direction(SERVO_GPIO, "out");
    set_gpio_direction(SENSOR_GPIO, "in");

    printf("Sistema de controle de portão com servo motor\n");

    while (1) {
        int obstaculo = read_gpio(SENSOR_GPIO);
        printf("Obstáculo detectado: %d\n", obstaculo);
        usleep(500000);  // Espera 500ms entre leituras

        if (obstaculo == 0) {
            move_servo(1); // Abrir portão
            usleep(5000000);  // Manter aberto por 5s
        } else {
            move_servo(0); // Fechar portão
            usleep(5000000);  // Manter fechado por 5s
        }
    }
    return 0;
}
