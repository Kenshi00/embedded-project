/*
 * pwm.c
 *
 * Created: 2026-06-18 오후 2:23:32
 *  Author: kccistc
 */ 
#include "pwm.h"
extern int get_button(int button_num, int button_pin);
void init_timer3_pwm(void);
void init_motor_driver(void);
void dcmotor_pwm_control_main(void);
// 16bit 3번 timer/counter를 사용
/*
	PWM 출력 신호
	============
	PE3 : OC3A
	PE4 : OC3B : INT4 사용(초음파 센서)
	PE5 : OC3C ==> DC MOTOR (PWM 출력)
	BTN1 : speed-up
	BTN2 : speed-down
*/
void init_timer3_pwm(void)
{
	// 타이머 3번 PE3, PE4, PE5 101로 활성해놓음.
	DDRE |= 1 << 3 | 1 << 5;
	// 3. 타이머3 모드 5 (8비트 고속 PWM 모드)
	TCCR3A |= (1 << WGM30);
	TCCR3B |= (1 << WGM32);
	
	// 4. 비반전 모드 설정 (사진 맨 아래 주석 내용)
	// 모터 제어용인 C채널(PE5)을 비반전 모드로 설정
	TCCR3A |= (1 << COM3C1);
	
	// (만약 PE3 핀도 LED 제어 등으로 쓴다면 아래 줄도 주석 풀고 사용하세요)
	// TCCR3A |= (1 << COM3A1);

	// 5. 분주비 64 설정 (타이머 속도)
	TCCR3B |= (1 << CS31) | (1 << CS30);
	
	OCR3C = 0; // 초기 속도 0 (정지) - init함수이므로 motor가 돌지 않도록 safety로 막아둠.
}

/*
	PE5 : PWM control
	PE6 : IN1 방향 설정
	PE7 : IN2
*/

void init_motor_driver(void)
{
	// 1. 방향 제어 핀(PE6, PE7)을 출력(Output)으로 설정
	DDRF |= (1 << 6) | (1 << 7);
	
	// 2. 초기 모터 회전 방향 설정 (예: 정회전)
	// IN1(PE6)을 High, IN2(PE7)를 Low로 주어 안전하게 한 방향으로 초기화
	PORTF |= (1 << 6);    // PE6 = 1
	PORTF &= ~(1 << 7);   // PE7 = 0
}

void dcmotor_pwm_control_main(void)
{
	int start_button = 0;
	int forward = 0;
	
	while(1)
	{
		if(get_button(BUTTON0, BUTTON0PIN)) // start stop
		{
			start_button = !start_button;
			if(start_button) OCR3C = 250;
			else OCR3C = 0;
		}
		else if(get_button(BUTTON1, BUTTON1PIN)) // speed up
		{
			if(OCR3C >= 250) OCR3C = 250;
			else OCR3C += 20;
		}
		else if(get_button(BUTTON2, BUTTON2PIN)) // speed down
		{
			if(OCR3C <= 70) OCR3C = 70;
			else OCR3C -= 20;
		}
		else if(get_button(BUTTON3, BUTTON3PIN)) // 방향설정
		{
			forward = !forward;
			PORTF &= ~(1 << 6 | 1 << 7); // reset
			if(forward) PORTF |= 1 << 7; // 역회전
			else PORTF |= 1 << 6; // 정회전
		}
	}
	
}

