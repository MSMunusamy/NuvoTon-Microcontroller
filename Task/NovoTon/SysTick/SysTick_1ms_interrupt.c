/**************************************************************************************
 * NAME       : Munusamy M
 * FILENAME   : SysTick_1Ms_interrupt.c
 * DESCRIPTION: Configure 1 Ms SysTick timer Interrupt
 * DATE       : 16-02-2022
***************************************************************************************/
#include <stdio.h>
#include "NuMicro.h"

void UART_Open(UART_T *uart, uint32_t u32baudrate);
void SysTick_Handler(void)
{
	uint32_t i=1;
	printf("systick value is %d",i);
	PA6=~PA6;
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

int main(int PA_MODE)
{
    SYS_Init();


    /* Init UART0 to 115200-8n1 for print message */
    UART_Open(UART0, 115200);


    /* Connect UART to PC, and open a terminal tool to receive following message */
    printf("SysTick Working\n");
    PA->MODE =4096;  // Selecting a particular pin via decimal values
      Systick();     // Function call for Systick

    /* Got no where to go, just loop forever */
    while(1);
}
