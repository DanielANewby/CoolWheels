#include "HostControl.h"

#include "..\Shared\Config.h"

#ifdef TRACE_PROTO
#define trace printf
#else
#define trace(...)
#endif

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
    wc.Echo(str.c_str(), str.length());
    trace("Recv ECHO: %s\n", str.c_str());
}

void HostControl::OnPing(unsigned id) 
{
    wc.Ping(id);
    trace("Recv PING: %d\n", id);
}

void HostControl::OnWaiting()
{ 
    wc.BadCommand(WirelessConnection::eProto_Waiting);
    trace("Recv WAITING\n");
}

// Movement protocol
void HostControl::OnSetLeftWheelBias(float bias) 
{
     wc.NotImplemented(WirelessConnection::eProto_LeftBias);
     trace("Recv LEFTBIAS: %f"\n, bias);
}

void HostControl::OnSetRightWheelBias(float bias)
{
    wc.NotImplemented(WirelessConnection::eProto_RightBias);
    trace("Recv RIGHTBIAS: $f\n", bias);
}

void HostControl::OnTurnLeftDegrees(unsigned degrees) 
{
    wc.NotImplemented(WirelessConnection::eProto_TurnLeftD);
    trace("Recv TurnLeftD: %d\n", degrees);
}

void HostControl::OnTurnLeftTimed(unsigned ms)
{
    wc.NotImplemented(WirelessConnection::eProto_TurnLeftT);
    trace("Recv TurnLeftT: %d\n", ms);
}

void HostControl::OnTurnRightDegrees(unsigned degrees)
{
    wc.NotImplemented(WirelessConnection::eProto_TurnRightD);
    trace("Recv TurnRightD: %d\n", degrees);
}

void HostControl::OnTurnRightTimed(unsigned ms) 
{ 
    wc.NotImplemented(WirelessConnection::eProto_TurnRightT); 
    trace("Recv TurnRightT: %d\n", ms);
}

void HostControl::OnSetSpeed(float speed) 
{ 
    wc.NotImplemented(WirelessConnection::eProto_SetSpeed); 
    trace("Recv SetSpeed: %f\n", speed);
}

void HostControl::OnGetSpeed() 
{
    wc.NotImplemented(WirelessConnection::eProto_GetSpeed);
    trace("Recv GetSpeed\n");
}

void HostControl::OnGo()
{
    wc.NotImplemented(WirelessConnection::eProto_Go);
    trace("Recv Go\n");
}

void HostControl::OnStop() 
{ 
    wc.NotImplemented(WirelessConnection::eProto_Stop); 
    trace("Recv STOP\n");
}

// Positioning protocol
void HostControl::OnSetPosition(unsigned xPos, unsigned yPos) 
{ 
    wc.NotImplemented(WirelessConnection::eProto_SetPosition); 
    trace("Recv SetPos: %d, %y\n", xPos, yPos);
}

void HostControl::OnGetPosition() 
{ 
    wc.NotImplemented(WirelessConnection::eProto_GetPosition); 
    trace("Recv GetPos\n");
}

void HostControl::OnSetOrientation(float radians) 
{ 
    wc.NotImplemented(WirelessConnection::eProto_SetOrientation); 
    trace("Recv SetOrientation: %f\n", radians);
}

void HostControl::OnGetOrientation() 
{ 
    wc.NotImplemented(WirelessConnection::eProto_GetOrientation); 
    trace("Recv GetOrientation\n");
}

// Navigation
void HostControl::OnSetDestination(unsigned xPos, unsigned yPos) 
{ 
    wc.NotImplemented(WirelessConnection::eProto_SetDest); 
    trace("Recv SetDest: %d, %d\n", xPos, yPos);
}

void HostControl::OnGetDestination() 
{ 
    wc.NotImplemented(WirelessConnection::eProto_GetDest); 
    trace("Recv GetDest\n");
}

void HostControl::OnRequestPath() 
{ 
    wc.NotImplemented(WirelessConnection::eProto_RequestPath); 
    trace("Recv Request Path\n");
}

void HostControl::OnRelayPath(unsigned step, unsigned nodeX, unsigned nodeY)
{ 
    wc.NotImplemented(WirelessConnection::eProto_RelayPath); 
    trace("Recv RelayPath: step: %d, (%d, %d)\n", step, nodeX, nodeY);
}

void HostControl::OnNotifyObstacle(unsigned xPos, unsigned yPos) 
{ 
    wc.NotImplemented(WirelessConnection::eProto_ObstacleNotify); 
    trace("Recv NotifyObstacle: %d, %d", xPos, yPos);
}
