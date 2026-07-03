/*
 * 01.LED_CONTROL.c
 *
 * Created: 2026-06-10 오전 10:20:29
 * Author : kccistc
 */ 

#define F_CPU 16000000UL // 16MHz
#include <avr/io.h> // PORTA PORTD 등의 I/O register들이 들어있다.
#include <util/delay.h> // _delay_ms _delay_us 등의 함수가 들어있다.
#include "button.h"
// 디바이스 드라이버를 깔아줘야 장치관리자에 포트부분이 생겨서 COM3이 뜬다. jtag같은거인듯
// 빌드 후 실행 : ctrl + alt + f5
// 지정한 함수로 이동 : alt + g

// 이 함수는 main.c에 있는거 아니고 led.c에있으니 여기다 알려줘야함.(compiler에 신고)
extern void init_led(void); 
extern void init_button(void);
extern void led_left_on(void);
extern void led_right_on(void);
extern void led_odd_on(void);
extern void led_even_on(void);
extern void led_all_off(void);
extern void led_all_on(void);
extern int get_button(int button_num, int button_pin);

// left right
#if 1
int main(void)
{
	int button0_state = 0; // 초기 상태를 led all off로 출발함.
	// 버튼, led 초기화
	init_button();
	init_led();
	
	while(1)
	{
		// toggle off <--> on
		// get_button은 스위치를 누르면 실행되는 것..
		if (get_button(BUTTON3, BUTTON3PIN))
		{
			button0_state = 0;
			led_all_off();
		}
		
		else if (get_button(BUTTON0, BUTTON0PIN))
		{
			button0_state++;
			button0_state = button0_state % 4; // 반전 0 <--> 1
			// 1이면 다킨다
			if(button0_state == 1) led_all_on();
			else if(button0_state == 2) led_right_on();
			else if(button0_state == 3) led_left_on();
			else led_all_off();
		}
	}
}
#endif

// odd even
#if 0
int main(void)
{
	int button0_state = 0; // 초기 상태를 led all off로 출발함.
	// 버튼, led 초기화
	init_button();
	init_led();
	
	while(1)
	{
		if (get_button(BUTTON3, BUTTON3PIN))
		{
			button0_state = 0;
			led_all_off();
		}
		// get_button은 스위치를 누르면 실행되는 것..
		else if (get_button(BUTTON0, BUTTON0PIN))
		{
			button0_state++;
			button0_state = button0_state % 4; // 반전 0 <--> 1
			// 1이면 다킨다
			if(button0_state == 1) led_all_on();
			else if(button0_state == 2) led_odd_on();
			else if(button0_state == 3) led_even_on();
			else led_all_off();
		}
	}
}
#endif


