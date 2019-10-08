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

void SPI_CMD_Handler(char8* buffer, char8 * text, uint8_t * conBool, char8* last){
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

/* [] END OF FILE */
