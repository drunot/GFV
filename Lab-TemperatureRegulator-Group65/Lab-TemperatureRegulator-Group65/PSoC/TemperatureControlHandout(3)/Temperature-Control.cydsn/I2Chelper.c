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
#include "I2Chelper.h"
#include <stdio.h>


float convertTofloat(void * number, uint8_t decimal)
{
    int8_t * numPtr = (int8_t *)number; //Converts from uint8_t to int8_t
    return ((float)*numPtr + ((float)(decimal >> 7)/2)); //Converts to float and adds the half degree.
}

float readFromI2C(uint8_t Address, uint8_t * buf, uint8_t size)
{
    size = size < 2 ? size : 2; //Checks for size, to avoid overflow.
    I2C_1_MasterReadBuf(Address,buf,size, I2C_1_MODE_COMPLETE_XFER); //Reads from temp sensor.
    if(size == 2)
    {
        return convertTofloat(&buf[0], buf[1]); //Returns converted value. Se convertTofloat for conversion.
    }
    if(size == 1)
    {
        return convertTofloat(&buf[0], 0); //Returns only hole degrees if only buf length is one.
    }
    return 0.0;
}

/* [] END OF FILE */