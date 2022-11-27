#ifndef CW_MOTOR_CONTROLLER_H
#define CW_MOTOR_CONTROLLER_H

#include "mbed.h"

class MotorController
{
public:
    MotorController(PinName _forwardPin, PinName _reversePin, PinName pwmPin, float period);

    void Forward(float percentMax);
    void Reverse(float percentMax);    
    void Stop();

private:
    DigitalOut forwardPin;
    DigitalOut reversePin;
    PwmOut pwmControl;
};

#endif // CW_MOTOR_CONTROLLER_H
