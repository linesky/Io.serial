
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
//sudo chmod 777 /dev/ttyS0
int main() {
    int serial_port;
    struct termios tty;
    printf("\033c\033[40;37m");
    // Abrir a porta serial (COM1 é /dev/ttyS0 no Linux)
    serial_port = open("/dev/ttyS0", O_RDWR);

    if (serial_port < 0) {
        printf("Erro ao abrir a porta serial: %s\n", strerror(errno));
        return 1;
    }

    // Obter os parâmetros atuais da porta
    if (tcgetattr(serial_port, &tty) != 0) {
        printf("Erro ao obter atributos da porta serial: %s\n", strerror(errno));
        close(serial_port);
        return 1;
    }

    // Configurar os parâmetros da porta serial
    tty.c_cflag &= ~PARENB;             // Sem paridade
    tty.c_cflag &= ~CSTOPB;             // 1 bit de parada
    tty.c_cflag &= ~CSIZE;              // Limpar o tamanho dos bits
    tty.c_cflag |= CS8;                 // 8 bits por byte
    tty.c_cflag &= ~CRTSCTS;            // Desabilitar controle de hardware
    tty.c_cflag |= CREAD | CLOCAL;      // Habilitar leitura e ignorar controle de modem

    // Configura a taxa de transmissão para 9600 bps
    cfsetispeed(&tty, B9600);
    cfsetospeed(&tty, B9600);

    tty.c_cc[VMIN] = 1;                 // Ler pelo menos 1 caractere
    tty.c_cc[VTIME] = 5;                // Tempo de espera (0,5 segundos)

    // Aplicar os atributos da porta serial
    if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
        printf("Erro ao configurar a porta serial: %s\n", strerror(errno));
        close(serial_port);
        return 1;
    }

    // Mensagem a ser enviada
    char message[] = "hello world\n";

    // Enviar a mensagem
    int bytes_written = write(serial_port, message, sizeof(message) - 1);

    if (bytes_written < 0) {
        printf("Erro ao escrever na porta serial: %s\n", strerror(errno));
        close(serial_port);
        return 1;
    }

    printf("Mensagem enviada: %s\n", message);

    // Fechar a porta serial
    close(serial_port);

    return 0;
}
