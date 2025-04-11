#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define PWM_CHIP "/sys/class/pwm/pwmchip0"
#define PWM_CHANNEL PWM_CHIP "/pwm0"

// Notas musicais (frequências em Hz)
#define C4  261
#define D4  294
#define E4  329
#define F4  349
#define G4  392
#define A4  440
#define B4  493
#define C5  523

typedef struct {
    int frequency;
    int duration; // em milissegundos
} Note;

// Melodia simplificada
Note melody[] = {
    {G4, 500}, {E4, 500}, {F4, 500}, {G4, 500},
    {G4, 500}, {E4, 500}, {F4, 500}, {G4, 500},
    {C5, 500}, {A4, 500}, {G4, 500}, {F4, 500},
    {E4, 1000}, {0, 500} // Última nota é silêncio
};

void writeToFile(const char *path, int value) {
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%d", value);
    
    int fd = open(path, O_WRONLY);
    if (fd < 0) {
        perror("Erro ao abrir arquivo");
        exit(1);
    }
    write(fd, buffer, strlen(buffer));
    close(fd);
}

void setupPWM() {
    if (access(PWM_CHANNEL, F_OK) == -1) {
        writeToFile(PWM_CHIP "/export", 0);
        usleep(500000); // Espera meio segundo para garantir que o sistema cria os arquivos
    }
    writeToFile(PWM_CHANNEL "/enable", 1);
}

void playNote(int frequency, int duration) {
    if (frequency == 0) {
        usleep(duration * 1000); // Pausa (silêncio)
        return;
    }

    int period_ns = 1000000000 / frequency;
    writeToFile(PWM_CHANNEL "/period", period_ns);
    writeToFile(PWM_CHANNEL "/duty_cycle", period_ns / 2);

    usleep(duration * 1000);
}

void stopPWM() {
    writeToFile(PWM_CHANNEL "/enable", 0);
    writeToFile(PWM_CHIP "/unexport", 0);
}

int main() {
    printf("Tocando 'Se Essa Rua Fosse Minha' 🎵\n");

    setupPWM();

    for (int i = 0; i < sizeof(melody) / sizeof(Note); i++) {
        playNote(melody[i].frequency, melody[i].duration);
        usleep(100000); // Pequena pausa entre as notas
    }

    stopPWM();
    printf("Música encerrada.\n");

    return 0;
}
