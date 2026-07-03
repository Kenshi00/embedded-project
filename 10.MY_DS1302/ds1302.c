/*
 * ds1302.c
 *
 * Created: 2026-06-26 오후 2:42:18
 *  Author: kccistc
 */ 
#define F_CPU 16000000UL

#include "ds1302.h"
#include "uart0.h"

//t_ds1302 ds1302_static;

void ds1302_main(void)
{
	// 2. 지역변수로 구조체 선언
	t_ds1302 ds1302;
	
	init_date_time(&ds1302);
	init_gpio_ds1302();
	init_ddr_ds1302(); // all low로 설정
	init_ds1302(&ds1302);
	
	sei();
	
	while(1)
	{
		read_burst_ds1302(&ds1302);
		/*
		//pc_command_processing(&ds1302);
		// 1. read time
		read_time_ds1302(&ds1302);
		// 2. read date
		read_date_ds1302(&ds1302);
		*/
		// 3. printf date & time
		printf("%d-%d-%d ", ds1302.year,ds1302.month,ds1302.date);
		printf("%d-%d-%d\n", ds1302.hours,ds1302.minutes,ds1302.seconds);
		// 4. delay_ms(1000)
		_delay_ms(1000);
	}
}

// ds1302.c에 추가
void read_burst_ds1302(t_ds1302 *p_ds1302)
{
	uint8_t buffer[8];
	
	// 1. CE low -> high
	DS1302_RST_PORT |= 1 << DS1302_RST;
	
	// 2. Burst Read 명령어 전송 (0xBF)
	tx_ds1302(0xBF);
	
	// 3. 8바이트 연속 읽기
	DS1302_DAT_DDR &= ~(1 << DS1302_DAT); // Read Mode로 전환
	for(int i = 0; i < 8; i++)
	{
		uint8_t temp = 0;
		for(int j = 0; j < 8; j++)
		{
			if(DS1302_DAT_PIN & (1 << DS1302_DAT))
			temp |= (1 << j);
			
			clock_ds1302();
		}
		buffer[i] = temp;
	}
	
	// 4. CE HIGH --> LOW
	DS1302_RST_PORT &= ~(1 << DS1302_RST);
	
	// 5. 데이터를 구조체에 매핑 (BCD를 10진수로 변환)
	p_ds1302->seconds   = bcd2dec(buffer[0] & 0x7F);
	p_ds1302->minutes   = bcd2dec(buffer[1]);
	p_ds1302->hours     = bcd2dec(buffer[2]);
	p_ds1302->date      = bcd2dec(buffer[3]);
	p_ds1302->month     = bcd2dec(buffer[4]);
	p_ds1302->dayofweek = bcd2dec(buffer[5]);
	p_ds1302->year      = bcd2dec(buffer[6]);
}

uint8_t read_ds1302(uint8_t addr)
{
	uint8_t data8bits = 0; // 1bit씩 읽어서 담을 변수
	// 1. CE low -> high
	DS1302_RST_PORT |= 1 << DS1302_RST;
	// 2. ADDR 전송
	tx_ds1302(addr + 1); // read addr
	// 3. DATA 읽어들인다.
	rx_ds1302(&data8bits);
	// 4. CE HIGH --> LOW
	DS1302_RST_PORT &= ~(1 << DS1302_RST);
	// 5. return (bcd to decimal)
	return(bcd2dec(data8bits));
}

void rx_ds1302(uint8_t *pdata8bits)
{
	uint8_t temp = 0;
	// 1. input mode로 설정
	DS1302_DAT_DDR &= ~(1 << DS1302_DAT); // read mode
	
	for(int i = 0; i < 8; i++)
	{
		if(DS1302_DAT_PIN & (1 << DS1302_DAT))
		{
			temp |= 1 << i; //1의 조건만 set
		}
		if(i != 7) // 마지막 bit를 읽을 때는 clk을 보내주지 x
			clock_ds1302();
	}
	*pdata8bits = temp;
}

void read_time_ds1302(t_ds1302 *p_ds1302)
{
	p_ds1302->seconds = read_ds1302(ADDR_SECONDS);
	p_ds1302->minutes = read_ds1302(ADDR_MINUTES);
	p_ds1302->hours = read_ds1302(ADDR_HOURS);
}
void read_date_ds1302(t_ds1302 *p_ds1302)
{
	p_ds1302->date = read_ds1302(ADDR_DATE);
	p_ds1302->month = read_ds1302(ADDR_MONTH);
	p_ds1302->dayofweek = read_ds1302(ADDR_DAYOFWEEK);
	p_ds1302->year = read_ds1302(ADDR_YEAR);
}

void init_ds1302(t_ds1302 *p_ds1302)
{
	write_ds1302(ADDR_SECONDS, p_ds1302->seconds);
	write_ds1302(ADDR_MINUTES, p_ds1302->minutes);
	write_ds1302(ADDR_HOURS, p_ds1302->hours);
	write_ds1302(ADDR_DATE, p_ds1302->date);
	write_ds1302(ADDR_MONTH, p_ds1302->month);
	write_ds1302(ADDR_DAYOFWEEK, p_ds1302->dayofweek);
	write_ds1302(ADDR_YEAR, p_ds1302->year);
}

// 우리가 쓰는 일반 숫자(10진수)를 DS1302가 이해하는 BCD로 변환
uint8_t dec2bcd(uint8_t dec)
{
	return ((dec / 10) << 4) | (dec % 10);
}

// DS1302가 보내준 BCD 데이터를 우리가 읽기 편한 10진수로 변환
uint8_t bcd2dec(uint8_t bcd)
{
	return ((bcd >> 4) * 10) + (bcd & 0x0F);
}

void write_ds1302(uint8_t addr, uint8_t data)
{
	// 1. CE low -> high
	DS1302_RST_PORT |= 1 << DS1302_RST;
	// 2. ADDR 전송
	tx_ds1302(addr);
	// 3. DATA 전송
	tx_ds1302(dec2bcd(data));
	// 4. CE HIGH --> LOW
	DS1302_RST_PORT &= ~(1 << DS1302_RST);
}

void tx_ds1302(uint8_t data)
{
	// 1. output mode로 설정
	DS1302_DAT_DDR |= 1 << DS1302_DAT; // write mode
	// 예 0x80
	// M       L
	// 1000 0000
	for(int i = 0; i < 8; i++)
	{
		if(data & (1 << i))
			DS1302_DAT_PORT |= 1 << DS1302_DAT; // 1
		else DS1302_DAT_PORT &= ~(1 << DS1302_DAT); // 0
		
		clock_ds1302();
	}
}

void clock_ds1302(void)
{
	// LOW --> HIGH --> LOW
	DS1302_CLK_PORT &= ~(1 << DS1302_CLK);
	DS1302_CLK_PORT |= 1 << DS1302_CLK;
	DS1302_CLK_PORT &= ~(1 << DS1302_CLK);
}

void init_ddr_ds1302(void)
{
	DDRF &= ~(1 << DS1302_CLK | 1 << DS1302_DAT | 1 << DS1302_RST);
	DDRF |= 1 << DS1302_CLK | 1 << DS1302_DAT | 1 << DS1302_RST; // 출력 mode로 설정
}

void init_gpio_ds1302(void)
{
	PORTF &= ~(1 << DS1302_CLK | 1 << DS1302_DAT | 1 << DS1302_RST);
	_delay_ms(2);
}

void init_date_time(t_ds1302 *p_ds1302)
{
	p_ds1302->year = 26;
	p_ds1302->month = 06;
	p_ds1302->date = 26;
	p_ds1302->dayofweek = 6; // fri
	p_ds1302->hours = 15;
	p_ds1302->minutes = 19;
	p_ds1302->seconds = 00;
}