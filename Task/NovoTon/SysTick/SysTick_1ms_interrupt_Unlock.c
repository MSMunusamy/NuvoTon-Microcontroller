/**************************************************************************************
 * NAME       : Munusamy M
 * FILENAME   : Systick.c
 * DESCRIPTION: Configure 1 Ms SysTick timer Interrupt
 * DATE       : 16-02-2022
***************************************************************************************/
#include <stdio.h>
#include "numicro.h"
#include "Systick.h"
void Systick()
{
	SysTick-> LOAD = 48000-1;   //Given formula calculated value 1 Sec Delay
	SysTick-> CTRL = 7;         //CLKSRC and TICKINT and ENABLE Selecting in SYST_CTRL register value.
	SysTick-> VAL  = 0;         //I don't want any current counter value
}
