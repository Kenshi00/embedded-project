/*
 * ds1302.h
 *
 * Created: 2026-06-26 오후 2:42:09
 *  Author: kccistc
 */ 
#ifndef _DS1302_H_
#define _DS1302_H_

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include <avr/interrupt.h>
#define DS1302_CLK_PORT PORTF
#define DS1302_CLK_DDR  DDRF

#define DS1302_DAT_PORT PORTF
#define DS1302_DAT_DDR  DDRF
#define DS1302_DAT_PIN  PINF

#define DS1302_RST_DDR  DDRF
#define DS1302_RST_PORT  PORTF

#define DS1302_CLK 0
#define DS1302_DAT 1
#define DS1302_RST 2


#define SCLK_PIN 0  // PD0: 클럭
#define IO_PIN   1  // PD1: 데이터 입출력
#define CE_PIN   2  // PD2: 칩 활성화 (RST = reset)

#define ADDR_SECONDS 0x80 // write만 define 하자 read는 write에 +1만 하면 되니까
#define ADDR_MINUTES 0x82 
#define ADDR_HOURS 0x84 
#define ADDR_DATE 0x86
#define ADDR_MONTH 0x88
#define ADDR_DAYOFWEEK 0x8a
#define ADDR_YEAR 0x8c
#define ADDR_WRITEPROTECTED 0x8e

typedef struct _ds1302
{
	uint8_t seconds;
	uint8_t minutes;
	uint8_t hours;
	uint8_t date;
	uint8_t month;
	uint8_t dayofweek; // 1:sun  2:mon
	uint8_t year;
	uint8_t ampm; // 1: pm 0:am
	uint8_t hourmode; // 0:24 1:12
	
} t_ds1302;

extern void ds1302_main(void);
extern uint8_t dec2bcd(uint8_t dec);
extern uint8_t bcd2dec(uint8_t bcd);
extern void write_ds1302(uint8_t addr, uint8_t data);
extern void tx_ds1302(uint8_t data);
extern void clock_ds1302(void);
extern void init_ddr_ds1302(void);
extern void init_gpio_ds1302(void);
extern uint8_t read_ds1302(uint8_t addr);
extern void rx_ds1302(uint8_t *pdata8bits);

extern void read_date_ds1302(t_ds1302 *p_ds1302);
extern void read_time_ds1302(t_ds1302 *p_ds1302);
extern void init_date_time(t_ds1302 *p_ds1302);
extern void init_ds1302(t_ds1302 *p_ds1302);
extern void pc_command_processing(t_ds1302 *p_ds1302);
#endif