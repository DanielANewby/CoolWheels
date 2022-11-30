#include "mbed.h"
#include "..\Shared\nRF24L01P.h"

#include <signal.h>

#include <string>

#include "..\Shared\WirelessController.h"
#include "..\Shared\SevenSegment.h"
#include "..\Shared\PCCommunication.h"

// main() runs in its own thread in the OS

DigitalOut Led(D9);

volatile sig_atomic_t userButtonSignal{ 0 };
volatile sig_atomic_t userButtonDebounce{ 0 };
InterruptIn userButton(BUTTON1);
Timeout debounceTimer;

void UserButtonPressed()
{
    userButtonSignal = 1;
}

void UserDebounceDisable()
{
    userButtonDebounce = 0;
}

SevenSegmentController ss_x{
    D4,
    D5,
    D6,
    D7,
    D8,
    D9,
    D15,
    D0
};

SevenSegmentController ss_y{
    D2, // Bottom
    D3, // Bottom left
    PC_0, // Bottom right
    PB_0, // Middle
    PA_4, // top left
    PA_0, // Top right
    PA_1, // Top
    PC_1, // Dot
};
volatile sig_atomic_t SSSignal{ 0 };
unsigned segmentValue = 9; // So it rolls over immediately
Timeout segmentTimer;
void segmentSignal() { 
    SSSignal = 1; 
    segmentTimer.attach(segmentSignal, 500ms);
}

int main()
{
    PCCommunication pc;

    const std::string startMsg{ "Wireless Controller Starting\n" };
    pc.Write(startMsg);

    segmentSignal();
    userButton.rise(UserButtonPressed);
    while (true) {
        bool hasCommand = pc.Update();
        if (hasCommand)
            std::string cmdString = pc.ReadCommand();

        if (userButtonSignal && !userButtonDebounce)
        {
            userButtonDebounce = 1;
            debounceTimer.attach(UserDebounceDisable, 250ms);
            userButtonSignal = 0;
            Led = !Led;
        }
        if (SSSignal)
        {
            segmentValue = (segmentValue + 1) % 10;
            ss_x.Put(segmentValue);
            ss_y.Put((segmentValue + 5) % 10);
            SSSignal = 0;
        }
    }
}

