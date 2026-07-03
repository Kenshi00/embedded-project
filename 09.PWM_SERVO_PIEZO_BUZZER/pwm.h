/*
 * pwm.h
 *
 * Created: 2026-06-18 오후 2:23:43
 *  Author: kccistc
 */ 
#define F_CPU 16000000UL
#include <avr/interrupt.h>
#include <stdio.h>
#include <util/delay.h>
extern void init_timer1_pwm(void);
extern int servo_motor_main(void);