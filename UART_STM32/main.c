#include "stm32f10x.h"
#include <stdio.h>
#include <string.h>
#define MAX_CHUOI 50
char CHUOI[MAX_CHUOI]; 
int demdodai = 0;

void delay(int a);
char receiveChar();
void sendChar(char data);
void sendstring(char *str);
void receivestring();
void delay(int a) {
    for(int i = 0; i < a; i++) {
        for(int j = 0; j < 1000; j++) {
        }
    }
}
char receiveChar() {
    while (!(USART1->SR & USART_SR_RXNE));
    char receivedChar = USART1->DR;
    return receivedChar;
}

void sendChar(char data) {
    while (!(USART1->SR & USART_SR_TXE));
    USART1->DR = data;
}
void sendstring(char *str){
	while(*str){
		sendChar(*str++);
	}
}
void receivestring(){
	char receiedchar;
	do{
		receiedchar = receiveChar();
		if(receiedchar != '\r' && receiedchar != '\n'){
			CHUOI[demdodai++] = receiedchar;
		}
	}
	while (receiedchar != '\0' && demdodai < MAX_CHUOI - 1);
    CHUOI[demdodai] = '\0';
}

int main() {
		//ENABLE gpioA,C va USART;
    RCC->APB2ENR |= 1<<4;
		RCC->APB2ENR |= 1<<2;
    RCC->APB2ENR |= 1<<14;
		//config cac che do input output cua cac gpio9, 10, 13;
		GPIOA->CRH &= 0xFFFFFF0F;
		GPIOA->CRH |= 0x000000B0;
		GPIOA->CRH &= 0xFFFFF0FF;
		GPIOA->CRH |= 0x00000400;
    GPIOC->CRH &= 0xFF0FFFFF;
    GPIOC->CRH |= 0x00200000;
		//set buadrate cho bo usart;
    USART1->BRR = 0x1D4C;
		//khoi dong bo phat, thu, ngat du lieu va ENABLE USART;
    USART1->CR1 = (1<<2) | (1<<3) | (1<<13) ;

    while (1) {
				receivestring();
				sendstring("receied: ");
				sendstring(CHUOI);
				sendstring("\n");
				if(strcmp(CHUOI, "ledon") == 0){
					GPIOC->BSRR = 1<<13;
				}
				else if(strcmp(CHUOI, "ledoff") == 0){
					GPIOC->BRR = 1<<13;
				}
				demdodai = 0;
    }
}