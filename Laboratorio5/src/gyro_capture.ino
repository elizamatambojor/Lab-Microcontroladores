/*
  IMU Capture
  This example uses the on-board IMU to start reading acceleration and gyroscope
  data from on-board IMU and prints it to the Serial Monitor for one second
  when the significant motion is detected.
  This example code is in the public domain.
*/

/*
Este programa detecta un evento de movimiento cuando la suma de 
las componentes de aceleración supera 2.5 G, luego registra una serie de datos 
de aceleración y giro para análisis y los envía en formato CSV 
para su posterior procesamiento.
*/

// Se incluyé la librería de LSM9DS1 para acceder al IMU 
#include <Arduino_LSM9DS1.h>

const float accelerationThreshold = 2.5;
const int numSamples = 119;

int samplesRead = numSamples;

/* Se inica la comunicaión seria a 9600 bps y se espera
hasta que esté lista, además se inicializa el IMU */ 
void setup() {
  Serial.begin(9600);
  while (!Serial);

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
}

void loop() {
  float aX, aY, aZ, gX, gY, gZ;

  /* Se comprueban datos de aceleración disponibles
  Se leen aX, aY, aZ y se suma el valor absoluto
  Si aSum es mayor igual a threshold se reinicia samplesRead 
  a 0 y se sale de este ucle para empezar la captura */ 
  while (samplesRead == numSamples) {
    if (IMU.accelerationAvailable()) {
      IMU.readAcceleration(aX, aY, aZ);

      float aSum = fabs(aX) + fabs(aY) + fabs(aZ);

      if (aSum >= accelerationThreshold) {
        samplesRead = 0;
        break;
      }
    }
  }

   /*Se espera que haya tantos datos de aceleración como el giroscopio
   se leen aX, Ay, aZ y gX, gY, gZ
   se incremneta samplesRead y se envían las 6 lecturas al Monitor Serie 
   en una línea CSV*/ 
  while (samplesRead < numSamples) {
    
    if (IMU.accelerationAvailable() && IMU.gyroscopeAvailable()) {
      
      IMU.readAcceleration(aX, aY, aZ);
      IMU.readGyroscope(gX, gY, gZ);

      samplesRead++;

      Serial.print(aX, 3);
      Serial.print(',');
      Serial.print(aY, 3);
      Serial.print(',');
      Serial.print(aZ, 3);
      Serial.print(',');
      Serial.print(gX, 3);
      Serial.print(',');
      Serial.print(gY, 3);
      Serial.print(',');
      Serial.print(gZ, 3);
      Serial.println();

      // Tras 199 muestras se repite el proceso
      if (samplesRead == numSamples) {
        Serial.println();
      }
    }
  }
}
