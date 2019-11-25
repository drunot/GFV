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
#include "UARTHelper.h"
#include "PIDControl.h"
#include <stdlib.h>
#include <stdio.h>

void handleByteReceived(char * byteReceved, uint8 * currentPlace, uint8 * run, char * outputBuffer , const unsigned int bufSize)
{
    char *currPoint;
    switch(byteReceved[0])
    {
        case 'I' :
            PID_change_setPoint(StdResetTemp);    //Reset temp to initial stage
            
        case 'T' :  //Run test
            printSettings(outputBuffer, bufSize);
            UART_1_PutString("Taget:;Current:;Delta:;PWM:;pp:;ip:;dp:\r\n"); //Wirtes header
            PinLED_Write(0); //Sets LED off
            *run = 1;
            break;
            
            
        case 'S' :
                       
            PID_change_setPoint(strtof((char*) &byteReceved[1], (char **)&currentPlace));    //Set parameters (target temp, Kp, Ki, Kd)
            float tempKp = strtof(currPoint, &currPoint);
            float tempKi = 0.0;
            float tempKd = 0.0;
            if(currPoint != NULL && currPoint[0] != 0)
            {
                tempKi = strtof(currPoint, &currPoint);
                if(currPoint != NULL && currPoint[0] != 0)
                {
                    tempKd = strtof(currPoint, &currPoint);
                }
                else
                {
                    UART_1_PutString("Not enough arguments.\r\n");
                    break;
                }
            }
            else
            {
                UART_1_PutString("Not enough arguments.\r\n");
                break;
            }
            PID_change_Kp(tempKp);
            PID_change_Ki(tempKi);
            PID_change_Kd(tempKd);
            break;
            
        case 'i' :
            PID_change_integral_extremes(strtof((char*) &byteReceved[1], &currPoint), 
                                        strtof(currPoint, &currPoint)); //Set integral min/max (Imin, Imax)
            break;
            
        case 'c' :
            printSettings(outputBuffer, bufSize);    //Print current settings
            break;
            
        case 's' :
            *run = 0;
            PIDControl_init();
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
                     "After any full command type '\\r' to run the command\r\n"
                     "If none of the above this menu gets printed\r\n");
}

void printSettings(char * outputBuffer , const unsigned int size){
    snprintf(outputBuffer, size, "KP:;%.3f;Ki:;%.3f;Kd:;%.3f;Imin:;%.3f;Imax:;%.3f;Target:;%.3f;\r\n", 
                                                PID_settings_t.Kp, PID_settings_t.Ki, PID_settings_t.Kd, PID_settings_t.Imin,
                                                PID_settings_t.Imax, PID_settings_t.Target); //Semicolon sepperated for standard dansih ".csv" 
        
    strRep(outputBuffer, '.',','); // dot changed to comma for standard dansih excel decimal seperator.
    
    UART_1_PutString(outputBuffer);
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
/* [] END OF FILE */
