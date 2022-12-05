#include "mbed.h"

#include "..\Shared\PCCommunication.h"
#include "..\Shared\WirelessController.h"
#include "..\Shared\WirelessConnection.h"
#include "CW_RemoteControl.h"
#include "..\Shared\CLI.h"

std::string CharToHex(char c) {
    const char codes[] = "0123456789ABCDEF";
    std::string out;
    out += codes[(c >> 4) & 0xF];
    out += codes[c & 0xF];
    return out;
}

// main() runs in its own thread in the OS
int main()
{
    PCCommunication pc;
    pc.Write("Bot Wireless Test starting\n");
    ThisThread::sleep_for(250ms);

/*
    WirelessController wc{
        WirelessController::BotIdentifier, // Remote id,
        WirelessController::HostIdentifier, // Host id
        WirelessController::HostAddress
    };
*/
    WirelessConnection wc {false};
    CLI cli(pc, wc);

    pc.Write("Wireless Controller initialized\n");
    ThisThread::sleep_for(250ms);

    while (true) {
        cli.Update();
        wc.Update();
    }
}

