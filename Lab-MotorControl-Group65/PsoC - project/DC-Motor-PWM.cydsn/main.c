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
#define forwards 0u
#define backwards 1u

CY_ISR_PROTO(ISR_UART_rx_handler);
void handleByteReceived(uint8_t byteReceived);
void decreaseSpeed(void);
void increaseSpeed(void);
void driveForwards(void);
void driveBackwards(void);
void stop(void);

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    isr_uart_rx_StartEx(ISR_UART_rx_handler);
    UART_1_Start();
    PWM_1_Start();
    
    UART_1_PutString("DC-Motor-PWM application started\r\n");
    UART_1_PutString("0: Stop\r\n");
    UART_1_PutString("1: Drive forwards\r\n");
    UART_1_PutString("2: Drive backwards\r\n");
    UART_1_PutString("q: Decrease speed\r\n");
    UART_1_PutString("w: Increase speed\r\n");

    for(;;)
    {
        /* Place your application code here. */
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
            decreaseSpeed();
        }
        break;
        case 'w' :
        {
            increaseSpeed();
        }
        break;
        case '1' :
        {
            driveForwards();
        }
        break;
        case '2' :
        {
            driveBackwards();
        }
        break;
        case '0' :
        {
            stop();
            
        }
        break;
        default :
        {
            // nothing
        }
        break;
    }
}

void decreaseSpeed()
{
    uint8 duty = PWM_1_ReadCompare();
    if(duty > 0)
    {
        UART_1_PutString("Decreasing speed\r\n");
    }
    if(duty >= 100)
    {
        PWM_1_WriteCompare(90);
        
    }
    else if(duty > 10)
    {
        PWM_1_WriteCompare(duty - 10);
    }
    else
    {
        UART_1_PutString("Min speed achieved\r\n");
        PWM_1_WriteCompare(0);
    }
}

void increaseSpeed()
{
    uint8 duty = PWM_1_ReadCompare();
    if(duty < 101)
    {
        UART_1_PutString("Increasing speed\r\n");
    }
    if(duty < 90)
    {
        PWM_1_WriteCompare(duty + 10);
    }
    else
    {
        UART_1_PutString("Max speed achieved\r\n");
        PWM_1_WriteCompare(101);
    }
}

void driveForwards()
{
    UART_1_PutString("Set direction: forwards\r\n");
    if (!OUT1_Read()) {
        OUT2_Write(0);
        OUT1_Write(1);
    }
}

void driveBackwards()
{
    UART_1_PutString("Set direction: backwards\r\n");
    if (!OUT2_Read()) {
        OUT1_Write(0);
        OUT2_Write(1);
    }
}

void stop()
{
    UART_1_PutString("Stop\r\n");
    PWM_1_Stop();
}
/* [] END OF FILE */
