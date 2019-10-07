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
#include "SPImaster.h"
#include "project.h"

void turnOnLED() {
    SPIM_1_WriteTxData(49);
}
void turnOffLED() {
    SPIM_1_WriteTxData('2');
}

void handleByteReceived(uint8_t byteReceived, uint8_t * poller, char * buffer, uint8_t * counter)
{
    switch(byteReceived)
    {
        case 13 : 
        {
            sendString(buffer, counter);
            LED1_Write(1);
        }
        break;
        case 'p' :
        {
            *poller = ~(*poller);
            SPIM_1_WriteTxData(GetSWConst); //Indsæt her kommandoen til at starte polling, kunne ikke lige se hvad det var
        }
        break;
        default :
        {
            buffer[*counter] = byteReceived;
            LED1_Write(0);
            UART_1_PutChar(buffer[*counter]);
            (*counter)++;            
        }
        break;
    }
}


uint8 pollSlave() {
    SPIM_1_WriteTxData(0);
    uint8_t r = SPIM_1_ReadRxData();
    return r;
}

void sendString(char * buffer, uint8_t * counter) 
{
    UART_1_PutString("\r\n");
    buffer[*counter] = '\r';
    buffer[*counter+1] = '\n';
    //UART_1_PutString(buf);
    for(int i = 0; buffer[i] != '\n'; i++) {
        UART_1_PutChar(buffer[i]);
        SPIM_1_WriteTxData(buffer[i]);
    }
    UART_1_PutChar('\n');
    *counter = 0;
}
/* [] END OF FILE */
