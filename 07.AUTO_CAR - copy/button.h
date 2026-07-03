/*
 * button.h
 *
 * Created: 2026-06-10 오전 11:45:01
 *  Author: user
 */ 

#define F_CPU 16000000UL  //16MHz
#include <avr/io.h>  // PORTA PORTB PORTD... IO관련 reg가 들어 있다. 
#include <util/delay.h>  // _delay_ms _delay_us 등 

#define  BUTTON_DDR  DDRG
#define  BUTTON_PIN  PING   // PORTD를 읽는 register 5v:1  0v:0 
#define  BUTTON0PIN 2    // PORTD.4
#define  BUTTON0   0   // PORTD.3의 가상 index (sw번호)
#define  BUTTON_NUMBER  1   // 버튼 갯수 
#define  BUTTON_PRESS  1      // 버튼을 누르면 high (active-high)
#define  BUTTON_RELEASE 0     // 버튼을 뗀 상태 (low)

extern void init_button(void);