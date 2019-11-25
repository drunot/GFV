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
    
<<<<<<< HEAD
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
=======
/**
 * Initialize the PID controller.
>>>>>>> parent of aeda303... SW updated and ready for testing
 * @param Kp the Proportional gain.
 * @param Ki the Integral gain.
 * @param Kd the Derivative gain.
 * @param integralMax max limit for the integral.
 * @param integralMin min limit for the integral.
 * @param dt time between each sample, measured in seconds.
 */
void PIDControl_init(float Kp, float Ki, float Kd, float integralMax, float integralMin, float dt);

/**
 * Change the setpoint, i.e. the desired system output value.
 * @param setPoint the PID setpoint.
 */
void PIDControl_changeSetPoint(float setPoint);

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