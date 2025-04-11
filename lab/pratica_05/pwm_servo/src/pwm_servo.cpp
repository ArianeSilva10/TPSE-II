#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unistd.h>

class PWM {
public:
    PWM(int pwm_number) {
        pwm_pin = pwm_number;
        export_pwm();
    }

    void configure(int frequency, float duty_cycle) {
        set_pwm_enable(false); // Desabilitar PWM para configuração

        // Calcular o período com base na frequência (em nanosegundos)
        float period = 1.0 / frequency;
        period = period * 1000000000; // Converter para nanosegundos

        // Configurar o período
        set_pwm_property("period", period);

        // Calcular o duty cycle
        duty_cycle = duty_cycle * period / 100;

        // Configurar o duty cycle
        set_pwm_property("duty_cycle", duty_cycle);

        // Habilitar PWM novamente
        set_pwm_enable(true);
    }

private:
    int pwm_pin;

    void export_pwm() {
        std::ofstream export_file("/sys/class/pwm/pwmchip0/export");
        if (export_file.is_open()) {
            export_file << pwm_pin;
            export_file.close();
        } else {
            std::cerr << "Erro ao exportar o PWM" << std::endl;
        }
    }

    void set_pwm_enable(bool enable) {
        std::ofstream enable_file("/sys/class/pwm/pwmchip0/pwm" + std::to_string(pwm_pin) + "/enable");
        if (enable_file.is_open()) {
            enable_file << (enable ? "1" : "0");
            enable_file.close();
        } else {
            std::cerr << "Erro ao definir o enable para o PWM" << std::endl;
        }
    }

    void set_pwm_property(const std::string& property, float value) {
        std::ofstream property_file("/sys/class/pwm/pwmchip0/pwm" + std::to_string(pwm_pin) + "/" + property);
        if (property_file.is_open()) {
            property_file << static_cast<int>(value);
            property_file.close();
        } else {
            std::cerr << "Erro ao configurar a propriedade " << property << std::endl;
        }
    }
};

int main() {
    int freq;
    float duty_cycle;

    // Solicitar entrada do usuário
    std::cout << "Digite a frequência do PWM (normalmente 50 Hz para servo): ";
    std::cin >> freq;

    std::cout << "Digite o duty cycle do PWM (5% a 10% para controlar o servo): ";
    std::cin >> duty_cycle;

    // Criar objeto PWM para o canal 0 (pwm0)
    PWM pwm(0);

    // Configurar PWM com a frequência e o duty cycle
    pwm.configure(freq, duty_cycle);

    return 0;
}
