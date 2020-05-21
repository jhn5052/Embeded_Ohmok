#include "2450addr.h"
#include "option.h"
#include "my_lib.h" //touch screen
#include "disp.h"
#include "images.h"
#include "scape.h"


#if defined ( x1010 )

#define  NonPal_Fb   ((volatile unsigned short(*)[480]) FRAME_BUFFER)

//**ISR Declaration
void Touch_ISR(void) __attribute__ ((interrupt ("IRQ")));
void Timer0_ISR(void) __attribute__ ((interrupt ("IRQ")));
unsigned int x;
unsigned int y;
volatile  int ADC_x, ADC_y;
volatile  int Touch_Pressed;


volatile  int ADC_x, ADC_y;
volatile  int Touch_Pressed=0;
volatile  int Mode = 0;


void Main(){
   Uart_Init(115200);   
   Touch_Init(); //touch Init and check x, y
   Timer_Init();
#if 0   
/*   Lcd.c */   
   Lcd_Port_Init();
   NonPal_Lcd_Init();
#else   
   
/*   disp.c */   
   init_Lcdc();
#endif
   Lcd_SetTranMode( 0 );
   //draw_bms(15,15, white);
   //draw_bms(15,30, black);
   pISR_ADC = (unsigned int)Touch_ISR;
   pISR_TIMER0 = (unsigned int )Timer0_ISR;
   rINTSUBMSK &= ~(0x1<<9);
   rINTMSK1 &= ~(0x1<<31);   
   rINTMSK1 &= ~(1<<10);
   
   draw_bms(0,0, logo);      // background.bms
	while(1)
   {
   		if(Touch_Pressed&&!Mode){
        	x = (double)480 / (850- 194) * (ADC_x - 194); // do calibrate x = 480 / (850- 194) * (ADC_x - 194)
         	y = (double)272 / (660 - 330) * (ADC_y - 330);   // do calibrate y = 272 / (660 - 330) * (ADC_y - 330)
         	if(y>=50&&y<=85&&x>=185&&y<=284){
            	Mode = 1;
            	Battle();
       		}
        
		}
   }
   
}
#endif 
