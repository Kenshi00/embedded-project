/*
 * uart1.c
 *
 * Created: 2026-06-22 오전 10:38:38
 *  Author: kccistc
 */ 
#include "uart1.h"

// PC와의 통신
extern void UART0_transmit(uint8_t data);
/*
1. 전송속도 : 9600bps
2. start/stop 설정
3. RX(수신) : interrupt로 설정
*/

void init_uart1(void);
void UART1_transmit(uint8_t data);

volatile uint8_t bt_data;
// P278 표 12-3
// BT로부터 1byte가 들어오면 자동적으로 이곳으로 진입한다.


// UART1에는 블루투스에서 들어오는 데이터 (그냥 하드웨어적으로 정해져있음)
ISR(USART1_RX_vect)
{
	bt_data = UDR1; // UDR1의 내용이 data에 복사된 후 UDR1의 내용은 빈 상태가 된다. UDR1이 수신버퍼임
	UART0_transmit(bt_data); // bt로부터 들어온 byte를 확인하기 위해서 comport로 출력한다.
}

void init_uart1(void)
{
	//1. 전송속도 : 9600bps
	UBRR1H = 0x00;
	UBRR1L = 207; //9600bps 표 8-9a
	UCSR1A |= 1 << U2X1; // 2배속 설정 (sampling 8)
	// UART0을 송신,수신이 다 가능하고 RX INT가 가능하도록 설정한다.
	UCSR1B |= 1 << RXEN1 | 1 << TXEN1 | 1 << RXCIE1;
}

void UART1_transmit(uint8_t data)
{
	// 1. 아직 이전 데이터를 보내는중 (바구니가 꽉참)
	// 하드웨어 상태: 아직 바구니가 안 비었으므로 UDRE0 전구는 0입니다.
	//
	while (!(UCSR1A & 1 << UDRE1)); // data가 송신중이면 송신이 끝날때까지 기다림
	
	UDR1 = data; // FW 전송 register-------------------------------------
}
