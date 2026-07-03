/*
 * pwm.h
 *
 * Created: 2026-06-18 오후 2:23:43
 *  Author: kccistc
 */ 


#include "button.h"
#include <avr/interrupt.h>
#include <stdio.h>

extern void init_timer1_pwm(void);
extern void init_motor_driver(void);
extern void forward(int speed);
extern void backward(int speed);
extern void turn_left(int speed);
extern void turn_right(int speed);
extern void stop();