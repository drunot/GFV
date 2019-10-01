/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"
#include "I2CHelper.h"



int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    I2C_1_Start();
    UART_1_Start();
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    for(;;)
    {
        uint8_t DataToRead[2];
        I2C_1_MasterReadBuf(0x48,DataToRead,2, I2C_1_MODE_COMPLETE_XFER);
        char8 printString[30];
        GetStringToPrint(printString,DataToRead);
        UART_1_PutString(printString);
        CyDelay(100);
        
        /* Place your application code here. */
    }
}




/* [] END OF FILE */
