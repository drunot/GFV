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
#include "SPImaster.h"
#include "project.h"

void turnOnLED() {
    SPIM_1_WriteTxData(49);
}
void turnOffLED() {
    SPIM_1_WriteTxData('2');
}

/* [] END OF FILE */
