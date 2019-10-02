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

CY_ISR_PROTO(ISR_UART_rx_handler);
CY_ISR_PROTO(ISR_SW_handler);
void handleByteReceived(uint8_t byteReceived);

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    isr_uart_rx_StartEx(ISR_UART_rx_handler);
    isr_sw_StartEx(ISR_SW_handler);
    UART_1_Start();
    SPIM_1_Start();
    Sel_Write(1);

    for(;;)
    {
        /* Place your application code here. */
    }
}

CY_ISR(ISR_SW_handler) {
    SPIM_1_WriteTxData(SW1_Read());
    UART_1_PutString("Butang Clan ain't nuthin to fuck with\r\n");
    SW1_ClearInterrupt();
}

CY_ISR(ISR_UART_rx_handler) {
    uint8_t bytesToRead = UART_1_GetRxBufferSize();
    while (bytesToRead > 0)
    {
        uint8_t byteReceived = UART_1_ReadRxData();
        UART_1_WriteTxData(byteReceived); // echo back
        
        handleByteReceived(byteReceived);
        
        bytesToRead--;
    }
}

void handleByteReceived(uint8_t byteReceived)
{
    switch(byteReceived)
    {
        case '1' :
        {
            //tænd lampe
            turnOnLED();
            break;
        }
        case '2' :
        {
            //sluk lampe
            turnOffLED();
            break;
        }
        default :
        {
            // nothing
        }
        break;
    }
}

/* [] END OF FILE */
