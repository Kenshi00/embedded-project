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

// TRIG 핀 정의 (PORTA)
#define TRIG_DDR DDRA
#define TRIG_PORT PORTA
#define TRIG_LEFT_PIN   0
#define TRIG_CENTER_PIN 1
#define TRIG_RIGHT_PIN  2

// ECHO 핀 정의 (PORTE)
#define ECHO_DDR DDRE
#define ECHO_PORT PINE
#define ECHO_LEFT_PIN   4 // INT5
#define ECHO_CENTER_PIN 5 // INT6
#define ECHO_RIGHT_PIN  6 // INT7

extern volatile int dist_left;
extern volatile int dist_center;
extern volatile int dist_right;
extern volatile int ultrasonic_distance;

extern void pc_command_processing(void);
extern void init_ultrasonic();
extern void ultrasonic_processing(void);
extern void trigger_sensor(int pin);
extern void make_trigger(void);