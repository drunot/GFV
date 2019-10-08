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
char buf[MAX_BUF_LENGTH];
uint8_t ptr = 0;
uint8_t polling = 0;
int numprinted = 0;

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    init(ISR_UART_rx_handler);

    for(;;)
    {
        if (polling) {
            LED1_Write(pollSlave());
            CyDelay(100);
        }
        /* Place your application code here. */ 
    }
}


CY_ISR(ISR_UART_rx_handler) {
    
    UARTHandler(&polling, buf, &ptr);
}


/* [] END OF FILE */
