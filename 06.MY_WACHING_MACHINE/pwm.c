/*
 * pwm.c
 *
 * Created: 2026-06-18 오후 2:23:32
 *  Author: kccistc
 */ 
#include "pwm.h"
void init_timer3_pwm(void);
void init_motor_driver(void);

void motor_run(int speed, int is_reverse);
void motor_stop(void);

void init_timer3_pwm(void)
{
	DDRE |= 1 << 3 | 1 << 5;
	TCCR3A |= 1 << WGM30 | 1 << COM3C1;
	TCCR3B |= 1 << WGM32;
	TCCR3B |= (1 << CS31) | (1 << CS30);
	OCR3C = 0;
}

void init_motor_driver(void)
{
	DDRF |= (1 << 6) | (1 << 7);
	PORTF |= (1 << 6);
	PORTF &= ~(1 << 7);  
}

// 속도(speed: 0~255)와 방향(is_reverse: 0 정회전, 1 역회전)을 제어하는 함수
void motor_run(int speed, int is_reverse)
{
	PORTF &= ~((1 << 6) | (1 << 7)); // 방향 핀 초기화
	
	if(is_reverse) PORTF |= (1 << 7); // 역회전
	else PORTF |= (1 << 6);           // 정회전
	
	OCR3C = speed; // 속도 적용
}

// 모터 정지 함수
void motor_stop(void)
{
	OCR3C = 0;
}

