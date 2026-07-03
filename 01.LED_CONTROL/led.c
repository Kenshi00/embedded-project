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
	led_all_off();
	
	while(1)
	{
		led_shift_left_on();
		//led_shift_left_keepon();
		//led_shift_right_keepon();
		//led_flower_on();
		//led_flower_off();
	}
	
	return 0;
}

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

void led_shift_left_on(void) // for문으로 정의 되어있으면 i가 0~8 돌 동안 버튼을 눌러도 못나옴. ->
{                            
#if 1 // new                  // 함수 한번당 30ms 소요 -> 30ms 동안 못나옴
	static int i = 0;
	
	*(unsigned char *) 0x3B = 1 << i;
	_delay_ms(30);
	
	i = (i + 1) % 8;
	
#else // original
	for(int i = 0; i < 8; i++) // 240ms 소요.. 임베디드업계에서는 이렇게쓰면 안된다.
	{                          // for문으로 정의 되어있으면 i가 0~8 돌 동안 버튼을 눌러도 못나옴. -> 240ms 동안 못나옴
		//PORTA = 1 << i;    // PORTA : 0x1B
		*(unsigned char *) 0x3B = 1 << i;
		_delay_ms(30);
	}	
#endif
}

void led_shift_right_on(void)
{
#if 1
	static int i = 0;
	
	*(unsigned char *) 0x3B = 0x80 >> i;
	_delay_ms(30);
	
	i = (i + 1) % 8;
	
#else // original
	for(int i = 0; i < 8; i++)
	{
		*(unsigned char *) 0x3B = 0x80 >> i;
		_delay_ms(30);
	}
#endif
}

void led_shift_left_keepon(void)
{
	PORTA = 0x00;
	_delay_ms(300);
	for(int i = 0; i < 8; i++)
	{
		PORTA += 1 << i;    // PORTA : 0x1B
		_delay_ms(300);
	}
}

void led_shift_right_keepon(void)
{
	PORTA = 0x00;
	_delay_ms(300);
	for(int i = 7; i >= 0; i--)
	{
		PORTA += 1 << i;    // PORTA : 0x1B
		_delay_ms(300);
	}
}
//
void led_flower_on(void)
{
	PORTA = 0x00;
	_delay_ms(300);
	for(int i = 0; i < 4; i++)
	{
		PORTA += ((1 << i+4) | (1 << 3-i));
		_delay_ms(300);
	}
}

void led_flower_off(void)
{
	PORTA = 0xff;
	_delay_ms(300);
	for(int i = 3; i >= 0; i--)
	{
		PORTA -= ((1 << i+4) | (1 << 3-i));
		_delay_ms(300);
	}
}
