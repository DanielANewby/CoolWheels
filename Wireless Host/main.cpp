#include "mbed.h"

#include "..\Shared\SevenSegment.h"
#include "..\Shared\PCCommunication.h"
#include "..\Shared\WirelessController.h"
#include "..\Shared\WirelessConnection.h"
#include "..\Shared\CLI.h"

#include "HostControl.h"

// main() runs in its own thread in the OS

SevenSegmentController ss_x{ D4, D5, D6, D7, D8, D9, D15, D0 };
SevenSegmentController ss_y{ D2, D3, PC_0, PB_0, PA_4, PA_0, PA_1, PC_1 };

int main()
{
    PCCommunication pc;
    pc.Write("Host starting\n");
    ThisThread::sleep_for(250ms);

    WirelessConnection wc { true };
    CLI cli(pc, wc);

    pc.Write("Wireless Controller initialized\n");
    ThisThread::sleep_for(250ms);

    HostControl host(pc, wc);

    while (true) {
        cli.Update();
        wc.Update();
    }
}

