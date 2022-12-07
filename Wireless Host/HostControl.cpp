#include "HostControl.h"

#define trace printf

void HostControl::OnHostPair() 
{ 
    wc.Ack(0);    
    handshake = 1;
    trace("Recv PAIR\n");
}

void HostControl::OnAck(unsigned seq) {
    if (handshake)
    {
        if (seq == handshake)
        {
            wc.SetSequenceCounter(seq);
        }
        handshake = 0;
    }
    trace("Recv ACK: %d\n", seq);
}

void HostControl::OnSync(unsigned seq)
{
    wc.Ack(seq);
    if (handshake)
    {
        handshake = seq + 1;
        wc.Sync(seq + 1);
    }
    trace("Recv SYNC: %d\n", seq);
}

void HostControl::OnBadCommand(unsigned seq) 
{
    trace("Recv Bad command: %d\n", seq);
}

void HostControl::OnNotImpl(WirelessConnection::eProtocolCommand command)
{
    trace("Recv NotImpl: %d\n", (unsigned)command);
}

void HostControl::OnOK(unsigned seq) 
{
    trace("OK: %d\n", seq);
}

void HostControl::OnDrop(unsigned seq)
{
     wc.NotImplemented(WirelessConnection::eProto_Drop);
     trace("Recv DROP: %d\n", seq);
}

void HostControl::OnEcho(const std::string str)
{
    trace("Recv ECHO: %s\n", str.c_str());
    printf("%s\n", str.c_str());
}

void HostControl::OnPing(unsigned id) 
{
    trace("Recv PING: %d\n", id);
}

void HostControl::OnWaiting()
{ 
    wc.OK(0);
    trace("Recv WAITING\n");
}

// Movement protocol
void HostControl::OnSetLeftWheelBias(float bias) 
{
     wc.OK(0);
     trace("Recv LEFTBIAS: %f\n", bias);
}

void HostControl::OnSetRightWheelBias(float bias)
{
    wc.OK(0);
    //wc.NotImplemented(WirelessConnection::eProto_RightBias);
    trace("Recv RIGHTBIAS: %f\n", bias);
}

void HostControl::OnForward(unsigned ms)
{
    wc.BadCommand(WirelessConnection::eProto_Forward);
    trace("Recv Forward: %d\n", ms);
}

void HostControl::OnReverse(unsigned ms)
{
    wc.BadCommand(WirelessConnection::eProto_Reverse);
    trace("Recv Reverse: %d\n", ms);
}

void HostControl::OnTurnLeftDegrees(unsigned degrees) 
{
    wc.BadCommand(WirelessConnection::eProto_TurnLeftD);
    trace("Recv TurnLeftD: %d\n", degrees);
}

void HostControl::OnTurnLeftTimed(unsigned ms)
{
    wc.BadCommand(WirelessConnection::eProto_TurnLeftT);
    trace("Recv TurnLeftT: %d\n", ms);
}

void HostControl::OnTurnRightDegrees(unsigned degrees)
{
    wc.BadCommand(WirelessConnection::eProto_TurnRightD);
    trace("Recv TurnRightD: %d\n", degrees);
}

void HostControl::OnTurnRightTimed(unsigned ms) 
{ 
    wc.BadCommand(WirelessConnection::eProto_TurnRightT); 
    trace("Recv TurnRightT: %d\n", ms);
}

void HostControl::OnSetSpeed(float speed) 
{ 
    wc.OK(0);
    trace("Recv SetSpeed: %f\n", speed);
}

void HostControl::OnGetSpeed() 
{
    wc.BadCommand(WirelessConnection::eProto_GetSpeed);
    trace("Recv GetSpeed\n");
}

void HostControl::OnGo()
{
    wc.BadCommand(WirelessConnection::eProto_Go);
    trace("Recv Go\n");
}

void HostControl::OnStop() 
{ 
    //wc.NotImplemented(WirelessConnection::eProto_Stop); 
    wc.OK(0);
    trace("Recv STOP\n");
}

// Positioning protocol
void HostControl::OnSetPosition(unsigned xPos, unsigned yPos) 
{ 
    wc.OK(0);
    if (xPos > 8) xPos = 9;
    if (yPos > 8) yPos = 9;
    if (ssx)
       ssx->Put(xPos);
    if (ssy)
        ssy->Put(yPos);
    trace("Recv SetPos: %d, %d\n", xPos, yPos);
}

void HostControl::OnGetPosition() 
{ 
    wc.BadCommand(WirelessConnection::eProto_GetPosition); 
    trace("Recv GetPos\n");
}

void HostControl::OnSetOrientation(float radians) 
{ 
    wc.OK(0);
    trace("Recv SetOrientation: %f\n", radians);
}

void HostControl::OnGetOrientation() 
{ 
    wc.BadCommand(WirelessConnection::eProto_GetOrientation); 
    trace("Recv GetOrientation\n");
}

// Navigation
void HostControl::OnSetDestination(unsigned xPos, unsigned yPos) 
{ 
    wc.OK(0);
    trace("Recv SetDest: %d, %d\n", xPos, yPos);
}

void HostControl::OnGetDestination() 
{ 
    wc.BadCommand(WirelessConnection::eProto_GetDest); 
    trace("Recv GetDest\n");
}

void HostControl::OnRequestPath() 
{ 
    wc.BadCommand(WirelessConnection::eProto_RequestPath);
    trace("Recv Request Path\n");
}

void HostControl::OnRelayPath(unsigned step, unsigned nodeX, unsigned nodeY)
{ 
    wc.OK(0);
    trace("Recv RelayPath: step: %d, (%d, %d)\n", step, nodeX, nodeY);
}

void HostControl::OnNotifyObstacle(unsigned xPos, unsigned yPos) 
{ 
    wc.OK(0);
    trace("Recv NotifyObstacle: %d, %d", xPos, yPos);
}

void HostControl::OnTurnTime(unsigned ms)
{
    wc.BadCommand(WirelessConnection::eProto_TurnTime);
    trace("Recv TurnTime: %d\n", ms);
}

void HostControl::OnForwardTime(unsigned ms)
{
    wc.BadCommand(WirelessConnection::eProto_ForwardTime);
    trace("Recv ForwardTime: %d\n", ms);
}

void HostControl::OnStep()
{
    wc.BadCommand(WirelessConnection::eProto_Step);
    trace("Recv Step\n");
}