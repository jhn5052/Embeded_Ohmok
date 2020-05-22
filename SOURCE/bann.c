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

#include "2450addr.h"
#include "disp.h"
#include "Uart.h"


/*
*	DOT MATRIX GRAPHIC LCD BANNER FONT SIEVE
*/
static const char	message[][2]= { 
	3,3, 4,3, 5,3, 6,3, 7,3, 8,3, 9,3, 10,3, 11,3, 12,3, 13,3, 14,3, 15,3, 16,3, 17,3, 18,3, 19,3, 20,3, 21,3, 22,3, 23,3, 24,3, 25,3, 26,3, 27,3, 
	3,5, 4,5, 5,5, 6,5, 7,5, 9,5, 10,5, 11,5, 13,5, 14,5, 15,5, 16,5, 24,5, 25,5, 26,5, 27,5, 
	3,6, 4,6, 7,6, 10,6, 13,6, 24,6, 
	3,7, 4,7, 10,7, 13,7, 18,7, 22,7, 24,7, 	
	3,8, 4,8, 5,8, 6,8, 7,8, 10,8, 13,8, 14,8, 15,8, 18,8, 22,8, 24,8, 25,8, 26,8,
	6,9, 7,9, 10,9, 13,9, 18,9, 22,9, 24,9, 
	6,10, 7,10, 10,10, 13,10, 19,10, 21,10, 24,10,
	3,11, 4,11, 5,11, 6,11, 7,11, 9,11, 10,11, 11,11, 13,11, 14,11, 15,11, 16,11, 20,11, 24,11, 25,11, 26,11, 27,11, 
	3,5, 4,5, 5,5, 6,5, 7,5, 9,5, 10,5, 11,5, 13,5, 14,5, 15,5, 16,5, 24,5, 25,5, 26,5, 27,5, 
	3,13, 4,13, 5,13, 6,13, 7,13, 8,13, 9,13, 10,13, 11,13, 12,13, 13,13, 14,13, 15,13, 16,13, 17,13, 18,13, 19,13, 20,13, 21,13, 22,13, 23,13, 24,13, 25,13, 26,13, 27,13, 
  	
  	{0xFF,0xFF} 
};







void banner(void)
{

	Uart_Send_String("\n");	/* line feed at top */

	Uart_Send_String("################################################################################\n");
	Uart_Send_String("#                                                                               \n");
	Uart_Send_String("#     ####   ###   #####           #####   <T.M>                                \n");
	Uart_Send_String("#    #        #    #      #     #  #                                            \n");
	Uart_Send_String("#     ####    #    ####    #   #   ####    Powered by Course-Ware Framework     \n");
	Uart_Send_String("#         #   #    #        # #    #          (C) Copyrights, In4MaySun Inc     \n");
	Uart_Send_String("#    #####   ###   #####     #     #####                                        \n");
	Uart_Send_String("#                                                         www.in4maysun.com     \n");
	Uart_Send_String("#                                                          by Seo Kwang-Won     \n");
	Uart_Send_String("#    ");

//#if 	defined __M_DEMO
//	Uart_Send_String(MSG_DEMO);	
//#elif	defined __M_EXER
//	Uart_Send_String(MSG_EXER);
//#elif	defined	__M_WORK
//	Uart_Send_String(MSG_WORK);
//#endif
//
//	Uart_Send_String(MSG_BOAD);
	Uart_Send_Byte('\n');
//	
//#if		defined __M_DEMO_LEDS
//	Uart_Send_String(MSG_DEMO_LEDS);
//#endif
//#if		defined __M_DEMO_KEYS
//	Uart_Send_String(MSG_DEMO_KEYS);
//#endif
//#if		defined __M_DEMO_INTR
//	Uart_Send_String(MSG_DEMO_INTR);
//#endif
//#if		defined __M_DEMO_LCDS
//	Uart_Send_String(MSG_DEMO_LCDS);
//#endif
//#if		defined __M_DEMO_BUZZ
//	Uart_Send_String(MSG_DEMO_BUZZ);
//#endif
//#if		defined __M_DEMO_UART
//	Uart_Send_String(MSG_DEMO_UART);
//#endif
//
	Uart_Send_String("################################################################################\n");

	return;
}

#if 1
void draw_banner(void)
{
	unsigned int i;

//	rTPAL = (0x1F << 03)|(0x1 << 24);
	for( i=0; i << 0xFFFFFF ; i++);

	i=0;
	while(message[i][0] != 0xFF)
	{
		draw_pixel_DOTMGRP(message[i][0], message[i][1], WHITE);
		i++; //next dot index
	}
//	rTPAL =0;

}
#endif