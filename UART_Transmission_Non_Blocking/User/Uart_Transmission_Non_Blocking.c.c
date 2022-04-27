/**************************************************************************************
 * NAME       : Munusamy M
 * FILENAME   : UART_BLOCKING_CALL.c
 * DESCRIPTION: USING UART_PROTOCOL TRANSMISSION DATA NON BLOCKING CALL
 * DATE       : 17-02-2022
***************************************************************************************/

#include <stdio.h>
#include "NuMicro.h"

unsigned char value[70]="HI I AM";

volatile int count=0;

void SYS_Init(void)
{

    SYS_UnlockReg();

    CLK->PWRCTL =0x4;

    CLK_WaitClockReady(CLK_STATUS_HIRCSTB_Msk);

    CLK_SetHCLK(CLK_CLKSEL0_HCLKSEL_HIRC, CLK_CLKDIV0_HCLK(1));

    SystemCoreClockUpdate();

    SYS->GPB_MFPH =0x00660000;

    SYS_LockReg();
}


void uart_init()
{

    CLK->APBCLK0=(1<<16); /*APBCLK for UART0*/

    CLK->CLKSEL1=0x03000000;/*HIRC 48MHZ*/

    CLK->CLKDIV0=0x0; /*HIRC div 0*/

    UART0->FUNCSEL=0x0;/*UART function mode enable*/

    UART0->LINE=0x3;   /*set data length 8bit, with no parity ,and 1 stop bit*/

    UART0->FIFO=0x0;  /* select the receiver byte level*/

    UART0->BAUD=0x3000019E; /*baud rate115200*/

    NVIC->ISER[0]|=(1<<12);

    UART0->INTEN|=(1<<1);
    UART0->INTEN|=(1<<0);

}

int main()
{

	    SYS_Init();

	    uart_init();


	while(1);

	}


void UART02_IRQHandler(void)

{

   if(((UART0->INTSTS & 1)))

     {

	  UART0->DAT= value[count];
      count++;

         if(count==7)
         {
	        UART0->INTEN|=~(1<<22);
	        //printf("Hi ");
         }

     }
}


