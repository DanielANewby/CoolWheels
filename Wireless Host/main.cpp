#include "mbed.h"
#include "..\Shared\nRF24L01P.h"

#include <signal.h>

#include "..\Shared\WirelessController.h"

// main() runs in its own thread in the OS

DigitalOut Led(D9);
DigitalOut TestLight(D0);

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

int main()
{
    userButton.rise(UserButtonPressed);
    TestLight = 0;
    while (true) {
        if (userButtonSignal && !userButtonDebounce)
        {
            userButtonDebounce = 1;
            debounceTimer.attach(UserDebounceDisable, 250ms);
            userButtonSignal = 0;
            Led = !Led;
        }
    }
}

