// ——— Definición de pines ———
const int led         = 3;    // LED indicador
const int buzzer      = 2;    // Buzzer activo
const int trigPin     = 9;    // Trigger HC-SR04
const int echoPin     = 10;   // Echo HC-SR04
const int pwmPin      = 5;    // PWM de salida al ESP32

// ——— Parámetros de mapeo PWM ———
const float maxDistance = 100.0;  // Ahora la distancia máxima mapeada es 100 cm

void setup() {
  Serial.begin(9600);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(led,    OUTPUT);
  pinMode(pwmPin, OUTPUT);

  digitalWrite(buzzer, LOW);
  digitalWrite(led,    LOW);
  analogWrite(pwmPin,  0);    // Duty = 0% al inicio
}

void loop() {
  // 1) Disparo
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(15);
  digitalWrite(trigPin, LOW);

  // 2) Duración del eco
  long duration = pulseIn(echoPin, HIGH);

  // 3) Cálculo de distancia
  float distance = duration * 0.034 / 2;  // cm

  // 4) Serial
  Serial.print("Distancia: ");
  Serial.print(distance);
  Serial.println(" cm");

  // 5) Alarma (3 cm para río arriba  5 cm para río abajo)
  if (distance <= 3) {
    digitalWrite(buzzer, HIGH);
    digitalWrite(led,    HIGH);
  } else {
    digitalWrite(buzzer, LOW);
    digitalWrite(led,    LOW);
  }

  // 6) PWM proporcional con máxima distancia = 100 cm
  float d = constrain(distance, 0, maxDistance);
  int duty = map(int(d), 0, int(maxDistance), 0, 255);
  analogWrite(pwmPin, duty);

  // 7) Pausa
  delay(300);
}