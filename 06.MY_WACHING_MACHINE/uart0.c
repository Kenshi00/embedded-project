/*
 * uart0.c
 *
 * Created: 2026-06-16 오전 9:58:01
 *  Author: kccistc
 */ 
#include "uart0.h"

void init_uart0(void);
void UART0_transmit(uint8_t data);

void init_uart0(void)
{
	//1. 전송속도 : 9600bps
	UBRR0H = 0x00;
	UBRR0L = 207; //9600bps 표 8-9
	UCSR0A |= 1 << U2X0; // 2배속 설정 (sampling 8)
	// UART0을 송신,수신이 다 가능하고 RX INT가 가능하도록 설정한다.
	// UCSR0B |= 1 << RXEN0 | 1 << TXEN0 | 1 << RXCIE0;
	
	// UART0 송신만 가능하도록 설정
	UCSR0B |=  1 << TXEN0;
}

void UART0_transmit(uint8_t data)
{
	// 1. 아직 이전 데이터를 보내는중 (바구니가 꽉참)
	// 하드웨어 상태: 아직 바구니가 안 비었으므로 UDRE0 전구는 0입니다.
	// 
	while (!(UCSR0A & 1 << UDRE0)); // data가 송신중이면 송신이 끝날때까지 기다림
	
	UDR0 = data; // FW 전송 register-------------------------------------
}