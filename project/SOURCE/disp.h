/*
################################################################################
#                                                                               
#     ####   ###   #####  #     #  #####   <T.M>                                
#    #        #    #      #     #  #                                            
#     ####    #    ####    #   #   ####    Powered by Course-Ware Framework     
#         #   #    #        # #    #          (C) Copyrights, In4MaySun Inc     
#    #####   ###   #####     #     #####                                        
#                                                         www.in4maysun.com     
#    													   by Seo Kwang-Won
################################################################################
*/
//2014.06.05 1420

#include "stdarg.h"


#ifndef __DISP_H__
#define __DISP_H__

/*	dongsoo board
 */
#define LCD_SIZE_X (480)
#define LCD_SIZE_Y (272)
#define CLKVAL  	13
#define HOZVAL 	   (LCD_SIZE_X-1)
#define LINEVAL    (LCD_SIZE_Y-1)

#define VBPD (10)	
#define VFPD (1)	
#define VSPW (1)	
#define HBPD (43)	
#define HFPD (1)	
#define HSPW (1)	


#define	FRAME_BUFFER	0x33800000


#define BLACK		0x0000
#define WHITE		0xffff
#define BLUE		0x001f
#define GREEN		0x07e0
#define RED			0xf800
#define YELLOW		(RED|GREEN)
#define CYAN		(GREEN|BLUE)
#define MAGENTA		(RED|BLUE)


void init_Lcdc(void);


void clr(int color);

void draw_pixel(int x,int y,unsigned short color);
void draw_line(int x1, int y1, int x2, int y2, unsigned int cvalue);
void draw_hline(int y, int x1, int x2, int color);
void draw_vline(int x, int y1, int y2, int color);
void draw_bar(int x1, int y1, int x2, int y2, int color);
void draw_rectangle(int x1, int y1, int dx, int dy, unsigned int cvalue);

void draw_putc(int x,int y,int color,int bkcolor,int data, int zx, int zy);
void draw_puts(int x, int y, int color, int bkcolor, char *str, int zx, int zy);
void draw_printf(int x, int y, int color, int bkcolor, int zx, int zy, char *fmt,...);

void draw_bms(unsigned int x, unsigned int y, const unsigned char *fp);

void draw_banner(void);
void draw_pixel_DOTMGRP(int dotx, int doty, int color);

void Lcd_SetTranMode(int mode);
void Lcd_GetInfoBMP(int * x, int  * y, const unsigned short int *fp);

void Lcd_PutRectangle(int x1, int y1, int dx, int dy, unsigned int cvalue);
void Lcd_Rectangular(int x1, int y1, int x2, int y2, int color);

typedef enum
{
	PTN1, PTN2
}PTN_TYPE;

void Lcd_DrawPattern(unsigned int uPosX, unsigned int uPosY, unsigned int uHsz, unsigned int uVsz, PTN_TYPE eType);
void Lcd_DrawBMP(int x, int y, const unsigned short int *fp);


#endif	//__DISP_H__
