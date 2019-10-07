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

void turnOnLED();
void turnOffLED();
void handleByteReceived(uint8_t byteReceived, uint8_t * poller, char * buffer, uint8_t * counter);
uint8 pollSlave();
void sendString(char * buffer, uint8_t * counter) ;

/* [] END OF FILE */
