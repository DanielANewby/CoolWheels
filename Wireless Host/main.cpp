#include "mbed.h"

#include "..\Shared\SevenSegment.h"
#include "..\Shared\PCCommunication.h"
#include "..\Shared\WirelessController.h"
#include "..\Shared\WirelessConnection.h"
#include "..\Shared\CLI.h"

#include "HostControl.h"

// main() runs in its own thread in the OS

// Undef to build for F411RE and enable seven segments; these pins don't exist on FRDM
#define FRDM

#ifdef FRDM
SevenSegmentController* ss_arg_x = nullptr;
SevenSegmentController* ss_arg_y = nullptr;
#else
SevenSegmentController ss_x{ D4, D5, D6, D7, D8, D9, D15, D0 };
SevenSegmentController ss_y{ D2, D3, PC_0, PB_0, PA_4, PA_0, PA_1, PC_1 };
SevenSegmentController* ss_arg_x = &ss_x;
SevenSegmentController* ss_arg_y = &ss_y;
#endif // FRDM

void SendInitialConfiguration(WirelessConnection& wc)
{
    wc.HostPair();
    wc.ForwardTime(600);
    wc.TurnTime(400);
    wc.SetSpeed(1.0);
    wc.SetOrientation(0);
}

int main()
{
    PCCommunication pc;
    pc.Write("Host starting\n");
    ThisThread::sleep_for(250ms);

    WirelessConnection wc { true };
    CLI cli(pc, wc);

    pc.Write("Wireless Controller initialized\n");
    ThisThread::sleep_for(250ms);

    HostControl host(pc, wc, ss_arg_x, ss_arg_y);

    // SendInitialConfiguration(wc);

    while (true) {
        cli.Update();
        wc.Update();
    }
}

