/*
 * pwm.c
 *
 * Created: 2026-06-18 오후 2:23:32
 *  Author: kccistc
 */ 
#include "pwm.h"
extern int get_button(int button_num, int button_pin);
void init_timer1_pwm(void);
void init_motor_driver(void);

void forward(int speed);
void backward(int speed);
void turn_left(int speed);
void turn_right(int speed);
void stop();

// 16bit 1번 timer/counter를 사용
/*
	PWM 출력 신호
	============
	PB5 : OC1A : 왼쪽바퀴
	PE6 : OC1B : 오른쪽바퀴
	BTN0 : auto/manual mode 설정
	
	방향 설정
	========
	1. 왼쪽 바퀴
	PORTF0 -- IN1 (DC motor Driver)
	PORTF1 -- IN2 
	2. 오른쪽 바퀴
	PORTF2 -- IN3
	PORTF3 -- IN4
	
	IN1/IN3    IN2/IN4
	=======    =======
	   0          1     : 역회전
	   1          0     : 정회전
	   1          1     : stop
*/

#define MOTOR_PWM_DDR DDRB
#define MOTOR_LEFT_PORT_DDR 5 // OC1A
#define MOTOR_RIGHT_PORT_DDR 6 // OC1B

#define MOTOR_DIRECTION_PORT PORTF // OC1A
#define MOTOR_DIRECTION_PORT_DDR DDRF // OC1B


// Timer0는 소프트웨어 주도방식인데 반해,Timer1은 하드웨어 주도 방식(PWM)이기 때문에 
// ISR(timer0_OVF~~) 이렇게 소프트웨어적으로 호출하지 않음. 
// 하드웨어가 스스로 TCNT1 값과 OCR1A/B를 비교하기 때문에 TCNT1 = 6; 이런식으로 내가 선언하지 않아도 됨.
void init_timer1_pwm(void)
{
	
	// 5. 분주비 64 설정 (타이머 속도)
	TCCR1B |= (1 << CS11) | (1 << CS10);
	
	// 3. 타이머1 모드 5 (8비트 고속 PWM 모드)
	TCCR1A |= (1 << WGM11);  // TOP --> ICR1으로 설정
	TCCR1B |= 1 << WGM12 | 1 << WGM13;
	
	// 4. 비반전 모드 설정 (사진 맨 아래 주석 내용)
	TCCR1A |= (1 << COM1A1);
	TCCR1A |= (1 << COM1B1);
	
	ICR1 = 0x3ff; // 1023 * 4us ==> 4ms TOP 값
}

/*
	PE5 : PWM control
	PE6 : IN1 방향 설정
	PE7 : IN2
*/

void init_motor_driver(void)
{
	// 1. 출력모드로 설정
	MOTOR_PWM_DDR &= ~(1 << 5 | 1 << 6);
	MOTOR_PWM_DDR |= 1 << 5 | 1 << 6;
	MOTOR_DIRECTION_PORT_DDR &= ~(1 << 0 | 1 << 1 | 1 << 2 | 1 << 3);
	MOTOR_DIRECTION_PORT_DDR |= 1 << 0 | 1 << 1 | 1 << 2 | 1 << 3;
	// 2. 모터를 전진 모드로
	MOTOR_DIRECTION_PORT &= ~(1 << 0 | 1 << 1 | 1 << 2 | 1 << 3);
	MOTOR_DIRECTION_PORT |= 1 << 2 | 1 << 0; // 모터를 전진모드로 IN4IN3IN2IN1 0101
}

// speed 변수의 유효 범위는 0~1023까지
void forward(int speed)
{
	MOTOR_DIRECTION_PORT &= ~(1 << 0 | 1 << 1 | 1 << 2 | 1 << 3);
	MOTOR_DIRECTION_PORT |= 1 << 2 | 1 << 0;
	OCR1A = OCR1B = speed; // OCR1A PWM left, OCR1B PWM right   IN4IN3IN2IN1 0101
}

void backward(int speed)
{
	MOTOR_DIRECTION_PORT &= ~(1 << 0 | 1 << 1 | 1 << 2 | 1 << 3);
	MOTOR_DIRECTION_PORT |= 1 << 3 | 1 << 1;
	OCR1A = OCR1B = speed; // OCR1A PWM left, OCR1B PWM right   IN4IN3IN2IN1 1010
}

void turn_left(int speed)
{
	MOTOR_DIRECTION_PORT &= ~(1 << 0 | 1 << 1 | 1 << 2 | 1 << 3);
	MOTOR_DIRECTION_PORT |= 1 << 2 | 1 << 0;
	OCR1A = 0; // OCR1A PWM left, OCR1B PWM right   IN4IN3IN2IN1 0101
	OCR1B = speed;
}

void turn_right(int speed)
{
	MOTOR_DIRECTION_PORT &= ~(1 << 0 | 1 << 1 | 1 << 2 | 1 << 3);
	MOTOR_DIRECTION_PORT |= 1 << 2 | 1 << 0;
	OCR1A = speed; // OCR1A PWM left, OCR1B PWM right   IN4IN3IN2IN1 0101
	OCR1B = 0;
}

void stop()
{
	MOTOR_DIRECTION_PORT &= ~(1 << 0 | 1 << 1 | 1 << 2 | 1 << 3);
	MOTOR_DIRECTION_PORT |= 1 << 3 | 1 << 2 | 1 << 1 | 1 << 0;
	OCR1A = 0; // OCR1A PWM left, OCR1B PWM right   
	OCR1B = 0;
}

