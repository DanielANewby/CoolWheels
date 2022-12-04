#include "mbed.h"

DigitalOut Led1(LED1);
DigitalOut Led2(LED2);

// main() runs in its own thread in the OS
int main()
{
    while (true) {
        Led1 = Led2;
        Led2 = !Led2;
        ThisThread::sleep_for(500ms);
    }
}

