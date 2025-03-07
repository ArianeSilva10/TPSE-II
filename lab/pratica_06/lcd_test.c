#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <string.h>

#define I2C_DEVICE_FILE_PATH "/dev/i2c-2"  // Verifique o caminho do seu I2C
#define SLAVE_ADDRESS_LCD 0x27  // Endereço do LCD, pode ser 0x3F ou outro dependendo do seu modelo

int fd;

void lcd1602_init() {
    // Função de inicialização do LCD (isso pode variar dependendo do modelo)
    unsigned char init_sequence[] = {
        0x33, 0x32, 0x06, 0x0C, 0x01, 0x02  // Sequência de inicialização do LCD
    };

    for (int i = 0; i < sizeof(init_sequence); i++) {
        write(fd, &init_sequence[i], 1);
        usleep(1000);  // Aguarde um pouco entre os comandos
    }
}

void lcd1602_sendCommand(unsigned char command) {
    unsigned char buf[] = {0x00, command};  // 0x00 é o bit de comando
    write(fd, buf, 2);
}

void lcd1602_sendData(unsigned char data) {
    unsigned char buf[] = {0x01, data};  // 0x01 é o bit de dados
    write(fd, buf, 2);
}

void lcd1602_setCursorPosition(int row, int col) {
    unsigned char pos;
    if (row == 0) pos = 0x80 + col;  // Linha 1
    else if (row == 1) pos = 0xC0 + col;  // Linha 2
    lcd1602_sendCommand(pos);
}

void lcd1602_sendString(const char* str) {
    while (*str) {
        lcd1602_sendData(*str++);
    }
}

int main() {
    // Abrir o dispositivo I2C
    fd = open(I2C_DEVICE_FILE_PATH, O_RDWR);
    if (fd < 0) {
        perror("Failed to open I2C device file.");
        return -1;
    }

    // Configurar o endereço I2C do LCD
    if (ioctl(fd, I2C_SLAVE, SLAVE_ADDRESS_LCD) < 0) {
        perror("Failed to set I2C slave address.");
        close(fd);
        return -1;
    }

    // Inicializar o LCD
    lcd1602_init();

    // Exibir "Hello, World!" no LCD
    lcd1602_setCursorPosition(0, 0);  // Definir posição na linha 1, coluna 0
    lcd1602_sendString("Hello, World!");

    // Ficar em loop para manter a mensagem visível
    while (1) {
        sleep(1);
    }

    close(fd);
    return 0;
}
