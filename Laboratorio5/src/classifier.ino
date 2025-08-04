/*
  IMU Classifier
  This example uses the on-board IMU to start reading acceleration and gyroscope
  data from on-board IMU, once enough samples are read, it then uses a
  TensorFlow Lite (Micro) model to try to classify the movement as a known gesture.

  This example code is in the public domain.
*/

/* Se importa la librería para acceder al IMU (acelerómetro+giroscopio).
  Además, TensorFlow Lite Micro y el “resolver” que registra 
  todas las operaciones necesarias. También se incluye model.h
*/
#include <Arduino_LSM9DS1.h>
#include <TensorFlowLite.h>
#include <tensorflow/lite/micro/all_ops_resolver.h>
#include <tensorflow/lite/micro/micro_interpreter.h>
#include <tensorflow/lite/schema/schema_generated.h>
#include "model.h"   // tu header con el array `model[]`

// Umbral de movimiento significativo en G's
const float accelerationThreshold = 2.5f;
// Número de muestras 
const int numSamples = 119;

int samplesRead = numSamples;

// Resolver de operaciones TFLM
tflite::AllOpsResolver tflOpsResolver;

// Punteros para el modelo y tensores
const tflite::Model*      tflModel        = nullptr;
tflite::MicroInterpreter* tflInterpreter  = nullptr;
TfLiteTensor*             tflInputTensor  = nullptr;
TfLiteTensor*             tflOutputTensor = nullptr;

// Arena de memoria para TFLM
constexpr int tensorArenaSize = 8 * 1024;
static uint8_t tensorArena[tensorArenaSize] __attribute__((aligned(16)));

// Etiquetas de los gestos en el mismo orden que la salida del modelo
const char* GESTURES[] = {
  "circles",
  "clap",
  "updown"
};
#define NUM_GESTURES (sizeof(GESTURES) / sizeof(GESTURES[0]))

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // Inicializa la IMU
  if (!IMU.begin()) {
    Serial.println("Error al inicializar IMU!");
    while (1);
  }

  // Muestra tasas de muestreo
  Serial.print("Acelerómetro: "); Serial.print(IMU.accelerationSampleRate());  Serial.println(" Hz");
  Serial.print("Giroscopio:    "); Serial.print(IMU.gyroscopeSampleRate());     Serial.println(" Hz");
  Serial.println();

  // Carga el modelo desde el array `model[]`
  tflModel = tflite::GetModel(model);
  if (tflModel->version() != TFLITE_SCHEMA_VERSION) {
    Serial.println("Versión de esquema de modelo no compatible!");
    while (1);
  }

  // Crea e inicializa el intérprete
  static tflite::MicroInterpreter staticInterpreter(
    tflModel, tflOpsResolver, tensorArena, tensorArenaSize
  );
  tflInterpreter = &staticInterpreter;
  tflInterpreter->AllocateTensors();

  // Punteros a tensores de entrada y salida
  tflInputTensor  = tflInterpreter->input(0);
  tflOutputTensor = tflInterpreter->output(0);
}

void loop() {
  float aX, aY, aZ, gX, gY, gZ;

  // Espera a movimiento significativo
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

  // Recoge muestras
  while (samplesRead < numSamples) {
    if (IMU.accelerationAvailable() && IMU.gyroscopeAvailable()) {
      IMU.readAcceleration(aX, aY, aZ);
      IMU.readGyroscope(gX, gY, gZ);

      // Normaliza y escribe en el tensor de entrada
      tflInputTensor->data.f[samplesRead * 6 + 0] = (aX + 4.0f)   / 8.0f;
      tflInputTensor->data.f[samplesRead * 6 + 1] = (aY + 4.0f)   / 8.0f;
      tflInputTensor->data.f[samplesRead * 6 + 2] = (aZ + 4.0f)   / 8.0f;
      tflInputTensor->data.f[samplesRead * 6 + 3] = (gX + 2000.0f) / 4000.0f;
      tflInputTensor->data.f[samplesRead * 6 + 4] = (gY + 2000.0f) / 4000.0f;
      tflInputTensor->data.f[samplesRead * 6 + 5] = (gZ + 2000.0f) / 4000.0f;

      samplesRead++;

      // Al completar, ejecuta inferencia
      if (samplesRead == numSamples) {
        if (tflInterpreter->Invoke() != kTfLiteOk) {
          Serial.println("Inferencia fallida!");
          while (1);
        }
        // Imprime probabilidades
        for (int i = 0; i < NUM_GESTURES; ++i) {
          Serial.print(GESTURES[i]);
          Serial.print(": ");
          Serial.println(tflOutputTensor->data.f[i], 6);
        }
        Serial.println();
      }
    }
  }
}

