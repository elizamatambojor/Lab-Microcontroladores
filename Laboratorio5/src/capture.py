# Este script conecta tu ordenador al Arduino por el puerto serie
# lee un número fijo de muestras de aceleración y giroscopio 
# enviadas en formato CSV, y las guarda en un fichero dentro de data. 

# Librerías necesarias para el flujo del código
import sys
import os
import serial
import time
import csv

# Define la cabecera de columnas, el número de muestras a capturar
# el puerto serie, la velocidad de transmisión y y el tiempo de espera. 
HEADER     = ['aX','aY','aZ','gX','gY','gZ']
SAMPLES    = 3000
PORT       = 'COM10'  # en este caso COM10
BAUDRATE   = 9600
TIMEOUT    = 1

# Comprueba que al invocar el script se pase al menos un argumento 
# (el nombre del archivo CSV de salida). 
# Si no, muestra cómo usarlo y termina.
if len(sys.argv) < 2:
    print("Uso: python capture.py <gesture>.csv")
    sys.exit(1)
filename = sys.argv[1]

# Obtiene la ruta del directorio donde está el script,
# crea (si no existe) una subcarpeta data y construye la ruta completa 
# del archivo de salida dentro de ella.
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
DATA_DIR   = os.path.join(SCRIPT_DIR, 'data')
os.makedirs(DATA_DIR, exist_ok=True)
out_path   = os.path.join(DATA_DIR, filename)

# Intenta abrir el puerto con los parámetros dados; si hay error, lo informa y sale. 
# Tras abrirlo espera 2 s para que el Arduino se reinicie y luego confirma la conexión.
try:
    ser = serial.Serial(port=PORT, baudrate=BAUDRATE, timeout=TIMEOUT)
    time.sleep(2)  # espera al reset del Arduino
    print(f"Conectado en {PORT}")
except serial.SerialException as e:
    print("Error al conectar con la placa:", e)
    sys.exit(1)

# Abre el CSV de salida con ; como separador, escribe la cabecera 
# y entra en un bucle hasta llegar a SAMPLES lecturas.
# Cuando completa todas las muestras, cierra el archivo y desconecta el puerto.
counter = 0
with open(out_path, 'w', newline='', encoding='utf8') as f:
    writer = csv.writer(f, delimiter=';')
    writer.writerow(HEADER)
    print(f"Grabando {SAMPLES} muestras en {out_path}")
    while counter < SAMPLES:
        line = ser.readline().decode('utf-8', errors='ignore').strip()
        parts = line.replace(';', ',').split(',')
        if len(parts) == 6:
            writer.writerow(parts)
            counter += 1
            print(f"\rMuestra {counter}/{SAMPLES}", end='')

print("\nCaptura finalizada.")
ser.close()
