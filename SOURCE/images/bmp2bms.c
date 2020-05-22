// bmp2bms.cpp : Defines the entry point for the console application.
//

/*
 * BITMAP FORMAT *
----	----	--------------------------------------------------------
offs	Byte	description
----	----	--------------------------------------------------------
0x00	2		type
0x02	4		file size
0x06	2		reserved
0x08	2		reserved
0x0A	4		data offset position
0x0E	4		size of the header          from /Here to /There
0x12	4		picture image width
0x16	4		picture image height
0x1A	2		color planes ( shoud be '1' )
0x1C	2		number of bit per pixel { 1,4,8
,0x10,0x18,0x20 }
0x1E	4		compression method
0x22	4		picture image size =( width x height x Bite_per_pixels )
0x26	4		horizontal resolution (pixels/meter)
0x2A	4		vertical   resolution (pixels/meter)
0x2E	4		the number of colors in the pallette. (0:default, 2^n )
0x32	4       the number of important colors,ignored to /There
0x36	xxxx	image data ....
----	----	--------------------------------------------------------
offs	Byte	description
----	----	--------------------------------------------------------
*/

#include "stdio.h"
#include "stdlib.h"

#include "string.h"
#define DEBUGGING	0

#define ENCODING565	1
#define ENCODING555	2

typedef struct bmpheader_t {

unsigned int short 	bfType;		//0
unsigned int		bfSize;		//2
unsigned int		bfOffbits;	//0x0A
unsigned int		biWidth;	//0x12
unsigned int		biHeight;	//0x16

} bmpheader_s;

bmpheader_s mybmpheader;

int main(int argc, char* argv[])
{

	puts("###############################################################################");
	puts("#                                                                              ");
	puts("#     ####   ###   #####           #####   <T.M>                               ");
	puts("#    #        #    #      #     #  #                                           ");
	puts("#     ####    #    ####    #   #   ####    Powered by Course-Ware Framework    ");
	puts("#         #   #    #        # #    #          (C) Copyrights, In4MaySun Inc    ");
	puts("#    #####   ###   #####     #     #####                                       ");
	puts("#                                                         www.in4maysun.com    ");
	puts("#                                                          by Seo Kwang-Won    ");
	puts("###############################################################################");

	puts("/BMP2BMS");
	puts("");

	puts(">>");


	if (argc < 2)
	{
		puts("########################################");
		puts("Usage: bmp2bms <filename> <555|565>");
		puts("");
		puts("Translate regular bitmap file :");
		puts("   Fr: 24bits color RGB = 8:8:8 BMP file");
		puts("   To: 16bits color RGB = 5:6:5 BMP file");
		puts("   To: 16bits color RGB = 5:5:5 BMP file");
		puts("########################################");
		exit(1);
	
	};

	FILE*	fpr;
	FILE*	fpw;

	int numread;
	unsigned char* buf;


	char str1[80];
	char str2[80];
	char strc[80];
	int  strl;

	int  encoding5x5 = ENCODING565;
	

	if (NULL != strstr( argv[2], "555")) encoding5x5 = ENCODING555 ;

	if (NULL != strstr( argv[2], "565")) encoding5x5 = ENCODING565 ;

//	strcpy(strc, argv[1]);
	strcpy(strc, "back.bmp");
	
	strl = strlen(strc);
	
	char *pdest;
	int  result;

	pdest = strstr( strc, ".bmp" );
	if( pdest == NULL )
	{
		sprintf(str1, "%s.bmp", strc);
		sprintf(str2, "%s.bms", strc);
	}
	else
	{
		strc[ strl - 4 ] = '\0';

		sprintf(str1, "%s.bmp", strc);
		sprintf(str2, "%s.bms", strc);		
	}

	fpr = fopen(str1, "rb");
	if (!fpr)
	{
		puts(" Error: use only file name for argument ");
		exit(1);
	}

	fpw = fopen(str2, "wb");

//	###############################################################################
//	#	put instruction message how to print on screen using draw_bms function
//	###############################################################################
	fprintf(fpw, "/* BMS, the BitMap for Sieve projects */ \n")		;
	fprintf(fpw, "/* Produced pixel format is RGB %s */ \n", argv[2]);
	fprintf(fpw, "/* Freely provided by IN4MAYSUN, Inc. */ \n\n")	;

	fprintf(fpw, "/* Insert draw_bms( x, y, %s); <- in your code */ \n\n", strc)	;

	buf = (unsigned char*)malloc(0x36);	//buffer for header
	
    numread = fread( buf, sizeof( char ), 0x36 , fpr );	//read header

	mybmpheader.bfType		= *(unsigned short int*)(&buf[   0])	;
	mybmpheader.bfSize		= *(unsigned       int*)(&buf[   2])	;
	mybmpheader.bfOffbits	= *(unsigned       int*)(&buf[0x0A])	;
	mybmpheader.biWidth	    = *(unsigned       int*)(&buf[0x12])	;
	mybmpheader.biHeight	= *(unsigned       int*)(&buf[0x16])	;


	int bmpwidth = mybmpheader.biWidth		;
	bmpwidth = (bmpwidth % 4)				;

	if ( bmpwidth != 0 ) 
	{
		free(buf)	;
		
		/* close file */
		fclose(fpr)	;
		fclose(fpw)	;

		puts("")	;
		  puts("FAILED : input BMP width should be 4's multiples ! ")	;
		printf(" WIDTH : %03d \n", mybmpheader.biWidth )				;
		puts("<<")	;

		return -1	;
	}


#if DEBUGGING
	printf("bfType;		%04x //0  \n", mybmpheader.bfType	);	
	printf("bfSize;		%04x //4  \n", mybmpheader.bfSize	);	
	printf("bfOffbits;	%04x //10 \n", mybmpheader.bfOffbits );	
	printf("biWidth;	%04x //18 \n", mybmpheader.biWidth	);
	printf("biHeight;	%04x //22 \n", mybmpheader.biHeight	);

#endif

    result = fseek( fpr, mybmpheader.bfOffbits, SEEK_SET);
	
	if (result)
	{
		puts("fseek error");
	}

	unsigned int	f_width, f_widthL, f_widthH;
	unsigned int	f_height, f_heightL, f_heightH;

//	###############################################################################
//	#	Line: static const unsigned char %s[] = {
//	###############################################################################
	fprintf(fpw, "static const unsigned char %s[] = { \n\n", strc);

	f_width	= mybmpheader.biWidth;
	f_height= mybmpheader.biHeight;

	f_widthH = ( f_width  >> 8) & 0xFF;
	f_heightH =( f_height >> 8) & 0xFF;
	f_widthL = ( f_width  >> 0) & 0xFF;
	f_heightL =( f_height >> 0) & 0xFF;

//	###############################################################################
//	#	Line: width and height 0x00, 0x00 format
//	###############################################################################
#if 0
	fprintf(fpw, "0x%02X," ,f_widthL );		// width
	fprintf(fpw, "0x%02X," ,f_widthH );		// width
	fprintf(fpw, "0x%02X," ,f_heightL );	// height
	fprintf(fpw, "0x%02X," ,f_heightH );	// height
#else
//	###############################################################################
//	#	Line: width and height 0x0000 format
//	###############################################################################
	fprintf(fpw, "0x%02X, 0x%02X, // %04d : bms width   \n" ,f_widthL,	f_widthH,	f_width ); // width
	fprintf(fpw, "0x%02X, 0x%02X, // %04d : bms height  \n" ,f_heightL, f_heightH,	f_height); // height
#endif

	fputc('\n', fpw);	// insert lines for visual clearance
//	fputc('\n', fpw);

	unsigned	int ch =0;
	unsigned int short chW =0;

	unsigned int lf=1;	//be careful. it's due to logical.


	result = fseek( fpr, 0x36, SEEK_SET);

	if (result)
	{
		puts("fseek error");
	}

	while(1)
	{
		ch  = fgetc(fpr);

		/* RGB value translation */

		if(!feof(fpr))
		{

			switch (lf%3)	// switch by remainder
			{
				case 1:		// R: 8 -> 5 bits shrink
					ch = (ch >> 3);		
					chW |= ch<< 0 ;
					break;
				case 2:		// G: 8 -> 6 or 5 bits shrink
					ch = (ch >> ( (encoding5x5 == ENCODING565) ? 2 : 3 ) );
					chW |= ch<< 5 ;
					break;
				case 0:		// B: 8 -> 5 bits shrink
					ch = (ch >> 3);
					chW |= ch<< ( (encoding5x5 == ENCODING565) ? 11 : 10 );
					break;
			}

			ch =0;	// reset temp char, ch.

			if( !(lf%3) )
			{
				
				fprintf(fpw, "0x%02X,", (chW & 0xFF) ); // Low byte of chW
				fprintf(fpw, "0x%02X,", (chW >>   8) ); // High ..........

				chW = 0;	// reset accumulator, chW.			

				if ( !(lf % 8) )	/* line feed every 16 0xNN */
				{
					fputc('\n', fpw);
					putchar('.');
				}
			}

			lf++;	// next
			
		}	
			
		else
			break;
	}


	/* finishing header file	*/
	fputc('}', fpw);
	fputc(';', fpw);

	free(buf)	;

	/* close file */
	fclose(fpr);
	fclose(fpw);

	puts("");
	printf("success ! producing %s", ( encoding5x5 == ENCODING555) ? "555" : "565" );

	puts("success!");
	puts("<<");

	return 0;
}

