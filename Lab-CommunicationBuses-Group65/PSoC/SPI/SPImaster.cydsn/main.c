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
char buf[25];
uint8_t ptr = 0;
uint8_t polling = 0;
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
    SW1_ClearInterrupt();
    UART_1_PutChar(SPIM_1_ReadRxData());
}

CY_ISR(ISR_UART_rx_handler) {
    uint8_t bytesToRead = UART_1_GetRxBufferSize();
    while (bytesToRead > 0)
    {
        uint8_t byteReceived = UART_1_ReadRxData();
        handleByteReceived(byteReceived, &polling, buf, &ptr);
        //UART_1_WriteTxData(byteReceived); // echo back
        bytesToRead--;
    }
}


/* [] END OF FILE */
