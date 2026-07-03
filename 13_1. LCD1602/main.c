/*
 * 13_1. LCD1602.c
 *
 * Created: 2026-07-01 오전 10:30:12
 * Author : kccistc
 */ 

#define F_CPU 16000000
#include "Text_LCD.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>


#include "I2C_RTC.h"
#include "uart0.h"

FILE OUTPUT = FDEV_SETUP_STREAM(UART0_transmit, NULL, _FDEV_SETUP_WRITE);

int count = 0;

ISR(TIMER0_OVF_vect)
{
	TCNT0 = 6;   // TCNT0 6~256 : 250개 펄스 count하기 위해
	count++; // 반영 안됨
}

uint8_t MODE = 4;

void init_timer0(void)
{
	TCNT0 = 6;   // TCNT0 6~256 : 250개 펄스 count하기 위해
	
	TCCR0 &= ~(1 << CS02 | 1 << CS01 | 1 << CS00);   //
	TCCR0 |= 1 << CS02 | 0 << CS01 | 0 << CS00;   // 64분주
	TIMSK |= 1 << TOIE0;     // TIMER0 Overflow INT
}

int main(void)
{
	// 텍스트 LCD 초기화
	init_lcd();
	
	uint8_t i;
	uint8_t sec, min, hour, day_of_week, day, month, year;
	char lcd_buff[16]; // lcd에 문자를 찍기 전에 임시로 저장할 배열(버퍼)
	_delay_ms(10000);
	I2C_init();
	init_uart0();
	stdout = &OUTPUT;
	
	printf("\r\n\r\n--- System Start ---\r\n");
	uint8_t address = 0x68;
	
	sei();
	init_timer0();
	
	uint8_t data[] = {0, 20, 11, 5, 2, 7, 26};
	printf("* Setting RTC module...\r\n");
	
	// RTC 모듈에 시간 설정
	I2C_start();                           // I2C 시작
	I2C_transmit(address << 1);            // I2C 주소 전송, 쓰기 모드
	// RTC에 데이터를 기록할 메모리 시작 주소 전송
	I2C_transmit(0);
	
	for(i = 0; i < 7; i++){
		printf(" %dth byte written ... \r\n", i);
		I2C_transmit(decimal_to_bcd(data[i])); // 시간 설정
	}
	I2C_stop();                            // I2C 정지
	
	_delay_ms(2000);                       // 2초 대기
	
	printf("* RTC Setting Complete! Starting Real-Time Output...\r\n\n");
	
	while (1)
	{
		
		// 1) RTC에서 데이터를 읽어 올 메모리 시작 주소 전송 (더미 쓰기)
		
		I2C_start();
		I2C_transmit(address << 1);
		I2C_transmit(0);
		I2C_stop();                            // 일단 멈추고
		
		
		// 2) 읽기 모드로 다시 시작해서 7바이트 쭈욱 읽기
		I2C_start();
		I2C_transmit((address << 1) + 1);
		
		sec         = bcd_to_decimal(I2C_receive_ACK());
		min         = bcd_to_decimal(I2C_receive_ACK());
		hour        = bcd_to_decimal(I2C_receive_ACK());
		day_of_week = bcd_to_decimal(I2C_receive_ACK());
		day         = bcd_to_decimal(I2C_receive_ACK());
		month       = bcd_to_decimal(I2C_receive_ACK());
		year        = bcd_to_decimal(I2C_receive_NACK()); // 마지막은 NACK!
		
		I2C_stop();
		
		// 3) PC 시리얼 모니터로 예쁘게 출력
		// %02d를 쓰면 '1초'가 '01초'로 예쁘게 두 자리로 맞춰져서 나옵니다.
		printf("20%02d-%02d-%02d %02d:%02d:%02d\r\n", year, month, day, hour, min, sec);
		
		// 1번째 줄: 날짜 출력 (LCD_clear 제거)
		LCD_goto_XY(0, 2);
		sprintf(lcd_buff, "%02d-%02d-%02d", year, month, day); // %02d를 사용하여 01, 02 형식으로 출력
		LCD_write_string(lcd_buff);
		
		// 2번째 줄: 시간 출력 (LCD_clear 제거)
		LCD_goto_XY(1, 2);
		sprintf(lcd_buff, "%02d:%02d:%02d", hour, min, sec);   // 시간 포맷 수정
		LCD_write_string(lcd_buff);
		
		_delay_ms(1000);
		// 화면 지움
		LCD_clear();
	}
}

