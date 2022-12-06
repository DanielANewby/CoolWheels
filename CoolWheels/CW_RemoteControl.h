#ifndef CW_REMOTE_CONTROL_H
#define CW_REMOTE_CONTROL_H
#pragma once

#include "..\Shared\RC_Base.h"
#include "..\Shared\WirelessConnection.h"
#include "CW_MotionController.h"

class RemoteControl : public RC_Base
{
public:
    RemoteControl(WirelessConnection& conn, MotionController& _mc):
    RC_Base(conn),
    mc(_mc)
    {
        printf("Remote Control created\n");
    }

    void OnHostPair();
    void OnAck(unsigned seq);
    void OnSync(unsigned seq);
    void OnBadCommand(unsigned seq);
    void OnNotImpl(WirelessConnection::eProtocolCommand command);
    void OnOK(unsigned seq);
    void OnDrop(unsigned seq);
    void OnEcho(const std::string);
    void OnPing(unsigned id);
    void OnWaiting();

    // Movement protocol
    void OnSetLeftWheelBias(float bias);
    void OnSetRightWheelBias(float bias);
    void OnTurnLeftDegrees(unsigned degrees);
    void OnTurnLeftTimed(unsigned ms);
    void OnTurnRightDegrees(unsigned degrees);
    void OnTurnRightTimed(unsigned ms);
    void OnSetSpeed(float speed);
    void OnGetSpeed();
    void OnGo();
    void OnStop();

    // Positioning protocol
    void OnSetPosition(unsigned xPos, unsigned yPos);
    void OnGetPosition();
    void OnSetOrientation(float radians);
    void OnGetOrientation();

    // Navigation
    void OnSetDestination(unsigned xPos, unsigned yPos);
    void OnGetDestination();
    void OnRequestPath();
    void OnRelayPath(unsigned step, unsigned nodeX, unsigned nodeY);
    void OnNotifyObstacle(unsigned xPos, unsigned yPos);

private:
    MotionController& mc;
    unsigned handshake { 0 };
};

#endif //
