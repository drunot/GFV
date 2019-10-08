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
#include "../SPI_cmd.h"
#include "SPI_slave.h"
#include "string.h"

void SPI_CMD_Handler(char8* buffer, char8 * text, uint8_t * conBool, char8* last){ //Chehcks witch cmd from SPI_cmd.h that was receved and acts upon it.
    if(*buffer == TurnOffLed){
        LED_Write(0);
        text = TurnOffLedTEXT; 
    }
            
    else if(*buffer == TurnOnLed){
        LED_Write(1);
        text = TurnOnLedTEXT;
    }
            
    else if(*buffer == GetSWStatus){
        SPIS_1_WriteTxData(SW_Read());
        text = GetSWStatusTEXT;
    }
        
    else if(*buffer == GetSWConst){
        *conBool = 1;   
        text = GetSWConstTEXT;
    }
        
    else if(*buffer == StopSWConst){
        *conBool = 0;
        text = StopSWConstTEXT;        
    }  
    else
    {
        text = 0;
    }
    if(text != 0 && strcmp(text, last))
    {
        UART_1_PutString(text);
        UART_1_PutString("\n\r");
        strcpy(last, text);
        text = 0;
        *buffer = 0;
        Timer_1_WriteCounter(0);
    }
}

void init(void * isr_rx, void * isr_timeout, void * isr_switch,  char8 * last) //Interrupts are passed as pointers to voids.
{
    isr_spi_rx_StartEx(isr_rx);
    isr_timeout_StartEx(isr_timeout);
    isr_sw_StartEx(isr_switch);
    UART_1_Start();
    UART_1_PutString(StartUpTEXT);
    UART_1_PutString("\r\n");
    SPIS_1_Start();
    SPIS_1_EnableRxInt();
    SPIS_1_SetRxInterruptMode(SPIS_1_STS_RX_FIFO_NOT_EMPTY);
    LED_Write(1);   
    strcpy(last, " ");
    Timer_1_Start();
    
}

void poll(void)
{
            SPIS_1_WriteTxData(SW_Read());
            SPIS_1_ClearTxBuffer();
}

/* [] END OF FILE */
