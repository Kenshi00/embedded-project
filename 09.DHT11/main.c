/*
 * 09.DHT11.c
 *
 * Created: 2026-06-26 오전 9:25:37
 * Author : kccistc
 */ 


#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h> // sei 등 함수
#include <stdio.h>


extern void init_uart0(void);
extern void UART0_transmit(uint8_t data);
extern void dht11_main(void);;

FILE OUTPUT = FDEV_SETUP_STREAM(UART0_transmit, NULL, _FDEV_SETUP_WRITE);

int main(void)
{
	init_uart0();
	stdout = &OUTPUT; // printf가 동작할수 있도록 stdout을 설정
	while (1)
	{
		dht11_main();
		_delay_ms(2000);
	}
}
