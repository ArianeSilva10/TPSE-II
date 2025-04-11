#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define PWM_CHIP "/sys/class/pwm/pwmchip0"
#define PWM_CHANNEL PWM_CHIP "/pwm0"

// Notas musicais (frequências em Hz)
#define C4  262
#define D4  294
#define E4  330
#define F4  349
#define G4  392
#define A4  440
#define B4  494
#define C5  523

typedef struct {
    int frequency;
    int duration;  // milissegundos
} Note;

Note melody[] = {
    {G4, 500}, {E4, 500}, {F4, 500}, {G4, 500},  // "Se essa rua"
    {G4, 500}, {E4, 500}, {F4, 500}, {G4, 500},  // "se essa rua"
    {C5, 500}, {A4, 500}, {G4, 500}, {F4, 500},  // "fosse minha"
    {E4, 1000},                                  // "eu mandava"
};

void writeToFile(const char *path, const char *value) {
    int fd = open(path, O_WRONLY);
    if (fd < 0) {
        perror("Erro ao abrir arquivo");
        exit(1);
    }
    write(fd, value, strlen(value));
    close(fd);
}

void setupPWM() {
    writeToFile(PWM_CHIP "/export", "0");
    writeToFile(PWM_CHANNEL "/enable", "1");
}

void playNote(int frequency, int duration) {
    char period[20], duty[20];

    if (frequency == 0) {
        usleep(duration * 1000);  // Pausa (silêncio)
        return;
    }

    int period_ns = 1000000000 / frequency;
    snprintf(period, sizeof(period), "%d", period_ns);
    snprintf(duty, sizeof(duty), "%d", period_ns / 2);

    writeToFile(PWM_CHANNEL "/period", period);
    writeToFile(PWM_CHANNEL "/duty_cycle", duty);

    usleep(duration * 1000);
}

void stopPWM() {
    writeToFile(PWM_CHANNEL "/enable", "0");
    writeToFile(PWM_CHIP "/unexport", "0");
}

int main() {
    printf("Tocando 'Se Essa Rua Fosse Minha' 🎵\n");

    setupPWM();

    for (int i = 0; i < sizeof(melody) / sizeof(Note); i++) {
        playNote(melody[i].frequency, melody[i].duration);
        usleep(100000);  // Pequena pausa entre as notas
    }

    stopPWM();
    printf("Música encerrada.\n");

    return 0;
}
