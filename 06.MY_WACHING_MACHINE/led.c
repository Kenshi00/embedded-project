/*
 * led.c
 *
 * Created: 2026-06-10 오후 3:10:22
 *  Author: user
 */ 

#include "led.h"
void init_led(void);
void led_all_on(void);
void led_all_off(void);

void set_state_led(int state);


extern volatile uint32_t msec_count; // volatile 최적화 방지


void init_led(void)
{
	DDRA=0xff;  // PORTA 를 출력 모드로 설정
	PORTA=0x00;  // PORTA에 물려있는 led를 all off
}

void led_all_on(void)
{
	PORTA=0xff;
}

void led_all_off(void)
{
	PORTA=0x00;
}

// 상태에 따라 특정 LED만 켭니다.
void set_state_led(int state)
 {
	led_all_off();
	if(state == 0) PORTA |= (1 << 0);      // 세탁: LED0 ON
	else if(state == 1) PORTA |= (1 << 1); // 헹굼: LED1 ON
	else if(state == 2) PORTA |= (1 << 2); // 탈수: LED2 ON
}
