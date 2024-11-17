#ifndef __UART_H
#define __UART_H

void USART_init( unsigned int ubrr );
void USART_transmit( unsigned char data );
unsigned int USART_receive( void );

#endif