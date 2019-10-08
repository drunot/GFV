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

char8* readFromI2C(uint8_t Address, char8* printString, uint8_t* DataToRead)
{
    I2C_1_MasterReadBuf(Address,DataToRead,1, I2C_1_MODE_COMPLETE_XFER);
    int8_t temp = convertToSigned(DataToRead[0]);
    sprintf(printString,"Button status from adress 0x%02X: %d\n\r",Address, temp);
    return printString;
}

/* [] END OF FILE */
