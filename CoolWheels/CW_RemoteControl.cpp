#include "CW_RemoteControl.h"

#define trace printf

void RemoteControl::OnHostPair() 
{ 
    wc.Ack(0);    
    handshake = 1;
    trace("Recv PAIR\n");
}

void RemoteControl::OnAck(unsigned seq) {
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

void RemoteControl::OnSync(unsigned seq)
{
    wc.Ack(seq);
    if (handshake)
    {
        handshake = seq + 1;
        wc.Sync(seq + 1);
    }
    trace("Recv SYNC: %d\n", seq);
}

void RemoteControl::OnBadCommand(unsigned seq) 
{
    trace("Recv Bad command: %d\n", seq);
}

void RemoteControl::OnNotImpl(WirelessConnection::eProtocolCommand command)
{
    trace("Recv NotImpl: %d\n", (unsigned)command);
}

void RemoteControl::OnOK(unsigned seq) 
{
    trace("OK: %d\n", seq);
}

void RemoteControl::OnDrop(unsigned seq)
{
     wc.NotImplemented(WirelessConnection::eProto_Drop);
     trace("Recv DROP: %d\n", seq);
}

void RemoteControl::OnEcho(const std::string str)
{
    wc.Echo(str.c_str(), str.length());
    trace("Recv ECHO: %s\n", str.c_str());
}

void RemoteControl::OnPing(unsigned id) 
{
    wc.Ping(id);
    trace("Recv PING: %d\n", id);
}

void RemoteControl::OnWaiting()
{ 
    wc.BadCommand(WirelessConnection::eProto_Waiting);
    trace("Recv WAITING\n");
}

// Movement protocol
void RemoteControl::OnSetLeftWheelBias(float bias) 
{
    mc.SetLeftWheelBias(bias);
    wc.OK(0);
    trace("Recv LEFTBIAS: %f\n", bias);
}

void RemoteControl::OnSetRightWheelBias(float bias)
{
    mc.SetRightWheelBias(bias);    
    wc.OK(0);
    trace("Recv RIGHTBIAS: %f\n", bias);
}

void RemoteControl::OnForward(unsigned ms)
{
    wc.OK(0);
    mc.Forward(ms);
    trace("Recv Forward: %d ms\n", ms);
}

void RemoteControl::OnReverse(unsigned ms)
{
    wc.OK(0);
    mc.Reverse(ms);
    trace("Recv Reverse: %d ms\n", ms);
}

void RemoteControl::OnTurnLeftDegrees(unsigned degrees) 
{
    mc.TurnLeftDegrees(degrees);
    wc.OK(0);
    trace("Recv TurnLeftD: %d\n", degrees);
}

void RemoteControl::OnTurnLeftTimed(unsigned ms)
{
    mc.TurnLeftTimed(ms);
    wc.OK(0);
    trace("Recv TurnLeftT: %d\n", ms);
}

void RemoteControl::OnTurnRightDegrees(unsigned degrees)
{
    mc.TurnRightDegrees(degrees);
    wc.OK(0);
    trace("Recv TurnRightD: %d\n", degrees);
}

void RemoteControl::OnTurnRightTimed(unsigned ms) 
{ 
    mc.TurnRightTimed(ms);
    wc.OK(0);
    trace("Recv TurnRightT: %d\n", ms);
}

void RemoteControl::OnSetSpeed(float speed) 
{
    trace("Setting speed: %f\n", speed);
    mc.SetSpeed(speed);
    trace("Recv SetSpeed: %f\n", speed);
}

void RemoteControl::OnGetSpeed() 
{
    trace("Recv GetSpeed\n");
    wc.SetSpeed(mc.GetSpeed());
}

void RemoteControl::OnGo()
{
    wc.OK(0);
    nav.Go();
    trace("Recv GO\n");
}

void RemoteControl::OnStop() 
{ 
    mc.Stop();
    printf("STOPPED\n");
    wc.OK(0);
}

// Positioning protocol
void RemoteControl::OnSetPosition(unsigned xPos, unsigned yPos) 
{
    nav.SetPosition(xPos, yPos);
    wc.OK(0);
    trace("Recv SetPos: %d, %d\n", xPos, yPos);
}

void RemoteControl::OnGetPosition() 
{
    unsigned xPos, yPos;
    nav.GetPosition(xPos, yPos);
    wc.SetPosition(xPos, yPos);
    trace("Recv GetPos\n");
}

void RemoteControl::OnSetOrientation(float radians) 
{ 
    nav.SetOrientation(radians);
    wc.OK(0);
    trace("Recv SetOrientation: %f\n", radians);
}

void RemoteControl::OnGetOrientation() 
{ 
    float o = nav.GetOrientation();
    wc.SetOrientation(o);
    trace("Recv GetOrientation\n");
}

// Navigation
void RemoteControl::OnSetDestination(unsigned xPos, unsigned yPos) 
{ 
    nav.SetDestination(xPos, yPos);
    wc.OK(0);
    trace("Recv SetDest: %d, %d\n", xPos, yPos);
}

void RemoteControl::OnGetDestination() 
{ 
    unsigned xPos, yPos;
    nav.GetDestination(xPos, yPos);
    wc.SetDestination(xPos, yPos);
    trace("Recv GetDest\n");
}

void RemoteControl::OnRequestPath() 
{ 
    auto nodes = nav.GetPath();
    unsigned step{ 0 };
    for (const auto& pair: nodes)
    {
        wc.RelayPath(++step, pair.first, pair.second);
        wait_us(50);
    }
    trace("Recv Request Path\n");
}

void RemoteControl::OnRelayPath(unsigned step, unsigned nodeX, unsigned nodeY)
{ 
    wc.BadCommand(WirelessConnection::eProto_RelayPath); 
    trace("Recv RelayPath: step: %d, (%d, %d)\nREPLY: BAD COMMAND\n", step, nodeX, nodeY);
}

void RemoteControl::OnNotifyObstacle(unsigned xPos, unsigned yPos) 
{ 
    nav.AddObstacle(xPos, yPos);
    wc.OK(0);
    trace("Recv NotifyObstacle: %d, %d", xPos, yPos);
}

void RemoteControl::OnTurnTime(unsigned ms)
{
    mc.SetTurnTimeMs(ms);
    wc.OK(0);
    trace("Recv TurnTime: %d\n", ms);
}

void RemoteControl::OnForwardTime(unsigned ms)
{
    mc.SetForwardTimeMs(ms);
    wc.OK(0);
    trace("Recv ForwardTime: %d\n", ms);
}
