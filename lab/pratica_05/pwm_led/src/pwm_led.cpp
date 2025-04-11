#include <iostream>
#include <fstream>
#include <unistd.h>  // Para usleep()

#define PWM_CHIP "/sys/class/pwm/pwmchip0"
#define PWM_CHANNEL "pwm0"
#define PWM_PATH PWM_CHIP "/" PWM_CHANNEL

void writeToFile(const std::string &path, const std::string &value) {
    std::ofstream file(path);
    if (file.is_open()) {
        file << value;
        file.close();
    } else {
        std::cerr << "Erro ao acessar " << path << std::endl;
    }
}

void setBrightness(int percent) {
    int duty_cycle = (percent * 1000000) / 100;  // Mapeia para 0% - 100%
    writeToFile(PWM_PATH "/duty_cycle", std::to_string(duty_cycle));
}

int main() {
    // 1. Habilita o PWM
    writeToFile(PWM_CHIP "/export", "0");
    usleep(100000);  // Delay para garantir a configuração

    // 2. Configura o período para 20ms (50Hz)
    writeToFile(PWM_PATH "/period", "20000000");

    // 3. Ativa o PWM
    writeToFile(PWM_PATH "/enable", "1");

    // Teste: Aumenta e diminui o brilho do LED
    for (int i = 0; i <= 100; i += 10) {
        setBrightness(i);
        usleep(500000);  // 0.5s entre mudanças
    }

    for (int i = 100; i >= 0; i -= 10) {
        setBrightness(i);
        usleep(500000);
    }

    // 4. Desativa o PWM ao finalizar
    writeToFile(PWM_PATH "/enable", "0");
    writeToFile(PWM_CHIP "/unexport", "0");

    return 0;
}
