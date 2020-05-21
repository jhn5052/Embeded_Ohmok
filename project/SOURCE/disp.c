#include "2450addr.h"
#include "disp.h"



static unsigned int bfOffbits;
static unsigned int biWidth, biWidth2;
static unsigned int biHeight;

#define  NonPal_Fb   ((volatile unsigned short(*)[480]) FRAME_BUFFER)

static volatile unsigned short *pFbAddr;
	
void init_GPIO(void)
{

	rGPCUDP = 0xffffffff; 
	rGPCCON &= ~(0xffffffff);
	rGPCCON |= 0xaaaa02aa; //GPC5,6,7 = output
	
	rGPDUDP = 0xffffffff; 
	rGPDCON &= ~(0xffffffff);
	rGPDCON |= 0xaaaaaaaa;
	
	/* GPG12 is setted as LCD_Display ON/OFF */
	rGPGCON=rGPGCON&(~(3<<24))|(1<<24); //GPG12=OUTPUT
	rGPGDAT|=(1<<12);
	
	/* GPB0 backlight Enable */
	rGPBCON=rGPBCON&(~(3<<0))|(1<<0);
	rGPBDAT|= (1<<0);

}

void init_LCD_NonPalletteMode()
{		
	rVIDW00ADD0B0 = FRAME_BUFFER;	// option.h

	rVIDW00ADD1B0 = 0;
	rVIDW00ADD2B0 = (0<<13)|((LCD_SIZE_X*4*2)&0x1fff);

	rVIDW00ADD1B0 = 0+(LCD_SIZE_X*LCD_SIZE_Y);
	
	/* setting for LCD control 
	* RGB I/F,PNRMODE(BGR),VCLK=9MHz,VCLK=Enable,CLKSEL=HCLK,CLKDIR=Divided,ENVID=disable
	*/
	rVIDCON0=(0x0<<22)+(0x1<<13)+((CLKVAL)<<6)+(1<<5)+(1<<4)+(0<<2);
	rVIDCON1= (1<<6)+(1<<5);
	
	rVIDTCON0=((VBPD)<<16)+((VFPD)<<8)+(VSPW);
	rVIDTCON1=((HBPD)<<16)+((HFPD)<<8)+(HSPW);
	rVIDTCON2=(LINEVAL<<11)+(HOZVAL);
	
	rVIDOSD0A		= 	(((0)&0x7FF)<<11) | (((0)&0x7FF)<<0);
	rVIDOSD0B	 	= 	(((LCD_SIZE_X-1)&0x7FF)<<11) | (((LCD_SIZE_Y-1)&0x7FF)<<0);

	rVIDOSD1A 		= 	(((0)&0x7FF)<<11) | (((0)&0x7FF)<<0);
	rVIDOSD1B	 	= 	(((LCD_SIZE_X-1)&0x7FF)<<11) | (((LCD_SIZE_Y-1)&0x7FF)<<0);

	/* TO DO : Half swap Enable, 5:5:5:I format, Window0 ON
	 */
rWINCON0=(0x1<<16)+(0x7<<2)+(1<<0); 
	
	/* TO DO : ENVID Enable for video output and LCD control signal
	 */
rVIDCON0 |= (3<<0);	

}

void init_FrameBufferPointer(void)
{
	pFbAddr = FRAME_BUFFER ;	// option.h  or 2450addr.h
}


void init_Lcdc(void)
{

	init_GPIO();
	init_LCD_NonPalletteMode();
	init_FrameBufferPointer ();

}


void Put_Pixel_NonPal(int x, int y, int color)
{
	/* TO DO : Fill FRAMEBUFFER with color value 
	 */
	NonPal_Fb[y][x] = (unsigned short int)color;

}



#include "font.h"

#define COPY(A,B) for(loop=0;loop<32;loop++) *(B+loop)=*(A+loop);
#define OR(A,B) for(loop=0;loop<32;loop++) *(B+loop)|=*(A+loop);

unsigned int Trans_Mode = 0;


void Lcd_SetTranMode(int mode)
{
	Trans_Mode = mode;
}


void draw_pixel(int x,int y,unsigned short color)
{

	if((x>= LCD_SIZE_X )||(y>= LCD_SIZE_Y))
		return;

	if(Trans_Mode==0)
	{
		(*(pFbAddr+y* LCD_SIZE_X +x))  = color;		// Opaque mode
	}
	else
	{
		(*(pFbAddr+y* LCD_SIZE_X +x)) |= color;		// Tranparent mode
	}
	
}

void clr(int color)
{
	int x,y;
	volatile unsigned short *pLcdFb= pFbAddr;

	for(y=0;y< LCD_SIZE_Y ;y++)
	{
		for(x=0;x< LCD_SIZE_X ;x++)
		{
			*pLcdFb++ = color;
		}
	}

}

void draw_line(int x1, int y1, int x2, int y2, unsigned int color)
{
	int dx, dy, e;

	dx=x2-x1;
	dy=y2-y1;

	if(dx>=0)
	{
		if(dy >= 0) // dy>=0
		{
			if(dx>=dy) // 1/8 octant
			{
				e=dy-dx/2;
				while(x1<=x2)
				{
					draw_pixel(x1, y1, color);
					if(e>0){y1+=1;e-=dx;}
					x1+=1;
					e+=dy;
				}
			}
			else		// 2/8 octant
			{
				e=dx-dy/2;
				while(y1<=y2)
				{
					draw_pixel(x1, y1, color);
					if(e>0){x1+=1;e-=dy;}
					y1+=1;
					e+=dx;
				}
			}
		}
		else		   // dy<0
		{
			dy=-dy;   // dy=abs(dy)

			if(dx>=dy) // 8/8 octant
			{
				e=dy-dx/2;
				while(x1<=x2)
				{
					draw_pixel(x1, y1, color);
					if(e>0){y1-=1;e-=dx;}
					x1+=1;
					e+=dy;
				}
			}
			else		// 7/8 octant
			{
				e=dx-dy/2;
				while(y1>=y2)
				{
					draw_pixel(x1, y1, color);
					if(e>0){x1+=1;e-=dy;}
					y1-=1;
					e+=dx;
				}
			}
		}
	}
	else //dx<0
	{
		dx=-dx;		//dx=abs(dx)
		if(dy >= 0) // dy>=0
		{
			if(dx>=dy) // 4/8 octant
			{
				e=dy-dx/2;
				while(x1>=x2)
				{
					draw_pixel(x1, y1, color);
					if(e>0){y1+=1;e-=dx;}
					x1-=1;
					e+=dy;
				}
			}
			else		// 3/8 octant
			{
				e=dx-dy/2;
				while(y1<=y2)
				{
					draw_pixel(x1, y1, color);
					if(e>0){x1-=1;e-=dy;}
					y1+=1;
					e+=dx;
				}
			}
		}
		else		   // dy<0
		{
			dy=-dy;   // dy=abs(dy)

			if(dx>=dy) // 5/8 octant
			{
				e=dy-dx/2;
				while(x1>=x2)
				{
					draw_pixel(x1, y1, color);
					if(e>0){y1-=1;e-=dx;}
					x1-=1;
					e+=dy;
				}
			}
			else		// 6/8 octant
			{
				e=dx-dy/2;
				while(y1>=y2)
				{
					draw_pixel(x1, y1, color);
					if(e>0){x1-=1;e-=dy;}
					y1-=1;
					e+=dx;
				}
			}
		}
	}
}

void draw_hline(int y, int x1, int x2, int color)
{
	int i, xx1, xx2;

	if(x1<x2)
	{
		xx1=x1;
		xx2=x2;
	}
	else
	{
		xx1=x2;
		xx2=x1;
	}

	for(i=xx1;i<=xx2;i++)
	{
		draw_pixel(i,y,color);
	}
}

void draw_vline(int x, int y1, int y2, int color)
{
	int i, yy1, yy2;

	if(y1<y2)
	{
		yy1=y1;
		yy2=y2;
	}
	else
	{
		yy1=y2;
		yy2=y1;
	}

	for(i=yy1;i<=yy2;i++)
	{
		draw_pixel(x,i,color);
	}
}

void Lcd_Rectangular(int x1, int y1, int x2, int y2, int color)
{
	int xx1, yy1, xx2, yy2;

	if(x1<x2)
	{
		xx1=x1;
		xx2=x2;
	}
	else
	{
		xx1=x2;
		xx2=x1;
	}

	if(y1<y2)
	{
		yy1=y1;
		yy2=y2;
	}
	else
	{
		yy1=y2;
		yy2=y1;
	}

	draw_hline(yy1,xx1,xx2,color);
	draw_hline(yy2,xx1,xx2,color);
	draw_vline(xx1,yy1,yy2,color);
	draw_vline(xx2,yy1,yy2,color);
}

void draw_bar(int x1, int y1, int x2, int y2, int color)
{
     int i, j;
     int xx1, yy1, xx2, yy2;

     if(x1<x2){
     	xx1=x1;
     	xx2=x2;
     }
     else {
     	xx1=x2;
     	xx2=x1;
     } // end of if-else

     if(y1<y2){
     	yy1=y1;
     	yy2=y2;
     }
     else {
     	yy1=y2;
     	yy2=y1;
     } // end of if-else

     for(i=yy1;i<=yy2;i++)
         for(j=xx1;j<=xx2;j++)
             draw_pixel(j,i,color);

}

void draw_rectangle(int x1, int y1, int dx, int dy, unsigned int color)
{
	unsigned int x2 = x1+dx-1;
	unsigned int y2 = y1+dy-1;
	
	draw_hline(y1,x1,x2,color);
	draw_vline(x1,y1,y2,color);	
	draw_hline(y2,x1,x2,color);	
	draw_vline(x2,y1,y2,color);	
	
}

void draw_pixel_DOTMGRP(int dotx, int doty, int color)
{
	draw_bar( 
		dotx	* 15 , \
		doty	* 15 , \
		dotx	* 15 +10, \
		doty	* 15 +10, \
		color \
	);

	
}


void Lcd_PutRectangle(int x1, int y1, int dx, int dy, unsigned int color)
{
	unsigned int x2 = x1+dx-1;
	unsigned int y2 = y1+dy-1;

	draw_line(x1, y1, x2, y1, color);
	draw_line(x2, y1, x2, y2, color);
	draw_line(x1, y2, x2, y2, color);
	draw_line(x1, y1, x1, y2, color);
}

void Lcd_DrawPattern(unsigned int uPosX, unsigned int uPosY, unsigned int uHsz, unsigned int uVsz, PTN_TYPE eType)
{
	unsigned int uSubHsz, uSubVsz;

	if (eType == PTN1)
	{
		clr(WHITE);
		// Draw Color Bar
		uSubHsz = uHsz/8;
		uSubVsz = uVsz/2;

		draw_rectangle(uPosX, uPosY, uSubHsz, uSubVsz, RED);
		draw_rectangle(uSubHsz*1, uPosY, uSubHsz, uSubVsz, GREEN);
		draw_rectangle(uSubHsz*2, uPosY, uSubHsz, uSubVsz, BLUE);
		draw_rectangle(uSubHsz*3, uPosY, uSubHsz, uSubVsz, CYAN);
		draw_rectangle(uSubHsz*4, uPosY, uSubHsz, uSubVsz, MAGENTA);
		draw_rectangle(uSubHsz*5, uPosY, uSubHsz, uSubVsz, YELLOW);
		draw_rectangle(uSubHsz*6, uPosY, uSubHsz, uSubVsz, BLACK);
		draw_rectangle(uSubHsz*7, uPosY, uSubHsz, uSubVsz, WHITE);

		// Draw Pattern
		uSubHsz = uHsz/2;
		Lcd_PutRectangle(uPosX, uSubVsz, uHsz, uSubVsz, RED);
		draw_line(uPosX, uSubVsz, uHsz-1, uVsz-1, BLACK);
		draw_line(uHsz, uSubVsz, uPosX, uVsz-1, MAGENTA);

		uSubHsz = uHsz/2;
		draw_line(uSubHsz, uSubVsz, uSubHsz, uVsz-1, GREEN);

		uSubVsz = uVsz*3/4;
		draw_line(uPosX, uSubVsz, uHsz-1, uSubVsz, BLUE);
	}
	else // eType == PTN2
	{
		uSubHsz = uHsz/2;
		uSubVsz = uVsz/4;
		draw_rectangle(uPosX, uPosY, uSubHsz, uSubVsz, GREEN);
		draw_rectangle(uSubHsz, uPosY, uSubHsz, uSubVsz, BLUE);
		draw_rectangle(uPosX, uSubVsz, uSubHsz, uSubVsz, WHITE);
		draw_rectangle(uSubHsz, uSubVsz, uSubHsz, uSubVsz, YELLOW);

		uSubHsz = uHsz/4;
		uSubVsz = uVsz/2;
		draw_rectangle(uPosX, uSubVsz, uSubHsz, uSubVsz, MAGENTA);
		draw_rectangle(uSubHsz*1, uSubVsz, uSubHsz, uSubVsz, YELLOW);
		draw_rectangle(uSubHsz*2, uSubVsz, uSubHsz, uSubVsz, CYAN);
		draw_rectangle(uSubHsz*3, uSubVsz, uSubHsz, uSubVsz, WHITE);

		uSubHsz = uHsz/2;
		uSubVsz = uVsz/2;
		draw_line(uSubHsz, uPosY, uPosX, uSubVsz-1, BLACK);
		draw_line(uSubHsz, uPosY, uHsz-1, uSubVsz-1, BLACK);
		draw_line(uPosX, uSubVsz-1, uSubHsz-1, uVsz-1, BLACK);
		draw_line(uSubHsz-1, uVsz-1, uHsz-1, uSubVsz-1, BLACK);

		draw_line(uPosX, uPosY, uHsz-1, uVsz-1, RED);
		draw_line(uHsz-1, uPosY, uPosX, uVsz-1, GREEN);
		Lcd_PutRectangle(uPosX, uPosY, uHsz, uVsz, RED);
	}
}

void draw_putc(int x,int y,int color,int bkcolor,int data, int zx, int zy)
{
	unsigned offset,loop;
	unsigned char xs,ys;
	unsigned char temp[32];
	unsigned char bitmask[]={128,64,32,16,8,4,2,1};
	
	offset=data*16;
	COPY(eng8x16+offset,temp);
	
	for(ys=0;ys<16;ys++)
	for(xs=0;xs<8;xs++)
	if(temp[ys]&bitmask[xs])
	{
		if( (zx==1)&&(zy==1) )
			draw_pixel(x+xs,y+ys,color);
		else if( (zx==2)&&(zy==1) ){
			draw_pixel(x+2*xs,y+ys,color);
			draw_pixel(x+2*xs+1,y+ys,color);
		}
		else if( (zx==1)&&(zy==2) ){
			draw_pixel(x+xs,y+2*ys,color);
			draw_pixel(x+xs,y+2*ys+1,color);
		}
		else if( (zx==2)&&(zy==2) ){
			draw_pixel(x+2*xs,y+2*ys+1,color);
			draw_pixel(x+2*xs+1,y+2*ys,color);
			draw_pixel(x+2*xs,y+2*ys,color);
			draw_pixel(x+2*xs+1,y+2*ys+1,color);
		}
	}
	else
	{
		if( (zx==1)&&(zy==1) )
			draw_pixel(x+xs,y+ys,bkcolor);
		else if( (zx==2)&&(zy==1) ){
			draw_pixel(x+2*xs,y+ys,bkcolor);
			draw_pixel(x+2*xs+1,y+ys,bkcolor);
		}
    	else if( (zx==1)&&(zy==2) ){
			draw_pixel(x+xs,y+2*ys,bkcolor);
			draw_pixel(x+xs,y+2*ys+1,bkcolor);
    	}
    	else if( (zx==2)&&(zy==2) ){
			draw_pixel(x+2*xs,y+2*ys+1,bkcolor);
			draw_pixel(x+2*xs+1,y+2*ys,bkcolor);
			draw_pixel(x+2*xs,y+2*ys,bkcolor);
			draw_pixel(x+2*xs+1,y+2*ys+1,bkcolor);
		}
	}

}


void draw_puts(int x, int y, int color, int bkcolor, char *str, int zx, int zy)
{
     unsigned data;

     while(*str)
     {
        data=*str++;
        if(data>=128) 		// Hangul
        {
             data*=256;
             data|=*str++;
//             Lcd_HanPutchxyh(x, y, color, bkcolor, data, zx, zy);
             x+=zx*16;
        }
        else 			// English
        {
             draw_putc(x, y, color, bkcolor, data, zx, zy);
             x+=zx*8;
        }

     }

}


void draw_printf(int x, int y, int color, int bkcolor, int zx, int zy, char *fmt,...)
{
     va_list ap;
     char string[256];

     va_start(ap,fmt);
     vsprintf(string,fmt,ap);
     draw_puts(x, y, color, bkcolor, string, zx, zy);
     va_end(ap);

}

void draw_bms(unsigned int x, unsigned int y, const unsigned char *fp)
{

	unsigned int xx=0, yy=0;		
	unsigned short tmpRGB;

	unsigned int biWidth;
	unsigned int biHeight;

	biWidth  =*(unsigned short *)(fp+0);
	biHeight =*(unsigned short *)(fp+2);

	unsigned short	*pFp = (unsigned short *)(fp+4);

//	for(yy=0;yy<biHeight;yy++)
	for(yy=biHeight;yy>0;yy--)
	{
		for(xx=0;xx<biWidth;xx++)
		{
			tmpRGB = *(unsigned short *)(pFp++);
			
			draw_pixel(x+xx,y+yy,tmpRGB);
	    } 
	}

}
