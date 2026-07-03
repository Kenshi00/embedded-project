/*
 * led.c
 *
 * Created: 2026-06-10 오후 3:10:18
 *  Author: kccistc
 */ 

#include "led.h"

void init_led(void);
void led_all_off(void);
void led_all_on(void);
void led_left_on(void);
void led_right_on(void);
void led_odd_on(void);
void led_even_on(void);

void init_led(void)
{
	DDRA = 0xff; // PORTA 를 출력 모드로 설정
	PORTA = 0x00; // PORTA에 물려있는 led를 all off
}

void led_all_on(void)
{
	PORTA = 0xff;
}

void led_all_off(void)
{
	PORTA = 0x00;
}

void led_right_on(void)
{
	PORTA = 0x0f;
}

void led_left_on(void)
{
	PORTA = 0xf0;
}

void led_odd_on(void)
{
	PORTA = 0b10101010;
}

void led_even_on(void)
{
	PORTA = PORTA >> 1;
}