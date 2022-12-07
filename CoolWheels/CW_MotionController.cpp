#include "CW_MotionController.h"

MotionController* MotionController::activeController { nullptr };
void MotionController::StopCallback()
{
    if (MotionController::activeController)
        activeController->Stop();
}

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

float MotionController::GetSpeed()
{
    return speed;
}

void MotionController::Forward(unsigned ms)
{
    Stop();
    if (!ms) 
    {
        ms = forwardTime;
        printf("MC: forward time = %d ms\n", ms);
    }
    ms /= speed;
    printf("MC: forward time = %d ms, speed = %f\n", ms, speed);
    printf("MC: forward %d ms (real)\n", ms);
    Activate(ms);
    leftWheel.Forward(speed * leftBias);
    rightWheel.Forward(speed * rightBias);
}

void MotionController::Reverse(unsigned ms)
{
    Stop();
    if (!ms) 
        ms = forwardTime;
    ms /= speed;
    printf("MC: reverse %d ms (real)\n", ms);
    Activate(ms);
    leftWheel.Reverse(speed * leftBias);
    rightWheel.Reverse(speed * rightBias);
}

void MotionController::TurnLeft(unsigned ms)
{
    Stop();
    if (!ms) 
        ms = turnTime;
    ms /= speed;
    printf("MC: turn left %d ms (real)\n", ms);
    Activate(ms);
    leftWheel.Reverse(speed * leftBias);
    rightWheel.Forward(speed * rightBias);
}

void MotionController::TurnRight(unsigned ms)
{
    Stop();
    if (!ms) 
        ms = turnTime;
    ms /= speed;
    printf("MC: turn right %d ms (real)\n", ms);
    Activate(ms);
    leftWheel.Forward(speed * leftBias);
    rightWheel.Reverse(speed * rightBias);
}

void MotionController::TurnLeftDegrees(unsigned degrees)
{
    // Assume calibrated turn time
    unsigned ms = (unsigned)((turnTime * degrees) * speed / 90.f);
    TurnLeft(ms);
}

void MotionController::TurnRightDegrees(unsigned degrees)
{
    // Assume calibrated turn time
    unsigned ms = (unsigned)((turnTime * degrees) * speed / 90.f);
    TurnRight(ms);
}

void MotionController::TurnLeftTimed(unsigned ms)
{
    TurnLeft(ms);
}

void MotionController::TurnRightTimed(unsigned ms)
{
    TurnRight(ms);
}

void MotionController::Stop()
{
    activeController = nullptr;
    //printf("MC: stopping\n");
    leftWheel.Stop();
    rightWheel.Stop();
}

void MotionController::MoveForward()
{
    auto duration = std::chrono::milliseconds{ unsigned(forwardTime * speed) };
    stopTimer.attach(&MotionController::StopCallback, duration);
}
