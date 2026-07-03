/*
 * ds1302.c
 *
 * Created: 2026-06-26 오후 2:42:18
 *  Author: kccistc
 */ 
#define F_CPU 16000000UL

#include "ds1302.h"
void ds1302_main(void);
void init_ds1302(void);
uint8_t dec2bcd(uint8_t dec);
uint8_t bcd2dec(uint8_t bcd);
void write_ds1302(uint8_t addr, uint8_t data);
void tx_ds1302(uint8_t data);
void clock_ds1302(void);
void init_ddr_ds1302(void);
void init_gpio_ds1302(void);
void init_date_time(void);
uint8_t read_ds1302(uint8_t addr);
void rx_ds1302(uint8_t *pdata8bits);
void read_date_ds1302(void);
void read_time_ds1302(void);

void ds1302_main(void)
{
	init_date_time();
	init_gpio_ds1302();
	init_ddr_ds1302(); // all low로 설정
	init_ds1302();
	
	while(1)
	{
		
		// 1. read time
		read_time_ds1302();
		// 2. read date
		read_time_ds1302();
		// 3. printf date & time
		printf("%d-%d-%d ", ds1302.year,ds1302.month,ds1302.date);
		printf("%d-%d-%d\n", ds1302.hours,ds1302.minutes,ds1302.seconds);
		// 4. delay_ms(1000)
		_delay_ms(1000);
	}
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

void read_time_ds1302(void)
{
	ds1302.seconds = read_ds1302(ADDR_SECONDS);
	ds1302.minutes = read_ds1302(ADDR_MINUTES);
	ds1302.hours = read_ds1302(ADDR_HOURS);
	/*
	ds1302.date = read_ds1302(ADDR_DATE);
	ds1302.month = read_ds1302(ADDR_MONTH);
	ds1302.dayofweek = read_ds1302(ADDR_DAYOFWEEK);
	*/
}
void read_date_ds1302(void)
{
	ds1302.date = read_ds1302(ADDR_DATE);
	ds1302.month = read_ds1302(ADDR_MONTH);
	ds1302.dayofweek = read_ds1302(ADDR_DAYOFWEEK);
	ds1302.year = read_ds1302(ADDR_YEAR);
	/*
	ds1302.date = read_ds1302(ADDR_DATE);
	ds1302.month = read_ds1302(ADDR_MONTH);
	ds1302.dayofweek = read_ds1302(ADDR_DAYOFWEEK);
	*/
}

void init_ds1302(void)
{
	write_ds1302(ADDR_SECONDS, ds1302.seconds);
	write_ds1302(ADDR_MINUTES, ds1302.minutes);
	write_ds1302(ADDR_HOURS, ds1302.hours);
	write_ds1302(ADDR_DATE, ds1302.date);
	write_ds1302(ADDR_MONTH, ds1302.month);
	write_ds1302(ADDR_DAYOFWEEK, ds1302.dayofweek);
	write_ds1302(ADDR_YEAR, ds1302.year);
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

void init_date_time(void)
{
	ds1302.year = 26;
	ds1302.month = 06;
	ds1302.date = 26;
	ds1302.dayofweek = 6; // fri
	ds1302.hours = 15;
	ds1302.minutes = 19;
	ds1302.seconds = 00;
}

