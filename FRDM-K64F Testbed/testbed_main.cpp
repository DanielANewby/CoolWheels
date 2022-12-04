#include "mbed.h"

#include "..\Shared\PCCommunication.h"
#include "..\Shared\WirelessController.h"

DigitalOut green_builtin_led(LED1);
DigitalOut red_builtin_led(LED2);

// main() runs in its own thread in the OS
int main()
{
    PCCommunication pc;
    pc.Write("Testbed starting\n");
    ThisThread::sleep_for(250ms);

    WirelessController wc{
        WirelessController::HostIdentifier, // Host id
        WirelessController::BotIdentifier, // Remote id,
        WirelessController::HostAddress
    };

    pc.Write("Wireless Controller initialized\n");
    ThisThread::sleep_for(250ms);

    while (true) {
        bool hasCommand = pc.Update();
        if (hasCommand)
        {
            std::string cmdString = pc.ReadCommand();
            pc.Write("Sending command: ");
            pc.Write(cmdString);
            pc.Write("\n");

            wc.Send(cmdString.c_str(), cmdString.length());
            pc.Write("Sent\n");

            /*
            while (!wc.Readable())
                wait_us(500);
            pc.Write("Wait finished\n");
            */
            
            char buffer[256] { 0 };
            wc.Recv(buffer, 255);
            pc.Write("Received: ");
            pc.Write(buffer);
            pc.Write("\n");
            
            green_builtin_led = 1;
            red_builtin_led = 0;
            ThisThread::sleep_for(200ms);
        }
        else
        {
            red_builtin_led = 1;
            green_builtin_led = 0;
        }
    }
}

