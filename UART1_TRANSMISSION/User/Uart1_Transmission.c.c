/**************************************************************************************
 * NAME       : Munusamy M
 * FILENAME   : UART_SEND_RECEIVE.c
 * DESCRIPTION: USING INTERRUPT TRANSMISSION DATA VIA UART BOARD TO BOARD COMMUNICATION
 * DATE       : 25-02-2022
***************************************************************************************/

#include <stdio.h>
#include "NuMicro.h"

void uartopen0(void);
void uartopen0(void);
void uartclockenable0(void);
void uartclockenable1(void);
void recieve(void);

uint8_t rx_receive;
uint8_t j;
uint8_t tx_send[500]="ABCDEFGHIJKLMNOPQRSTUVWXYZ";
uint8_t i;

void UART02_IRQHandler(void)
{

	if(UART0->INTSTS &1)
	{

	rx_receive=UART0->DAT;

	printf("%c\n", rx_receive);
	}

	      if(UART0->INTSTS&1<<1)
	      {
	    	  if(i<26)
	    	  	 {
	    		  	 UART0->DAT=tx_send[i];
	    		  	 printf("%c" ,tx_send);
	    		  	 i++;
	    	  	 }
	 }
	// UART0->INTEN &=~(1<<1);
	}

void SYS_Init(void)
{
    /* Unlock protected registers */
    SYS_UnlockReg();

    /* Enable HIRC clock (Internal RC 48MHz) */
    CLK_EnableXtalRC(CLK_PWRCTL_HIRCEN_Msk);

    /* Wait for HIRC clock ready */
    CLK_WaitClockReady(CLK_STATUS_HIRCSTB_Msk);

    /* Select HCLK clock source as HIRC and HCLK source divider as 1 */
    CLK_SetHCLK(CLK_CLKSEL0_HCLKSEL_HIRC, CLK_CLKDIV0_HCLK(1));

    /* Enable UART0 clock */
    CLK_EnableModuleClock(UART0_MODULE);
    uartclockenable0();
    uartclockenable1();
       /* Switch UART0 clock source to HIRC */
    CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UART0SEL_HIRC, CLK_CLKDIV0_UART0(1));

    /* Update System Core Clock */
    SystemCoreClockUpdate();

    /* Set PB multi-function pins for UART0 RXD=PB.12 and TXD=PB.13 */
    SYS->GPB_MFPH = (SYS->GPB_MFPH & ~(SYS_GPB_MFPH_PB12MFP_Msk | SYS_GPB_MFPH_PB13MFP_Msk))    |       \
                    (SYS_GPB_MFPH_PB12MFP_UART0_RXD | SYS_GPB_MFPH_PB13MFP_UART0_TXD);

    
    /* Lock protected registers */
    SYS_LockReg();
}
void uartclockenable0(void)
   {
   CLK-> CLKSEL1 =(CLK-> CLKSEL1&(~(7<<24)))|3<<24;      //Source Selection
   CLK-> CLKDIV0 =(CLK-> CLKDIV0&(~(0XF<<8)))|0X0<<8;    //   UART0 Clock Divide Number From UART0 Clock Source
   	   	   	   	   	   	   	   	   	   	   	   	   	   	 //UART0 clock frequency = (UART0 clock source frequency) / (UART0DIV + 1).

   CLK-> APBCLK0|= 1<<16;                                // UART0=1 clock Enabled.
   }
   void uartopen0(void)
   {
   UART0->FUNCSEL =0<<0;                                //000 = UART function.
   UART0->LINE = 3<<0|0<<3|0<<2;                        
   UART0->FIFO &=~(15<<4 |15<<16);
   UART0->BAUD = 1<<28|1<<29|415;
   }

   void uartclockenable1(void)
   {
   CLK-> CLKSEL1 &=~(7<<28)|3<<28;
   CLK-> CLKDIV0 &=~ (0Xf<<12)|0X0<<12;
   CLK-> APBCLK0 |= 1<<17;
   }

   void uartopen1(void)
   {
   UART1->FUNCSEL = 0<<0;
   UART1->LINE = 3<<0|0<<3|0<<2;
   UART1->FIFO &=~(15<<4 | 15<<16);
   UART1->BAUD = 1<<28 |1<<29|415;
   }

   void enableinterupt(void)
   {
   UART0->INTEN |=1<<1;
   UART0->INTEN |=1<<0;
   NVIC->ISER[0U] |=1<<12;
   }


/*
 * This is a template project for M031 series MCU. Users could based on this project to create their
 * own application without worry about the IAR/Keil project settings.
 *
 * This template application uses external crystal as HCLK source and configures UART0 to print out
 * "Hello World", users may need to do extra system configuration based on their system design.
 */

int main()
{
    SYS_Init();
    uartopen0();
    uartopen1();
    enableinterupt();

    while(1);
}

void recieve(void)
{
	rx_receive=UART0->DAT;
   printf("%c", rx_receive);
}
