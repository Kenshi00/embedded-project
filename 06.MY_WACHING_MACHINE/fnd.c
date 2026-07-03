/*
 * fnd.c
 *
 * Created: 2026-06-12 오전 10:52:31
 *  Author: kccistc
 */

#include "fnd.h"


void init_fnd(void);
int init_count(void);

void fnd_display(void);
void fnd_display_second(void);

extern int remain_sec;
extern volatile uint32_t msec_count;
extern int TEST_TIME[3];
uint8_t dot_display = 0;
 
 // 이 check 전역 변수로 스탑워치 제어 (인터럽트역할..?)
int check = -1;



void init_fnd(void)
{
	// DDR 설정
	// PORTC - DATA가 C
	FND_DATA_DDR = 0xff; //출력모드 설정
	// PORTB - DIGIT이 B
	FND_DIGIT_DDR |= 1 << FND_DIGIT_D1 | 1 << FND_DIGIT_D2 | 1 << FND_DIGIT_D3 | 1 << FND_DIGIT_D4;
	
	FND_DATA_PORT = 0xff;
}


void fnd_display(void)
{
	
	static uint32_t prev_ms_count = 0;
	
	if (msec_count - prev_ms_count < 1) return; // 1ms가 안지났으면 함수 종료 (속도 조절)
	prev_ms_count = msec_count;
	
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
		FND_DATA_PORT = fnd_font[remain_sec%10]; // 0~9
		break;
		
		case 1: // 십의자리
#if 1
		FND_DIGIT_PORT = 0x40; // common 애노우드
#else
		FND_DIGIT_PORT = ~0x40; // common 캐서우드
#endif
		FND_DATA_PORT = fnd_font[remain_sec/10%6]; // 0~9
		break;
		
		case 2:
#if 1
		FND_DIGIT_PORT = 0x20; // common 애노우드
#else
		FND_DIGIT_PORT = ~0x20; // common 캐서우드
#endif
		FND_DATA_PORT = fnd_font[remain_sec/60%10];
		if ((msec_count % 1000) < 500)
		{
			FND_DATA_PORT &= fnd_font[10]; // fnd_font[10]인 0x7f를 AND 연산하여 점(DP)의 불을 켬
		}
		break;
		case 3:
#if 1
		FND_DIGIT_PORT = 0x10; // common 애노우드
#else
		FND_DIGIT_PORT = ~0x10; // common 캐서우드
#endif
		FND_DATA_PORT = fnd_font[remain_sec/600%6]; // 0~9
		break;
	}
	digit_select = (digit_select + 1) % 4;
}


void fnd_display_second(void)
{
	static uint32_t prev_ms_count = 0;
	
	if (msec_count - prev_ms_count < 1) return; // 1ms가 안지났으면 함수 종료 (속도 조절)
	prev_ms_count = msec_count;
	#if 1
	// 0    1      2    3      4    5      6     7    8     9      .
	uint8_t fnd_font[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xd8, 0x80, 0x98, 0x7f}; // common 애노우드
	// 순서:           백의자리(a, b, c, d) -> 천의자리(e, f, g, a)
	// 인덱스:        0(a)  1(b)  2(c)  3(d)  4(d)  5(e)  6(f)  7(a)
	uint8_t anode_font[] = {0xfe, 0xfd, 0xfb, 0xf7, 0xf7, 0xef, 0xdf, 0xfe};
	uint8_t fnd_off = 0xff; // FND 완전히 끄기 (Anode는 1이 꺼짐)
	#else
	// 0     1      2      3      4      5      6       7     8      9      .
	uint8_t fnd_font[] = {~0xc0, ~0xf9, ~0xa4, ~0xb0, ~0x99, ~0x92, ~0x82, ~0xd8, ~0x80, ~0x98, ~0x7f}; // common 캐서우드
	#endif
	int cycle = remain_sec % 8;
	static int digit_select = 0; // 자리수 선택

	switch(digit_select)
	{
		case 0: // 일의자리
		#if 1
		FND_DIGIT_PORT = 0x80; // common 애노우드
		#else
		FND_DIGIT_PORT = ~0x80; // common 캐서우드
		#endif
		FND_DATA_PORT = fnd_font[remain_sec % 10]; // 0~9
		break;
		
		case 1: // 십의자리
		#if 1
		FND_DIGIT_PORT = 0x40; // common 애노우드
		#else
		FND_DIGIT_PORT = ~0x40; // common 캐서우드
		#endif
		FND_DATA_PORT = fnd_font[remain_sec/10 % 10]; // 0~9
		break;
		
		case 2:
		#if 1
		FND_DIGIT_PORT = 0x20; // common 애노우드
		#else
		FND_DIGIT_PORT = ~0x20; // common 캐서우드
		#endif
		// 0~3초일 때만 백의자리에 애니메이션 출력, 그 외에는 끔
		if(cycle >= 0 && cycle <= 3) FND_DATA_PORT = anode_font[cycle];
		else FND_DATA_PORT = fnd_off;
		if ((msec_count % 1000) < 500)
		{
			FND_DATA_PORT &= fnd_font[10]; // fnd_font[10]인 0x7f를 AND 연산하여 점(DP)의 불을 켬
		}
		break;
		
		case 3:
		#if 1
		FND_DIGIT_PORT = 0x10; // common 애노우드
		#else
		FND_DIGIT_PORT = ~0x10; // common 캐서우드
		#endif
		// 4~7초일 때만 천의자리에 애니메이션 출력, 그 외에는 끔
		if(cycle >= 4 && cycle <= 7) FND_DATA_PORT = anode_font[cycle];
		else FND_DATA_PORT = fnd_off;
		break;
	}
	digit_select = (digit_select + 1) % 4;
}
