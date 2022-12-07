#include "mbed.h"

#include <signal.h>

#include "..\Shared\WirelessConnection.h"
#include "..\Shared\PCCommunication.h"
#include "..\Shared\CLI.h"

#include "CW_MotorController.h"
#include "CW_MotionController.h"
#include "CW_RemoteControl.h"

#include "CW_BasicNavigation.h"

constexpr float k_BotSpeed = 0.5f; // Set 0.0 - 1.0 to control how fast everything goes
constexpr auto k_fwdTime = 1000ms; // Move Time in ms
constexpr auto k_turnTime = 800ms; // Turn time in ms

MotionController motionControl(D6, D7, D9, D5, D4, D3, 0.1, k_BotSpeed);

volatile sig_atomic_t userButtonSignal{ 0 };
volatile sig_atomic_t stopTriggerSignal{ 0 };

InterruptIn switch1(BUTTON1);

void UserButtonPressed()
{
    userButtonSignal = 1;
}

enum MoveMode {
    kMode_Forward,
    kMode_Moving,
    kMode_Turn,
    kMode_Turning,
    kMode_Count,
    kMode_Invalid = 0xFFFFFFFF
};

BasicNavigation nav;

// main() runs in its own thread in the OS
int main()
{
    PCCommunication pc;
    pc.Write("Bot starting\n");

    pc.Write("Starting wireless connection\n");
    WirelessConnection wc{ false };

    pc.Write("Wireless connection initialized\n");
    pc.Write("Initializing CLI\n");
    CLI cli(pc, wc);

    pc.Write("Initializing control module\n");
    RemoteControl rc(wc, motionControl, nav);

    switch1.rise(UserButtonPressed);

    pc.Write("Ready\n");

    while (true) {
        cli.Update();
        wc.Update();
        nav.Update();
    }
}
