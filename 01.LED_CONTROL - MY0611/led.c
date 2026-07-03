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
void led_shift_left_on(void);
void led_shift_right_on(void);
void led_shift_left_keepon(void);
void led_shift_right_keepon(void);
void led_flower_on(void);
void led_flower_off(void);
int led_main(void);

int led_main(void)
{
	init_led();
	led_all_off();
	
	while(1)
	{
		//led_shift_left_on();
		//led_shift_left_keepon();
		//led_shift_right_keepon();
		//led_flower_on();
		//led_flower_off();
	}
	
	return 0;
}

#define D 30

void init_led(void)
{
	DDRA=0xff;  // PORTA 를 출력 모드로 설정
	PORTA=0x00;  // PORTA에 물려있는 led를 all off
}

void led_all_on(void) // 1
{
	PORTA=0xff;
}

void led_all_off(void) // 2
{
	PORTA=0x00;
}

void led_shift_left_on(void) // for문으로 정의 되어있으면 i가 0~8 돌 동안 버튼을 눌러도 못나옴. ->
{                            // 함수 한번당 30ms 소요 -> 30ms 동안 못나옴
	static int i = 0;
	
	*(unsigned char *) 0x3B = 1 << i;
	_delay_ms(D);
	
	i = (i + 1) % 8;
}

void led_shift_right_on(void)
{
	static int i = 0;
	if(i == 0) PORTA = 0x00;
	*(unsigned char *) 0x3B = 0x80 >> i;
	_delay_ms(D);
	
	i = (i + 1) % 8;
}

void led_shift_left_keepon(void)
{
	static int i = 0;
	if(i == 0) PORTA = 0x00;
	PORTA |= 1 << i;   
	_delay_ms(D);
	
	i = (i + 1) % 8;
}

void led_shift_right_keepon(void)
{
	static int i = 0;
	if(i == 0) PORTA = 0x00; // 초기화 조건 추가
	PORTA |= (1 << (7 - i));
	_delay_ms(D);
	
	i = (i + 1) % 8;
}

void led_flower_on(void)
{
	static int i = 0;
	if(i == 0) PORTA = 0x00; // 초기화 조건 추가
	PORTA |= (1 << i + 4) | (1 << 3 - i);
	_delay_ms(D);
	
	i = (i + 1) % 4;
}

void led_flower_off(void)
{
	static int i = 0;
	if(i == 0) PORTA = 0xff;
	PORTA &= ~((1 << (7 - i)) | (1 << i)); //01111110
	_delay_ms(D);
	
	i = (i + 1) % 4;
}
