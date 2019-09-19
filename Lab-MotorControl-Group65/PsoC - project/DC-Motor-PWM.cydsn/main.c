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

void setDirection(uint8 dir);
uint8 getDirection();

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
    setDirection(1);
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
    if(duty > 0) //Skriver at hastigheden bliver mindre hvis denne ikke er max.
    {
        UART_1_PutString("Decreasing speed\r\n");
    }
    if(duty >= 100) //Da max hastigheden er 101, men vi ellers bruger intervaller på 10, bruges denne funktion til at sikre at vi går til 90 og ikke 91.
    {
        PWM_1_WriteCompare(90);
        
    }
    else if(duty > 10) //trækker 10 procentpoint fra duty-cycle, hvis denne ikke er under 10.
    {
        PWM_1_WriteCompare(duty - 10);
    }
    else //Sætter duty-cycle til nul og skriver at minimum hastighed er nået.
    {
        UART_1_PutString("Min speed achieved\r\n");
        PWM_1_WriteCompare(0);
    }
}

void increaseSpeed()
{
    uint8 duty = PWM_1_ReadCompare();
    if(duty < 101) //Skriver at hastigheden bliver større hvis denne ikke er max.
    {
        UART_1_PutString("Increasing speed\r\n");
    }
    if(duty < 90) //Lægger 10 procentpoint til duty-cycle, hvis denne ikke er over 90%.
    {
        PWM_1_WriteCompare(duty + 10);
    }
    else  //Da duty-cycle ikke er helt 100% hvis man skriver 100 til Write Compare så skriver vi 101 til, når duty er over 90% for at få en duty-cycle på 100%
    {
        UART_1_PutString("Max speed achieved\r\n"); //Giver besked hvis maksimal hastighed er nået.
        PWM_1_WriteCompare(101);
    }
}

void driveForwards()
{
    UART_1_PutString("Set direction: forwards\r\n");
    if (getDirection()) { //Denne if tjækker om rettningen skal ændres og stopper moteren i 100 millisekunder med CyDelay og starter den i modsatte retning.
        PWM_1_Stop();     //Den stoppes kort for at undgå den hårde opbremsning som måtte ske, hvis man skiftede retning iden et delay.
        CyDelay(100);
        setDirection(0);
    }
    PWM_1_Start();
}

void driveBackwards()
{
    UART_1_PutString("Set direction: backwards\r\n");
    if (!getDirection()) { //Denne if tjækker om rettningen skal ændres og stopper moteren i 100 millisekunder med CyDelay og starter den i modsatte retning.
        PWM_1_Stop();      //Den stoppes kort for at undgå den hårde opbremsning som måtte ske, hvis man skiftede retning iden et delay.
        CyDelay(100);
        setDirection(1);
    }
    PWM_1_Start();
}

void stop() //PWM-signalet stoppes, så der ikke føres nogen strøm til motoren.
{
    UART_1_PutString("Stop\r\n");
    PWM_1_Stop();
}

void setDirection(uint8 dir)
{
    if(dir) //Man sætter retningen og udgangenne styres så der altid kun er et output tændt. Inputtet "dir" bestemmer retningen.
    {
    OUT2_Write(0);
    OUT1_Write(1);
    }
    else
    {
    OUT1_Write(0);
    OUT2_Write(1);
    }
}

uint8 getDirection()
{
    return OUT1_Read(); //Læser retningen, ved at se om en af de to pins forbundet til h-broen 
}
/* [] END OF FILE */
