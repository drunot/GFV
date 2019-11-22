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
#include <stdio.h>

#define SAMPLES_PER_SECOND 3
static uint16_t sampleWaitTimeInMilliseconds = 1000 / SAMPLES_PER_SECOND;
static float setPoint = 50;
static char outputBuffer[256] = {0};
static uint8_t tempBuf [2] = {0};

void strRep(char * buf, char find, char replace);


int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    I2C_1_Start();
    UART_1_Start();
    PWM_1_Start();
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    float Kp = 2.0f;
    float Ki = 1.0f/10.0f;
    float Kd = 1.0f/10.0f;
    float integralMax = 3000;
    float integralMin = -3000;
    float temp = 0;
    float controlSignal = 0;

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
        CyDelay(sampleWaitTimeInMilliseconds);
        /* Place your application code here. */
    }
}

void strRep(char * buf, char find, char replace)
{
    uint8_t i = 0;
    while(buf[i] != 0)
    {
        if(buf[i] == find)
        {
            buf[i] = replace;
        }
        i++;
    }
}


/* [] END OF FILE */
