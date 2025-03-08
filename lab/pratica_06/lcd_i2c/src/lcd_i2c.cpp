#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <cstdint>
#include <cerrno>
#include <vector>

#define I2C_BUS "/dev/i2c-2"
#define LCD_ADDR 0x27

// mascaras para controle do LCD
#define LCD_BACKLIGHT 0x08
#define LCD_ENABLE 0x04
#define LCD_RS 0x01

// Delays configuráveis em microssegundos
#define PULSE_DELAY 500
#define CMD_DELAY 1000
#define INIT_DELAY 4500
#define TEXT_DELAY 1000000

// funcao para gerar pulso no pino Enable
void lcd_pulse(int fd, uint8_t data) {
    uint8_t buf[1];
    
    buf[0] = data;
    if (write(fd, buf, 1) != 1) {
        perror("Erro no pulso Enable");
        return;
    }
    usleep(PULSE_DELAY);
    
    buf[0] = data | LCD_ENABLE;
    if (write(fd, buf, 1) != 1) {
        perror("Erro no pulso Enable");
        return;
    }
    usleep(PULSE_DELAY);
    
    buf[0] = data;
    if (write(fd, buf, 1) != 1) {
        perror("Erro no pulso Enable");
        return;
    }
    usleep(PULSE_DELAY);
}

// funcao para enviar comandos/dados ao LCD
int lcd_send(int fd, uint8_t data, uint8_t mode) {
    uint8_t high_nibble = (data & 0xF0) | LCD_BACKLIGHT | mode;
    uint8_t low_nibble = ((data << 4) & 0xF0) | LCD_BACKLIGHT | mode;

    lcd_pulse(fd, high_nibble);
    lcd_pulse(fd, low_nibble);
    
    usleep(CMD_DELAY);
    return 0;
}

// Inicializar o LCD
int lcd_init(int fd) {
    usleep(INIT_DELAY);

    uint8_t init_sequence[] = {0x03, 0x03, 0x03, 0x02};
    for (int i = 0; i < 4; i++) {
        if (lcd_send(fd, init_sequence[i], 0) < 0) {
            return -1;
        }
        usleep(INIT_DELAY);
    }

    if (lcd_send(fd, 0x28, 0) < 0 ||  // modo 4 bits, 2 linhas
        lcd_send(fd, 0x08, 0) < 0 ||  // display off
        lcd_send(fd, 0x01, 0) < 0 ||  // clear display
        lcd_send(fd, 0x06, 0) < 0 ||  // entry mode set
        lcd_send(fd, 0x0C, 0) < 0) {  // display on
        return -1;
    }

    return 0;
}

// envia uma string ao LCD
int lcd_print(int fd, const std::string &str) {
    for (char ch : str) {
        if (lcd_send(fd, static_cast<uint8_t>(ch), LCD_RS) < 0) {
            return -1;
        }
    }
    return 0;
}

// posicionar o cursor
void lcd_set_cursor(int fd, int row, int col) {
    int row_offsets[] = {0x00, 0x40, 0x14, 0x54};
    lcd_send(fd, 0x80 | (row_offsets[row] + col), 0);
}

// limpar uma linha especifica
void lcd_clear_line(int fd, int row) {
    lcd_set_cursor(fd, row, 0);
    for (int i = 0; i < 16; i++) {
        lcd_print(fd, " ");
    }
    lcd_set_cursor(fd, row, 0);
}

// exibir texto com animacao
void display_text_animation_with_repeats(int fd, const std::vector<std::string> &texts, int repeats) {
    for (int i = 0; i < repeats; i++) {
        for (const auto &text : texts) {
            lcd_clear_line(fd, 0);
            lcd_clear_line(fd, 1);
            
            int text_len = text.length();
            int start_pos = (16 - text_len) / 2;
            
            lcd_set_cursor(fd, 0, start_pos);
            lcd_print(fd, text);
            
            usleep(TEXT_DELAY);
        }
    }
}

int main() {
    int fd;
    std::string input;

    if ((fd = open(I2C_BUS, O_RDWR)) < 0) {
        std::cerr << "Erro ao abrir o barramento I2C: " << strerror(errno) << std::endl;
        return EXIT_FAILURE;
    }

    if (ioctl(fd, I2C_SLAVE, LCD_ADDR) < 0) {
        std::cerr << "Erro ao configurar o endereço I2C: " << strerror(errno) << std::endl;
        close(fd);
        return EXIT_FAILURE;
    }

    if (lcd_init(fd) < 0) {
        std::cerr << "Erro ao inicializar o LCD" << std::endl;
        close(fd);
        return EXIT_FAILURE;
    }

    while (true) {
        lcd_send(fd, 0x01, 0);
        usleep(CMD_DELAY);

        std::cout << "Digite uma palavra (ou 'sair' para encerrar): ";
        std::getline(std::cin, input);

        if (input == "sair") {
            break;
        }

        int text_len = input.length();
        int start_pos = (16 - text_len) / 2;
        
        lcd_set_cursor(fd, 0, start_pos);
        lcd_print(fd, input);

        usleep(TEXT_DELAY);
    }

    close(fd);
    return EXIT_SUCCESS;
}