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
#include "../SPI_cmd.h"
//#include dinmor

CY_ISR_PROTO(isr_spi_rx_handler);
void SPI_CMD_Handler(void);
static char CMD_buffer[MAX_CMD_LENGTH];
static uint8_t buffer_index = 0;
static uint8_t continuous = 0;

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
        UART_1_PutChar(CMD_buffer[buffer_index]);
        UART_1_PutString("\r\n");
        if(continuous){
            SPIS_1_WriteTxData(SW_Read());
            SPIS_1_ClearTxBuffer();
        }
    }
}

CY_ISR(isr_spi_rx_handler) {
    CMD_buffer[buffer_index] = SPIS_1_ReadRxData();
    //if (CMD_buffer[buffer_index] != CharTerminator)
    buffer_index = 0;
    SPIS_1_ClearFIFO();
}

void SPI_CMD_Handler(void){
    if(!strncmp(CMD_buffer, TurnOffLed, sizeof(TurnOffLed))){
        LED_Write(0);
    }
            
    else if(!strncmp(CMD_buffer, TurnOnLed, sizeof(TurnOnLed))){
        LED_Write(1);
    }
            
    else if(!strncmp(CMD_buffer, GetSWStatus, sizeof(GetSWStatus))){
        SPIS_1_WriteTxData(SW_Read());
    }
        
    else if(!strncasecmp(CMD_buffer, GetSWConst, sizeof(GetSWConst))){
        continuous = 1;   
    }
        
    else if(!strncasecmp(CMD_buffer, StopSWConst, sizeof(StopSWConst))){
        continuous = 0;
        
    }  
    UART_1_PutString(CMD_buffer);
    buffer_index = 0;
}
/* [] END OF FILE */
