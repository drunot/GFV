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

<<<<<<< HEAD
static float integral = 0;
static float previousError = 0;

void PID_change_Kp(float Kp) {PID_settings_t.Kp = Kp;}
void PID_change_Ki(float Ki) {PID_settings_t.Ki = Ki;}
void PID_change_Kd(float Kd) {PID_settings_t.Kd = Kd;}
void PID_change_integral_extremes(float Imin, float Imax) 
    {
        if (Imin < Imax){
            PID_settings_t.Imin = Imin; 
            PID_settings_t.Imax = Imax; 
        }
    }
void PID_change_setPoint(float setPoint) {PID_settings_t.Target = setPoint;}
=======
static float Kp = 0;
static float Ki = 0;
static float Kd = 0;
static float dt = 0;
static float integralMax = 0;
static float integralMin = 0;
static float setPoint = 0;
static float integral = 0;
static float previousError = 0;

void PIDControl_init(float _Kp, float _Ki, float _Kd, float _integralMax, float _integralMin, float _dt)
{
    Kp = _Kp;
    Ki = _Ki;
    Kd = _Kd;
    integralMax = _integralMax;
    integralMin = _integralMin;
    dt = _dt;
}
>>>>>>> parent of aeda303... SW updated and ready for testing

void PIDControl_changeSetPoint(float _setPoint)
{
    setPoint = _setPoint;
}

float PIDControl_doStep(float systemOutput, float* proportionalPart, float* integralPart, float* derivativePart)
{
    float output = 0;
    float proportional = 0;
    float derivative = 0;
    float currentError = 0;
    
    // calculate current error
    currentError = setPoint - systemOutput;
    
    // calculate proportional part
    proportional = currentError;
    
    // calculate integral part
    integral = integral + (currentError * dt());
    
    // limit the integral
    if (integral > integralMax) integral = integralMax;
    if (integral < integralMin) integral = integralMin;
    
    // calculate derivative part
    derivative = (currentError - previousError) / dt();
    
    output = proportional * Kp + integral * Ki + derivative * Kd;
    *proportionalPart = proportional * Kp;
    *integralPart = integral * Ki;
    *derivativePart = derivative * Kd;
    
    previousError = currentError;
    return output;
}

/* [] END OF FILE */