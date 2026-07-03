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
void led_distance_display(int distance);

extern volatile uint32_t msec_count; // volatile 최적화 방지

#define FNUC_SU 6

int func_state = 0;
void (*fp[]) () =
{
	led_shift_left_on, // func_state = 0
	led_shift_right_on,
	led_shift_left_keepon,
	led_shift_right_keepon,
	led_flower_on,
	led_flower_off // func_state = 5
};

int led_main(void)
{
	uint8_t led_toggle = 0;
	
	led_all_off();
	
	while(1)
	{
#if 1
		fp[func_state] ();
#else
		if(msec_count >= 500) // 500ms if (msec_count == 500) 499에서 501로 튀는경우 있음
		{
			msec_count = 0;
			led_toggle = !led_toggle;
			if(led_toggle) led_all_on();
			else led_all_off();	
		}
		//led_shift_left_on();
		//led_shift_left_keepon();
		//led_shift_right_keepon();
		//led_flower_on();
		//led_flower_off();		
#endif
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
	static int i = 0;
	
	if(msec_count >= 100) // 30초 넘을경우 if문에 들어와서 이 행위를 하는것 자체가 delay 30ms를 만든 것
	{
		msec_count = 0;
		*(unsigned char *) 0x3B = 1 << i;
		i = (i + 1) % 8;
/*		
		if ( (i = (i + 1) % 8) == 0) // i가 7까지 쭉 돌면, 다음 실행할 func으로 jump
		{
			func_state = (func_state + 1) % FNUC_SU;
		}
		*/
	}
}

void led_shift_right_on(void)
{
	static int i = 0;

	if(msec_count >= 100) // 30초 넘을경우 if문에 들어와서 이 행위를 하는것 자체가 delay 30ms를 만든 것
	{
		msec_count = 0;
		*(unsigned char *) 0x3B = 0x80 >> i;
		i = (i + 1) % 8;
		
/*		if ( (i = (i + 1) % 8) == 0) // i가 7까지 쭉 돌면, 다음 실행할 func으로 jump
		{
			func_state = (func_state + 1) % FNUC_SU;
		}
		*/
	}
}

void led_shift_left_keepon(void)
{
	static int i = 0;

	if(msec_count >= 100) // 30초 넘을경우 if문에 들어와서 이 행위를 하는것 자체가 delay 30ms를 만든 것
	{
		msec_count = 0;
		if(i == 0) PORTA = 0x00; // 초기화 조건 추가
		PORTA |= 1 << i;
		i = (i + 1) % 8;
		
/*		if ( (i = (i + 1) % 8) == 0) // i가 7까지 쭉 돌면, 다음 실행할 func으로 jump
		{
			func_state = (func_state + 1) % FNUC_SU;
		}
		*/
	}
}

void led_shift_right_keepon(void)
{
	static int i = 0;

	if(msec_count >= 100) // 30초 넘을경우 if문에 들어와서 이 행위를 하는것 자체가 delay 30ms를 만든 것
	{
		msec_count = 0;
		if(i == 0) PORTA = 0x00; // 초기화 조건 추가
		PORTA |= (1 << (7 - i));
		i = (i + 1) % 8;
		
/*		if ( (i = (i + 1) % 8) == 0) // i가 7까지 쭉 돌면, 다음 실행할 func으로 jump
		{
			func_state = (func_state + 1) % FNUC_SU;
		}
		*/
	}
}

void led_flower_on(void)
{
	static int i = 0;
	if(msec_count >= 100) // 30초 넘을경우 if문에 들어와서 이 행위를 하는것 자체가 delay 30ms를 만든 것
	{
		msec_count = 0;
		if(i == 0) PORTA = 0x00; // 초기화 조건 추가
		PORTA |= (1 << i + 4) | (1 << 3 - i);
		i = (i + 1) % 4;
		
/*		if ( (i = (i + 1) % 4) == 0) // i가 7까지 쭉 돌면, 다음 실행할 func으로 jump
		{
			func_state = (func_state + 1) % FNUC_SU;
		}
		*/
	}
}

void led_flower_off(void)
{
	static int i = 0;

	if(msec_count >= 100) // 30초 넘을경우 if문에 들어와서 이 행위를 하는것 자체가 delay 30ms를 만든 것
	{
		msec_count = 0;
		if(i == 0) PORTA = 0xff;
		PORTA &= ~((1 << (7 - i)) | (1 << i)); //01111110
		i = (i + 1) % 4;
		
/*		if ( (i = (i + 1) % 4) == 0) // i가 7까지 쭉 돌면, 다음 실행할 func으로 jump
		{
			func_state = (func_state + 1) % FNUC_SU;
		}
		*/
	}
}


void led_distance_display(int distance)
{
	if (distance <= 2) {
		PORTA = 0x01;
		} else if (distance <= 3) {
		PORTA = 0x03;
		} else if (distance <= 4) {
		PORTA = 0x07;
		} else if (distance <= 5) {
		PORTA = 0x0F;
		} else if (distance <= 6) {
		PORTA = 0x1F;
		} else if (distance <= 7) {
		PORTA = 0x3F;
		} else if (distance <= 10) {
		PORTA = 0x7F;
		} else {
		PORTA = 0xFF; // 10cm 초과 (메모상 '10cm 이상' 처리)
	}
}