/*
 * uart0.c
 *
 * Created: 2026-06-16 오전 9:58:01
 *  Author: kccistc
 */ 
#include "uart0.h"

extern int func_state;
extern void (*fp[])();

/*
1. 전송속도 : 9600bps
2. start/stop 설정
3. RX(수신) : interrupt로 설정
*/

// UART0에는 PC에서 들어오는 데이터 (그냥 하드웨어적으로 정해져있음)
void init_uart0(void);
void UART0_transmit(uint8_t data);
//void pc_command_processing(void);

// P278 표 12-3
// PC로부터 1byte가 들어오면 자동적으로 이곳으로 진입한다.
// 예) led_all_on\n 이면 11번 이곳으로 진입한다.
ISR(USART0_RX_vect)
{
	volatile uint8_t data;
	volatile static int i = 0;
	data = UDR0; // UDR0의 내용이 data에 복사된 후 UDR0의 내용은 빈 상태가 된다. UDR0이 수신버퍼임
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
