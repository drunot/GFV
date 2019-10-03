/* ========================================
 *
 * Copyright anton.exe 1289.elfx
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"
#include <stdio.h>
//#include dinmor

CY_ISR_PROTO(isr_spi_rx_handler);
static char CMD_buffer[25];
static uint8_t buffer_index = 0;

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    isr_spi_rx_StartEx(isr_spi_rx_handler);
    UART_1_Start();
    SPIS_1_Start();
    SPIS_1_EnableRxInt();
    SPIS_1_SetRxInterruptMode(SPIS_1_STS_RX_FIFO_NOT_EMPTY);
    LED_Write(0);

    for(;;)
    {
        
    }
}

CY_ISR(isr_spi_rx_handler) {
    
    LED_Write(~LED_Read());
    UART_1_PutChar(SPIS_1_ReadRxData());
}

/* [] END OF FILE */
