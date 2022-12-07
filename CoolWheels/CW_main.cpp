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

MotionController motionControl(D6, D7, D9, D5, D4, D3, 0.1, k_BotSpeed);


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

    BasicNavigation nav(wc, motionControl);
    
    pc.Write("Initializing control module\n");
    RemoteControl rc(wc, motionControl, nav);

    pc.Write("Ready\n");

    while (true) {
        motionControl.Update();
        cli.Update();
        wc.Update();
        nav.Update();
    }
}
