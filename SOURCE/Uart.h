#ifndef __UART_H__
#define __UART_H__

#define printf	Uart_Printf
#define getchar Uart_GetChar
#define putchar Uart_Send_Byte
#define puts    Uart_Send_String

void Uart_Init(int baud);
void Uart_Printf(const char *fmt,...);
void Uart_Send_String(char *pt);
void Uart_Send_Byte(int data);

char Uart_GetChar(void);

#endif//__UART_H__
