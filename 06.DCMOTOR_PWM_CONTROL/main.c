/*
 * 06. DCMOTOR_PWM_CONTROL
 *
 * Created: 2026-06-15 오전 11:37:00
 * Author : kccistc
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h> // sei 등 함수
#include <stdio.h>

extern int led_main(void);
extern void init_uart0(void);
extern void UART0_transmit(uint8_t data);
extern void pc_command_processing(void);
extern void init_ultrasonic();
extern void ultrasonic_processing(void);
extern void init_timer3_pwm(void);
extern void init_motor_driver(void);
extern void init_button(void);
extern void dcmotor_pwm_control_main(void);

FILE OUTPUT = FDEV_SETUP_STREAM(UART0_transmit, NULL, _FDEV_SETUP_WRITE);

volatile uint32_t msec_count = 0; // volatile 최적화 방지
volatile int ultrasonic_check_time = 0;

/*
ISR (Interrupt Service Routine) : 인터럽트 처리 함수 ISR로 시작
TIMER0_OVF_vect : Timer 0 Overflow INT가 발생이되면 이곳을로 진입함
250개의 펄스를 count(lms)하면 이곳으로 자동 진입한다.
ISR은 가능한 짧게 작성한다. => 1ms마다 인터럽트를 호출하는데 그 안에 pritnf함수같은거 넣으면 1ms이상 걸리기 때문..
*/

ISR(TIMER0_OVF_vect)
{
	TCNT0 = 6; // TCNT0 6~256 : 250개 펄스 count함.
	msec_count++; // 1ms count
	ultrasonic_check_time++;
}

int main(void)
{
	init_led();
	init_timer0();
	init_uart0();
	init_button();
	init_motor_driver();
	init_timer3_pwm();
	//init_ultrasonic();
	dcmotor_pwm_control_main();
	stdout = &OUTPUT; // printf가 동작할수 있도록 stdout을 설정
	sei(); // 전역(대문) interrupt 허용
	//led_main();
    /*
	while (1)
    {
		//pc_command_processing();
		ultrasonic_processing();
	}*/
}

void init_timer0(void)
{
	TCNT0 = 6; // TCNT0 6~256 : 250개 펄스 count함.
	TCCR0 &= ~(1 << CS02 | 1 << CS01 | 1 << CS00); // 초기화
	TCCR0 |= 1 << CS02 | 0 << CS01 | 0 << CS00; // 64분주
	TIMSK |= 1 << TOIE0; // TIMER0 Overflow interrupt
}

