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

#define MAX_CMD_LENGTH 25U

static char* const StartUpTEXT = "Started";
static char* const DoNothingTEXT = "Do nothing";
static char* const TurnOffLedTEXT = "Turn Led Off";
static char* const TurnOnLedTEXT = "Turn Led On";
static char* const GetSWConstTEXT = "Constant Send SW";
static char* const StopSWConstTEXT = "Stop Constant Send";
static char* const GetSWStatusTEXT = "Send SW status";



void init(void * isr_rx, void * isr_timeout, void * isr_switch,  char8 * last);
void SPI_CMD_Handler(char8* buffer, char8 * text, uint8_t * conBool, char8* last);
void poll(void);

/* [] END OF FILE */
