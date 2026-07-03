/*
 * ultrasonic.h
 *
 * Created: 2026-06-17 오후 1:50:20
 *  Author: kccistc
 */ 

#define F_CPU 16000000UL  // 16MHz
#include <avr/io.h>  // PORTA PORTB PORTD... IO관련 reg가 들어 있다.
#include <util/delay.h>  // _delay_ms _delay_us 등
#include <avr/interrupt.h>

#define TRIG_DDR DDRG
#define TRIG_PORT PORTG
#define TRIG_PIN 4

#define ECHO_DDR DDRE
#define ECHO_PORT PINE
#define ECHO_PIN 4
