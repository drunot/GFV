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



void init(void * UARTISR); //Initializes the interrupts and the components. Takes to interrupts at parameters.
void turnOnLED(); //Turns on LED1
void turnOffLED(); //Turns off LED1
void handleByteReceived(uint8_t byteReceived, uint8_t * poller, char * buffer, uint8_t * counter); //Handles the reading and understanding of receved input from the UART
uint8 pollSlave(); //Is used to send a blank byte to the slave and then return the byte receved from the slave.
void sendString(char * buffer, uint8_t * counter); //Is used to send a string to the slave. (Is called by handleByteReceived when carage return is receved)
void UARTHandler(uint8_t * poller, char * buffer, uint8_t * counter); //Handles the receved bytes from the UART. (Is using handleByteReceived to make decisions on thies)

/* [] END OF FILE */
