#ifndef CW_MOTION_CONTROLLER_H
#define CW_MOTION_CONTROLLER_H

#include "mbed.h"

#include "CW_MotorController.h"

class MotionController
{
public:
    MotionController(MotorController left, MotorController right, float _speed = 1.0);
    MotionController(
        PinName fwdLeft, PinName revLeft, PinName pwmLeft,
        PinName fwdRight, PinName revRight, PinName pwmRight,
        float period = 0.1,
        float speed = 1.0
    );

    void SetSpeed(float percent);

    void Forward();
    void Reverse();
    void TurnLeft();
    void TurnRight();
    void Stop();

private:
    MotorController leftWheel;
    MotorController rightWheel;

    float speed;
};

#endif // CW_MOTION_CONTROLLER_H
