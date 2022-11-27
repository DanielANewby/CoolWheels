#include "CW_MotionController.h"

MotionController::MotionController(MotorController left, MotorController right, float _speed):
leftWheel(left), rightWheel(right), speed(_speed)
{
    leftWheel.Stop();
    rightWheel.Stop();
}

MotionController::MotionController(
    PinName fwdLeft, PinName revLeft, PinName pwmLeft, 
    PinName fwdRight, PinName revRight, PinName pwmRight,
    float period,
    float _speed):
    leftWheel(fwdLeft, revLeft, pwmLeft, period),
    rightWheel(fwdRight, revRight, pwmRight, period),
    speed(_speed)
{
    leftWheel.Stop();
    rightWheel.Stop();
}

void MotionController::SetSpeed(float percent)
{
    speed = percent;
}

void MotionController::Forward()
{
    leftWheel.Forward(speed);
    rightWheel.Forward(speed);
}

void MotionController::Reverse()
{
    leftWheel.Reverse(speed);
    rightWheel.Reverse(speed);
}

void MotionController::TurnLeft()
{
    leftWheel.Stop();
    rightWheel.Forward(speed);
}

void MotionController::TurnRight()
{
    leftWheel.Forward(speed);
    rightWheel.Stop();
}

void MotionController::Stop()
{
    leftWheel.Stop();
    rightWheel.Stop();
}
