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
#include <math.h>
#include <stdio.h>

CY_ISR_PROTO(ISR_UART_rx_handler);
void handleByteReceived(uint8_t byteReceived);

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    char uartBuffer[256];

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    isr_uart_rx_StartEx(ISR_UART_rx_handler);
    UART_1_Start();
    ADC_SAR_1_Start();
    ADC_SAR_1_StartConvert();
    
    UART_1_PutString("Scale application started\r\n");
    while (!ADC_SAR_1_IsEndConversion(ADC_SAR_1_WAIT_FOR_RESULT)) {}
    uint16_t result = ADC_SAR_1_GetResult16();
    int avgcnt = 0;
    for(;;)
    {
        /* Place your application code here. */
        if (ADC_SAR_1_IsEndConversion(ADC_SAR_1_WAIT_FOR_RESULT)) {
            if (!SW1_Read() || avgcnt > 500) {
                double linear = 4.9310*result - 5425.5;
                snprintf(uartBuffer, sizeof(uartBuffer), "ADC result: %d, estimated weight - linear: %0.2f\r\n", result, linear);
                UART_1_PutString(uartBuffer);
                while (!SW1_Read()) {}
                result = ADC_SAR_1_GetResult16();
                avgcnt = 0;
            } else {
                result = (result + ADC_SAR_1_GetResult16()) / 2;
            }
        }
        avgcnt++;
        CyDelay(10);
    }
}

CY_ISR(ISR_UART_rx_handler)
{
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
        case 'q' :
        {
        }
        break;
        case 'w' :
        {
        }
        break;
        default :
        {
            // nothing
        }
        break;
    }
}

/* [] END OF FILE */
