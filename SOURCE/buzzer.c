#include "2450addr.h"
#include "option.h"
#include "my_lib.h"


void Timer_Init_buzzer()
{
	int Tone[8] = {261, 293, 329, 349, 392, 440, 493, 523};	
	Timer1_Init();
	// counting down frequency calculation:
	// 	freq = 66MHz / 256 / 8 -> 32226 Hz
/*
TODO: Find what value should be set in TCNTB, TCMPB for given condition !
*/	
	rTCNTB1 = 100;		// 100 Hz
	rTCMPB1 = 2;		// 50% duty (maximum power delivery) PWM	
	rTCON |=  (1<<9);	// 1	manual update	1 @bit9 of Timer1
	rTCON &= ~(1<<9);	// 0 	manual update^	0 @bit9	

/*
TODO: Decide to use auto reload ON or OFF !
**/

	rTCON  |= (1<<11);	// 1 @ bit11 for Timer1 auto reload ON !
	
	// TCON : Timer1 START !
	
	rTCON |=  (1<<8);	
	//while(1); //should beep here
	int i = 0;
	int freq = 0;
	for(i = 0; i< 21;i++){
		int j = 0;
		int num = i%7;
		switch(num){
			case 1:
				freq = Tone[num];
				break;
			case 2:
				freq = Tone[num];
				break;
			case 3:
				freq = Tone[num];
				break;
			case 4:
				freq = Tone[num];
				break;
			case 5:
				freq = Tone[num];
				break;
			case 6:
				freq = Tone[num];
				break;
			case 0:
				freq = Tone[num];
				break;
		}
		rTCNTB1 = 32226/(2*freq);
		rTCMPB1 = 32226/(2*freq)/2;	
		rTCON |= (0x1<<9);
		rTCON &= ~(0x1<<9);
		rTCON |= (0x1<<8);		
		for(j = 0; j < 0x10000; j++);	
		rTCON &= ~(0x1<<8);
		
	}
//	while(rTCNTO1);
	
	rTCON &= ~(1<<8);	// Timer1 STOP
	rTCON &= ~(1<<11);
	rGPBCON&=(~(3<<0))|(1<<0);
}
