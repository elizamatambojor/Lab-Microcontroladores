/*
Proyecto Final - Laboratorio de Microcontroladores IE0624

Este código se encargar de leer señales PWM de dos 
sensores de nivel, reconstruye la distancia en cm 
a partir del duty cycle, envía telemetría a ThingsBoard vía HTTP
y dispara alarmas en ThingsBoard si se superan umbrales.
*/

// Librerías necesarias 
#include <WiFi.h>
#include <HTTPClient.h>

//  Nombre de la red Wi-Fi y contraseña
const char* ssid   = "Redmi13";
const char* pass   = "polloconpapas";
// URL del endpoint de telemetría en ThingsBoard 
const char* tbUrl  = "http://iot.eie.ucr.ac.cr:8080/api/v1/jkoaeack1qa61q7h6wd3/telemetry";

// Pines de entrada PWM 
const int pwmInputPin1 = 14;   // Señal PWM sensor 1  D14 = GPIO14
const int pwmInputPin2 = 27;   // Señal PWM sensor 2  D27 = GPIO27

// Distancia máxima mapeada (cm)
const float maxDistance = 100.0;

void setup() {
  // Inicia el puerto serie a 9600 baudios para debug
  Serial.begin(9600);
  // Configuración de los pines PWM como entrada
  pinMode(pwmInputPin1, INPUT);
  pinMode(pwmInputPin2, INPUT);
   // Conexión a la red Wi-Fi
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi OK");
}

void loop() {
  // Lectura del PWM del sensor 1
  // Se mide el tiempo en HIGH con timeout de 3 s
  unsigned long tHigh1 = pulseIn(pwmInputPin1, HIGH, 3'000'000UL);
  // Se mide el tiempo en LOW con timeout de 3 s
  unsigned long tLow1  = pulseIn(pwmInputPin1, LOW,  3'000'000UL);
  // Se calcula el período completo
  unsigned long period1 = tHigh1 + tLow1;
  // Se calcula el duty cycle 
  float duty1  = period1>0 ? (float)tHigh1/period1 : 0.0;
  // Se reconstruye la distancia en cm a partir del duty cycle
  float dist1  = duty1 * maxDistance;

  // Lectura del PWM del sensor 2 
  unsigned long tHigh2 = pulseIn(pwmInputPin2, HIGH, 3'000'000UL);
  unsigned long tLow2  = pulseIn(pwmInputPin2, LOW,  3'000'000UL);
  unsigned long period2 = tHigh2 + tLow2;
  float duty2  = period2>0 ? (float)tHigh2/period2 : 0.0;
  float dist2  = duty2 * maxDistance;

  // Debug por serie
  // Se muestran tiempos, duty cycles y distancias calculadas
  Serial.printf(
    "S1: T_HIGH=%lums T_LOW=%lums Duty=%.1f%% → %.2fcm | "
    "S2: T_HIGH=%lums T_LOW=%lums Duty=%.1f%% → %.2fcm\n",
    tHigh1/1000, tLow1/1000, duty1*100.0, dist1,
    tHigh2/1000, tLow2/1000, duty2*100.0, dist2
  );

  // Enviar a ThingsBoard 
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(tbUrl); // Inicia conexión HTTP
    http.addHeader("Content-Type", "application/json");
    // Construye el JSON con los dos valores de nivel
    String body = String("{\"nivel_1\":") + String(dist1,2)
                + String(",\"nivel_2\":") + String(dist2,2) + String("}");
    // Envía la petición POST y obtiene el código de estado
    int status = http.POST(body);
    Serial.printf("HTTP %d  payload: %s\n", status, body.c_str());
    http.end(); // Cierra conexión
  }

  // Espera 300 ms antes de la siguiente iteración
  delay(300);
}

