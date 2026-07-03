/*
 * uart0.c
 *
 * Created: 2026-06-16 오전 9:58:01
 *  Author: kccistc
 */ 
#include "uart0.h"
#include <stdio.h>
#include <avr/io.h>
#include "ds1302.h" // 💡 필수: 구조체 포인터를 매개변수로 쓰기 위해 추가

volatile int rear = 0;
volatile int front = 0;
volatile unsigned char rx_buff[QUEUE_SIZE][QUEUE_LENGTH];

extern int func_state;
extern void (*fp[])();

/*
1. 전송속도 : 9600bps
2. start/stop 설정
3. RX(수신) : interrupt로 설정
*/

void init_uart0(void);
void UART0_transmit(uint8_t data);
void pc_command_processing(t_ds1302 *p_ds1302);

extern t_ds1302 ds1302_static;

// P278 표 12-3
// PC로부터 1byte가 들어오면 자동적으로 이곳으로 진입한다.
// 예) led_all_on\n 이면 11번 이곳으로 진입한다.
ISR(USART0_RX_vect)
{
	volatile uint8_t data;
	volatile static int i = 0;
	data = UDR0; // UDR0의 내용이 data에 복사된 후 UDR0의 내용은 빈 상태가 된다. UDR0이 수신버퍼임
	
	if(data == '\n' || data == '\r')
	{
		if((rear + 1) % QUEUE_SIZE == front % QUEUE_SIZE) return;// queue full 상태
		rx_buff[rear][i] = '\0'; // 문장의 끝인 NULL을 넣는다.
		i = 0; // 다음 string을 저장하기 위해서 i를 0으로 만든다.
		rear = (rear + 1) % QUEUE_SIZE; // 0~9
	}
	else
	{
		if((rear + 1) % QUEUE_SIZE == front % QUEUE_SIZE) return;// queue full 상태
		rx_buff[rear][i++] = data;
	}
}

void init_uart0(void)
{
	//1. 전송속도 : 9600bps
	UBRR0H = 0x00;
	UBRR0L = 207; //9600bps 표 8-9
	UCSR0A |= 1 << U2X0; // 2배속 설정 (sampling 8)
	// UART0을 송신,수신이 다 가능하고 RX INT가 가능하도록 설정한다.
	UCSR0B |= 1 << RXEN0 | 1 << TXEN0 | 1 << RXCIE0;
}

void UART0_transmit(uint8_t data)
{
	// 1. 아직 이전 데이터를 보내는중 (바구니가 꽉참)
	// 하드웨어 상태: 아직 바구니가 안 비었으므로 UDRE0 전구는 0입니다.
	// 
	while (!(UCSR0A & 1 << UDRE0)); // data가 송신중이면 송신이 끝날때까지 기다림
	
	UDR0 = data; // FW 전송 register-------------------------------------
}

void pc_command_processing(t_ds1302 *p_ds1302)
{
	if(front != rear) // data가 rx_buff에 존재 하는지 check
	{
		// printf("%s", rx_buff[front]); // 디버깅용 출력
		
		// [추가된 부분] 시간 설정 명령어 파싱
		int yy, MM, dd, hh, mm, ss;
		
		// sscanf가 포맷에 맞춰 6개의 정수(%d)를 모두 성공적으로 찾았는지 확인 (반환값이 6)
		if (sscanf((char *)rx_buff[front], "%d %d %d %d %d %d", &yy, &MM, &dd, &hh, &mm, &ss) == 6)
		{
			p_ds1302->year = yy;
			p_ds1302->month = MM;
			p_ds1302->date = dd;
			p_ds1302->hours = hh;
			p_ds1302->minutes = mm;
			p_ds1302->seconds = ss;
			p_ds1302->dayofweek = 1; // 요일 데이터는 문자열에 없으므로 기본값(예: 일요일) 지정
			
			init_ds1302(p_ds1302);
			printf("RTC Time Updated: %d-%d-%d %d:%d:%d\n", yy, MM, dd, hh, mm, ss);
		}
		front = (front + 1) % QUEUE_SIZE;
	}
}