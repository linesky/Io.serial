
import serial
import time
#pip install pyserial
def send_message():
    try:
        # Configura a porta COM1 com a velocidade de 9600 bps
        ser = serial.Serial('COM1', 9600, timeout=1)
        time.sleep(2)  # Aguardar a inicialização da conexão

        # Envia a mensagem "hello world\n"
        ser.write(b'hello world\n')
        print("send message: 'hello world'")

        # Fecha a conexão serial
        ser.close()

    except serial.SerialException as e:
        print(f"Error acess serial: {e}")
    except Exception as e:
        print(f" error: {e}")

if __name__ == "__main__":
    print("\033c\033[40;37m")
    send_message()
