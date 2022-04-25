#include <stdio.h>
#include "NuMicro.h"

void convertion(void)
{
/*CONVERTER ENABLE*/
ADC->ADCR|=0X01<<0;

/*SINGLE MeODE SELECT*/
ADC->ADCR=(ADC->ADCR & ~(1<<10)) | (0X03<<2);

/*To select the chennelPB.2*/
ADC->ADCHER |=(0X01<<2);

/*To start the convertion*/
ADC->ADCR |= (0X01<<11);
ADC->ADSR0 |=(0X01<<0);

/* interupt disable*/
}
void current (float c)
{
uint32_t n,i;
char b[10];
if(c>1)
    {
    i=0;
    n=c*10000;
    while(n>0)
    {
    if (i==5)
    {
    b[i]='.';
    i++;
    }
    else
    {
    b[i]=n%10+48;
    n=n/10;
    i++;

    }

    }
    for(i=i-1;i>0;i--)
    {

    printf("%c",b[i]);
    }
    printf("current value is      ");

    }
    else
    {
               i=0;
         n=(c+1)*10000;
        while(n>0)
        {
        if (i==5)
        {
        b[i]='.';
        i++;
        }
        else
        {
        b[i]=n%10+48;
        n=n/10;
        i++;
        }
        }
        for(i=i-1;i>0;i--)
        {
        if(i==6)
        {
        b[i]=b[i]-1;

        }

        printf("%c",b[i]);

        }
   
    }

        }

void voltage (float a)
{
uint32_t n,i;
char b[10];
if(a>1)
    {
       i=0;
    n=a*100000;
    printf("n value is%d\n",n);
    while(n>0)
    {
    if (i==5)
    {
    b[i]='.';
    i++;
    }
    else
    {
    b[i]=n%10+48;
    n=n/10;
    i++;

    }

    }
    for(i=i-1;i>0;i--)
    {

    printf("%c",b[i]);
    }
    printf("voltage value is   ");

    }
    else
    {
        i=0;
        n=(a+1)*100000;
        while(n>0)
        {
        if (i==5)
        {
        b[i]='.';
        i++;
        }
        else
        {
        b[i]=n%10+48;
        n=n/10;
        i++;
        }
        }
        for(i=i-1;i>0;i--)
        {
        if(i==6)
        {
        b[i]=b[i]-1;

        }

        printf("%c",b[i]);

        }
        printf("voltage value is   ");

    }

}

void systic(void)
{
SysTick->LOAD =48000-1;
SysTick->VAL =0UL;
SysTick->CTRL = 7<<0;
}
