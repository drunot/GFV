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

//Function prototypes
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

//Definitions 
#define ARRAYLENGTH 8
#define WAVE_DRIVE "Wave_drive\r\n"
#define FULL_STEP "Full_step\r\n"
#define HALF_STEP "Half_step\r\n"
#define ONE_ROT_STEPS 2038  //Amount of steps needed for 1 rotation. 

//Initialize the three step modes as 2D array
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

    //Initialization of UART
    isr_uart_rx_StartEx(ISR_UART_rx_handler);
    UART_1_Start();
    
    //Manu for the stepper functions
    printMenu();

    //For loop without end
    for(;;)
    {
        if(on){ //If stepper is on, take a step
            one_step();
        }
    }
}

CY_ISR(ISR_UART_rx_handler) //UART interrupt
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

//Function handler for the UART input
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

//Decreasing speed
void decreaseSpeed()
{
    UART_1_PutString("Decreasing speed\r\n");
    delay += 1; //add 1 to the delay
    if (delay > 100){
        delay = 100;    //Max delay = 100 ms
    }
    char number[50] = {};
    sprintf(number,"Current speed: %d\r\n",delay);
    UART_1_PutString(number);
}

//Increasing speed
void increaseSpeed()
{
    UART_1_PutString("Increasing speed\r\n");
    delay -= 1; //Decrease 1 from the delay
    if (delay <= 0){    //Minimum is 1
        delay = 1;
    }
    char number[50]= {};
    sprintf(number,"Current delay in ms: %d\r\n",delay);
    UART_1_PutString(number);
}

//Change direction
void changeDirection()
{
    if(direction == FORWARD_)
        direction = BACKWARD_;
    else
        direction = FORWARD_;
}

//Mode change
void changeMode(){
    modeSelect++;
    modeSelect %= 3;
    
    //Print current mode
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

//Rotate one round, dir is direction
void rotate(uint8_t dir){
    
    //Set dir
    if (dir)
        direction = FORWARD_;
    else
        direction = BACKWARD_;
    
    //init var for steps
    one_rot_var = ONE_ROT_STEPS;
    //double if in mode 2 (Half_step)
    if (modeSelect == 2)
        one_rot_var *= 2;
    
    //Execute number of steps
    while(one_rot_var){
        one_step();
        one_rot_var--;
    }
    stop();
}

//Stop the motor
void stop()
{
    UART_1_PutString("Stop\r\n");
    
    //Kill the motor
    on = 0;
    
    //Stop all interfasing IO to motor
    a1_Write(0b00000000);
}

//Start the motor
void start(){
    UART_1_PutString("Start\r\n");
    on = 1;
}

//Print the menu
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

//Take one step
void one_step(void){
    
    //Change index in array
    curr_step += direction;
    
    //Check for end
    if(curr_step >= ARRAYLENGTH)
        curr_step = 0;
    else if(curr_step <= -1)
        curr_step = ARRAYLENGTH -1;
    
    //Write current step sequence to the IO
    a1_Write(mode[modeSelect][curr_step]);
    
    //Delay
    CyDelay(delay);
}
/* [] END OF FILE */
