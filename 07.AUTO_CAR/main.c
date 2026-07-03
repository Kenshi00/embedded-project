/*
 * 06. DCMOTOR_PWM_CONTROL
 *
 * Created: 2026-06-15 오전 11:37:00
 * Author : kccistc
 */

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>

#include "def.h"
#include "button.h"
#include "ultrasonic.h"
#include "uart1.h"
#include "uart0.h"
#include "pwm.h"
#include "fnd.h"

void manual_mode(void);
void auto_mode(void);
void auto_mode_check(void);
void distance_check(void);
void init_timer0(void);

FILE OUTPUT = FDEV_SETUP_STREAM(UART0_transmit, NULL, _FDEV_SETUP_WRITE);

volatile int ultrasonic_check_time = 0;

int func_state = MANUAL_MODE;
extern uint8_t bt_data;
int current_sensor_turn = 0;

/*
ISR (Interrupt Service Routine)
*/
/*
ISR(TIMER0_OVF_vect)
{
	TCNT0 = 6;
	ultrasonic_check_time++;
	
	ms_count++;
	if(ms_count >= 1000) // 1000ms -> 1sec
	{
		ms_count = 0;
		sec_count++;
		dot_display = !dot_display;
	}
}
*/
/*
ISR(TIMER0_OVF_vect)
{
	TCNT0 = 6;
	ultrasonic_check_time++;
	fnd_display();
	// ★ 자율주행 모드일 때만 시간이 카운트되도록 조건 추가
	if (func_state == AUTO_MODE)
	{
		ms_count++;
		if(ms_count >= 1000)
		{
			ms_count = 0;
			sec_count++;
			dot_display = !dot_display;
		}
	}
}
*/
ISR(TIMER0_OVF_vect)
{
	TCNT0 = 6;
	ultrasonic_check_time++;
	
	fnd_display();
	
	// ★ 수정: 수동 모드가 아닐 때(즉, 자율주행 관련 모드일 때) 카운트 증가
	if (func_state != MANUAL_MODE)
	{
		ms_count++;
		if(ms_count >= 1000)
		{
			ms_count = 0;
			sec_count++;
			dot_display = !dot_display;
		}
	}
}
// 16000000 / 64 = 250000 , 즉 1초에 250000번 카운트 발생 -> 1ms당 250카운트 발생
// 0번 타이머는 8bit타이머 이므로 TCNT0 = 6로 설정해서 TCNT0이 250번 뛸때마다 1ms측정
void init_timer0(void)
{
	TCNT0 = 6;
	TCCR0 &= ~(1 << CS02 | 1 << CS01 | 1 << CS00);
	// 64분주
	TCCR0 |= 1 << CS02 | 0 << CS01 | 0 << CS00;
	TIMSK |= 1 << TOIE0;
}

void (*pfunc[]) () = 
{
	manual_mode,
	auto_mode,
	auto_mode_check,
	distance_check
};

// 1. 수동 모드 (블루투스 제어)
void manual_mode(void)
{
	switch(bt_data){
		case 'F': case 'f': forward(400); break;
		case 'B': case 'b': backward(400); break;
		case 'L': case 'l': turn_left(500); break;
		case 'R': case 'r': turn_right(500); break;
		case 'S': case 's': stop(); break;
		default: break;
	}
}

// 2. 자율주행 진입
void auto_mode(void)
{
	func_state = DISTANCE_CHECK; // 바로 거리 감지 및 주행 로직으로 넘김
}


void auto_mode_check(void)
{

}
void distance_check(void)
{
	// 초음파 센서 여러개를 동시에 키면 신호간섭 발생, 30ms마다 하나씩 번갈아 가며 센서 발사
	if(ultrasonic_check_time >= 10)
	{
		ultrasonic_check_time = 0;
		
		// 1. 트리거 발사
		if(current_sensor_turn == 0) trigger_sensor(TRIG_LEFT_PIN);
		else if(current_sensor_turn == 1) trigger_sensor(TRIG_CENTER_PIN);
		else if(current_sensor_turn == 2) trigger_sensor(TRIG_RIGHT_PIN);
		
		// 2. 3번의 스캔이 모두 끝난 시점에만 판단 로직 실행
		if (current_sensor_turn == 2)
		{
			if (0 < dist_left && dist_left <= 20) { turn_right(400);}
			else if (0 < dist_center && dist_center <= 20) { stop();}
			else if (0 < dist_right && dist_right <= 20) { turn_left(400);}
			else { forward(400); }
		}
		
		current_sensor_turn = (current_sensor_turn + 1) % 3;
	}
}

int main(void)
{
	init_timer0();
	init_fnd();
	init_uart0();
	init_uart1();
	init_button();
	init_motor_driver();
	init_timer1_pwm();
	init_ultrasonic();
	init_auto_led();
	
	stdout = &OUTPUT;
	
	sei();
	
	while (1)
    {
		// while(1) 내부의 버튼 처리 부분
		if (get_button(BUTTON0, BUTTON0PIN))
		{
			// 수동 주행 -> 자율 주행
			if(func_state == MANUAL_MODE)
			{
				func_state = AUTO_MODE;
				PORTG |= (1 << 3);
				
				// ★ 버튼을 누른 시점부터 0초로 시작하도록 변수 초기화
				ms_count = 0;
				sec_count = 0;
			}
			// 자율 주행 -> 수동 주행
			else
			{
				func_state = MANUAL_MODE;
				PORTG &= ~(1 << 3);
				stop();
				bt_data = 'S';
			}
		}
		/*
		if (get_button(BUTTON0, BUTTON0PIN))
		{
			// 수동 주행 -> 자율 주행
			if(func_state == MANUAL_MODE)
			{
				func_state = AUTO_MODE;
				PORTG |= (1 << 3);   
			}
			// 자율 주행 -> 수동 주행
			else
			{
				func_state = MANUAL_MODE;
				PORTG &= ~(1 << 3);       
				stop();                   
				bt_data = 'S';            
			}
		}
		*/
		
		//fnd_display();
		//_delay_ms(1);
		
		pfunc[func_state]();
	}
}



void init_auto_led()
{
	DDRG |= (1 << 3);
	PORTG &= ~(1 << 3);
}