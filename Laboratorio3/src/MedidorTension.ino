#include <PCD8544.h>
#include <math.h>

// Pines analógicos para los cuatro canales de medición
static const byte CanalV1 = A0;
static const byte CanalV2 = A1;
static const byte CanalV3 = A2;
static const byte CanalV4 = A3;

// Pines digitales de control
static const byte ALARM_LED_PIN     = 13;  // LED que indica sobre/subvoltaje
static const byte SERIAL_ENABLE_PIN = 12;  // Switch que habilita la salida por USART
static const byte MODE_SWITCH_PIN   = 11;   // Switch que selecciona modo DC (LOW) o AC (HIGH)

// Pines de la pantalla LCD PCD8544 (según asignación de tu compañero)
#define PIN_SCLK  2  // Reloj de SPI
#define PIN_DIN   4  // Datos de SPI
#define PIN_DC    7  // Indicador Data/Command
#define PIN_CE    8  // Chip Enable
#define PIN_RST   9  // Reset del display

// Objeto para controlar la pantalla LCD
PCD8544 lcd(PIN_SCLK, PIN_DIN, PIN_DC, PIN_RST, PIN_CE);

// Variables de medición
float V1, V2, V3, V4;

// Prototipos de funciones
bool leerCanalDC(int pinCanal, float &salidaVar);
bool leerCanalAC(int pinCanal, float &salidaVar, int muestras = 1000);

void setup() {
  // Inicializa la pantalla con resolución 84×48 y contraste medio
  lcd.begin(84, 48);
  lcd.setContrast(60);
  lcd.clear();

  // Inicia la comunicación serial a 9600 baudios
  Serial.begin(9600);

  // Configura los pines de control como entradas o salidas
  pinMode(ALARM_LED_PIN,     OUTPUT);  // Salida para el LED de alarma
  pinMode(SERIAL_ENABLE_PIN, INPUT);   // Entrada del switch de habilitación serial
  pinMode(MODE_SWITCH_PIN,   INPUT);   // Entrada del switch AC/DC
}

void loop() {
  bool isDC    = (digitalRead(MODE_SWITCH_PIN) == LOW);  // LOW => DC
  bool sendCSV = (digitalRead(SERIAL_ENABLE_PIN) == LOW);
  bool errorFlag = false;

    // Limpia y e inicializa la pantalla
    lcd.clear();
    lcd.setCursor(0, 0);
    // Lectura de canales y elección del título del LCD
    if (isDC) {
      lcd.print(" V en DC:");
      errorFlag |= leerCanalDC(CanalV1, V1);
      errorFlag |= leerCanalDC(CanalV2, V2);
      errorFlag |= leerCanalDC(CanalV3, V3);
      errorFlag |= leerCanalDC(CanalV4, V4);
      // Imprimimos los valores
      lcd.setCursor(0, 1);
      lcd.print("V1: "); lcd.print(V1); lcd.print(" V");

      lcd.setCursor(0, 2);
      lcd.print("V2: "); lcd.print(V2); lcd.print(" V");

      lcd.setCursor(0, 3);
      lcd.print("V3: "); lcd.print(V3); lcd.print(" V");

      lcd.setCursor(0, 4);
      lcd.print("V4: "); lcd.print(V4); lcd.print(" V");
      
    } else {
      lcd.print(" V en AC:");
      errorFlag |= leerCanalAC(CanalV1, V1);
      errorFlag |= leerCanalAC(CanalV2, V2);
      errorFlag |= leerCanalAC(CanalV3, V3);
      errorFlag |= leerCanalAC(CanalV4, V4);
      // Imprimimos los valores
      lcd.setCursor(0, 1);
      lcd.print("V1: "); lcd.print(V1); lcd.print(" V");

      lcd.setCursor(0, 2);
      lcd.print("V2: "); lcd.print(V2); lcd.print(" V");

      lcd.setCursor(0, 3);
      lcd.print("V3: "); lcd.print(V3); lcd.print(" V");

      lcd.setCursor(0, 4);
      lcd.print("V4: "); lcd.print(V4); lcd.print(" V");
    }

    if (sendCSV == true && isDC == true) {
      Serial.println(" V en DC:");
      Serial.print("V1: "); Serial.println(V1);
      Serial.print("V2: "); Serial.println(V2);
      Serial.print("V3: "); Serial.println(V3);
      Serial.print("V4: "); Serial.println(V4);
    } else if (sendCSV == true && isDC == false){
      Serial.println(" Vrms en AC:");
      Serial.print("V1: "); Serial.println(V1);
      Serial.print("V2: "); Serial.println(V2);
      Serial.print("V3: "); Serial.println(V3);
      Serial.print("V4: "); Serial.println(V4);
    }
    delay(1000);

    // Control del LED de alarma por sobre/subvoltaje
    if ( (V1 > 20.0) || (V2 > 20.0) ||
         (V3 > 20.0) || (V4 > 20.0) ||
         (V1 < -20.0) || (V2 < -20.0) ||
         (V3 < -20.0) || (V4 < -20.0) ) {
      digitalWrite(ALARM_LED_PIN, HIGH);
    } else {
      digitalWrite(ALARM_LED_PIN, LOW);
    }
}

// ============ Funciones ============

// Retorna true si hubo error en lectura DC
bool leerCanalDC(int pinCanal, float &salidaVar) {
  float raw = analogRead(pinCanal) * (5.0 / 1023.0);
  if (raw < 0 || raw > 5.0) {
    salidaVar = NAN;
    return true;
  }
  salidaVar = raw * (48.0 / 5.0) - 24.0;
  return false;
}

// Retorna true si hubo error en lectura AC
bool leerCanalAC(int pinCanal, float &salidaVar, int muestras) {
  float raw = 0;
  float new_raw = 0;
  for (int i = 0; i < muestras; i++) {
    new_raw = analogRead(pinCanal) * (5.0 / 1023.0);
    if (new_raw > raw){
      raw = new_raw;
    } 
    if (raw == 1023){
      break;
    }
  }
  if (raw < 0 || raw > 5.0) {
    salidaVar = NAN;
    return true;
  }
  salidaVar = (raw * (48.0 / 5.0) - 24.0) / sqrt(2.0);
  return false;
}

