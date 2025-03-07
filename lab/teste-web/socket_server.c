#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define LED_PATH_TEMPLATE "/sys/class/leds/beaglebone:green:usr%d/brightness"

// Função para controlar o LED
void controlLED(int led, const char *estado) {
    char ledPath[100];
    snprintf(ledPath, sizeof(ledPath), LED_PATH_TEMPLATE, led);

    FILE *file = fopen(ledPath, "w");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo do LED");
        return;
    }

    fprintf(file, "%s", estado);
    fclose(file);
}

// Função para processar as requisições
void processRequest(int client_socket) {
    char buffer[1024];
    char method[10];
    char url[100];

    // Lê a requisição HTTP
    read(client_socket, buffer, sizeof(buffer));
    sscanf(buffer, "%s %s", method, url);  // Extrai o método (GET) e o URL

    if (strcmp(method, "GET") == 0) {
        int led = -1;
        char estado[2] = "0";  // Default para desligar

        char *query_string = strchr(url, '?');
        if (query_string != NULL) {
            query_string++;
            sscanf(query_string, "led=%d&estado=%s", &led, estado);
        }

        if (led >= 0 && led < 4) {
            controlLED(led, estado);
            write(client_socket, "HTTP/1.1 200 OK\r\n", 17);
            write(client_socket, "Content-Type: text/html\r\n\r\n", 26);
            write(client_socket, "LED modificado com sucesso.", 27);
        } else {
            write(client_socket, "HTTP/1.1 400 Bad Request\r\n", 26);
            write(client_socket, "Content-Type: text/html\r\n\r\n", 26);
            write(client_socket, "LED inválido.", 14);
        }
    }

    close(client_socket);
}

int main() {
    int server_fd, client_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("Falha ao criar socket");
        exit(EXIT_FAILURE);
    }

    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Falha no bind");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("Falha ao ouvir");
        exit(EXIT_FAILURE);
    }

    printf("Servidor rodando em http://localhost:%d\n", PORT);

    while (1) {
        client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        if (client_socket < 0) {
            perror("Falha ao aceitar conexão");
            exit(EXIT_FAILURE);
        }

        processRequest(client_socket);
    }

    return 0;
}
