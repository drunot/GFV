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
#include "I2CHelper.h"
#include <stdio.h>


int8_t convertToSigned(uint8_t number)
{
    if(number >> 7) //Checks if most seignificant is 1.
    {
        //2s component
        number = (~number)+1; 
        //Saves the number after converted with 2s component as negative.
        return (int8_t)number * -1; 
    }
    return number;
}

uint8_t getDecimal(uint8_t number)
{
    if(number >> 7)
    {
      return 5;   
    }
    return 0;
}

void GetStringToPrint(char8* stringToPrint, uint8_t* dataRead)
{
    int8_t temp = convertToSigned(dataRead[0]);
    uint8_t decimal = getDecimal(dataRead[1]);
    sprintf(stringToPrint,"Temperatur: %d,%d\n\r",temp,decimal);
}


/* [] END OF FILE */
