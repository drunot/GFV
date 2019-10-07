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

char* SHIT = "OHMYFUXKINGGODGLOCKDIGSELV";
char* DoNothingTEXT = "Do nothing";
char* TurnOffLedTEXT = "Turn Led Off";
char* TurnOnLedTEXT = "Turn Led On";
char* GetSWConstTEXT = "Constant Send SW";
char* StopSWConstTEXT = "Stop Constant Send";
char* GetSWStatusTEXT = "Send SW status";


CY_ISR_PROTO(isr_spi_rx_handler);
void SPI_CMD_Handler(void);
static volatile char CMD_buffer;
static char* CMD_TEXT;
static uint8_t continuous = 0;

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    isr_spi_rx_StartEx(isr_spi_rx_handler);
    UART_1_Start();
    UART_1_PutString(SHIT);
    UART_1_PutString("\r\n");
    SPIS_1_Start();
    SPIS_1_EnableRxInt();
    SPIS_1_SetRxInterruptMode(SPIS_1_STS_RX_FIFO_NOT_EMPTY);
    LED_Write(1);        
    

    

    for(;;)
    {
        SPIS_1_WriteTxData(SW_Read());
        /*if(continuous){
            SPIS_1_ClearTxBuffer();
        }*/
    }
}

CY_ISR(isr_spi_rx_handler) {
    //if (CMD_buffer[buffer_index] != CharTerminator)
    //CMD_buffer = SPIS_1_ReadRxData();
    //LED_Write(~LED_Read());
    UART_1_PutChar(SPIS_1_ReadRxData());
    CMD_buffer = SPIS_1_ReadRxData();
    //char msg[30];
    //sprintf(msg, "%d", SPIS_1_ReadRxData());
    //UART_1_PutString(msg);
    SPI_CMD_Handler();
    SPIS_1_ClearFIFO();
}

void SPI_CMD_Handler(void){
    if(CMD_buffer == TurnOffLed){
        LED_Write(0);
        CMD_TEXT = TurnOffLedTEXT;
    }
            
    else if(CMD_buffer == TurnOnLed){
        LED_Write(1);
        CMD_TEXT = TurnOnLedTEXT;
    }
            
    else if(CMD_buffer == GetSWStatus){
        SPIS_1_WriteTxData(SW_Read());
        CMD_TEXT = GetSWStatusTEXT;
    }
        
    else if(CMD_buffer == GetSWConst){
        continuous = 1;   
        CMD_TEXT = GetSWConstTEXT;
    }
        
    else if(CMD_buffer == StopSWConst){
        continuous = 0;
        CMD_TEXT = StopSWConstTEXT;        
    }  
    else
        CMD_TEXT = SHIT;
    UART_1_PutString(CMD_TEXT);
    UART_1_PutString("\r\n");
}
/* [] END OF FILE */
