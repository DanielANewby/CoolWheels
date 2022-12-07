#ifndef CW_MOTION_CONTROLLER_H
#define CW_MOTION_CONTROLLER_H

#include "mbed.h"

#include "CW_MotorController.h"
#include <ratio>

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
    float GetSpeed();
    
    void SetLeftWheelBias(float percent) { leftBias = percent; }
    void SetRightWheelBias(float percent) { rightBias = percent; }
    float GetLeftWheelBias() { return leftBias; }
    float GetRightWheelBias() { return rightBias; }

    void SetForwardTimeMs(unsigned ms) { 
        printf("MC: setting forward time: %d ms\n", ms);
        forwardTime = ms; 
    }
    unsigned GetForwardTimeMs() { return forwardTime; }
    void SetTurnTimeMs(unsigned ms) { 
        printf("MC: setting turn time: %d ms\n", ms);
        turnTime = ms; 
    }
    unsigned GetTurnTimeMs() { return turnTime; }

    void Forward(unsigned ms = 0);
    void Reverse(unsigned ms = 0);
    void TurnLeft(unsigned ms = 0);
    void TurnRight(unsigned ms = 0);
    void TurnLeftDegrees(unsigned degrees);
    void TurnRightDegrees(unsigned degrees);
    void TurnLeftTimed(unsigned ms);
    void TurnRightTimed(unsigned ms);
    void Stop();

    void MoveForward();

    bool Busy() { return !!activeController; }

private:
    void Activate(unsigned ms) {
        activeController = this;
        stopTimer.attach(&MotionController::StopCallback, std::chrono::milliseconds(ms));
    }

    MotorController leftWheel;
    MotorController rightWheel;

    float speed;
    float leftBias { 1.f };
    float rightBias { 1.f };

    unsigned forwardTime{ 800 };
    unsigned turnTime{ 400 };

    Timeout stopTimer;

    static MotionController* activeController;
    static void StopCallback();
};

#endif // CW_MOTION_CONTROLLER_H
