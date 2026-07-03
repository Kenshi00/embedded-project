/*
 * 01.LED_CONTROL.c
 *
 * Created: 2026-06-10 오전 10:20:27
 * Author : user
 */ 

#define F_CPU 16000000UL   // 16MHz
#include <avr/io.h>  // PORTA PORTD 등의 I/O register들이 들어 있다. 
#include <util/delay.h>   // _delay_ms _delay_us 등의 함수 가 들어 있다. 

#include "button.h"

extern void init_led(void);  // init_led함수는 다른 화일에 들어 있다고 compiler 신고 
extern void init_button(void);
extern int get_button(int button_num, int button_pin);
extern int led_main(void);

extern void led_all_off(void);
extern void led_all_on(void);
extern void led_shift_left_on(void);
extern void led_shift_right_on(void);
extern void led_shift_left_keepon(void);
extern void led_shift_right_keepon(void);
extern void led_flower_on(void);
extern void led_flower_off(void);

int main(void)
{
	int button0_state = 0;   // 초기 상태를 led all off로 출발 하자 
	
	init_button();
	init_led();
	
	// led_main();
	
	while (1)
	{
		// 버튼 0~7까지 계속 돈다.
		if (get_button(BUTTON0, BUTTON0PIN))
		{
			button0_state = (button0_state + 1) % 8;
		}
		
		if (button0_state == 0) led_all_off();
		else if(button0_state == 1) led_all_on();
		else if(button0_state == 2) led_shift_left_on();
		else if(button0_state == 3) led_shift_right_on();
		else if(button0_state == 4) led_shift_left_keepon();
		else if(button0_state == 5) led_shift_right_keepon();
		else if(button0_state == 6) led_flower_on();
		else if(button0_state == 7) led_flower_off();
	}
}