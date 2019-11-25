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
#include "I2CHelper.h"
#include "PIDControl.h"
#include "UARTHelper.h"
#include <stdio.h>


#define BYTE_SIZE 50
# define BUF_SIZE 256
static char byteReceived[BYTE_SIZE];
static uint8 currPlace = 0;

static char outputBuffer[BUF_SIZE] = {0};
static uint8_t tempBuf [2] = {0};
static uint8_t run = 0;
CY_ISR_PROTO(ISR_UART_rx_handler);

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    I2C_1_Start();
    UART_1_Start();
    PWM_1_Start();
    PIDControl_init();
    UART_RX_isr_StartEx(ISR_UART_rx_handler);
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    
    printHelp();
    printSettings(outputBuffer,sizeof(outputBuffer));
    
    float controlSignal = 0.0;
    float temp = 0;
    float error = 0.0;
    float proportionalPart = 0.0;
    float integralPart = 0.0;      
    float derivativePart = 0.0;

    PIDControl_init();
    for(;;)
    {
        if (run){
            temp = readFromI2C(0x48, tempBuf, 2);
            error = PID_settings_t.Target - temp;
            strRep(outputBuffer, '.',','); 
    
            controlSignal = PIDControl_doStep(temp, &proportionalPart, &integralPart, &derivativePart); //Gets PWM compare.
            PWM_1_WriteCompare(controlSignal); //Sets PWM compare.
            snprintf(outputBuffer, sizeof(outputBuffer), "%.1f;%.1f;%.1f;%.3f;%.3f;%.3f;%.3f\r\n", 
                                                          PID_settings_t.Target, temp, error, controlSignal, 
                                                          proportionalPart, integralPart, derivativePart); //Semicolon sepperated for standard dansih ".csv" 
            strRep(outputBuffer, '.',','); // dot changed to comma for standard dansih excel decimal seperator.
            
            UART_1_PutString(outputBuffer); //Writes current status.
            
            if(temp >= PID_settings_t.Target -0.5 && temp <= PID_settings_t.Target + 0.5) //If temp is reaced turn on LED.
            {   
                run++;
            }
            else //Else not there yet
            {
                PinLED_Write(0);
                run = 1;
            }
            if (run >= samples_to_stable){  //Stable at current temp
                run = 0;
                PinLED_Write(1);
            }
            CyDelay(sampleWaitTimeInMilliseconds());
        }
        /* Place your application code here. */
    }
}



CY_ISR(ISR_UART_rx_handler)
{
    uint8_t bytesToRead = UART_1_GetRxBufferSize(); 
    
    while (bytesToRead--)
    {
        byteReceived[currPlace++] = UART_1_ReadRxData();   
        byteReceived[currPlace] = '\0';
    }
    
    if(strchr((char*)byteReceived, 'k'))
    {
        
        handleByteReceived(byteReceived, &currPlace, &run, outputBuffer, sizeof(outputBuffer));
        currPlace = 0;
        for(size_t i = 0; i < sizeof(byteReceived) && byteReceived[i] != 0; i++)
        {
            byteReceived[i] = 0;
        }

    }
}

/* [] END OF FILE */
