#include "CW_MotorController.h"

MotorController::MotorController(PinName _forwardPin, PinName _reversePin, PinName pwmPin, float period):
forwardPin{ _forwardPin },
reversePin{ _reversePin },
 pwmControl{ pwmPin }
{
    pwmControl.period(period);
    pwmControl = 0;
}

void MotorController::Forward(float percentMax)
{
    forwardPin = 1;
    reversePin = 0;
    pwmControl = percentMax;
}

void MotorController::Reverse(float percentMax)
{
    forwardPin = 0;
    reversePin = 1;
    pwmControl = percentMax;
}
    
void MotorController::Stop()
{
    pwmControl = 0;
    forwardPin = 0;
    reversePin = 0;
}
