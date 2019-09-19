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
#include <stdio.h>
#define forwards 0u
#define backwards 1u

CY_ISR_PROTO(ISR_UART_rx_handler);
void handleByteReceived(uint8_t byteReceived);
void decreaseSpeed(void);
void increaseSpeed(void);
void changeDirection();
void rotate(uint8_t dir);
void start(void);
void stop(void);
void changeMode(void);
void printMenu(void);
void one_step(void);

#define ARRAYLENGTH 8
#define WAVE_DRIVE "Wave_drive\r\n"
#define FULL_STEP "Full_step\r\n"
#define HALF_STEP "Half_step\r\n"
#define ONE_ROT_STEPS 2038

//Initialize the three step modes
uint8_t mode[][ARRAYLENGTH] = {
    //Pin output sequence for the waved-drive mode (repeated 2 times)
    {0b00000001, 0b00000010, 0b00000100, 0b00001000,
     0b00000001, 0b00000010, 0b00000100, 0b00001000},
    
    //Pin output sequence for the full-step mode (repeated 2 times)
    {0b00000011, 0b00000110, 0b00001100, 0b00001001,
     0b00000011, 0b00000110, 0b00001100, 0b00001001},

    //Pin output sequence for the half-step mode
    {0b00000001, 0b00000011, 0b00000010, 0b00000110,
     0b00000100, 0b00001100, 0b00001000, 0b00001001}
};

//Mode selelction variable
uint8 modeSelect = 0;

//Direction definitions and initialization
#define FORWARD_ 1
#define BACKWARD_ -1
int8_t direction = FORWARD_;

//Current step iterator
int8_t curr_step = 0;
int16_t delay = 2;
uint8_t on = 0;
uint16_t one_rot_var = 0;

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    
    
    isr_uart_rx_StartEx(ISR_UART_rx_handler);
    UART_1_Start();
    
    printMenu();

    for(;;)
    {
        if(on){
            one_step();
        }
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
        case 'r' :
        {
            rotate(1);
        }
        break;
        case 'l' :
        {
            rotate(0);
        }
        break;
        case 'h' :
        {
            printMenu();
        }
        break;
        case '1' :
        {
            start();
        }
        break;
        case '2' :
        {
            changeDirection();
        }
        break;
        case '3' :
        {
            changeMode(); 
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
    UART_1_PutString("Decreasing speed\r\n");
    delay += 1;
    if (delay > 100){
        delay = 100;
    }
    char number[50] = {};
    sprintf(number,"Current speed: %d\r\n",delay);
    UART_1_PutString(number);
}

void increaseSpeed()
{
    UART_1_PutString("Increasing speed\r\n");
    delay -= 1;
    if (delay <= 0){
        delay = 1;
    }
    char number[50]= {};
    sprintf(number,"Current delay in ms: %d\r\n",delay);
    UART_1_PutString(number);
}

void changeDirection()
{
    if(direction == FORWARD_)
        direction = BACKWARD_;
    else
        direction = FORWARD_;
}

void changeMode(){
    modeSelect++;
    modeSelect %= 3;
    switch (modeSelect)
    {
    case 0:
        UART_1_PutString(WAVE_DRIVE);
        break;
    case 1:
        UART_1_PutString(FULL_STEP);
        break;
    case 2:
        UART_1_PutString(HALF_STEP);
        break;
    }
}

void rotate(uint8_t dir){
    if (dir)
        direction = FORWARD_;
    else
        direction = BACKWARD_;
    
    one_rot_var = ONE_ROT_STEPS;
    if (modeSelect == 2)
        one_rot_var *= 2;
    
    while(one_rot_var){
        one_step();
        one_rot_var--;
    }
    stop();
}

void stop()
{
    UART_1_PutString("Stop\r\n");
    a1_Write(0b00000000);
    on = 0;
}

void start(){
    UART_1_PutString("Start\r\n");
    on = 1;
}

void printMenu(){
    UART_1_PutString("Stepper-Motor-Controller application started\r\n");
    UART_1_PutString("The three modes go form:\r\n wave-drive to full-step to half-step\r\n");
    UART_1_PutString("0: Stop\r\n");
    UART_1_PutString("1: Start\r\n");
    UART_1_PutString("2: Change direction\r\n");
    UART_1_PutString("3: Change mode\r\n");
    UART_1_PutString("q: Decrease speed\r\n");
    UART_1_PutString("w: Increase speed\r\n");
    UART_1_PutString("r: Rotate once clockwise\r\n");
    UART_1_PutString("l: Rotate once counterclockwise\r\n");
    UART_1_PutString("h: Print this menu\r\n");
}

void one_step(void){
    curr_step += direction;
    if(curr_step >= ARRAYLENGTH)
        curr_step = 0;
    else if(curr_step <= -1)
        curr_step = ARRAYLENGTH -1;
    a1_Write(mode[modeSelect][curr_step]);
    CyDelay(delay);
}
/* [] END OF FILE */
