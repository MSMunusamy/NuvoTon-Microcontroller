
#include <stdio.h>
#include "NuMicro.h"


#define RED "\e[0;31m"
#define GRN "\e[0;32m"
#define BLU "\e[0;34m"
#define YEL "\e[0;33m"

unsigned int read,read1,read2,read3,read4,read5,read6,read7,read8;
void SYS_Init(void)
{
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init System Clock                                                                                       */
    /*---------------------------------------------------------------------------------------------------------*/

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

    /* Enable I2C0 clock */
    CLK_EnableModuleClock(I2C0_MODULE);

    /* Update System Core Clock */
    /* User can use SystemCoreClockUpdate() to calculate SystemCoreClock and cyclesPerUs automatically. */
    SystemCoreClockUpdate();

    /*---------------------------------------------------------------------------------------------------------*/
    /* Init I/O Multi-function                                                                                 */
    /*---------------------------------------------------------------------------------------------------------*/
    /* Set PB multi-function pins for UART0 RXD=PB.12 and TXD=PB.13 */
    SYS->GPB_MFPH = (SYS->GPB_MFPH & ~(SYS_GPB_MFPH_PB12MFP_Msk | SYS_GPB_MFPH_PB13MFP_Msk)) |
                    (SYS_GPB_MFPH_PB12MFP_UART0_RXD | SYS_GPB_MFPH_PB13MFP_UART0_TXD);

    /* Set I2C0 multi-function pins */
    SYS->GPB_MFPL = (SYS->GPB_MFPL & ~(SYS_GPB_MFPL_PB4MFP_Msk | SYS_GPB_MFPL_PB5MFP_Msk)) |
                    (SYS_GPB_MFPL_PB4MFP_I2C0_SDA | SYS_GPB_MFPL_PB5MFP_I2C0_SCL);

    /* Lock protected registers */
    SYS_LockReg();
}

uint8_t I2C_ReadByteOneReg(I2C_T *i2c, uint8_t u8SlaveAddr, uint8_t u8DataAddr)
{
    uint8_t u8Xfering = 1U, u8Err = 0U, rdata = 0U, u8Ctrl = 0U;

	   uint32_t i;

    I2C_START(i2c);                                          /* Send START */

    while (u8Xfering && (u8Err == 0U))
    {
        I2C_WAIT_READY(i2c) {}

        switch (I2C_GET_STATUS(i2c))
        {
        case 0x08:
            I2C_SET_DATA(i2c, (uint8_t)(u8SlaveAddr << 1U | 0x00U));    /* Write SLA+W to Register I2CDAT */
            u8Ctrl = I2C_CTL_SI;                             /* Clear SI */
            break;

        case 0x18:                                           /* Slave Address ACK */
            I2C_SET_DATA(i2c, u8DataAddr);                   /* Write Lo byte address of register */
            break;

        case 0x20:                                           /* Slave Address NACK */
        case 0x30:                                           /* Master transmit data NACK */
            u8Ctrl = I2C_CTL_STO_SI;                         /* Clear SI and send STOP */
            u8Err = 1U;
            break;

        case 0x28:
            u8Ctrl = I2C_CTL_STA_SI;                         /* Send repeat START */
            break;

        case 0x10:
            I2C_SET_DATA(i2c, (uint8_t)((u8SlaveAddr << 1U) | 0x01U));  /* Write SLA+R to Register I2CDAT */
            u8Ctrl = I2C_CTL_SI;                             /* Clear SI */
            break;

        case 0x40:                                           /* Slave Address ACK */
            u8Ctrl = I2C_CTL_SI;                             /* Clear SI */
            break;

        case 0x48:                                           /* Slave Address NACK */
            u8Ctrl = I2C_CTL_STO_SI;                         /* Clear SI and send STOP */
            u8Err = 1U;
            break;

        case 0x58:
					for(i=0;i<8;i++)
            {
							rdata = (uint8_t) I2C_GET_DATA(i2c);             /* Receive Data */
						}
						u8Ctrl = I2C_CTL_STO_SI;                         /* Clear SI and send STOP */
            u8Xfering = 0U;
            break;

        case 0x38:                                           /* Arbitration Lost */
        default:                                             /* Unknow status */
            I2C_SET_CONTROL_REG(i2c, I2C_CTL_STO_SI);        /* Clear SI and send STOP */
            u8Ctrl = I2C_CTL_SI;
            u8Err = 1U;
            break;
        }

        I2C_SET_CONTROL_REG(i2c, u8Ctrl);                    /* Write controlbit to I2C_CTL register */
    }

    if (u8Err)
        rdata = 0U;                                           /* If occurs error, return 0 */

    return rdata;                                            /* Return read data */
}



uint8_t I2C_WriteByteOneReg(I2C_T *i2c, uint8_t u8SlaveAddr, uint8_t u8DataAddr, uint8_t data)
{
    uint8_t u8Xfering = 1U, u8Err = 0U, u8Ctrl = 0U;
    uint32_t u32txLen = 0U;

    I2C_START(i2c);                                        /* Send START */

    while (u8Xfering && (u8Err == 0U))
    {
        I2C_WAIT_READY(i2c) {}

        switch (I2C_GET_STATUS(i2c))
        {
        case 0x08:
            I2C_SET_DATA(i2c, (uint8_t)(u8SlaveAddr << 1U | 0x00U));  /* Send Slave address with write bit */
            u8Ctrl = I2C_CTL_SI;                           /* Clear SI */
            break;

        case 0x18:                                         /* Slave Address ACK */
            I2C_SET_DATA(i2c, u8DataAddr);                 /* Write Lo byte address of register */
            break;

        case 0x20:                                         /* Slave Address NACK */
        case 0x30:                                         /* Master transmit data NACK */
            u8Ctrl = I2C_CTL_STO_SI;                       /* Clear SI and send STOP */
            u8Err = 1U;
            break;

        case 0x28:
            if (u32txLen < 1U)
            {
                I2C_SET_DATA(i2c, data);
                u32txLen++;
            }
            else
            {
                u8Ctrl = I2C_CTL_STO_SI;                   /* Clear SI and send STOP */
                u8Xfering = 0U;
            }

            break;

        case 0x38:                                         /* Arbitration Lost */
        default:                                           /* Unknow status */
            I2C_SET_CONTROL_REG(i2c, I2C_CTL_STO_SI);      /* Clear SI and send STOP */
            u8Ctrl = I2C_CTL_SI;
            u8Err = 1U;
            break;
        }

        I2C_SET_CONTROL_REG(i2c, u8Ctrl);                  /* Write controlbit to I2C_CTL register */
    }

    return (u8Err | u8Xfering);                            /* return (Success)/(Fail) status */
}
uint32_t I2C_Open(I2C_T *i2c, uint32_t u32BusClock)
{
    uint32_t u32Div;
    uint32_t u32Pclk;

    if (i2c == I2C1)
    {
        u32Pclk = CLK_GetPCLK1Freq();
    }
    else
    {
        u32Pclk = CLK_GetPCLK0Freq();
    }

    u32Div = (uint32_t)(((u32Pclk * 10U) / (u32BusClock * 4U) + 5U) / 10U - 1U); /* Compute proper divider for I2C clock */
    i2c->CLKDIV = u32Div;

    /* Enable I2C */
    i2c->CTL0 |= I2C_CTL0_I2CEN_Msk;

    return (u32Pclk / ((u32Div + 1U) << 2U));
}

void I2C0_Init(void)
{
    /* Open I2C module and set bus clock */
    I2C_Open(I2C0, 100000);
}

void I2C0_Close(void)
{
       /* Disable I2C0 and close I2C0 clock */
    I2C_Close(I2C0);
    CLK_DisableModuleClock(I2C0_MODULE);

}

int main()
{
    uint32_t i;

    SYS_Init();

    /* Init UART0 to 115200-8n1 for print message */
    UART_Open(UART0, 115200);

     /* Init I2C0 */

    I2C0_Init();
	/*0x29 => is a slave address 0x80 => Must set a command value regaring the register if wont be set it wont work
	Every time we should mention a slave address because here passing via function with argument
	Here Writing a some specific value to given offset or address based on that it will perform */

	I2C_WriteByteOneReg(I2C0,0x29,0x80 | 0x00,0x03);                      // Power on =>PON RGBC Enable=> 0x00 is offset 0x03 is value;
	I2C_WriteByteOneReg(I2C0,0x29,0x80 | 0x01,0xD5);                      // Selecting ATIME value     => 0x01 is offset 0xD5 is value;
	I2C_WriteByteOneReg(I2C0,0x29,0x80 | 0x03,0xFF);                      // Selecting WTIME value     => 0x03 is offset 0xFF is value;
	I2C_WriteByteOneReg(I2C0,0x29,0x80 | 0x13,0x01);                      // Status AVALID Register    => 0x13 is offset 0x01 is value;

	/*0x29 => is a slave address 0x80 => Must set a command value regaring the register if wont be set it wont work
	Every time we should mention a slave address because here passing via function with argument
	Here Reading a address in that address have some value it will display via Terminal*/
	while(1)
	{
		read  = I2C_ReadByteOneReg(I2C0,0x29,0x80 | 0x12);                    // Device ID
		read1 = I2C_ReadByteOneReg(I2C0,0x29,0x80 | 0x14);                    // Clear data low byte
		read2 = I2C_ReadByteOneReg(I2C0,0x29,0x80 | 0x15);                    // Clear data high byte
		read3 = I2C_ReadByteOneReg(I2C0,0x29,0x80 | 0x16);                    // Red data low byte
		read4 = I2C_ReadByteOneReg(I2C0,0x29,0x80 | 0x17);                    // Red data high byte
		read5 = I2C_ReadByteOneReg(I2C0,0x29,0x80 | 0x18);                    // Green data low byte
		read6 = I2C_ReadByteOneReg(I2C0,0x29,0x80 | 0x19);                    // Green data high byte
		read7 = I2C_ReadByteOneReg(I2C0,0x29,0x80 | 0x1A);                    // Blue data low byte
		read8 = I2C_ReadByteOneReg(I2C0,0x29,0x80 | 0x1B);                    // Blue data high byte

		printf("Sensor ID is %x\n",read);
		printf("The Sensor value is Clear %X\n",read1);
		//printf("The Sensor value is %d\n",read2);
		printf("The Sensor value is RED %X\n",read3);
		//printf("The Sensor value is %d\n",read4);
		printf("The Sensor value is GREEN %X\n",read5);
		//printf("The Sensor value is %d\n",read6);
		printf("The Sensor value is BLUE %X\n",read7);
		//printf("The Sensor value is %d\n",read8);

	}
while(1);
}
