/*
 * ultrasonic.c
 *
 * Created: 2026-06-17 오후 1:50:05
 *  Author: kccistc
 */ 

#include "ultrasonic.h"

extern volatile int ultrasonic_check_time;
extern void led_distance_display(int distance);

volatile int ultrasonic_distance = 0;
volatile char scm[50];
// P278 표 12-3
// 0x000A (interrupt 발생시 이 번지로 옴)
// INT4 : PE4 외부 INT4 초음파 센서 상승,하강 엣지 발생시 이곳으로 진입
// 상승엣지에 1번, 하강엣지에 1번씩 들어옴
ISR(INT4_vect)
{
	// 1. 상승 엣지
	if(ECHO_PORT & (1 << ECHO_PIN))
	{
		TCNT1 = 0;
	}
	else // 2. 하강 엣지
	{
		/*
		TCNT1 : Timer count 1
		TCNT1이 10이 들어있으면, 1024분주한 15.625KHz의 1주기 : 64us
		10 * 64us = 640us, 640us / 58(1cm이동 하는데 소요시간) : 11cm
		1sec : 1000000us
		*/
		
		ultrasonic_distance = TCNT1 * 1000000.0 * 1024 / F_CPU;
		sprintf(scm, "dis: %dcm\n", ultrasonic_distance / 58); //cm로 환산
	}
}
void init_ultrasonic();
void make_trigger();
void ultrasonic_processing(void);

// TRIG는 타이머로, ECHO는 인터럽트로 구현

void init_ultrasonic()
{
	TRIG_DDR |= 1 << TRIG_PIN; // output mode로 설정
	ECHO_DDR &= ~(1 << ECHO_PIN); // input mode로 설정
	
	// 289p 표 12-6 그림 288 참조
	// 0 1 : 상승엣지 하강엣지 둘다 INT를 띄우도록 요청한다.
	EICRB |= 0 << ISC41 | 1 << ISC40;
	/*
	16bit timer/counter 1번을 사용하기로 하자. timer interrupt를 사용x
	16bit timer1 16bit로 표시할 수 있는 최대값 65535(max) : 0xffff
	16MHz / 1024 분주 : 15625Hz ---> 15.625KHz
	1주기(1개의 필수 소요시간) : T = 1/f 1/15625 --> 0.000064sec ==> 64us
	분주비 1024설정 p318 표 14-1
	*/
	TCCR1B |= 1 << CS12 | 1 << CS10; // 1024분주
	
	// ---- EINT4 설정 ----------
	EIMSK |= 1 << INT4; // 외부 INT4(ECHO핀) 활성
}

void make_trigger(void)
{
	TRIG_PORT &= ~(1 << TRIG_PIN); // low로 만든다
	_delay_us(1);
	TRIG_PORT |= 1 << TRIG_PIN; // high로 만든다
	_delay_us(15); // 규격에는 10us인데 reduandancy 때문에 15us로
	TRIG_PORT &= ~(1 << TRIG_PIN); // low로 만든다
}

void ultrasonic_processing(void)
{
	if(ultrasonic_check_time >= 1000) // 1초
	{
		ultrasonic_check_time = 0;
		printf("%s", scm);
		
		make_trigger();
	}
	
}