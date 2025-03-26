/*
Elizabeth Matamoros Bojorge C04652

El programa simula un dado electronico usando 
un PIC12F683. Al presionar un boton, se genera 
un numero aleatorio del 1 al 6 y se encienden LEDs
segun la cara del lado.   
*/

#include <pic14/pic12f683.h> // Biblioteca para el PIC12F683
#include <stdint.h> // Biblioteca para unit8_t
#include <stdio.h> // Biblioteca para funciones de entrada y salida estandar

// Desactiva el Watchdog Timer
typedef unsigned int word;
word __at 0x2007 __CONFIG = (_WDTE_OFF);

// Variables internas del generador aleatorio
uint8_t state[2] = { 0x00, 0xA3 };

// Declaracion de funciones
uint8_t rotateLeft(uint8_t value, int shift);
uint8_t generateRandomByte(void);
int generateDiceNumber(void);
void handleLEDOutput(uint8_t diceValue);
void customDelayAndSeed(uint16_t timeDelay);

// Funcion principal
void main(void) {
    // Configura GP5 como entrada, el resto como salida
    TRISIO = 0b00100000;
    GPIO = 0x00;

    uint16_t waitTime = 100;
    uint8_t diceResult = 0;

    // Bucle infinito
    while (1) {
        if (GP5) {
            // Genera el número aleatorio entre 1 y 6
            diceResult = generateDiceNumber();

            // Ejecuta la salida correspondiente al valor del dado
            handleLEDOutput(diceResult);

            // Delay con cambio interno en el estado del generador
            customDelayAndSeed(waitTime);
        } else {
            // Apaga todos los LEDs
            GPIO = 0x00;
        }
    }
}

// Funcion que enciende los LEDs segun el numero del dado
void handleLEDOutput(uint8_t diceValue) {
    // Evalua que numero salio y enciede el LEDs con una combinacion fija
    switch (diceValue) {
        case 1:
            GP1 = 1;
            break;
        case 2:
            GP0 = 1;
            break;
        case 3:
            GPIO = 0b00000011;
            break;
        case 4:
            GPIO = 0b00000101;
            break;
        case 5:
            GPIO = 0b00000111;
            break;
        case 6:
            GPIO = 0b00010101;
            break;
    }

    // Retardo y apagado de LEDs
    customDelayAndSeed(200);
    GPIO = 0x00;
    GP0 = 0;
    GP1 = 0;
}

// Funcion que simula un retardo y tambien cambia el estado interno del generador
void customDelayAndSeed(uint16_t timeDelay) {
    for (uint16_t i = 0; i < timeDelay; i++) {
        for (uint16_t j = 0; j < 1275; j++) {
            // Se actualiza el estado 
            state[0] ^= (j & 0xFF);
            state[1] ^= (i & 0xFF);
        }
    }
}

// Desplazamiento rotativo a la izquierda
uint8_t rotateLeft(uint8_t value, int shift) {
    return (value << shift) | (value >> (8 - shift));
}

// Generador pseudoaleatorio basado en Xorshift
uint8_t generateRandomByte(void) {
    uint8_t temp0 = state[0];
    uint8_t temp1 = state[1];
    uint8_t result = temp0 + temp1;

    temp1 ^= temp0;
    state[0] = rotateLeft(temp0, 6) ^ temp1 ^ (temp1 << 1);
    state[1] = rotateLeft(temp1, 3);

    return result;
}

// Genera numero entre 1 y 6 
int generateDiceNumber(void) {
    int randomVal = 0;
    while (1) {
        randomVal = generateRandomByte();
        if (randomVal > 0 && randomVal < 7) {
            return randomVal;
        }
    }
}