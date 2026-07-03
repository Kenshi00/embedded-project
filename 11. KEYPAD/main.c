/*
 * 04. UART.c
 *
 * Created: 2026-06-15 오전 11:37:00
 * Author : kccistc
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h> // sei 등 함수
#include <stdio.h>
#include "keypad.h"
#include "queue.h"
#include "cal.h"

extern int led_main(void);
extern void init_uart0(void);
extern void UART0_transmit(uint8_t data);

FILE OUTPUT = FDEV_SETUP_STREAM(UART0_transmit, NULL, _FDEV_SETUP_WRITE);

volatile uint32_t keypad_count = 0; // volatile 최적화 방지

ISR(TIMER0_OVF_vect)
{
	volatile uint8_t keydata = 0;
	
	TCNT0 = 6; // TCNT0 6~256 : 250개 펄스 count함.
	if(++keypad_count >= 60) // 1ms count
	{
		keypad_count = 0;
		if(keydata = keypad_scan()) // keypad를 check해서 눌려 진 것이 있으면 circular queue에 저장한다.
		{
			insert_queue(keydata); // circular queue에 저장
		}
	}
	
}

int main(void)
{
	uint8_t key_value;
	
	init_timer0();
	init_uart0();
	stdout = &OUTPUT; // printf가 동작할수 있도록 stdout을 설정
	sei(); // 전역(대문) interrupt 허용
	init_keypad();
	
	printf("Calculator Ready!\n");
	
    while (1) 
    {
		if(queue_empty() != TRUE)
		{
			key_value = read_queue();
			calculator_process(key_value);
			//printf("key_value: %c\n", key_value);			
		}
    }
}

void init_timer0(void)
{
	TCNT0 = 6; // TCNT0 6~256 : 250개 펄스 count함.
	TCCR0 &= ~(1 << CS02 | 1 << CS01 | 1 << CS00); // 초기화
	TCCR0 |= 1 << CS02 | 0 << CS01 | 0 << CS00; // 64분주
	TIMSK |= 1 << TOIE0; // TIMER0 Overflow interrupt
}

