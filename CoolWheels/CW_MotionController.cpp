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
    // Clamp speed as a percentage
    speed = max(0.0f, min(1.0f, percent));
}

void MotionController::Forward()
{
    leftWheel.Forward(speed * leftBias);
    rightWheel.Forward(speed * rightBias);
}

void MotionController::Reverse()
{
    leftWheel.Reverse(speed * leftBias);
    rightWheel.Reverse(speed * rightBias);
}

void MotionController::TurnLeft()
{
    leftWheel.Stop();
    rightWheel.Forward(speed * rightBias);
}

void MotionController::TurnRight()
{
    leftWheel.Forward(speed * leftBias);
    rightWheel.Stop();
}

void MotionController::Stop()
{
    leftWheel.Stop();
    rightWheel.Stop();
}
