#include <stdio.h>
#include "NuMicro.h"


void SYS_Init(void)
{

    SYS_UnlockReg();
    CLK->PWRCTL =0x4ul;
    CLK_WaitClockReady(CLK_STATUS_HIRCSTB_Msk);
    CLK_SetHCLK(CLK_CLKSEL0_HCLKSEL_HIRC, CLK_CLKDIV0_HCLK(1));

    SystemCoreClockUpdate();

    SYS_LockReg();
}

void pwm_init(void)
{
	CLK->APBCLK1=0x1<<16;
	CLK->CLKSEL2=0x1;
}

void pwm_start(void)
{
	PWM0->POEN |=1<<0;
	//pwm start//
	PWM0->CNTEN |=1<<0;

}


int main()
{
    SYS_Init();
    pwm_init();
    pwm_start();
    while(1);
}
