#define __USART2_H
#include "stm32f10x.h"
void USART2_Config();
void USART2_PutChar(char c);
uint16_t USART2_GetChar();
void USART2_PutString(char *s);
