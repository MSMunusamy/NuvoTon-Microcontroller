/**************************************************************************************
 * NAME       : Munusamy M
 * FILENAME   : Timer_10_Ms.c
 * DESCRIPTION: Configure Timer for 10 Ms 
 * DATE       : 17-02-2022
***************************************************************************************/

#include <stdio.h>
#include "NuMicro.h"


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

    CLK->CLKSEL1=(CLK->CLKSEL1 &~(0x7<<8))| 0x7<<8;
    CLK->APBCLK0=(CLK->APBCLK0 &~(0x7<<8))| 0x1<<2;

    /* Update System Core Clock */
    SystemCoreClockUpdate();

    PB->MODE=0x1<<10;
    SYS->GPB_MFPL=0xE<<24;

    /* Lock protected registers */
    SYS_LockReg();
}

void TMR0_IRQHandler(void)
{
	PB5=~PB5;
	TIMER0->INTSTS &=0x1<<0;
}

void TIF(void)
{
    TIMER0->CTL|=0x1<<29;      //Timer Interrrupt Flag(TIF)
    NVIC->ISER[0]|=0x1<<8;    //Inform to cpu interrupt is occured
}

void TIMER0_Enable(void)
{
	    TIMER0->CTL|=0x1<<27;      //OP mode
	    TIMER0->CTL|=0x5;          //pre-scaler value
	    TIMER0->CMP|=80000;        //Counter value for 10 Millisecond
	    TIMER0->CTL|=0x1<<30;      //Counter start
	    TIF();
	}

int main()
{

	SYS_Init();
    TIMER0_Enable();

    /* Got no where to go, just loop forever */
    while(1);
}

