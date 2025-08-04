#include "devices.h"
#include <libopencm3/stm32/spi.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/adc.h>
#include "console.h"
#include "clock.h"
#include "sdram.h"
#include <stdio.h>

uint8_t com_en;

// Control CS
void gyro_cs_low(void) { gpio_clear(GYRO_CS_PORT, GYRO_CS_PIN); }
void gyro_cs_high(void){ gpio_set(GYRO_CS_PORT, GYRO_CS_PIN); }


// Inicialización compartida SPI y GPIO
void spi_init(void) {
    rcc_periph_clock_enable(RCC_SPI5);
	rcc_periph_clock_enable(RCC_GPIOC);
	rcc_periph_clock_enable(RCC_GPIOF);

    // Configurar CS
    gpio_mode_setup(GYRO_CS_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GYRO_CS_PIN);
    gyro_cs_high();
    // Pines SPI: PF7=SCK, PF8=MISO, PF9=MOSI, AF5
    gpio_mode_setup(GPIOF, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO7 | GPIO8 | GPIO9);
    gpio_set_af(GPIOF, GPIO_AF5, GPIO7 | GPIO8 | GPIO9);


    spi_set_master_mode(SPI_BUS);
    spi_set_baudrate_prescaler(SPI_BUS, SPI_CR1_BR_FPCLK_DIV_64);
    spi_set_clock_polarity_0(SPI_BUS);      // CPOL = 0
    spi_set_clock_phase_0(SPI_BUS);         // CPHA = 0 (modo 0)
    spi_set_full_duplex_mode(SPI_BUS);
	spi_set_unidirectional_mode(SPI_BUS);
	spi_enable_software_slave_management(SPI_BUS);
	spi_send_msb_first(SPI_BUS);
    spi_set_nss_high(SPI_BUS);
    SPI_I2SCFGR(SPI_BUS) &= ~SPI_I2SCFGR_I2SMOD;
    spi_enable(SPI_BUS);

    /* Enable GPIOG clock. */
	rcc_periph_clock_enable(RCC_GPIOG);

	/* Set GPIO13 (in GPIO port G) to 'output push-pull'. */
	gpio_mode_setup(GPIOG, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO13  | GPIO14);
}


/*
 * int len = print_decimal(int value)
 *
 * Very simple routine to print an integer as a decimal
 * number on the console.
 */
int
print_decimal(int num)
{
	int		ndx = 0;
	char	buf[10];
	int		len = 0;
	char	is_signed = 0;
	
	if (com_en) {
		if (num < 0) {
			is_signed++;
			num = 0 - num;
		}
		buf[ndx++] = '\000';
		do {
			buf[ndx++] = (num % 10) + '0';
			num = num / 10;
		} while (num != 0);
		ndx--;
		if (is_signed != 0) {
			console_putc('-');
			len++;
		}
		while (buf[ndx] != '\000') {
			console_putc(buf[ndx--]);
			len++;
		}
		gpio_toggle(GPIOG, GPIO14);
	}
	else{
		len = 0;
		gpio_clear(GPIOG, GPIO14);
	}
	return len; /* number of characters printed */
}


void gyro_init(void) {
    gyro_cs_low();
	spi_send(SPI_BUS, GYR_CTRL_REG1); 
	spi_read(SPI_BUS);
	spi_send(SPI_BUS, GYRO_CTRL1_POWER_ON | GYRO_CTRL1_ENABLE_X | GYRO_CTRL1_ENABLE_Y | GYRO_CTRL1_ENABLE_Z | (3 << 4));
	spi_read(SPI_BUS);
	gyro_cs_high();

	gyro_cs_low();
	spi_send(SPI_BUS, GYR_CTRL_REG4);
	spi_read(SPI_BUS);
	spi_send(SPI_BUS, (1 << 4));
	spi_read(SPI_BUS);
	gyro_cs_high();
}

void set_led(bool on) {
    if(on)
        gpio_set(LED_PORT, LED_PIN);
    else
        gpio_clear(LED_PORT, LED_PIN);
}

void pin_analogic(void){

    rcc_periph_clock_enable(RCC_ADC1);
  	rcc_periph_clock_enable(RCC_GPIOA);
	gpio_mode_setup(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO0); // PA0 analógico
	adc_power_off(ADC1);
	adc_disable_scan_mode(ADC1);
	adc_set_sample_time_on_all_channels(ADC1, ADC_SMPR_SMP_3CYC);
	adc_power_on(ADC1);
}

uint16_t read_adc_naiive(uint8_t channel)
{
    uint8_t channel_array[16];
    channel_array[0] = channel;
    adc_set_regular_sequence(ADC1, 1, channel_array);
    adc_start_conversion_regular(ADC1);
    uint16_t reg16 = adc_read_regular(ADC1);
	return reg16;
}

void pin_digital_setup(void) {
    rcc_periph_clock_enable(RCC_GPIOB);
    gpio_mode_setup(GPIOB, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO4);
}