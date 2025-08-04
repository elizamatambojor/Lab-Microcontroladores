#ifndef DEVICES_H
#define DEVICES_H

#include <stdbool.h>
#include <stdint.h>

// LED
#define LED_PORT GPIOG
#define LED_PIN GPIO13

/* --- SPI y pines comunes --- */
#define SPI_BUS SPI5

// Pin CS para Giroscopio

#define GYRO_CS_PORT GPIOC
#define GYRO_CS_PIN  GPIO1


// Giroscopio - registros y constantes
#define GYRO_RNW_BIT            (1 << 7)   // Bit en '1' para lectura, '0' para escritura
#define GYRO_BURST_BIT          (1 << 6)   // Lectura múltiple 
#define GYRO_CTRL1_POWER_ON   (1 << 3)   // Poner el giroscopio en modo activo (Power-down=0, Active=1)
#define GYRO_CTRL1_ENABLE_X   (1 << 1)   // Habilitar eje X
#define GYRO_CTRL1_ENABLE_Y   (1 << 0)   // Habilitar eje Y
#define GYRO_CTRL1_ENABLE_Z   (1 << 2)   // Habilitar eje Z
#define GYRO_REG_STATUS         0x27       // Registro de estado
#define GYRO_REG_OUT_TEMP       0x26       // Registro de temperatura (solo fines de prueba)
#define GYR_WHO_AM_I    0x0F
#define GYR_CTRL_REG1   0x20
#define GYR_CTRL_REG4   0x23
#define GYR_OUT_X_L     0x28
#define GYR_OUT_X_H     0x29
#define GYR_OUT_Y_L     0x2A
#define GYR_OUT_Y_H     0x2B
#define GYR_OUT_Z_L     0x2C
#define GYR_OUT_Z_H     0x2D
#define L3GD20_SENS_500  (0.0175F)

// Prototipos
void spi_init(void);

void lcd_init(void);
 
void gyro_cs_low(void); 
void gyro_cs_high(void);

int print_decimal(int num);


void gyro_init(void);

void set_led(bool on);

void pin_analogic(void);
uint16_t read_adc_naiive(uint8_t channel);

extern uint8_t com_en;

void pin_digital_setup(void);

#endif
