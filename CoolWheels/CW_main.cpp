#include "mbed.h"

#include <signal.h>

#include "..\Shared\WirelessController.h"

#include "CW_MotorController.h"
#include "CW_MotionController.h"

constexpr float k_BotSpeed = 0.5f; // Set 0.0 - 1.0 to control how fast everything goes
constexpr auto k_fwdTime = 1000ms; // Move Time in ms
constexpr auto k_turnTime = 800ms; // Turn time in ms

MotionController motionControl(D6, D7, D9, D5, D4, D3, 0.1, k_BotSpeed);

volatile sig_atomic_t userButtonSignal{ 0 };
volatile sig_atomic_t stopTriggerSignal{ 0 };

InterruptIn switch1(BUTTON1);
Timeout stopTimer;

void UserButtonPressed()
{
    userButtonSignal = 1;
}

void stopTrigger()
{
    motionControl.Stop();
    stopTriggerSignal = 1;
}

enum MoveMode {
    kMode_Forward,
    kMode_Moving,
    kMode_Turn,
    kMode_Turning,
    kMode_Count,
    kMode_Invalid = 0xFFFFFFFF
};

// main() runs in its own thread in the OS
int main()
{
    printf("Starting\n");
    printf("Starting wireless controller");
    WirelessController wc{
        WirelessController::BotIdentifier, // Host id,
        WirelessController::HostIdentifier, // Remote id
        WirelessController::BotAddress
    };
    printf("Started\n");

    switch1.rise(UserButtonPressed);

    MoveMode mode{ kMode_Invalid };

    while (true) {
/*
        if (userButtonSignal)
        {
            userButtonSignal = 0;
            if (mode == kMode_Invalid)
                mode = kMode_Forward;
            else
                mode = kMode_Invalid;
            printf("Button pressed, running = %s\n", (mode == kMode_Invalid) ? "Stopped" : "running");
        }

        if (stopTriggerSignal)
        {
            stopTriggerSignal = 0;
            ThisThread::sleep_for(100ms);
            if (mode == kMode_Turning)
                mode = kMode_Forward;
            if (mode == kMode_Moving)
                mode = kMode_Turn;
        }

        if (wc.Readable())
        {
            char buffer[64] = { 0 };
            wc.Recv(buffer, 32);
            wc.Send(buffer + 4, 28);
        }
*/
        /*/ // This is a comment switch, replace * w/ ** to enable below
        switch (mode)
        {
        case kMode_Forward:
            mode = kMode_Moving;
            stopTimer.attach(stopTrigger, k_fwdTime);
            motionControl.Forward();
            break;

        case kMode_Turn:
            mode = kMode_Turning;
            stopTimer.attach(stopTrigger, k_turnTime);
            motionControl.TurnRight();
            break;

        case kMode_Moving: // Fallthrough
        case kMode_Turning: // Fallthrough
        default: // No-op
            break;
        }
        /**/
    }
}

