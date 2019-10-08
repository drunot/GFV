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
CY_ISR_PROTO(isr_sw_handler);
CY_ISR_PROTO(isr_timeout_handler);
static char8 CMD_buffer;
static char8 * CMD_TEXT;
uint8_t r = 1;
static uint8_t continuous = 0;
static char8 Last[MAX_CMD_LENGTH];

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    init(isr_spi_rx_handler, isr_timeout_handler, isr_sw_handler, Last); //initializes the interrupts and the top design components.

    for(;;)
    {
//        
    }
}

CY_ISR(isr_spi_rx_handler) {
    CMD_buffer = SPIS_1_ReadRxData();
    SPI_CMD_Handler(&CMD_buffer, CMD_TEXT, &continuous, Last);
    SPIS_1_ClearFIFO();
}
CY_ISR(isr_sw_handler){
    if(continuous)
    {
        poll();
    }
    SW_ClearInterrupt();
}

CY_ISR(isr_timeout_handler){
    strcpy(Last, " "); //Is added because we had problems with strings that was receved double. 
}



/* [] END OF FILE */
