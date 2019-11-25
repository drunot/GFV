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

#ifndef PID_CONTROL_H
#define PID_CONTROL_H
    
#define StdKp 5.0f
#define StdKi 1.0f/30.0f
#define StdKd 0.0f
#define StdImax 3000.0f
#define StdImin -3000.0f
#define StdResetTemp 30.0f
#define SAMPLES_PER_SECOND 3
#define sampleWaitTimeInMilliseconds(void) (1000 / SAMPLES_PER_SECOND)
#define dt(void) sampleWaitTimeInMilliseconds() / 1000 // dt is measured in seconds
#define samples_to_stable 50
    
/*
 * @param Kp the Proportional gain.
 * @param Ki the Integral gain.
 * @param Kd the Derivative gain.
 * @param integralMax max limit for the integral.
 * @param integralMin min limit for the integral.
 * @param dt time between each sample, measured in seconds.
*/
struct PID_settings {
    float Kp;
    float Ki;
    float Kd;
    float Imax;
    float Imin;
    float Target;
} PID_settings_t;

/**
* Change the settings
* @param Kp - the new Kp value
* @param Ki - the new Ki value
* @param Kd - the new Kd value
* @param Imin - the new Imin value
* @param Imax - the new Imax value
* @param setPoint the PID setpoint
*/
void PID_change_Kp(float Kp);
void PID_change_Ki(float Ki);
void PID_change_Kd(float Kd);
void PID_change_integral_extremes(float Imin, float Imax);
void PID_change_setPoint(float setPoint);

    
/**
 * Initialize the PID controller from the settings.
 */
void PIDControl_init(void);


/**
 * Do one step of the regulation algorithm.
 * Note that the proportionalPart, integralPart and derivativePart parameters are only
 * included, so they can be printed for analysis purposes.
 * @param systemOutput The current system output.
 * @param proportionalPart Pointer to a float, to which the value of this PID part will be copied.
 * @param integralPart Pointer to a float, to which the value of this PID part will be copied.
 * @param derivativePart Pointer to a float, to which the value of this PID part will be copied.
 * @return the PID controller output.
 */
float PIDControl_doStep(float systemOutput, float* proportionalPart, float* integralPart, float* derivativePart);    
    
#endif // PID_CONTROL_H

/* [] END OF FILE */