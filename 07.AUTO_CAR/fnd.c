/*
 * fnd.c
 *
 * Created: 2026-06-12 오전 10:52:31
 *  Author: kccistc
 */

#include "fnd.h"

void fnd_display(void);
void init_fnd(void);
int fnd_main(void);

volatile uint32_t ms_count = 0;  // ms를 재는 count uint32_t: unsigned int
volatile uint32_t sec_count = 0;  // sec를 재는 count uint32_t: unsigned int
volatile uint8_t dot_display = 0; 

void init_fnd(void)
{
	// DDR 설정
	// PORTC - DATA가 C
	FND_DATA_DDR = 0xff; //출력모드 설정
	// PORTB - DIGIT이 B
	FND_DIGIT_DDR |= 1 << FND_DIGIT_D1 | 1 << FND_DIGIT_D2 | 1 << FND_DIGIT_D3 | 1 << FND_DIGIT_D4;
	
	// PORT 설정
	// FND를 전체 off하는 작업
#if 1 // common 애노우드
	FND_DATA_PORT = 0xff;
#else // common 캐서우드
	FND_DATA_PORT = ~0xff;
#endif
}
void fnd_display(void)
{
	uint8_t fnd_font[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xd8, 0x80, 0x98, 0x7f}; // common 애노우드
	static int digit_select = 0; // 자리수 선택

	// ★ 현재 모터가 구동 중인 하위 4비트(PORTF0~3)의 상태를 읽어와서 저장
	uint8_t current_motor_state = FND_DIGIT_PORT & 0x0F;

	switch(digit_select)
	{
		case 0: // 일의자리
		FND_DIGIT_PORT = current_motor_state | 0x80;
		FND_DATA_PORT = fnd_font[sec_count%10];
		break;
		
		case 1: // 십의자리
		FND_DIGIT_PORT = current_motor_state | 0x40;
		FND_DATA_PORT = fnd_font[(sec_count/10)%6];
		break;
		
		case 2: // 분(일의자리)
		FND_DIGIT_PORT = current_motor_state | 0x20;
		FND_DATA_PORT = fnd_font[(sec_count/60)%10];
		break;
		
		case 3: // 분(십의자리)
		FND_DIGIT_PORT = current_motor_state | 0x10;
		FND_DATA_PORT = fnd_font[(sec_count/600)%6];
		break;
	}
	digit_select = (digit_select + 1) % 4;
}
/*
void fnd_display(void)
{
#if 1
	                     // 0    1      2    3      4    5      6     7    8     9      .
	uint8_t fnd_font[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xd8, 0x80, 0x98, 0x7f}; // common 애노우드
#else
						  // 0     1      2      3      4      5      6       7     8      9      .
	uint8_t fnd_font[] = {~0xc0, ~0xf9, ~0xa4, ~0xb0, ~0x99, ~0x92, ~0x82, ~0xd8, ~0x80, ~0x98, ~0x7f}; // common 캐서우드
#endif

	static int digit_select = 0; // 자리수 선택

	switch(digit_select)
	{
		case 0: // 일의자리
#if 1
		FND_DIGIT_PORT = 0x80; // common 애노우드
#else
		FND_DIGIT_PORT = ~0x80; // common 캐서우드
#endif
		FND_DATA_PORT = fnd_font[sec_count%10]; // 0~9
		break;
		
		case 1: // 십의자리
#if 1
		FND_DIGIT_PORT = 0x40; // common 애노우드
#else
		FND_DIGIT_PORT = ~0x40; // common 캐서우드
#endif
		FND_DATA_PORT = fnd_font[sec_count/10%6]; // 0~9
		break;
		case 2:
#if 1
		FND_DIGIT_PORT = 0x20; // common 애노우드
#else
		FND_DIGIT_PORT = ~0x20; // common 캐서우드
#endif
		FND_DATA_PORT = fnd_font[sec_count/60%10]; // 0~9
		break;
		case 3:
#if 1
		FND_DIGIT_PORT = 0x10; // common 애노우드
#else
		FND_DIGIT_PORT = ~0x10; // common 캐서우드
#endif
		FND_DATA_PORT = fnd_font[sec_count/600%6]; // 0~9
		break;
	}
	digit_select = (digit_select + 1) % 4;
}
*/