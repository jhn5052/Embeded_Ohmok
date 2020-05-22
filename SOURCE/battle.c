#include "2450addr.h"
#include "my_lib.h"
#include "images.h"

int i,j,k=0;
int color=0;
extern unsigned int x;
extern unsigned int y;
extern void Touch_ISR(void) __attribute__ ((interrupt ("IRQ")));
extern void Timer0_ISR(void) __attribute__ ((interrupt ("IRQ")));
int y_range[12] = {248,229,208,187,169,148,126,107,85,60,37,20};
int x_range[18] = {447,424,402,384,364,343,320,300,278,259,238,216,195,175,153,130,110,87};
int y_loc[12] = {17,36,57,78,99,120,141,162,183,204,226,246,267};
int x_loc[18] = {15,37,60,82,104,126,148,170,191,213,234,256,277,299,320,341,362,383};
int cur[12][18] = { -1 };
int i,j,k,color;
int win,flag;
int x_idx,y_idx;
int Sec; 
int tick_cnt = 0,cur_sec;
//검은돌, 흰돌 개수
int black_cnt = 0;
int white_cnt = 0;

int Update = 0;
extern volatile  int ADC_x, ADC_y;
extern volatile  int Touch_Pressed;
extern int Mode;


extern volatile  int ADC_x, ADC_y;
extern volatile  int Touch_Pressed;
extern int Mode;

void Timer0_ISR(void)
{
   /* Masking itself */ 

   rINTMSK1 |= (1<<10);   // block interrupt no.10
   
   /* TODO : Pending Register Clear */
   rSRCPND1 |= (0x1<<10);   //  w1clear on Timer0(=10)
   rINTPND1 |= (0x1<<10);   //  w1clear on Timer0(=10)

   tick_cnt--;
   if(tick_cnt%100==0){
      Sec = tick_cnt/100;
      Update = 1;
      }
   if(tick_cnt==0){
      if(flag==0)
         flag = 1;
      else
         flag = 0;
      tick_cnt = 600;
   }
   /* UnMasking */
   rINTMSK1 &= ~(1<<10);   // re-allow interrupt no.10 to come
}

void Touch_ISR()
{

/* TO DO: block interrupt. esp, stylus touch down ! */
   rINTSUBMSK |= (0x1<<9);
   rINTMSK1 |= (0x1<<31);   

   rSUBSRCPND |= (0x1<<9);
   rSRCPND1 |= (0x1<<31);
   rINTPND1 |= (0x1<<31);
   
   if(rADCTSC & 0x100)
   {
      rADCTSC &= (0xff); 
      Touch_Pressed = 0;

/*   TODO. slow UART printout in ISR ?   */
      Uart_Send_String("Detect Stylus Up Interrupt Signal \n");
   }
   
   else
   {
      Uart_Send_String(" Touch_ISR \n");
      
/* TO DO : Stylus Down, YM_out Enable, YP_out Disable, XM_out Disable, XP_out disable
 *          XP Pull-up Disable, Auto Sequential measurement of X/Y, No operation mode   */
   rADCTSC =(0<<8)|(1<<7)|(1<<6)|(1<<5)|(1<<4)|(1<<3)|(1<<2)|(0);

      /* TO DO : ENABLE_START */      
   rADCCON |=(1);
      
      /* wait until End of A/D Conversion */
   while(!(rADCCON & (1<<15)));
      
      /*store X-Position & Y-Position Conversion data value to ADC_x, ADC_y */
   ADC_x = (rADCDAT0 & 0x3ff);
   ADC_y = (rADCDAT1 & 0x3ff);
      
      Touch_Pressed = 1;
      
/* TO DO : change to Waiting for interrupt mode 
 *         Stylus Up, YM_out Enable, YP_out Disable, XM_out Disable, XP_out disable
 *          XP Pull-up enable, Normal ADC conversion, Waiting for interrupt mode       */
   rADCTSC =(1<<8)|(1<<7)|(1<<6)|(0<<5)|(1<<4)|(0<<3)|(0<<2)|(3);
   }
   
   /* allow interrupt */
   rINTSUBMSK &= ~(0x1<< 9);   // stylus touch down interrupt. of INT_ADC
   rINTMSK1   &= ~(0x1<<31);   // general INT_ADC
   
}

int find_x(){
   for(i =0;i<18;++i)
        if(x>=x_range[i] - 10 && x<=x_range[i]+10)
            return i;  
   return -1;  
}

int find_y(){
   for(i =0;i<18;++i)
        if(y>=y_range[i] - 10 && y<=y_range[i]+10)
            return i;  
   return -1;  
}

int Check(){
   
   int x_tmp = x_idx;
    int y_tmp = y_idx;
    int cnt = 0;
         
         //오목 체크 
         //양옆
         //1.왼쪽
    x_tmp = x_idx -1;
    while(cur[y_tmp][x_tmp] == cur[y_idx][x_idx] && x_tmp >= 0){
        cnt++;
        x_tmp--;
    }
         //2. 오른쪽
    x_tmp = x_idx + 1;
    while(cur[y_tmp][x_tmp] == cur[y_idx][x_idx] && x_tmp < 18){
        cnt++;
        x_tmp++;
    }
         
    if(cnt == 4)
        return cur[y_idx][x_idx];
        
         
         //위아래
         //1. 위
    cnt = 0;
    x_tmp = x_idx;
    y_tmp = y_idx-1;
    while(cur[y_tmp][x_tmp] == cur[y_idx][x_idx] && y_tmp >= 0){
        cnt++;
        y_tmp--;
    }
         //2. 아래 
    y_tmp = y_idx+1;
    while(cur[y_tmp][x_tmp] == cur[y_idx][x_idx] && y_tmp < 12){
        cnt++;
        y_tmp++;
    }
    if(cnt == 4)
        return cur[y_idx][x_idx];
            
         
         //오른쪽 대각선
         //1. 위
    cnt = 0;
    x_tmp = x_idx+1;
    y_tmp = y_idx-1;
    while(cur[y_tmp][x_tmp] == cur[y_idx][x_idx] && y_tmp >= 0 && x_tmp < 18){
        cnt++;
        y_tmp--;
        x_tmp++;
    }
         //2. 아래 
    x_tmp = x_idx-1;
    y_tmp = y_idx+1;
    while(cur[y_tmp][x_tmp] == cur[y_idx][x_idx] && y_tmp < 12 && x_tmp >= 0){
        cnt++;
        y_tmp++;
        x_tmp--;
    }
    if(cnt == 4)
        return cur[y_idx][x_idx];
 
         //왼쪽 대각선 
         //1. 위
    cnt = 0;
    x_tmp = x_idx-1;
    y_tmp = y_idx-1;
    while(cur[y_tmp][x_tmp] == cur[y_idx][x_idx] && y_tmp >= 0 && x_tmp >= 0){
        cnt++;
        y_tmp--;
        x_tmp--;
    }
         //2. 아래 
    x_tmp = x_idx+1;
    y_tmp = y_idx+1;
    while(cur[y_tmp][x_tmp] == cur[y_idx][x_idx] && y_tmp < 12 && x_tmp < 18){
        cnt++;
        y_tmp++;
        x_tmp++;
    }
    if(cnt == 4)
        return cur[y_idx][x_idx];
    return 0;   
}
 
void numberDraw(int num, int c, int r){
	switch(num){
		case 9:
			draw_bms(c, r, cal9);
			break;
		case 8:
			draw_bms(c, r, cal8);
			break;
		case 7:
			draw_bms(c, r, cal7);
			break;
		case 6:
			draw_bms(c, r, cal6);
			break;
		case 5:
			draw_bms(c, r, cal5);
			break;
		case 4:
			draw_bms(c, r, cal4);
			break;
		case 3:
			draw_bms(c, r, cal3);
			break;
		case 2:
			draw_bms(c, r, cal2);
			break;
		case 1:
			draw_bms(c, r, cal1);
			break;
		case 0:
			draw_bms(c, r, cal0);
			break;
	}
}
 
 
 
 
void Battle(){
	flag = 0;
	win = 0;
	draw_bms(0,0,back);
	tick_cnt = 600;
	numberDraw(Sec, 430, 30);
	black_cnt = 0;
	white_cnt = 0;
	//플레이어 바둑돌 개수 init
	numberDraw(0, 426, 111);
	numberDraw(0, 447, 111);
	numberDraw(0, 426, 191);
	numberDraw(0, 447, 191);
	
	while(1){
    	Timer_Delay(10);
    	if(Update==1){
            numberDraw(Sec,430,30);
         	Update=0;
        }
        
    	if(Touch_Pressed){
   //   TODO.   calibrate X, Y  to LCD size (0,0 - 479,271)   
   		if(Mode == 2){
        	draw_bms(0,0,logo);
        	for(i=0;i<12;++i)
            	for(j=0;j<18;++j)
               		cur[i][j] = -1;
        	Mode = 0;
	
        	break;
		}

        x = (double)480 / (850- 194) * (ADC_x - 194); // do calibrate x = 480 / (850- 194) * (ADC_x - 194)
        y = (double)272 / (660 - 330) * (ADC_y - 330);   // do calibrate y = 272 / (660 - 330) * (ADC_y - 330)
        x_idx = find_x();
        y_idx = find_y();

        if(x_idx == -1 || y_idx == -1) continue;
        if(cur[y_idx][x_idx] == 1 || cur[y_idx][x_idx] == 2) continue; //이미 돌이 놓여진 자리 
        if(flag == 0){
            draw_bms(x_loc[x_idx], y_loc[y_idx], black);
            cur[y_idx][x_idx] = 1;
            flag = 1;
            tick_cnt = 600;
			
			//그리기
			black_cnt++;
			Uart_Printf("십의자리 : %d, 일의자리: %d \n", black_cnt/10, black_cnt%10);
			numberDraw(black_cnt/10, 426, 111);
			numberDraw(black_cnt%10, 447, 111);
			
        }
        else{
            draw_bms(x_loc[x_idx], y_loc[y_idx], white);
            cur[y_idx][x_idx] = 2;
            flag = 0;
            tick_cnt = 600;
            
            //그리기
			white_cnt++;
			Uart_Printf("십의자리 : %d, 일의자리: %d \n", white_cnt/10, white_cnt%10);
			numberDraw(white_cnt/10, 426, 191);
			numberDraw(white_cnt%10, 447, 191);
            
        }
        Uart_Printf("X : %d, Y: %d \n", x, y);
        win = Check();
        if(win != 0){
            //이긴 처리	
			Mode = 2;
           	if(win == 1)
				draw_bms(99, 84, blackwin);
			else
				draw_bms(99, 84, whitewin);	
			Timer_Init_buzzer();
        }
        while(!(Touch_Pressed==0))
        Uart_Printf("Released\n\n");         
      }
   }
}
   
