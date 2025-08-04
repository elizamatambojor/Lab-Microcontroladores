import serial
import csv
from datetime import datetime

# Puerto por el que se reciben los datos
PORT = '/tmp/ttyS1'
BAUD = 9600
# Nombre del archivo CSV
CSV_FILE = 'Mediciones.csv'

# Abre la comunicación serial
ser = serial.Serial(PORT, BAUD, timeout=2)

# Se abre el CSV en modo de adicisión
with open(CSV_FILE, mode='a', newline='') as file:
    writer = csv.writer(file)
    if file.tell() == 0:
        # Títulos del SCV: Tiempo, modo y tensiones
        writer.writerow(['Timestamp', 'Mode', 'V1', 'V2', 'V3', 'V4'])

    try:
        mode = ''
        # Array para las 4 tensiones
        voltajes = [None] * 4
        # Lecutura de las información enviada por lineas
        while True:
            line = ser.readline().decode('utf-8').strip()
            if not line:
                continue
            # Deifne el modo de la tensión
            if "DC" in line:
                mode = "DC"
            elif "AC" in line:
                mode = "AC"
            # Define en el array el voltaje de cada tensión
            elif line.startswith("V1:"):
                voltajes[0] = float(line.split(":")[1].strip())
            elif line.startswith("V2:"):
                voltajes[1] = float(line.split(":")[1].strip())
            elif line.startswith("V3:"):
                voltajes[2] = float(line.split(":")[1].strip())
            elif line.startswith("V4:"):
                voltajes[3] = float(line.split(":")[1].strip())

            if all(v is not None for v in voltajes):
                # Escribe la hora y fecha
                timestamp = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
                writer.writerow([timestamp, mode] + voltajes)
                # Imprime en la terminal lo guardado en el csv
                print(f"[{timestamp}] {mode} | V1={voltajes[0]} V2={voltajes[1]} V3={voltajes[2]} V4={voltajes[3]}")
                # Reinciamos las tensiones para el siguiente valor
                voltajes = [None] * 4

    except KeyboardInterrupt:
        print("\nFinalizado.")
    finally:
        ser.close()
