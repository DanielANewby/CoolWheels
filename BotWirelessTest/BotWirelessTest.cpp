#include "mbed.h"

#include "..\Shared\PCCommunication.h"
#include "..\Shared\WirelessController.h"

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

    WirelessController wc{
        WirelessController::BotIdentifier, // Remote id,
        WirelessController::HostIdentifier, // Host id
        WirelessController::HostAddress
    };

    pc.Write("Wireless Controller initialized\n");
    ThisThread::sleep_for(250ms);

    while (true) {
        bool hasCommand = pc.Update();
        if (hasCommand)
        {
            std::string cmdString = pc.ReadCommand();
            pc.Write("OK\n");
        }

        if (wc.Readable())
        {
            pc.Write("Readable\n");
            char buffer[32] { 0 };
            wc.Recv(buffer, sizeof(buffer));
            std::string recvHex;
            for (unsigned n = 0; n < 16; ++n)
                recvHex += CharToHex(buffer[n]) + ' ';
            recvHex += '\n';
            for (unsigned n = 16; n < 32; ++n)
                recvHex += CharToHex(buffer[n]) + ' ';
            recvHex += '\n';
            pc.Write(buffer);
            pc.Write("\n");
            pc.Write(recvHex);
            pc.Write("End\n");
        }
        ThisThread::sleep_for(200ms);
    }
}
