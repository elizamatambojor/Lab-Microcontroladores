#include <avr/io.h>
#include <avr/interrupt.h>

int count = 0; // Contador para cada 0.5 segundos
int pedestrian_request = 0; // Flag en caso de que se active un botón
int pedestrian_crossing = 0; // Flag para iniciar el proceso de paso del peatón
int pedestrian_end = 0; // Flag para iniciar el proceso de paso de los Vehículos

void setupTimer() {
    // Configuración del temporizador 1 en modo CTC (Clear Timer on Compare Match)
    TCCR1A = 0;  // Configura en modo normal
    TCCR1B = (1 << WGM12) | (1 << CS12);  // Modo CTC, prescaler de 256 (CS12 = 1, CS10 = 0)
    
    // Establecer el valor de comparación (para que el temporizador cuente 0.5 segundos)
    OCR1A = 31250;  // 0.5 segundos con prescaler de 256 y contador de 16 bits
    
    // Habilitar la interrupción por comparación en OCR1A
    TIMSK |= (1 << OCIE1A);  // Habilita la interrupción por comparación
}

void setupInterrupt() {
    // Configurando el pin D2 para interrupciones
    MCUCR |= (1 << ISC01) | (1 << ISC00);  // Flanco ascendente en INT0
    GIMSK |= (1 << INT0);  // Habilita la interrupción externa INT0 (D2)
}

void setupPins() {
    /**Configuración de los pines de relacionados a los LED's:
    * B0: LDPV
    * B1: LDVD
    * B2: LDPP
    * B3: LDPD
    * */
    DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB3);  // ConfiguraDOS como salidaS

    // Valor inicial de los LEDs: LED's configurados para paso de peatones
    PORTB |= (1 << PB0);  // LED en B0 encendido
    PORTB &= ~(1 << PB1); // LED en B1 apagado
    PORTB &= ~(1 << PB2);  // LED en B2 apagado
    PORTB |= (1 << PB3); // LED en B3 encendido
}

ISR(INT0_vect) {
    // Interrupción del pin D2

    // Se activa el flag que refleja la petición de paso por parte del peatón
    pedestrian_request = 1;
}

// Función para manejar la petición de paso de peatones
void initialPedestrianCrossing() {
    // Si hay una solicitud de peatón y el tiempo ha transcurrido al menos 10 segundos
    if (count >= 20 && pedestrian_request == 1 && pedestrian_crossing == 0) {
        pedestrian_crossing = 1;  // Inicia el proceso cruce de peatones
        count = 0;  // Reinicia el contador
        pedestrian_request = 0; // Reinicia la petición
        
        PORTB &= ~(1 << PB0);  // LED en B0 apagado
    }
}

// Función para finalizar el cruce de peatones
void endPedestrianCrossing() {
    if (count == 28 && pedestrian_crossing == 1) {
        pedestrian_crossing = 0;  // Finaliza el proceso de cruce de peatones
        count = 0;  // Reinicia el contador
        pedestrian_request = 0;   // Resetea la solicitud de peatón
        pedestrian_end = 1; // Incia el proceso para el cruce de vehículos
        
        PORTB &= ~(1 << PB2); // LED en B2 apagado
    }
}

void flikerVehicleLED() {
  // Función que hace parpadear la luz verde para vehículos
  if ((count == 1 || count == 3 || count == 5) && pedestrian_crossing == 1){
    PORTB |= (1 << PB0);  // LED en B0 encendido
  } else if ((count == 2 || count == 4) && pedestrian_crossing == 1){
    PORTB &= ~(1 << PB0);  // LED en B0 apagado
  } else if (count == 6 && pedestrian_crossing == 1){
    PORTB &= ~(1 << PB0);  // LED en B0 apagado
    PORTB |= (1 << PB1);   // LED en B1 encendido
  } else if  (count == 8 && pedestrian_crossing == 1){
    PORTB |= (1 << PB2); // LED en B2 encendido
    PORTB &= ~(1 << PB3);  // LED en B3 apagado
  }
}

void flikerPedestrianLED() {
  // Función que hace parpadear la luz verde para peatones
  if ((count == 1 || count == 3 || count == 5) && pedestrian_end == 1){
    PORTB |= (1 << PB2); // LED en B2 encendido
  } else if ((count == 2 || count == 4) && pedestrian_end == 1){
    PORTB &= ~(1 << PB2); // LED en B2 apagado
  } else if (count == 6 && pedestrian_end == 1){
    PORTB &= ~(1 << PB2); // LED en B2 apagado
    PORTB |= (1 << PB3);  // LED en B3 encendido
  } else if  (count == 8 && pedestrian_end == 1){
    PORTB |= (1 << PB0); // LED en B0 encendido
    PORTB &= ~(1 << PB1);  // LED en B1 apagado
    pedestrian_end = 0; // Finalizamos el proceso de inicio de paso de vehículos
    count = 0; // Reiniciamos el contador
    pedestrian_request = 0; // Reiniciamos la petición del peatón para evitar problemas en medio del proceso
  }
}

ISR(TIMER1_COMPA_vect) {
    // Esta ISR se ejecuta cada vez que el temporizador alcanza el valor de OCR1A (15625)
    count++;  // Incrementa el contador cada vez que el temporizador se desborda
    initialPedestrianCrossing();
    flikerVehicleLED();
    endPedestrianCrossing();
    flikerPedestrianLED();
}

int main(void) {
    // Configuración de pines e interrupciones
    setupPins();
    setupInterrupt();
    setupTimer();
    
    sei();  // Habilita las interrupciones globales
    
    // while para mantener el programa encendido
    while (1) {
        // El microcontrolador sigue ejecutando el programa normal
        // Espera a que la interrupción ocurra en D2
    }
}