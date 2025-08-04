/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2014 Chuck McManis <cmcmanis@mcmanis.com>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/spi.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include "clock.h"
#include "console.h"
#include "sdram.h"
#include "devices.h"
#include "gfx.h"
#include "lcd-spi.h"

int16_t x = 0;
int16_t y = 0;
int16_t z = 0;

int main(void)
{
	bool usart_enable;
	int led_blink;
	float volts_f, volts_f_real;


	clock_setup();
	console_setup(115200);
	spi_init();
	pin_analogic();
	pin_digital_setup();
	gyro_init();
	com_en = 1;
	sdram_init();
	lcd_spi_init();
	msleep(2000);
	gfx_init(lcd_draw_pixel, 240, 320);
	gfx_fillScreen(LCD_GREY);
	gfx_fillRoundRect(10, 10, 220, 220, 5, LCD_WHITE);
	gfx_drawRoundRect(10, 10, 220, 220, 5, LCD_RED);
	gfx_fillCircle(20, 250, 10, LCD_RED);
	gfx_fillCircle(120, 250, 10, LCD_GREEN);
	gfx_fillCircle(220, 250, 10, LCD_BLUE);

	// vvv Pantalla de inicio vvv
	gfx_setTextSize(2);
	gfx_setCursor(15, 25);
	gfx_puts("Laboratorio 4");

	gfx_setCursor(50, 50);
	gfx_puts("IE-0625");

	gfx_setTextSize(1);

	gfx_setCursor(12, 80);
	gfx_puts("Docente:MSc. Marco Villalta");

	gfx_setCursor(12, 100);
	gfx_puts("Estudiantes:");

	gfx_setCursor(12, 120);
	gfx_puts("Luis Brenes-C21324");

	gfx_setCursor(12, 140);
	gfx_puts("Elizabeth Matamoros-C04652");

	// ^^^ Pantalla de inicio ^^^
	lcd_show_frame();
	msleep(1000);
	gfx_setTextColor(LCD_WHITE, LCD_BLACK);
	int i = 0;  // declarar fuera del while para que conserve valor
	char buffer[30];  // buffer para conversión de números
	led_blink = 0;
	while (1) {
		uint8_t temp;
        uint8_t who;
		uint16_t volts = read_adc_naiive(0);
		volts_f = (volts * 3.3)/4095;
		volts_f_real = volts_f*(9940+9800+7370)/9940;
		if (volts_f_real <= 7){
			if (led_blink <= 3){
				set_led(true);
				led_blink = led_blink + 1;
			} else {
				set_led(false);
				led_blink = 0;
			}
			
		}else{
			set_led(false);
		}

		usart_enable = gpio_get(GPIOB, GPIO4);
		if (usart_enable) {
				com_en = 0;
		}else {
				com_en = 1;
		}

		gfx_fillScreen(LCD_BLACK);
		gfx_setTextSize(3);
		gfx_setCursor(15, 30);
		gfx_puts("Datos:");
		gfx_setTextSize(2);
		gfx_setCursor(15, 70);
		gfx_puts("Ejes:");
		gfx_setTextSize(1);

		// Eje x
		gfx_setCursor(15, 110);
		gfx_puts("Eje x: ");
		sprintf(buffer, "%d", x);
		gfx_puts(buffer);

		// Eje y
		gfx_setCursor(15, 130);
		gfx_puts("Eje y: ");
		sprintf(buffer, "%d", y);
		gfx_puts(buffer);

		// Eje z
		gfx_setCursor(15, 150);
		gfx_puts("Eje z: ");
		sprintf(buffer, "%d", z);
		gfx_puts(buffer);

		// Tensión
		gfx_setTextSize(2);
		gfx_setCursor(15, 180);
		gfx_puts("Tension: ");

		gfx_setCursor(15, 220);
		sprintf(buffer, "%2.f V", volts_f_real);
		gfx_puts(buffer);
		gfx_setTextSize(1);

		// Comunicación USART
		gfx_setCursor(15, 290);
		gfx_puts("Comunicacion USART:");

		gfx_setCursor(180, 290);

		if (usart_enable == 0) {
			gfx_puts("high");
		} else {
			gfx_puts("low");
		}
		

		lcd_show_frame();

		// Obtención del valor de los ejes

		// Uso del WHOAMI
		gyro_cs_low();            
		spi_send(SPI_BUS, GYR_WHO_AM_I | 0x80);
		spi_read(SPI_BUS); 
		spi_send(SPI_BUS, 0);    
		who=spi_read(SPI_BUS);
		gyro_cs_high();

		gyro_cs_low();
		spi_send(SPI_BUS, GYRO_REG_STATUS | GYRO_RNW_BIT);
		spi_read(SPI_BUS);
		spi_send(SPI_BUS, 0);
		temp=spi_read(SPI_BUS);
		gyro_cs_high();

		gyro_cs_low();
		spi_send(SPI_BUS, GYRO_REG_OUT_TEMP | GYRO_RNW_BIT);
		spi_read(SPI_BUS);
		spi_send(SPI_BUS, 0);
		temp=spi_read(SPI_BUS);
		gyro_cs_high();  


		// Eje X
		gyro_cs_low();
		spi_send(SPI_BUS, GYR_OUT_X_L | GYRO_RNW_BIT);
		spi_read(SPI_BUS);
		spi_send(SPI_BUS, 0);
		x=spi_read(SPI_BUS);
		gyro_cs_high();

		gyro_cs_low();
		spi_send(SPI_BUS, GYR_OUT_X_H | GYRO_RNW_BIT);
		spi_read(SPI_BUS);
		spi_send(SPI_BUS, 0);
		x|=spi_read(SPI_BUS) << 8;
		gyro_cs_high();

		// Eje y
		gyro_cs_low();
		spi_send(SPI_BUS, GYR_OUT_Y_L | GYRO_RNW_BIT);
		spi_read(SPI_BUS);
		spi_send(SPI_BUS, 0);
		y=spi_read(SPI_BUS);
		gyro_cs_high();

		gyro_cs_low();
		spi_send(SPI_BUS, GYR_OUT_Y_H | GYRO_RNW_BIT);
		spi_read(SPI_BUS);
		spi_send(SPI_BUS, 0);
		y|=spi_read(SPI_BUS) << 8;
		gyro_cs_high();

		// Eje Z
		gyro_cs_low();
		spi_send(SPI_BUS, GYR_OUT_Z_L | GYRO_RNW_BIT);
		spi_read(SPI_BUS);
		spi_send(SPI_BUS, 0);
		z=spi_read(SPI_BUS);
		gyro_cs_high();

		gyro_cs_low();
		spi_send(SPI_BUS, GYR_OUT_Z_H | GYRO_RNW_BIT);
		spi_read(SPI_BUS);
		spi_send(SPI_BUS, 0);
		z|=spi_read(SPI_BUS) << 8;
		gyro_cs_high();

		// Sensibilidad
        x = x*L3GD20_SENS_500;
        y = y*L3GD20_SENS_500;
        z = z*L3GD20_SENS_500;

		print_decimal(x); console_puts("\t");
		print_decimal(y); console_puts("\t");
		print_decimal(z); console_puts("\t");
		print_decimal(volts_f_real); console_puts("\n");

		for (int g = 0; g < 80000; g++)    /* Wait a bit. */
			__asm__("nop");

		msleep(100);

	}
}
