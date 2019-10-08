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
#include "SPI_slave.h"
#include "../SPI_cmd.h"




CY_ISR_PROTO(isr_spi_rx_handler);
CY_ISR_PROTO(isr_timeout_handler);
static char8 CMD_buffer;
static char8 * CMD_TEXT;
static uint8_t continuous = 0;
static char8 Last[MAX_CMD_LENGTH];

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    isr_spi_rx_StartEx(isr_spi_rx_handler);
    isr_timeout_StartEx(isr_timeout_handler);
    UART_1_Start();
    UART_1_PutString(StartUpTEXT);
    UART_1_PutString("\r\n");
    SPIS_1_Start();
    SPIS_1_EnableRxInt();
    SPIS_1_SetRxInterruptMode(SPIS_1_STS_RX_FIFO_NOT_EMPTY);
    LED_Write(1);   
    strcpy(Last, " ");
    Timer_1_Start();

    

    for(;;)
    {
        if(continuous){
            SPIS_1_WriteTxData(SW_Read());
            SPIS_1_ClearTxBuffer();
        }
    }
}

CY_ISR(isr_spi_rx_handler) {
    CMD_buffer = SPIS_1_ReadRxData();
    SPI_CMD_Handler(&CMD_buffer, CMD_TEXT, &continuous, Last);
    SPIS_1_ClearFIFO();
}

CY_ISR(isr_timeout_handler){
    strcpy(Last, " ");
}
/* [] END OF FILE */
