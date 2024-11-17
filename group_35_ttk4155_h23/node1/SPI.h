#ifndef __SPI_H
#define __SPI_H

void spi_set_ss();
void spi_clear_ss();
void spi_master_init(void);
void spi_write(char cData);
uint8_t spi_read();
void test_mosi_clear();

#endif