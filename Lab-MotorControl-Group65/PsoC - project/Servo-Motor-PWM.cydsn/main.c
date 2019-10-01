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
#include "stdio.h"
#define forwards 0u
#define backwards 1u

CY_ISR_PROTO(ISR_UART_rx_handler);
void handleByteReceived(uint8_t byteReceived);
void turnLeft();
void turnRight();
void cycle();

void setDirection(uint8 dir);
uint8 getDirection();
uint8 left;

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    isr_uart_rx_StartEx(ISR_UART_rx_handler);
    UART_1_Start();
    PWM_1_Start();
    //Output til at fortælle hvordan systemet bruges
    UART_1_PutString("Servo-Motor-PWM application started\r\n");
    UART_1_PutString("a: Turn Left\r\n");
    UART_1_PutString("d: Turn Right\r\n");

    for(;;) //Efterladt tom for at fange systemet, men alt arbejdes med i ISR
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

void handleByteReceived(uint8_t byteReceived) //Switch-case til at finde indputtet
{
    switch(byteReceived)
    {
        case 'a' :
        {
            turnLeft();
        }
        break;
        case 'd' :
        {
            turnRight();
        }
        break;
        case ' ' :
        {
            cycle();
        }
        default :
        {
            // nothing
        }
        break;
    }
}

void cycle() {
    if (left) {
        turnLeft();
    } else {
        turnRight();
    }
}

void turnLeft() {
    uint16 deg = PWM_1_ReadCompare();
    UART_1_PutString("Turning left\r\n");
    if (deg > 7000) { //Hvis systemet er over de 10% dutycycle, retter signalet ind til at være 10%.
        PWM_1_WriteCompare(7000);
    } else if (deg > 1600) { //Hvis signalet kan få trukket 100 fra sig uden at falde under 5%, gør det
        PWM_1_WriteCompare(deg - 100);
    } else { //Ellers ret det til 5%, og skriv det ikke kan komme lavere
        UART_1_PutString("All the way left\r\n");
        PWM_1_WriteCompare(1500);
        left = 0;
    }
    char buffer[25];
    sprintf(buffer, "Current: %d\r\n", deg - 100);
    UART_1_PutString(buffer);
}

void turnRight() { //Er funktionelt meget ens den ovenstående, men omvendt.
    uint16 deg = PWM_1_ReadCompare();
    UART_1_PutString("Turning right\r\n");
    if (deg < 1500) { 
        PWM_1_WriteCompare(1500);
    } else if (deg < 6900) { 
        PWM_1_WriteCompare(deg + 100);
    } else {
        UART_1_PutString("All the way right\r\n");
        PWM_1_WriteCompare(7000);
        left = 1;
    }
    char buffer[25];
    sprintf(buffer, "Current: %d\r\n", deg + 100);
    UART_1_PutString(buffer);
}

/* [] END OF FILE */
