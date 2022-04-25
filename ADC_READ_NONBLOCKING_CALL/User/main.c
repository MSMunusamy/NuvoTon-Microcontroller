/**************************************************************************************
 * NAME       : Munusamy M
 * FILENAME   : READ_ADC_VOLTAGE_DIVIDER_NETWORK.c
 * DESCRIPTION: READ VALUE FROM VOLTAGE DIVIDER NETWORK CALCULATE THE APPLIED VOLTAGE
 * DATE       : 05-03-2022
***************************************************************************************/

#include <stdio.h>
#include "NuMicro.h"

/*Function Prototypes   */

void uart_transmit(void);
void adc_char_uart(void);
void Systick_config(void);
void uart_init(void);
void Adc_init(void);
void Adc_start(void);
short int adc_readvalue(void);
void ADC_IRQHandler(void);
void SysTick_Handler(void);
void adc_char_uart(void);
void uart_transmit(void);

/*variable initialization*/
volatile int i,readvalue,delay,delay1,temp,count,trans_data[100],rem,value;
void SYS_Init(void)
{
    /* Unlock protected registers */
    SYS_UnlockReg();

    /* Clock cycles delay Enabled*/
    CLK->PWRCTL=0x4;

    /* Enable HIRC clock (Internal RC 48MHz) */
    CLK_EnableXtalRC(CLK_PWRCTL_HIRCEN_Msk);

    /* Wait for HIRC clock ready */
    CLK_WaitClockReady(CLK_STATUS_HIRCSTB_Msk);

    /* Select HCLK clock source as HIRC and HCLK source divider as 1 */
    CLK_SetHCLK(CLK_CLKSEL0_HCLKSEL_HIRC, CLK_CLKDIV0_HCLK(1));

    /* Update System Core Clock */
    SystemCoreClockUpdate();

    SYS->GPB_MFPH =0x00660000;
    /* Lock protected registers */
    SYS_LockReg();
}


void Systick_config(void)
{
	SYS_UnlockReg();
	SysTick->CTRL=0x0;
	SysTick->LOAD=0x47999;
	SysTick->VAL=0;
	SysTick->CTRL=0x7;    			  // Core clock used for SysTick ,System Tick Interrupt Enabled ,System Tick Counter Enabled */
	SYS_LockReg();
}

void uart_init(void)
{
	CLK->APBCLK0=(1ul<<16);			  //  APBCLK for UART0
	CLK->CLKSEL1=0x03000000ul;		  //  HIRC 48MHZ
	CLK->CLKDIV0=0x0ul;               //  HIRC div 0
	UART0->FUNCSEL=0x0ul;             //  UART function mode enable
	UART0->LINE=0x3ul;                //  set data length 8bit, with no parity ,and 1 stop bit
	UART0->FIFO=0x0ul;                //  select the receiver byte level
	UART0->BAUD=0x3000019E;           //  baud rate115200

}

void Adc_init(void)
{
	CLK->CLKSEL2=0x00300000;          //  HIRC 48MHZ
	CLK->CLKDIV0=0x0;                 //  ADC Clock Divide Number From ADC Clock Source
	CLK->APBCLK0=0x1;                 //  Analog-digital-converter (ADC) Clock Enable Bit
	SYS->GPB_MFPL=0x1;                //  PB0 PIN SELECTION FOR ANALOG 0 CHANNEL
	ADC->ADCR=0xD;                    //  Continuous mode Enable
	ADC->ADCHER=0x1;                  //  Analog Input Channel Enable Control

}

void Adc_start(void)
{
	ADC->ADCR|=(0X1ul<<11);   		 // Start the conversion
}

short int adc_readvalue(void)
  {

	//while(!(ADC->ADSR1& 1UL));
    value=(short int)ADC->ADDR[0];
    return value;
  }

void ADC_IRQHandler(void)
{
	if(ADC->ADSR0&(1ul<<0)==1)
	{
		readvalue=adc_readvalue();
		printf("original value=%d\n",readvalue);
		ADC->ADCR&=~(1<<1);
	}
}

void SysTick_Handler(void)
{
	delay++;
}

void adc_char_uart(void)				/*convert the int value char */
{
	temp=readvalue;
	count=0;
	if(temp==0)

	{
	  temp=temp/10;
	  count++;
	  i=count;
	  trans_data[i]='\n';
	  trans_data[0]=temp+48;

	}
  else
    {

	 while(temp>0)   					 /*count the no digits from adc*/
       	   {
		 	 temp=temp/10;
		 	 count++;
		 	 i=count;
       	   }
	 	 trans_data[i]='\n';
	 	 i--;
	  while(readvalue>0)
	    {
		  rem=readvalue%10;
		  trans_data[i]=rem+48; 		 /*covert integer into char and stored in  the char array*/
		  readvalue=readvalue/10;
		  i--;

	   }
   }
}

void uart_transmit(void)
{
	int i=0;
	printf("transmitted data=\n");
	while((UART0->FIFOSTS&(1<<22))==0);
	for(i=0;i<=count;i++)
	{
		UART0->DAT=trans_data[i];
	}
}

int main()
{
	SYS_Init();
    uart_init();
    Adc_init();
    Adc_start();
    Systick_config();

    /* Got no where to go, just loop forever */
    while(1)
    {
    	  {
    		    	if((delay-delay1)>=1000)
    		    	{

    		    		delay1=delay;
    		    		adc_char_uart();
    		    	    uart_transmit();
                        ADC->ADCR|=(1<<1);
    		    	}

          }
   	}


}

/*** (C) COPYRIGHT 2017 Nuvoton Technology Corp. ***/
