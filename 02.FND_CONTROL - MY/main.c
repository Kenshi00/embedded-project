/*
 * 02.FND_CONTROL.cpp
 *
 * Created: 2026-06-12 오전 10:44:11
 * Author : kccistc
 */

#include <avr/io.h>
#include "button.h"
extern void init_button(void);
extern int get_button(int button_num, int button_pin);

extern int fnd_main(void);
extern int fnd_main_second(void);
extern int stopwatch(void);
extern int stopwatch_stop(void);
extern int init_count(void);

extern int check;
int main(void)
{
	int button0_state = 0;
	int button1_state = 0;
	int button2_state = 0;
	
    while (1)
    {
	    if (get_button(BUTTON0, BUTTON0PIN))
	    {
			//if(button0_state == 2) check = -1;
		    button0_state = (button0_state + 1) % 3;
			init_count();
		}
		if (get_button(BUTTON1, BUTTON1PIN))
		{
			button1_state = !button1_state;
		}
		if (get_button(BUTTON2, BUTTON2PIN))
		{
			button2_state = (button2_state + 1) % 3;
		}
		
	    // 1. 분초 시계
	    if (button0_state == 0) fnd_main();
	    
		// 2. 초 시계
		else if(button0_state == 1) fnd_main_second();
		
	    // 3. 스탑워치 모드
		else if(button0_state == 2)
		{
			stopwatch();
			// stopwatch stop
			if(button1_state == 0 && button2_state == 0)
			{
				check = 2;
			}
			// stopwatch run
			else if(button1_state == 1 && button2_state == 0)
			{
				check = 1;
			}
			// stopwatch reset
			else if(button2_state == 1)
			{
				check = 0;
			}
			else if(button2_state == 2)
			{
				button2_state = 0;
				button1_state = 1;
				check = 1;
			}
			
		}
	}
}

