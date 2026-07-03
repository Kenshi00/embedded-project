/*
 * 09.PWM_SERVO_PIEZO_BUZZER.c
 *
 * Created: 2026-06-25 오전 9:46:21
 * Author : kccistc
 */ 

#if 1
/*
 * main.c
 *
 * Created: 2021-04-05 오전 10:46:57
 * Author : kcci
 */ 

#define F_CPU	16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "button.h"

extern void Music_Player(int *tone, int *Beats);
extern void init_speaker(void);
extern void Beep(int  repeat);
extern void Siren(int repeat);
extern void RRR(void);

extern const int Elise_Tune[];
extern const int Elise_Beats[];

// 새롭게 추가한 Pretender 배열 외부 참조
extern int Pretender_Tune[];
extern const int Pretender_Beats[];

extern int Lemon_Tune[];
extern const int Lemon_Beats[];

extern int Zenzenzense_Tune[];
extern const int Zenzenzense_Beats[];

extern int power_on_melody_Tune[];
extern const int power_on_melody_Beats[];
extern int open_buzzer_Tune[];
extern const int open_buzzer_Beats[];
extern int drum_waching_stop_melody_Tune[];
extern const int drum_waching_stop_melody_Beats[];

// PE3 (OC3A) PWM 출력 사용.
// 16bit Timer/Counter
// OCR3A값이 같아지면 Low 출력.

int main(void)
{
	init_speaker();	
	init_button();
	
	while(1)
	{
		// btn0을 누르면 1번 동작
		if(get_button(BUTTON0, BUTTON0PIN) == BUTTON_PRESS) {
			Music_Player(power_on_melody_Tune, (int*)power_on_melody_Beats);
			_delay_ms(70);
		}
		
		// btn1을 누르면 1번 동작
		if(get_button(BUTTON1, BUTTON1PIN) == BUTTON_PRESS) {
			Music_Player(open_buzzer_Tune, (int*)open_buzzer_Beats);
			_delay_ms(70);
		}
		
		// btn3을 누르면 3번 동작
		if(get_button(BUTTON2, BUTTON2PIN) == BUTTON_PRESS) {
			Music_Player(drum_waching_stop_melody_Tune, (int*)drum_waching_stop_melody_Beats);
			_delay_ms(70);
		}
/*
		OCR3A=1702;   // 삐~뽀 레 
		_delay_ms(1000);
		OCR3A=1431;   // 파  
		_delay_ms(1000);

*/
		
		//Beep(5);
		// RRR();
// 		_delay_ms(3000);
//		Siren(5);
// 		_delay_ms(3000);
//		Music_Player(Elise_Tune, Elise_Beats);		
	}
}

#endif

#if 0
// servo motor

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <util/delay.h>
#include "pwm.h"

int main(void)
{
	servo_motor_main();
    while (1) 
    {

    }
}

#endif
