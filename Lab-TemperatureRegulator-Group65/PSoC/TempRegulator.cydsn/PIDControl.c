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
#include "PIDControl.h"

static float dt = 0;
static float integral = 0;
static float previousError = 0;

void PID_change_Kp(float Kp) {PID_settings_t.Kp = Kp;}
void PID_change_Ki(float Ki) {PID_settings_t.Ki = Ki;}
void PID_change_Kd(float Kd) {PID_settings_t.Kd = Kd;}
void PID_change_integral_extremes(float Imin, float Imax) 
    {PID_settings_t.Imin = Imin; PID_settings_t.Imax = Imax; }
void PID_change_setPoint(float setPoint) {PID_settings_t.Target = setPoint;}

void PIDControl_init(void)
{
    PID_change_Kp(StdKp);
    PID_change_Ki(StdKi);
    PID_change_Kd(StdKd);
    PID_change_integral_extremes(StdImin, StdImax);
    PID_change_setPoint(StdResetTemp); 
}

float PIDControl_doStep(float systemOutput, float* proportionalPart, float* integralPart, float* derivativePart)
{
    float output = 0;
    float proportional = 0;
    float derivative = 0;
    float currentError = 0;
    
    // calculate current error
    currentError = PID_settings_t.Target - systemOutput;
    
    // calculate proportional part
    proportional = currentError;
    
    // calculate integral part
    integral = integral + (currentError * dt);
    
    // limit the integral
    if (integral > PID_settings_t.Imax) integral = PID_settings_t.Imax;
    if (integral < PID_settings_t.Imin) integral = PID_settings_t.Imin;
    
    // calculate derivative part
    derivative = (currentError - previousError) / dt;
    
    output = proportional * PID_settings_t.Kp + integral * PID_settings_t.Ki + derivative * PID_settings_t.Kd;
    *proportionalPart = proportional * PID_settings_t.Kp;
    *integralPart = integral * PID_settings_t.Ki;
    *derivativePart = derivative * PID_settings_t.Kd;
    
    previousError = currentError;
    return output;
}

/* [] END OF FILE */
