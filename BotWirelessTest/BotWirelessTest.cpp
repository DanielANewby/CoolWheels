#include "mbed.h"

#include "..\Shared\PCCommunication.h"
#include "..\Shared\WirelessController.h"

// main() runs in its own thread in the OS
int main()
{
    PCCommunication pc;
    pc.Write("Bot Wireless Test starting\n");
    ThisThread::sleep_for(250ms);

    WirelessController wc{
        WirelessController::BotIdentifier, // Remote id,
        WirelessController::HostIdentifier, // Host id
        WirelessController::HostAddress
    };

    pc.Write("Wireless Controller initialized\n");
    ThisThread::sleep_for(250ms);

    while (true) {
        if (wc.Readable())
        {
            pc.Write(".");
        }
        ThisThread::sleep_for(200ms);
    }
}

