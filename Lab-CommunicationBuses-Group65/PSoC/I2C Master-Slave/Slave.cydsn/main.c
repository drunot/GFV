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
uint8_t r[2] = {0,0x00};

CY_ISR(ISR_SW_handler) {
    r[0] = SW1_Read();
    SW1_ClearInterrupt();
}

int main(void)
{
    isr_sw_StartEx(ISR_SW_handler);
    CyGlobalIntEnable; /* Enable global interrupts. */
    I2C_1_Start();
    I2C_1_SetBuffer1(2,0,r);

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    for(;;)
    {
        /* Place your application code here. */
    }
}


/* [] END OF FILE */
