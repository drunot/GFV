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
#include <stdlib.h>

static char outputBuffer[256] = {0};
static uint8_t tempBuf [2] = {0};
static uint8_t run = 0;

void strRep(char * buf, char find, char replace);
CY_ISR_PROTO(ISR_UART_rx_handler);
void handleByteReceived(void);
void printHelp(void);
void printSettings(void);

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    I2C_1_Start();
    UART_1_Start();
    PWM_1_Start();
    PIDControl_init();
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    
    printHelp();
    printSettings();

    float temp = 0;
    UART_RX_isr_StartEx(ISR_UART_rx_handler);
   
    
    for(;;)
    {
        if (run){
            temp = readFromI2C(0x48, tempBuf, 2);
            float error = PID_settings_t.Target - temp;
            float proportionalPart = 0;
            float integralPart = 0;
            float derivativePart = 0;
            
            float controlSignal = PIDControl_doStep(temp, &proportionalPart, &integralPart, &derivativePart); //Gets PWM compare.
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

void strRep(char * buf, char find, char replace) //Replace-function for correct format for danish csv.
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

uint8_t byteReceived[50];
uint8 currPlace = 0;

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
        
        handleByteReceived();
        currPlace = 0;

    }
}

void handleByteReceived(void)
{
    char *currPoint;
    switch(byteReceived[0])
    {
        case 'I' :
            PID_change_setPoint(StdResetTemp);    //Reset temp to initial stage
            
        case 'T' :  //Run test
            printSettings();
            UART_1_PutString("Taget:;Current:;Delta:;PWM:;pp:;ip:;dp:\r\n"); //Wirtes header
            PinLED_Write(0); //Sets LED off
            run = 1;
            break;
            
            
        case 'S' :
                       
            PID_change_setPoint(strtof((char*) &byteReceived[1], &currPoint));    //Set parameters (target temp, Kp, Ki, Kd)
            PID_change_Kp(strtof(currPoint, &currPoint));
            PID_change_Ki(strtof(currPoint, &currPoint));
            PID_change_Kd(strtof(currPoint, &currPoint));
            break;
            
        case 'i' :
            PID_change_integral_extremes(strtof((char*) &byteReceived[1], &currPoint), 
                                        strtof(currPoint, &currPoint)); //Set integral min/max (Imin, Imax)
            break;
            
        case 'c' :
            printSettings();    //Print current settings
            break;
            
        case 's' :
            run = 0;
            break;
        
        default :
            printHelp();     // Print help
            break;
    }
}

void printHelp(void){
    
    UART_1_PutString("TempRegulator commands: \r\n"
                     "I: Reset temp to initial stage (30 deg) \r\n"
                     "T: Run test\r\n"
                     "S: Set parameters (target_temp, Kp, Ki, Kd)\r\n"
                     "i: Set int min/max (min, max)\r\n"
                     "c: prints current settings\r\n"
                     "After any full command type 'k' to run the command\r\n"
                     "If none of the above this menu gets printed\r\n");
}

void printSettings(void){
    snprintf(outputBuffer, sizeof(outputBuffer), "KP:;%.3f;Ki:;%.3f;Kd:;%.3f;Imin:;%.3f;Imax:;%.3f;Target:;%.3f;\r\n", 
                                                PID_settings_t.Kp, PID_settings_t.Ki, PID_settings_t.Kd, PID_settings_t.Imin,
                                                PID_settings_t.Imax, PID_settings_t.Target); //Semicolon sepperated for standard dansih ".csv" 
        
    strRep(outputBuffer, '.',','); // dot changed to comma for standard dansih excel decimal seperator.
    
    UART_1_PutString(outputBuffer);
}

/* [] END OF FILE */
