// ADD TO MAKEFILE ! SOURCE_FILES := $(wildcard source_files/*.c)

void SPI_MasterInit(void)
{
/* Set MOSI and SCK output, all others input */
DDRB = (1<<DDB4)|(1<<DDB5)|(1<<DDB7);
/* Enable SPI, Master, set clock rate fck/16 */
SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
}

void SPI_MasterTransmit(char cData)
{
/* Start transmission */
SPDR = cData;
/* Wait for transmission complete */
while(!(SPSR & (1<<SPIF)));
}

void SPI_SlaveInit(void)
{
/* Set MISO output, all others input */
DDRB = (1<<DDB6);
/* Enable SPI */
SPCR = (1<<SPE);
}
char SPI_SlaveReceive(void)
{
/* Wait for reception complete */
while(!(SPSR & (1<<SPIF)));
/* Return data register */
return SPDR;
}