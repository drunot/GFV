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
#include "stdio.h"



void init(void * UARTISR)
{
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    //Initializes the interrupts with the given inputs
    isr_uart_rx_StartEx(UARTISR);
    //Starts the components from topdesign.
    UART_1_Start();
    SPIM_1_Start();
}

void handleByteReceived(uint8_t byteReceived, uint8_t * poller, char * buffer, uint8_t * counter)
{
    switch(byteReceived)
    {
        case '\r' : //Sends string on return carriage receved
        {
            sendString(buffer, counter);
            LED1_Write(1);
        }
        break;
        case 'p' : //If P is pressed, polling will begin, and the switch from the slave can be read.
        {
            *poller = ~(*poller);
            SPIM_1_WriteTxData(GetSWConst); //Indsæt her kommandoen til at starte polling, kunne ikke lige se hvad det var
        }
        break;
        default : //If non of above the function saves the receved byte in the buffer
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
    SPIM_1_WriteTxData(0); //Sends an empty message in order to be able to receve.
    uint8_t r = SPIM_1_ReadRxData(); //Reads receved data. 
    char8 msg[20];
    sprintf(msg, "Knap status: %d\r\n", r); //Creates the string with the given data.
    UART_1_PutString(msg); //Sends the string over the UART.
    return r;
}

void sendString(char * buffer, uint8_t * counter)
{
    UART_1_PutString("\r\n"); //New line, carriage return.
    buffer[*counter] = '\r'; //Sets end of buffer to carriage return.
    buffer[*counter+1] = 0; //Sets end of buffer to end of string char.
    //UART_1_PutString(buffer); //Sends the string to the UART.
    for(int i = 0; buffer[i] != 0; i++) {
        SPIM_1_WriteTxData(buffer[i]); //Sends the string one byte at the time over SPI
        UART_1_PutChar(buffer[i]);
    }
    UART_1_PutChar('\n'); //New line in the UART after done sending.
    *counter = 0; //Resets the buffer counter.
}

void UARTHandler(uint8_t * poller, char * buffer, uint8_t * counter)
{
    uint8_t bytesToRead = UART_1_GetRxBufferSize(); //Reads number of bytes to read.
    while (bytesToRead > 0) //Handles each byte one at the time.
    {
        uint8_t byteReceived = UART_1_ReadRxData();
        handleByteReceived(byteReceived, poller, buffer, counter);
        bytesToRead--;
    }
}
/* [] END OF FILE */
