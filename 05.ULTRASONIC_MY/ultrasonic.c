/*
 * ultrasonic.c
 *
 * Created: 2026-06-17 오후 1:50:05
 *  Author: kccistc
 */ 

#include "ultrasonic.h"

extern volatile int ultrasonic_check_time; // 사용 x
extern void led_distance_display(int distance);
volatile int ultrasonic_distance = 0;
volatile char scm[50];

volatile int flag = 0; // 측정이 끝났음을 메인에 알리는 깃발 flag

// P278 표 12-3
// 0x000A (interrupt 발생시 이 번지로 옴)
// INT4 : PE4 외부 INT4 초음파 센서 상승, 하강 엣지 발생시 이곳으로 진입(init에서 그렇게되도록 초기화한것)
// 상승엣지에 1번, 하강엣지에 1번씩 들어옴
// MCU의 하드웨어가 ECHO 핀이 low에서 high로 변하는 것(상승 엣지) 감지하고 ISR(INT4_vect) 인터럽트 발생시킴
ISR(INT4_vect)
{
	// 1. 상승 엣지
	if(ECHO_PORT & (1 << ECHO_PIN))
	{
		// 초음파가 출발하는 순간 초시계(Timer1)의 버튼을 눌러 0tick 부터 측정 시작. TCNT1은 초가 아니고 고속으로 증가하는 카운터임.
		TCNT1 = 0;
	}
	else // 2. 하강 엣지, 초음파가 센서로 돌아오면 ECHO핀을 다시 LOW(0V)로 떨어뜨리고 여기로 진입.(초시계 일시정지)
	{
		/*
		F_CPU (16,000,000): MCU의 심장 박동수(클럭)입니다. 1초에 1,600만 번 뜁니다.
		1024 (분주비): 심장이 너무 빨리 뛰어서 타이머가 금방 꽉 차버리는 것을 막기 위해, 심장 박동을 1024번에 한 번씩만 타이머 틱(Tick)으로 치겠다고 설정한 값입니다.
		타이머의 속도 : 16000000 / 1024 = 15.625Hz (1초에 15625번 뜀)
		*/
		/*
		TCNT1 : Timer count 1
		TCNT1이 10tick이 들어있으면, 1024분주한 15,625KHz의 1주기 : 64us (1틱이 걸리는시간 : 1/15625초)
		10 * 64us = 640us, 640us / 58us(1cm이동 하는데 소요시간) : 11cm
		1sec : 1000000us
		*/
		ultrasonic_distance = TCNT1 * 1000000.0 * 1024 / F_CPU; // (1024 / 160000000)이 1주기(0.000064cm = 64us)
		sprintf(scm, "dis: %dcm\n", ultrasonic_distance / 58); // cm 환산 (scm 배열에 dis: %dcm 자체를 저장)
		
		flag = 1;
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


void ultrasonic_processing(void)
{
	// 1초 대기 및 시간체크(초음파를 너무 자주쏘면 이전 초음파와 겹쳐 노이즈가 생김)
	//if(ultrasonic_check_time >= 1000)
	
	// flag로 하게되면 굳이 1000ms를 기다리지 않고 짧은 거리의 경우 빠르게 할 수 있다.
	if(flag)
	{
		flag = 0;
		
		//ultrasonic_check_time = 0;
		// 이전 사이클에 계산해둔 거리결과 전송
		printf("%s", scm);
		
		int current_cm = ultrasonic_distance / 58;
		led_distance_display(current_cm);
		
		// 초음파 발사 지시 (결과 출력 후 새로 거리 재야하므로)
		make_trigger();
	}
}

/*
초음파 발사 지시
MCU가 초음파 센서의 TRIG 핀에 15us 펄스 보냄
PORTG 레지스터의 4번핀을 0으로 만듬 & ~ 기법을 자주 사용

트리거 실행되면 타이머 시작됨.
*/
void make_trigger(void)
{
	TRIG_PORT &= ~(1 << TRIG_PIN); // low로 만든다(혹시라도 남아있을지 모르는 전기적 노이즈 없앰)
	_delay_us(1); // 0V유지
	TRIG_PORT |= (1 << TRIG_PIN); // high로 만든다(5V high)
	_delay_us(15); // 최소 10us이상 high신호를 주어야 센서가 인식함.(1인상태를 15us동안 유지) -> 센서가 인식하면 ECHO핀을 올림
	TRIG_PORT &= ~(1 << TRIG_PIN); // low로 만든다
}
