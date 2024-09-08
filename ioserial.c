#include <windows.h>
#include <stdio.h>

int main() {
    HANDLE hSerial;
    DCB dcbSerialParams = {0};
    COMMTIMEOUTS timeouts = {0};
    DWORD bytes_written;
    char message[] = "hello world\n";
    printf("\033c\033[40;37m");
    // Abrir a porta COM1
    hSerial = CreateFile(
        "COM1",                  // Nome da porta serial
        GENERIC_WRITE,           // Acesso de gravação
        0,                       // Compartilhamento
        NULL,                    // Segurança padrão
        OPEN_EXISTING,           // Abrir a porta existente
        0,                       // Atributos padrão
        NULL                     // Sem template
    );

    if (hSerial == INVALID_HANDLE_VALUE) {
        printf("Error open door\n");
        return 1;
    }

    // Configurar parâmetros da porta serial
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (!GetCommState(hSerial, &dcbSerialParams)) {
        printf("Error open door\n");
        CloseHandle(hSerial);
        return 1;
    }

    dcbSerialParams.BaudRate = CBR_9600;  // Configura a velocidade de 9600 bps
    dcbSerialParams.ByteSize = 8;         // Configura 8 bits por byte
    dcbSerialParams.StopBits = ONESTOPBIT; // 1 bit de parada
    dcbSerialParams.Parity   = NOPARITY;  // Sem paridade

    if (!SetCommState(hSerial, &dcbSerialParams)) {
        printf("Error config door\n");
        CloseHandle(hSerial);
        return 1;
    }

    // Configurar tempo limite de leitura/escrita
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;

    if (!SetCommTimeouts(hSerial, &timeouts)) {
        printf("Erro config timeouts\n");
        CloseHandle(hSerial);
        return 1;
    }

    // Enviar a mensagem "hello world\n" para a porta COM1
    if (!WriteFile(hSerial, message, sizeof(message) - 1, &bytes_written, NULL)) {
        printf("Error sending data\n");
        CloseHandle(hSerial);
        return 1;
    }

    printf("Mensage send ok!\n");

    // Fechar a porta serial
    CloseHandle(hSerial);

    return 0;
}

