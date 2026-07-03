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
int fnd_main_second(void);
int stopwatch_reset(void);
int stopwatch(void);
int init_count(void);

void fnd_display(void);
void fnd_display_second(void);
void fnd_display_stopwatch(void);


uint8_t dot_display = 0;
 
 // 이 check 전역 변수로 스탑워치 제어 (인터럽트역할..?)
int check = -1;

int init_count(void)
{
	ms_count = 0;
	sec_count = 0;  
}
// 1. 분초시계

int fnd_main(void)
{
	// 함수가 끝나도 이전 시간을 기억해야 하므로 static으로 선언
	static uint32_t prev_ms_count = 0;
	
	// 현재 타이머 시간과 마지막으로 실행했던 시간의 차이가 1ms 이상일 때만 진입
	if (int_ms_count - prev_ms_count >= 1)
	{
		prev_ms_count = int_ms_count; // 기준 시간을 현재 시간으로 갱신
		
		fnd_display(); // 1ms마다 화면 갱신 (잔상 효과)
		
		// _delay_ms(1); <-- 딜레이는 이제 필요 없으므로 삭제!
		
		ms_count++;
		if(ms_count >= 1000) // 1000ms -> 1sec
		{
			ms_count = 0;
			sec_count++;
			dot_display = !dot_display;
		}
	}
	
	return 0;
}

// 2. 초시계
int fnd_main_second(void)
{
	static uint32_t prev_ms_count = 0;
	if (int_ms_count - prev_ms_count >= 1)
	{
		prev_ms_count = int_ms_count; // 기준 시간을 현재 시간으로 갱신
		
		fnd_display_second();
		
		ms_count++;
		if(ms_count >= 1000) // 1000ms -> 1sec
		{
			ms_count = 0;
			sec_count++;
			dot_display = !dot_display;
		}
	}
	return 0;
}

// 3. 스탑워치
int stopwatch(void)
{
	// 1. 상태와 상관없이 공통으로 실행할 부분 (화면 켜기 + 잔상 유지용 1ms 대기)
	static uint32_t prev_ms_count = 0;
	if (int_ms_count - prev_ms_count >= 1)
	{
		prev_ms_count = int_ms_count; // 기준 시간을 현재 시간으로 갱신
		
		fnd_display_stopwatch();

	// 2. 상태별 시간 카운트 처리
		if (check == -1 || check == 0) // 초시계에서 넘어왔을 때(-1) 또는 리셋(0)
		{
			ms_count = 0;
			sec_count = 0;
		}
		else if (check == 1) // Run 상태
		{
			ms_count++;
			if(ms_count >= 1000) // 1000ms -> 1sec
			{
				ms_count = 0;
				sec_count++;
				dot_display = !dot_display;
			}
		}
		else if (check == 2) // Stop 상태
		{
			dot_display = !dot_display;
		}
	}
	return 0;
}

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
		FND_DATA_PORT = fnd_font[sec_count/60%10];
		if(dot_display) FND_DATA_PORT &= fnd_font[10];
		
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


void fnd_display_second(void)
{
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
	int cycle = sec_count % 8;
	static int digit_select = 0; // 자리수 선택

	switch(digit_select)
	{
		case 0: // 일의자리
		#if 1
		FND_DIGIT_PORT = 0x80; // common 애노우드
		#else
		FND_DIGIT_PORT = ~0x80; // common 캐서우드
		#endif
		FND_DATA_PORT = fnd_font[sec_count % 10]; // 0~9
		break;
		
		case 1: // 십의자리
		#if 1
		FND_DIGIT_PORT = 0x40; // common 애노우드
		#else
		FND_DIGIT_PORT = ~0x40; // common 캐서우드
		#endif
		FND_DATA_PORT = fnd_font[sec_count/10 % 10]; // 0~9
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


void fnd_display_stopwatch(void)
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
		FND_DATA_PORT = fnd_font[ms_count/10 % 10]; // 0~9
		break;
		
		case 1: // 십의자리
		#if 1
		FND_DIGIT_PORT = 0x40; // common 애노우드
		#else
		FND_DIGIT_PORT = ~0x40; // common 캐서우드
		#endif
		FND_DATA_PORT = fnd_font[ms_count/100 % 10]; // 0~9
		break;
		
		case 2:
		#if 1
		FND_DIGIT_PORT = 0x20; // common 애노우드
		#else
		FND_DIGIT_PORT = ~0x20; // common 캐서우드
		#endif
		FND_DATA_PORT = fnd_font[sec_count % 10];
		if(dot_display) FND_DATA_PORT &= fnd_font[10];
		
		break;
		case 3:
		#if 1
		FND_DIGIT_PORT = 0x10; // common 애노우드
		#else
		FND_DIGIT_PORT = ~0x10; // common 캐서우드
		#endif
		FND_DATA_PORT = fnd_font[sec_count/10 % 10]; // 0~9
		break;
	}
	digit_select = (digit_select + 1) % 4;
}

