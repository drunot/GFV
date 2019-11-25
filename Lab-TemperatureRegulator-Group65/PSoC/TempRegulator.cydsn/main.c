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

    float dt = ((float)sampleWaitTimeInMilliseconds) / 1000; // dt is measured in seconds
    PIDControl_init(Kp, Ki, Kd, integralMax, integralMin, dt);
    PIDControl_changeSetPoint(setPoint);
    sprintf(outputBuffer,"Kp:;%.3f;Ki:;%.3f;Kd:;%.3f\r\n",Kp,Ki,Kd);
    strRep(outputBuffer, '.',',');
    UART_1_PutString(outputBuffer);
    UART_1_PutString("Taget:;Current:;Delta:;PWM:;pp:;ip:;dp:\r\n");
    PinLED_Write(0);
    for(;;)
    {
        temp = readFromI2C(0x48, tempBuf, 2);
        float error = setPoint - temp;
        float proportionalPart = 0;
        float integralPart = 0;
        float derivativePart = 0;
        
        controlSignal = PIDControl_doStep(temp, &proportionalPart, &integralPart, &derivativePart);
        PWM_1_WriteCompare(controlSignal);
        snprintf(outputBuffer, sizeof(outputBuffer), "%.1f;%.1f;%.1f;%.3f;%.3f;%.3f;%.3f\r\n", 
                                                      setPoint, temp, error, controlSignal, 
                                                      proportionalPart, integralPart, derivativePart);
        strRep(outputBuffer, '.',',');
        
        UART_1_PutString(outputBuffer);
        if(temp == setPoint)
        {
            PinLED_Write(1);
        }
        else
        {
            PinLED_Write(0);
        }
        /* Place your application code here. */
    }
}

void strRep(char * buf, char find, char replace)
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
