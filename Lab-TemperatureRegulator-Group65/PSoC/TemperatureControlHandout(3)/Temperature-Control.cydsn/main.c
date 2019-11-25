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
#include <stdio.h>
#include "project.h"
#include "PIDControl.h"
#include "I2Chelper.h"

static char outputBuffer[256];
static uint8_t buffer[4];
extern float setPoint;

#define SAMPLES_PER_SECOND 3
static uint16_t sampleWaitTimeInMilliseconds = 1000 / SAMPLES_PER_SECOND;

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    PWM_1_Start();
    I2C_1_Start();
    UART_1_Start();
    
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    float Kp = 2.0f;
    float Ki = 1.0f/30.0f;
    float Kd = 5.0f;
    float integralMax = 3000;
    float integralMin = -3000;
    float temp = 0;
    float controlSignal = 0;
    float proportionalPart = 0;
    float integralPart = 0;
    float derivativePart = 0;
    float error = 0;
    int consistent = 0;

    float dt = ((float)sampleWaitTimeInMilliseconds) / 1000; // dt is measured in seconds
    PIDControl_init(Kp, Ki, Kd, integralMax, integralMin, dt);
    PIDControl_changeSetPoint(30);

    UART_1_PutString("Temperature control application started\r\n");
    snprintf(outputBuffer, sizeof(outputBuffer), "Kp: %f, Ki: %f, Kd: %f\r\n", Kp, Ki, Kd);
    UART_1_PutString(outputBuffer);
    
    LED_Write(0);

    for(;;)
    {
        /* Place your application code here. */
        temp = readFromI2C(0x48, buffer, 2);
        error = setPoint - temp;
        
        
        controlSignal = PIDControl_doStep(temp, &proportionalPart, &integralPart, &derivativePart);            
        snprintf(outputBuffer, sizeof(outputBuffer), "%0.2f, %0.2f, %0.2f, %0.3f, %0.4f, %0.4f, %0.4f \r\n", 
                                                      setPoint, temp, error, controlSignal, 
                                                      proportionalPart, integralPart, derivativePart);
        UART_1_PutString(outputBuffer);
        
        PWM_1_WriteCompare(controlSignal);
        
        if(temp == setPoint) {
            consistent++;
        } else {
            consistent = 0;
        }
        if (consistent == 30) {
            LED_Write(1);
        }
        
        if (SW_Read() == 0) {
            LED_Write(0);
            PIDControl_changeSetPoint(50);
        }

        CyDelay(sampleWaitTimeInMilliseconds);
    }
}

/* [] END OF FILE */
