/*
 * keypad.h
 *
 * Created: 2026-06-29 오후 2:11:20
 *  Author: kccistc
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h> // sei 등 함수
#include <stdio.h>

#define KEYPAD_DDR DDRA
#define KEYPAD_PIN PINA
#define KEYPAD_PORT PORTA


extern void init_keypad(void);
extern uint8_t keypad_scan(void);
extern uint8_t get_button_keypad(int row, int col);

