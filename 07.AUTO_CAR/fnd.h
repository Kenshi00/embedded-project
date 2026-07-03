/*
 * fnd.h
 *
 * Created: 2026-06-12 오전 10:46:08
 *  Author: kccistc
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

#define FND_DATA_DDR DDRC
#define FND_DATA_PORT PORTC

#define FND_DIGIT_DDR DDRF
#define FND_DIGIT_PORT PORTF
#define FND_DIGIT_D1 4
#define FND_DIGIT_D2 5
#define FND_DIGIT_D3 6
#define FND_DIGIT_D4 7


volatile extern uint32_t ms_count;  // ms를 재는 count uint32_t: unsigned int
volatile extern uint32_t sec_count;  // sec를 재는 count uint32_t: unsigned int
volatile extern uint8_t dot_display;
