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

void init_uart0(void);
void UART0_transmit(uint8_t data);
void pc_command_processing(void);

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
		if((rear + 1) % QUEUE_SIZE == front % QUEUE_SIZE) return;// queue full 상태 (PC로 데이터가 너무 한번에 다들어오면 그냥 버린다)
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

void pc_command_processing(void)
{
	if(front != rear) // data가 rx_buff에 존재 하는지 check
	{
		printf("%s", rx_buff[front]);
		if(strncmp((char *) rx_buff[front], "led_shift_left_on", strlen("led_shift_left_on")) == 0)
		{
			func_state = 0;
		}
		
		else if(strncmp((char *) rx_buff[front], "led_shift_right_on", strlen("led_shift_right_on")) == 0)
		{
			func_state = 1;
		}
		
		else if(strncmp((char *) rx_buff[front], "led_shift_left_keepon", strlen("led_shift_left_keepon")) == 0)
		{
			func_state = 2;
		}
		
		else if(strncmp((char *) rx_buff[front], "led_shift_right_keepon", strlen("led_shift_right_keepon")) == 0)
		{
			func_state = 3;
		}
		
		else if(strncmp((char *) rx_buff[front], "led_flower_on", strlen("led_flower_on")) == 0)
		{
			func_state = 4;
		}
		
		else if(strncmp((char *) rx_buff[front], "led_flower_off", strlen("led_flower_off")) == 0)
		{
			func_state = 5;
		}
		
		front = (front + 1) % QUEUE_SIZE;
	}
	
	fp[func_state]();
}