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
CY_ISR_PROTO(ISR_step);
void handleByteReceived(uint8_t byteReceived);
void decreaseSpeed(void);
void increaseSpeed(void);
void changeDirection();
void start(void);
void stop(void);
void changeMode(void);

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    isr_uart_rx_StartEx(ISR_UART_rx_handler);
    isr_timer_StartEx(ISR_step);
    UART_1_Start();
    
    UART_1_PutString("Stepper-Motor-Controller application started\r\n");
    UART_1_PutString("The three modes go form:\r\n wave-drive to full-step to half-step\r\n");
    UART_1_PutString("0: Stop\r\n");
    UART_1_PutString("1: Start\r\n");
    UART_1_PutString("2: Change direction\r\n");
    UART_1_PutString("3: Change mode\r\n");
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

#define ARRAYLENGTH 8

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

CY_ISR(ISR_step){
    curr_step += direction;
    if(curr_step >= ARRAYLENGTH)
        curr_step = 0;
    else if(curr_step <= -1)
        curr_step = ARRAYLENGTH -1;
    a1_Write(mode[modeSelect][curr_step]);
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
    uint16_t period = Timer_1_ReadPeriod();
    if(period > 400)
    {
        UART_1_PutString("Decreasing speed\r\n");
        Timer_1_Stop();
        Timer_1_WritePeriod(period-200);
        Timer_1_Start();
    }
    else if (period < 400){     
        UART_1_PutString("Min speed achieved\r\n");
        Timer_1_Stop();
        Timer_1_WritePeriod(200);
        Timer_1_Start();
    }
}

void increaseSpeed()
{
    uint16_t period = Timer_1_ReadPeriod();
    if(period < (65535-200))
    {
        UART_1_PutString("Increasing speed\r\n");
        Timer_1_Stop();
        Timer_1_WritePeriod(period+200);
        Timer_1_Start();
    }
    else
    {
        UART_1_PutString("Max speed achieved\r\n");
        Timer_1_Stop();
        Timer_1_WritePeriod(65535);
        Timer_1_Start();
    }
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
}

void stop()
{
    UART_1_PutString("Stop\r\n");
    Timer_1_Sleep();    //Stop, but keep configuration
}

void start(){
    UART_1_PutString("Start\r\n");
    Timer_1_Start();
}
/* [] END OF FILE */
