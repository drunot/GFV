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


static char* const StartUpTEXT = "Started";
static char* const DoNothingTEXT = "Do nothing";
static char* const TurnOffLedTEXT = "Turn Led Off";
static char* const TurnOnLedTEXT = "Turn Led On";
static char* const GetSWConstTEXT = "Constant Send SW";
static char* const StopSWConstTEXT = "Stop Constant Send";
static char* const GetSWStatusTEXT = "Send SW status";


void SPI_CMD_Handler(char8* buffer, char8 * text, uint8_t * conBool);

/* [] END OF FILE */
