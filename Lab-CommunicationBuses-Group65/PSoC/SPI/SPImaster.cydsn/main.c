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
#include "SPImaster.h"
#include "../SPI_cmd.h"

CY_ISR_PROTO(ISR_UART_rx_handler);
CY_ISR_PROTO(ISR_SW_handler);
void handleByteReceived(uint8_t byteReceived);
uint8 pollSlave();
void sendString();
char buf[25];
int ptr = 0;
int polling = 0;
int numprinted = 0;

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    isr_uart_rx_StartEx(ISR_UART_rx_handler);
    isr_sw_StartEx(ISR_SW_handler);
    UART_1_Start();
    SPIM_1_Start();

    for(;;)
    {
        if (polling) {
            LED1_Write(pollSlave());
            CyDelay(100);
        }
        /* Place your application code here. */ 
    }
}

CY_ISR(ISR_SW_handler) {
    SPIM_1_WriteTxData('c');
    UART_1_PutString("Butang Clan ain't nuthin to fuck with\r\n");
    SW1_ClearInterrupt();
    UART_1_PutChar(SPIM_1_ReadRxData());
}

CY_ISR(ISR_UART_rx_handler) {
    uint8_t bytesToRead = UART_1_GetRxBufferSize();
    while (bytesToRead > 0)
    {
        uint8_t byteReceived = UART_1_ReadRxData();
        //UART_1_WriteTxData(byteReceived); // echo back
        handleByteReceived(byteReceived);
        bytesToRead--;
    }
}


void handleByteReceived(uint8_t byteReceived)
{
    switch(byteReceived)
    {
        case 13 : 
        {
            sendString();
            LED1_Write(1);
        }
        break;
        case 'p' :
        {
            polling = ~polling;
            SPIM_1_WriteTxData('2'); //Indsæt her kommandoen til at starte polling, kunne ikke lige se hvad det var
        }
        break;
        default :
        {
            buf[ptr] = byteReceived;
            LED1_Write(0);
            UART_1_PutChar(buf[ptr]);
            ptr++;            
        }
        break;
    }
    //UART_1_PutChar(ptr + 48);
}


uint8 pollSlave() {
    SPIM_1_WriteTxData(0);
    return SPIM_1_ReadRxData();
}

void sendString() {
    UART_1_PutString("\r\n");
    buf[ptr] = '\r';
    buf[ptr+1] = '\n';
    //UART_1_PutString(buf);
    for(int i = 0; buf[i] != '\n'; i++) {
        UART_1_PutChar(buf[i]);
        SPIM_1_WriteTxData(buf[i]);
    }
    UART_1_PutChar('\n');
    SPIM_1_WriteTxData('\n');
    ptr = 0;
}


/* [] END OF FILE */
