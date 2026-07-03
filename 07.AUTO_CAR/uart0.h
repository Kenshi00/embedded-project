/*
 * uart0.h
 *
 * Created: 2026-06-16 오전 9:57:45
 *  Author: kccistc
 */ 

#define F_CPU 16000000UL  // 16MHz
#include <avr/io.h>  // PORTA PORTB PORTD... IO관련 reg가 들어 있다.
#include <util/delay.h>  // _delay_ms _delay_us 등
#include <avr/interrupt.h>
#include <string.h>
#include <stdio.h> // <--- 이 줄을 반드시 추가하세요!

extern void init_uart0(void);
extern void UART0_transmit(uint8_t data);