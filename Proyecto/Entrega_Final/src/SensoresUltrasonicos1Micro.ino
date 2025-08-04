// Definición de pines
const int led       = 3;  // LED indicador conectado al pin digital 3
const int buzzer    = 2;  // Buzzer activo conectado al pin digital 2
const int trigPin1  = 9;  // Trigger del sensor ultrasónico 1 en pin 9
const int echoPin1  = 10; // Echo del sensor ultrasónico 1 en pin 10
const int trigPin2  = 7;  // Trigger del sensor ultrasónico 2 en pin 7
const int echoPin2  = 8;  // Echo del sensor ultrasónico 2 en pin 8

void setup() {
  Serial.begin(9600);         // Inicia comunicación serial a 9600 baudios

  // Configuración de pines de los sensores HC-SR04
  pinMode(trigPin1, OUTPUT);  // Trigger 1 como salida
  pinMode(echoPin1, INPUT);   // Echo 1 como entrada
  pinMode(trigPin2, OUTPUT);  // Trigger 2 como salida
  pinMode(echoPin2, INPUT);   // Echo 2 como entrada

  // Configuración de pines de salida para alarma
  pinMode(buzzer, OUTPUT);    // Buzzer como salida
  pinMode(led, OUTPUT);       // LED como salida

  // Aseguramos estado inicial apagado
  digitalWrite(buzzer, LOW);
  digitalWrite(led, LOW);
}

void loop() {
  // —— 1) Generar pulso de disparo en ambos sensores —— 
  digitalWrite(trigPin1, LOW);   // Asegura nivel LOW antes del pulso
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);          // Pausa corta para estabilizar

  digitalWrite(trigPin1, HIGH);  // Inicia pulso HIGH de 10 µs
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);         // Duración mínima requerida
  digitalWrite(trigPin1, LOW);   // Fin del pulso
  digitalWrite(trigPin2, LOW);

  // —— 2) Medir duración del pulso de eco —— 
  long duration1 = pulseIn(echoPin1, HIGH); // Tiempo (µs) para sensor 1
  long duration2 = pulseIn(echoPin2, HIGH); // Tiempo (µs) para sensor 2

  // —— 3) Calcular distancia en centímetros —— 
  // Velocidad del sonido ≈ 0.034 cm/µs; dividimos entre 2 por ida y vuelta
  float distance1 = duration1 * 0.034 / 2;
  float distance2 = duration2 * 0.034 / 2;

  // —— 4) Mostrar distancias por el Monitor Serial —— 
  Serial.print("Distancia1: ");
  Serial.print(distance1);
  Serial.println(" cm");

  Serial.print("Distancia2: ");
  Serial.print(distance2);
  Serial.println(" cm");
  Serial.println(); // Línea en blanco para separar lecturas

  // —— 5) Lógica de alarma —— 
  // Si el objeto está a ≤15 cm del sensor 1
  // y la diferencia entre distancias es <3 cm, activamos alarma
  if (distance1 <= 15 && abs(distance1 - distance2) < 3) {
    digitalWrite(buzzer, HIGH);  // Enciende buzzer
    digitalWrite(led, HIGH);     // Enciende LED
  } else {
    digitalWrite(buzzer, LOW);   // Apaga buzzer
    digitalWrite(led, LOW);      // Apaga LED
  }

  // —— 6) Pequeña pausa antes de la próxima medición —— 
  delay(300); // 300 ms
}
