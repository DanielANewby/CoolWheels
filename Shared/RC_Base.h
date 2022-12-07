#ifndef RC_BASE_H
#define RC_BASE_H
#pragma once

#include "..\Shared\WirelessConnection.h"
#include "..\Shared\CWSignal.h"

class RC_Base
{
public:
    RC_Base(WirelessConnection& conn):wc(conn),
    Event_HostPair { wc.Recv_HostPair, this, &RC_Base::OnHostPair },
    Event_Ack { wc.Recv_Ack, this, &RC_Base::OnAck },
    Event_Sync{ wc.Recv_Sync, this, &RC_Base::OnSync },
    Event_BadCommand { wc.Recv_BadCommand, this, &RC_Base::OnBadCommand },
    Event_NotImplemented { wc.Recv_NotImplemented, this, &RC_Base::OnNotImpl },
    Event_OK{ wc.Recv_OK, this, &RC_Base::OnOK },
    Event_Drop{ wc.Recv_Drop, this, &RC_Base::OnDrop },
    Event_Echo{ wc.Recv_Echo, this, &RC_Base::OnEcho },
    Event_Ping{ wc.Recv_Ping, this, &RC_Base::OnPing },
    Event_Waiting { wc.Recv_Waiting, this, &RC_Base::OnWaiting },
    Event_SetLeftWheelBias { wc.Recv_SetLeftWheelBias, this, &RC_Base::OnSetLeftWheelBias },
    Event_SetRightWheelBias { wc.Recv_SetRightWheelBias, this, &RC_Base::OnSetRightWheelBias },
    Event_Forward{ wc.Recv_Forward, this, &RC_Base::OnForward },
    Event_Reverse{ wc.Recv_Reverse, this, &RC_Base::OnReverse },
    Event_TurnLeftDegrees{ wc.Recv_TurnLeftDegrees, this, &RC_Base::OnTurnLeftDegrees },
    Event_TurnLeftTimed { wc.Recv_TurnLeftTimed, this, &RC_Base::OnTurnLeftTimed },
    Event_TurnRightDegrees { wc.Recv_TurnRightDegrees, this, &RC_Base::OnTurnRightDegrees },
    Event_TurnRightTimed { wc.Recv_TurnRightTimed, this, &RC_Base::OnTurnRightTimed },
    Event_SetSpeed { wc.Recv_SetSpeed, this, &RC_Base::OnSetSpeed },
    Event_GetSpeed { wc.Recv_GetSpeed, this, &RC_Base::OnGetSpeed },
    Event_Go{ wc.Recv_Go, this, &RC_Base::OnGo },
    Event_Stop { wc.Recv_Stop, this, &RC_Base::OnStop },
    Event_SetPosition { wc.Recv_SetPosition, this, &RC_Base::OnSetPosition },
    Event_GetPosition { wc.Recv_GetPosition, this, &RC_Base::OnGetPosition },
    Event_SetOrientation { wc.Recv_SetOrientation, this, &RC_Base::OnSetOrientation },
    Event_GetOrientation { wc.Recv_GetOrientation, this, &RC_Base::OnGetOrientation },
    Event_SetDestination { wc.Recv_SetDestination, this, &RC_Base::OnSetDestination },
    Event_GetDestination { wc.Recv_GetDestination, this, &RC_Base::OnGetDestination },
    Event_RequestPath { wc.Recv_RequestPath, this, &RC_Base::OnRequestPath },
    Event_RelayPath { wc.Recv_RelayPath, this, &RC_Base::OnRelayPath },
    Event_NotifyObstacle { wc.Recv_NotifyObstacle, this, &RC_Base::OnNotifyObstacle },
    Event_TurnTime { wc.Recv_TurnTime, this, &RC_Base::OnTurnTime },
    Event_ForwardTime { wc.Recv_ForwardTime, this, &RC_Base::OnForwardTime}
    {}

    virtual ~RC_Base() {}

    virtual void OnHostPair() = 0;
    virtual void OnAck(unsigned seq) = 0;
    virtual void OnSync(unsigned seq) = 0;
    virtual void OnBadCommand(unsigned seq) = 0;
    virtual void OnNotImpl(WirelessConnection::eProtocolCommand command) = 0;
    virtual void OnOK(unsigned seq) = 0;
    virtual void OnDrop(unsigned seq) = 0;
    virtual void OnEcho(const std::string) = 0;
    virtual void OnPing(unsigned id) = 0;
    virtual void OnWaiting() = 0;

    // Movement protocol
    virtual void OnSetLeftWheelBias(float bias) = 0;
    virtual void OnSetRightWheelBias(float bias) = 0;
    virtual void OnForward(unsigned ms) = 0;
    virtual void OnReverse(unsigned ms) = 0;
    virtual void OnTurnLeftDegrees(unsigned degrees) = 0;
    virtual void OnTurnLeftTimed(unsigned ms) = 0;
    virtual void OnTurnRightDegrees(unsigned degrees) = 0;
    virtual void OnTurnRightTimed(unsigned ms) = 0;
    virtual void OnSetSpeed(float speed) = 0;
    virtual void OnGetSpeed() = 0;
    virtual void OnGo() = 0;
    virtual void OnStop() = 0;

    // Positioning protocol
    virtual void OnSetPosition(unsigned xPos, unsigned yPos) = 0;
    virtual void OnGetPosition() = 0;
    virtual void OnSetOrientation(float radians) = 0;
    virtual void OnGetOrientation() = 0;

    // Navigation
    virtual void OnSetDestination(unsigned xPos, unsigned yPos) = 0;
    virtual void OnGetDestination() = 0;
    virtual void OnRequestPath() = 0;
    virtual void OnRelayPath(unsigned step, unsigned nodeX, unsigned nodeY) = 0;
    virtual void OnNotifyObstacle(unsigned xPos, unsigned yPos) = 0;

    virtual void OnTurnTime(unsigned ms) = 0;
    virtual void OnForwardTime(unsigned ms) = 0;

protected:
    WirelessConnection& wc;

    SignalReceiver Event_HostPair;
    SignalReceiver Event_Ack;
    SignalReceiver Event_Sync;
    SignalReceiver Event_BadCommand;
    SignalReceiver Event_NotImplemented;
    SignalReceiver Event_OK;
    SignalReceiver Event_Drop;
    SignalReceiver Event_Echo;
    SignalReceiver Event_Ping;
    SignalReceiver Event_Waiting;

    SignalReceiver Event_SetLeftWheelBias;
    SignalReceiver Event_SetRightWheelBias;
    SignalReceiver Event_Forward;
    SignalReceiver Event_Reverse;
    SignalReceiver Event_TurnLeftDegrees;
    SignalReceiver Event_TurnLeftTimed;
    SignalReceiver Event_TurnRightDegrees;
    SignalReceiver Event_TurnRightTimed;
    SignalReceiver Event_SetSpeed;
    SignalReceiver Event_GetSpeed;
    SignalReceiver Event_Go;
    SignalReceiver Event_Stop;

    SignalReceiver Event_SetPosition;
    SignalReceiver Event_GetPosition;
    SignalReceiver Event_SetOrientation;
    SignalReceiver Event_GetOrientation;

    SignalReceiver Event_SetDestination;
    SignalReceiver Event_GetDestination;
    SignalReceiver Event_RequestPath;
    SignalReceiver Event_RelayPath;
    SignalReceiver Event_NotifyObstacle;

    SignalReceiver Event_TurnTime;
    SignalReceiver Event_ForwardTime;
};

#endif // RC_BASE_H
