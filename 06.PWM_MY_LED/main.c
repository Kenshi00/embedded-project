/*
 * 06.PWM_MY_LED.c
 *
 * Created: 2026-06-18 오전 10:25:43
 * Author : kccistc
 */ 

#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>

#define LED_TIME 20
#define PB4 4

// 소프트웨어 방식(CPU가 계속 잡혀있기 때문에 좋지않음)
void turn_on_LED_in_PWM_manner(int dim)
{
	int i;
	
	PORTA = 0xff;
	
	for(i = 0; i < 256; i++)
	{
		if(i > dim) PORTA = 0x00;
		_delay_us(LED_TIME);
	}
}

// 하드웨어 타이머 방식(CPU가 자유로워짐, 펄스(주파수)가 정확하고 안정적임, 정밀하고 복잡한 제어 가능)
int main(void)
{
	DDRA = 0xff; // PA0~7 핀을 출력으로 설정
	
	int dim = 0;  // 현재 LED 밝기
	int direction = 1; //밝기 증가(1) 또는 감소(-1)
	
	// 파형 출력 핀(PWM 신호가 나갈 핀)인 PB4 핀(OC0핀)을 출력으로 설정
	DDRB |= (1 << PB4);
	
	// 타이머 / 카운터 0번을 고속 PWM 모드로 설정 -> 0 - 255 한방향으로 달림(그래서 빠름)
	//TCCR0 |= (1 << WGM01) | (1 << WGM00);
	
	// 타이머 / 카운터 0번을 위상 PWM 모드로 설정 -> 0 - 255 - 0 왕복 운동을함(그래서 느림)
	TCCR0 |= (0 << WGM01) | (1 << WGM00);
	
	// 비반전 모드(counter가 OCR0보다 작을때 high 클때 low)
	TCCR0 |= (1 << COM01);
	// 분주비를 1024로 설정 (타이머 속도 조절)
	TCCR0 |= (1 << CS02) | (1 << CS01) | (1 << CS00);
	
    while (1)
	{
		//turn_on_LED_in_PWM_manner(dim);
		OCR0 = dim; // 듀티 사이클 설정
		_delay_ms(10);
		
		dim += direction;
		
		if(dim == 0) direction = 1;
		if(dim == 255) direction = -1;
	}
	
	return 0;
}

