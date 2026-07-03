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
#include "button.h"

extern void init_uart0(void);
extern void UART0_transmit(uint8_t data);
extern void init_timer3_pwm(void);
extern void init_motor_driver(void);
extern void init_button(void);

extern void set_state_led(int state);
extern void motor_run(int speed, int is_reverse);
extern void motor_stop(void);

extern void fnd_display(void);
extern void fnd_display_second(void);
extern void init_fnd(void);


void stop_time_init(void);

// 시간 설정 모드를 제외한 4가지 상태만 정의
typedef enum {
	STANDBY,            // 대기모드
	WASHING,            // 세탁 진행
	RINSING,            // 헹굼 진행
	SPINNING            // 탈수 진행
} FSM_STATE;

FILE OUTPUT = FDEV_SETUP_STREAM(UART0_transmit, NULL, _FDEV_SETUP_WRITE);

volatile uint32_t msec_count = 0; // volatile 최적화 방지

FSM_STATE current_state = STANDBY;

int remain_sec = 0;
int TEST_TIME[3] = {0, 0, 0};

ISR(TIMER0_OVF_vect)
{
	TCNT0 = 6; // TCNT0 6~256 : 250개 펄스 count함.
	msec_count++; // 1ms count
}

int change_mode = -1;

int main(void)
{
	init_led();
	init_timer0();
	init_fnd();
	init_uart0();
	init_button();
	init_motor_driver();
	init_timer3_pwm();
	
	// 0. 시작 시 세탁기 안돌아감.
	int start_button = 0;
	int forward = 0;
	
	uint32_t last_sec_count = 0;
	
	stdout = &OUTPUT; // printf가 동작할수 있도록 stdout을 설정
	sei(); // 전역(대문) interrupt 허용
	
	while(1)
	{
		// 1초씩 감소 타이머 처리 (남은 시간 카운트다운)
		if (msec_count - last_sec_count >= 1000)
		{
			last_sec_count = msec_count;
			if (current_state != STANDBY && remain_sec > 0)
			{
				remain_sec--;
				printf("State: %d, Remain Time: %d sec\n", current_state, remain_sec);
			}
		}
		if (current_state != STANDBY || change_mode != -1)
		{
			if (remain_sec >= 60) {
				fnd_display();        // 60초 이상: 분/초 표시
				} else {
				fnd_display_second(); // 60초 미만: 초 표시
			}
		}
		else
		{
			// STANDBY 상태일 때 FND 끄기 (PORT 조작)
			PORTC = 0xFF; // 예: DATA PORT 끄기 (결선에 따라 다를 수 있음)
		}
		// 상태천이도 (FSM) 로직
		switch (current_state)
		{
			case STANDBY:
				motor_stop();     // 모터 정지
				set_state_led(-1); // LED 모두 끄기 (led.c의 로직 활용)
				// 대기모드 -> 버튼0 누르면 -> 세탁 모드로 이동
				if(get_button(BUTTON1, BUTTON1PIN))
				{
					// 버튼 1 누를때마다 모드 바뀜 
					change_mode = (change_mode + 1) % 3;
					
					remain_sec = TEST_TIME[change_mode];
					// uart 적기
				}
				
				// 버튼 2 누를때마다 10초 추가
				if(change_mode != -1 && get_button(BUTTON2, BUTTON2PIN))
				{
					TEST_TIME[change_mode] += 10;
					
					remain_sec = TEST_TIME[change_mode];
					// uart 적기
				}
				if(get_button(BUTTON0, BUTTON0PIN))
				{
					current_state = WASHING;
					remain_sec = TEST_TIME[0];
					if(TEST_TIME[0] >= 60) fnd_display();
					else fnd_display_second();
					printf("START: WASHING\n");
				}
				break;

			case WASHING:
				set_state_led(0); // 세탁 LED(LED0) 점등
				motor_run(150, 0); // 모터 정회전
			
				// 세탁 중 -> 버튼0 누르면 -> 대기 모드로 강제 종료
				if(get_button(BUTTON0, BUTTON0PIN))
				{
					printf("STOP button pressed.\n");
					current_state = STANDBY;
					
					stop_time_init();
				}
				// 시간 종료 -> 헹굼 모드로 자동 이동
				else if(remain_sec <= 0)
				{
					current_state = RINSING;
					remain_sec = TEST_TIME[1];
					printf("NEXT: RINSING\n");
				}
				break;

			case RINSING:
				set_state_led(1); // 헹굼 LED(LED1) 점등
				motor_run(200, 1); // 모터 역회전
			
				// 헹굼 중 -> 버튼0 누르면 -> 대기 모드로 강제 종료
				if(get_button(BUTTON0, BUTTON0PIN))
				{
					printf("STOP button pressed.\n");
					current_state = STANDBY;
					
					stop_time_init();
				}
				// 시간 종료 -> 탈수 모드로 자동 이동
				else if(remain_sec <= 0)
				{
					current_state = SPINNING;
					remain_sec = TEST_TIME[2];
					printf("NEXT: SPINNING\n");
				}
				break;

			case SPINNING:
				set_state_led(2); // 탈수 LED(LED2) 점등
				motor_run(250, 0); // 모터 최고속도 정회전
			
				// 탈수 중 -> 버튼0 누르면 -> 대기 모드로 강제 종료
				if(get_button(BUTTON0, BUTTON0PIN))
				{
					printf("STOP button pressed.\n");
					current_state = STANDBY;
					
					stop_time_init();
				}
				// 시간 종료 -> 모든 사이클 끝, 대기 모드로 이동
				else if(remain_sec <= 0)
				{
					printf("CYCLE FINISHED.\n");
					current_state = STANDBY;
					
					stop_time_init();
				}
				break;
		}
	}
}

void init_timer0(void)
{
	TCNT0 = 6; // TCNT0 6~256 : 250개 펄스 count함.
	TCCR0 &= ~(1 << CS02 | 1 << CS01 | 1 << CS00); // 초기화
	TCCR0 |= 1 << CS02 | 0 << CS01 | 0 << CS00; // 64분주
	TIMSK |= 1 << TOIE0; // TIMER0 Overflow interrupt
}

void stop_time_init(void)
{
	for(int i = 0; i < 3; i++) TEST_TIME[i] = 0;
	change_mode = -1;
}