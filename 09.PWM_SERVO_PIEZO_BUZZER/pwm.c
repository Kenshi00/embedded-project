/*
 * pwm.c
 *
 * Created: 2026-06-18 오후 2:23:32
 *  Author: kccistc
 */ 
#include "pwm.h"
void init_timer1_pwm(void);
int servo_motor_main(void);

// Timer0는 소프트웨어 주도방식인데 반해,Timer1은 하드웨어 주도 방식(PWM)이기 때문에 
// ISR(timer0_OVF~~) 이렇게 소프트웨어적으로 호출하지 않음. 
// 하드웨어가 스스로 TCNT1 값과 OCR1A/B를 비교하기 때문에 TCNT1 = 6; 이런식으로 내가 선언하지 않아도 됨.
void init_timer1_pwm(void)
{
	
	// 분주비 8 설정
	// 16000000Hz / 8 --> 2MHz
	// T = 1/f 1/2000000Hz ==> 0.5us
	// 타이머1은 16bit 까지 count가능 (65535) => 65535 * 0.5 = 32.767ms
	// 한 펄스당 20ms을 만들어야 하므로 0.0000005sec * 40000개 => 20ms
	TCCR1B &= ~(1 << CS12 | 1 << CS11 | 1 << CS11);
	TCCR1B |= (1 << CS11);
	
	// 3. 타이머1 모드 5 (8비트 고속 PWM 모드)
	TCCR1A |= (1 << WGM11);  // TOP --> ICR1으로 설정
	TCCR1B |= 1 << WGM12 | 1 << WGM13;
	
	// 4. 비반전 모드 설정 (사진 맨 아래 주석 내용)
	TCCR1A |= (1 << COM1A1);
	
	ICR1 = 40000 - 1; // 40000카운트 돌아서 20ms
}

int servo_motor_main(void)
{
	// servo motor가 PBS를 출력으로 설정
	DDRB |= 1 << 5;
	// 한 펄스당 20ms
	init_timer1_pwm();
	
	// 0(1ms) --> 90(1.5ms) --> 180(2ms)
	while(1)
	{
		// 1ms : 2000개 count
		// 40000(20ms) / 20 --> 1ms(2000)
		// 0도
		OCR1A = 2000;
		_delay_ms(1000);
		
		// 1ms : 2000개 count
		// 40000(20ms) / 20 --> 1ms(2000)
		// 0도
		OCR1A = 3000;
		_delay_ms(1000);
		
		// 1ms : 2000개 count
		// 40000(20ms) / 20 --> 1ms(2000)
		// 0도
		OCR1A = 4000;
		_delay_ms(1000);
	}
}


