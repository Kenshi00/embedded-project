/*
 * ultrasonic.c
 *
 * Created: 2026-06-17 오후 1:50:05
 *  Author: kccistc
 */ 

#include "ultrasonic.h"

extern volatile int ultrasonic_check_time;
extern void led_distance_display(int distance);
volatile int dist_left = 0;
volatile int dist_center = 0;
volatile int dist_right = 0;
volatile int ultrasonic_distance = 0;
volatile char scm[50];
// P278 표 12-3
// 0x000A (interrupt 발생시 이 번지로 옴)
// INT4 : PE4 외부 INT4 초음파 센서 상승,하강 엣지 발생시 이곳으로 진입
// 상승엣지에 1번, 하강엣지에 1번씩 들어옴

// [1] 좌측 센서 (INT4)
ISR(INT4_vect) {
	if(ECHO_PORT & (1 << ECHO_LEFT_PIN)) TCNT3 = 0;
	else dist_left = TCNT3 * 1000000.0 * 1024 / F_CPU / 58;  // <--- 1024 분주비용 정수 계산식
}

// [2] 중앙 센서 (INT5)
ISR(INT5_vect) {
	if(ECHO_PORT & (1 << ECHO_CENTER_PIN)) TCNT3 = 0;
	else dist_center = TCNT3 * 1000000.0 * 1024 / F_CPU / 58;
}

// [3] 우측 센서 (INT6)
ISR(INT6_vect) {
	if(ECHO_PORT & (1 << ECHO_RIGHT_PIN)) TCNT3 = 0;
	else dist_right = TCNT3 * 1000000.0 * 1024 / F_CPU / 58;
}
void init_ultrasonic();
void make_trigger();
void ultrasonic_processing(void);

// TRIG는 타이머로, ECHO는 인터럽트로 구현

void init_ultrasonic() {
	// TRIG는 출력, ECHO는 입력
	TRIG_DDR |= (1 << TRIG_LEFT_PIN) | (1 << TRIG_CENTER_PIN) | (1 << TRIG_RIGHT_PIN);
	ECHO_DDR &= ~((1 << ECHO_LEFT_PIN) | (1 << ECHO_CENTER_PIN) | (1 << ECHO_RIGHT_PIN));
	
	// INT5, 6, 7 '모든 논리적 변화(Any Edge)'에 인터럽트 발생 설정 (EICRB 레지스터)
	// ISC51:50 = 01, ISC61:60 = 01, ISC71:70 = 01
	EICRB |= (1 << ISC40) | (1 << ISC50) | (1 << ISC60);
	EICRB &= ~((1 << ISC41) | (1 << ISC51) | (1 << ISC61));
	
	// INT5, 6, 7 인터럽트 허용
	EIMSK |= (1 << INT4) | (1 << INT5) | (1 << INT6);
	
	// 타이머3 초기화 (1024 분주비)
	TCCR3B |= (1 << CS32) | (1 << CS30);
	TCCR3B &= ~(1 << CS31);
}

// 특정 핀에만 Trigger 펄스를 쏘는 함수
void trigger_sensor(int pin) {
	TRIG_PORT &= ~(1 << pin);
	_delay_us(1);
	TRIG_PORT |= (1 << pin);
	_delay_us(15);
	TRIG_PORT &= ~(1 << pin);
}

/*
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
*/