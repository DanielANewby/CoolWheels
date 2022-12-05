#ifndef WIRELESS_CONNECTION_H
#define WIRELESS_CONNECTION_H
#pragma once

#include <string>
#include <memory>

#include "WirelessController.h"
#include "CWSignal.h"

class WirelessConnection
{
public:
    enum eProtocolCommand {
        eProto_NOTHING,
        eProto_HostPair,
        eProto_Ack,
        eProto_Sync,
        eProto_BadCommand,
        eProto_NotImpl,
        eProto_OK,
        eProto_Drop,
        eProto_Echo,
        eProto_Ping,
        eProto_Waiting,
        eProto_LeftBias,
        eProto_RightBias,
        eProto_TurnLeftD,
        eProto_TurnLeftT,
        eProto_TurnRightD,
        eProto_TurnRightT,
        eProto_SetSpeed,
        eProto_GetSpeed,
        eProto_Go,
        eProto_Stop,
        eProto_SetPosition,
        eProto_GetPosition,
        eProto_SetOrientation,
        eProto_GetOrientation,
        eProto_SetDest,
        eProto_GetDest,
        eProto_RequestPath,
        eProto_RelayPath,
        eProto_ObstacleNotify,

        eProto_Count,
        eProto_Invalid,
        eProto_Error = 0xFFFFFFFF // Force 32-bit
    };
    
    WirelessConnection(bool host):
    wc{ 
        host ? WirelessController::HostIdentifier : WirelessController::BotIdentifier,
        host ? WirelessController::BotIdentifier : WirelessController::HostIdentifier,
        host ? WirelessController::HostAddress : WirelessController::HostAddress
    }
    {}

    void SetSequenceCounter(unsigned seq) { sequenceCounter = seq; }
    
    void Update()
    {
        if (wc.Readable())
        {
            Datagram buffer;
            memset(&buffer, 0, sizeof(buffer));
            wc.Recv((char*)&buffer, sizeof(buffer));
            if (buffer.command != eProto_Ack)
            {
                Ack(buffer.seq);
                if (buffer.seq > sequenceCounter)
                    sequenceCounter = buffer.seq;
            }
            Dispatch(std::move(buffer));
        }
    }

    ///////////////////////////////////////////////////////////////////////////
    // Send commands
    ///////////////////////////////////////////////////////////////////////////
    void SendRaw(eProtocolCommand, const char* params, unsigned length, unsigned seq = 0);

    // Base protocol signals
    void HostPair();
    void Ack(unsigned seq);
    void Sync(unsigned seq);
    void BadCommand(unsigned seq);
    void NotImplemented(eProtocolCommand command);
    void OK(unsigned seq);
    void Drop(unsigned seq);
    void Echo(const char* data, unsigned length);
    void Ping(unsigned id);
    void Waiting();

    // Movement protocol
    void SetLeftWheelBias(float bias);
    void SetRightWheelBias(float bias);
    void TurnLeftDegrees(unsigned degrees);
    void TurnLeftTimed(unsigned ms);
    void TurnRightDegrees(unsigned degrees);
    void TurnRightTimed(unsigned ms);
    void SetSpeed(float speed);
    void GetSpeed();
    void Go();
    void Stop();

    // Positioning protocol
    void SetPosition(unsigned xPos, unsigned yPos);
    void GetPosition();
    void SetOrientation(float radians);
    void GetOrientation();

    // Navigation
    void SetDestination(unsigned xPos, unsigned yPos);
    void GetDestination();
    void RequestPath();
    void RelayPath(unsigned step, unsigned nodeX, unsigned nodeY);
    void NotifyObstacle(unsigned xPos, unsigned yPos);

    ///////////////////////////////////////////////////////////////////////////
    // Receive signals
    ///////////////////////////////////////////////////////////////////////////
    Signal<> Recv_HostPair;
    Signal<unsigned> Recv_Ack;
    Signal<unsigned> Recv_Sync;
    Signal<unsigned> Recv_BadCommand;
    Signal<eProtocolCommand> Recv_NotImplemented;
    Signal<unsigned> Recv_OK;
    Signal<unsigned> Recv_Drop;
    Signal<std::string> Recv_Echo;
    Signal<unsigned> Recv_Ping;
    Signal<> Recv_Waiting;

    Signal<float> Recv_SetLeftWheelBias;
    Signal<float> Recv_SetRightWheelBias;
    Signal<unsigned> Recv_TurnLeftDegrees;
    Signal<unsigned> Recv_TurnLeftTimed;
    Signal<unsigned> Recv_TurnRightDegrees;
    Signal<unsigned> Recv_TurnRightTimed;
    Signal<float> Recv_SetSpeed;
    Signal<> Recv_GetSpeed;
    Signal<> Recv_Go;
    Signal<> Recv_Stop;

    Signal<unsigned, unsigned> Recv_SetPosition;
    Signal<> Recv_GetPosition;
    Signal<float> Recv_SetOrientation;
    Signal<> Recv_GetOrientation;

    Signal<unsigned, unsigned> Recv_SetDestination;
    Signal<> Recv_GetDestination;
    Signal<> Recv_RequestPath;
    Signal<unsigned, unsigned, unsigned> Recv_RelayPath;
    Signal<unsigned, unsigned> Recv_NotifyObstacle;

private:
    struct Datagram {
        unsigned seq;
        eProtocolCommand command;

        // This is larger than necessary so we always have a crash pad or
        // trailing null set
        char payload[24];
    };

    // Some helper functions
    template <typename T> size_t convert_to_bytes(char* dst, T value) { 
        memcpy(dst, (char*)(&value), sizeof(T));
        return sizeof(T);
    }

    void Dispatch_Nothing(char*) {};
    void Dispatch_HostPair(char* payload);
    void Dispatch_Ack(char* payload);
    void Dispatch_Sync(char* payload);
    void Dispatch_BadCommand(char* payload);
    void Dispatch_NotImplemented(char* payload);
    void Dispatch_OK(char* payload);
    void Dispatch_Drop(char* payload);
    void Dispatch_Echo(char* payload);
    void Dispatch_Ping(char* payload);
    void Dispatch_Waiting(char* payload);
    void Dispatch_SetLeftWheelBias(char* payload);
    void Dispatch_SetRightWheelBias(char* payload);
    void Dispatch_TurnLeftDegrees(char* payload);
    void Dispatch_TurnLeftTimed(char* payload);
    void Dispatch_TurnRightDegrees(char* payload);
    void Dispatch_TurnRightTimed(char* payload);
    void Dispatch_SetSpeed(char* payload);
    void Dispatch_GetSpeed(char* payload);
    void Dispatch_Go(char* payload);
    void Dispatch_Stop(char* payload);
    void Dispatch_SetPosition(char* payload);
    void Dispatch_GetPosition(char* payload);
    void Dispatch_SetOrientation(char* payload);
    void Dispatch_GetOrientation(char* payload);
    void Dispatch_SetDestination(char* payload);
    void Dispatch_GetDestination(char* payload);
    void Dispatch_RequestPath(char* payload);
    void Dispatch_RelayPath(char* payload);
    void Dispatch_NotifyObstacle(char* payload);
    
    void Dispatch(Datagram&& dg)
    {
        typedef void (WirelessConnection::*fptr_type)(char*);
        constexpr fptr_type fptrs[eProto_Count] = {
            &WirelessConnection::Dispatch_Nothing,
            &WirelessConnection::Dispatch_HostPair,
            &WirelessConnection::Dispatch_Ack,
            &WirelessConnection::Dispatch_Sync,
            &WirelessConnection::Dispatch_BadCommand,
            &WirelessConnection::Dispatch_NotImplemented,
            &WirelessConnection::Dispatch_OK,
            &WirelessConnection::Dispatch_Drop,
            &WirelessConnection::Dispatch_Echo,
            &WirelessConnection::Dispatch_Ping,
            &WirelessConnection::Dispatch_Waiting,
            &WirelessConnection::Dispatch_SetLeftWheelBias,
            &WirelessConnection::Dispatch_SetRightWheelBias,
            &WirelessConnection::Dispatch_TurnLeftDegrees,
            &WirelessConnection::Dispatch_TurnLeftTimed,
            &WirelessConnection::Dispatch_TurnRightDegrees,
            &WirelessConnection::Dispatch_TurnRightTimed,
            &WirelessConnection::Dispatch_SetSpeed,
            &WirelessConnection::Dispatch_GetSpeed,
            &WirelessConnection::Dispatch_Go,
            &WirelessConnection::Dispatch_Stop,
            &WirelessConnection::Dispatch_SetPosition,
            &WirelessConnection::Dispatch_GetPosition,
            &WirelessConnection::Dispatch_SetOrientation,
            &WirelessConnection::Dispatch_GetOrientation,
            &WirelessConnection::Dispatch_SetDestination,
            &WirelessConnection::Dispatch_GetDestination,
            &WirelessConnection::Dispatch_RequestPath,
            &WirelessConnection::Dispatch_RelayPath,
            &WirelessConnection::Dispatch_NotifyObstacle,
        };

        if (dg.command < eProto_Count)
        {
            (this->*fptrs[dg.command])(dg.payload);
        }
        else
        {
            // Handle errors
        };
    }

    WirelessController wc;
    unsigned sequenceCounter{ 0 };
};

inline void WirelessConnection::Dispatch_HostPair(char* payload) 
{
    Recv_HostPair();
}

inline void WirelessConnection::Dispatch_Ack(char* payload)
{
    struct Params { unsigned a; } p(*(Params*)payload);
    Recv_Ack(p.a);
}

inline void WirelessConnection::Dispatch_Sync(char* payload)
{
    struct Params { unsigned a; } p(*(Params*)payload);
    Recv_Sync(p.a);
}

inline void WirelessConnection::Dispatch_BadCommand(char* payload) 
{
    struct Params { unsigned a; } p(*(Params*)payload);
    Recv_BadCommand(p.a);
}

inline void WirelessConnection::Dispatch_NotImplemented(char* payload) 
{
    Recv_NotImplemented(*(eProtocolCommand*)payload);
}

inline void WirelessConnection::Dispatch_OK(char* payload) 
{
    struct Params { unsigned a; } p(*(Params*)payload);
    Recv_OK(p.a);
}

inline void WirelessConnection::Dispatch_Drop(char* payload) 
{
    struct Params { unsigned a; } p(*(Params*)payload);
    Recv_Drop(p.a);
}

inline void WirelessConnection::Dispatch_Echo(char* payload)
{
    Recv_Echo(payload);
}

inline void WirelessConnection::Dispatch_Ping(char* payload) 
{
    struct Params { unsigned a; } p(*(Params*)payload);
    Recv_Ping(p.a);
}

inline void WirelessConnection::Dispatch_Waiting(char* payload) 
{
    Recv_Waiting();
}

inline void WirelessConnection::Dispatch_SetLeftWheelBias(char* payload) 
{
    struct Params { float a; } p(*(Params*)payload);
    Recv_SetLeftWheelBias(p.a);
}

inline void WirelessConnection::Dispatch_SetRightWheelBias(char* payload) 
{
    struct Params { float a; } p(*(Params*)payload);
    Recv_SetRightWheelBias(p.a);
}

inline void WirelessConnection::Dispatch_TurnLeftDegrees(char* payload) 
{
    struct Params { unsigned a; } p(*(Params*)payload);
    Recv_TurnLeftDegrees(p.a);
}

inline void WirelessConnection::Dispatch_TurnLeftTimed(char* payload) 
{
    struct Params { unsigned a; } p(*(Params*)payload);    
    Recv_TurnLeftTimed(p.a);
}

inline void WirelessConnection::Dispatch_TurnRightDegrees(char* payload)
{
    struct Params { unsigned a; } p(*(Params*)payload);
    Recv_TurnRightDegrees(p.a);
}

inline void WirelessConnection::Dispatch_TurnRightTimed(char* payload) 
{
    struct Params { unsigned a; } p(*(Params*)payload);
    Recv_TurnRightTimed(p.a);
}

inline void WirelessConnection::Dispatch_SetSpeed(char* payload) 
{
    struct Params { float a; } p(*(Params*)payload);
    Recv_SetSpeed(p.a);
}

inline void WirelessConnection::Dispatch_GetSpeed(char* payload) 
{
    Recv_GetSpeed();    
}

inline void WirelessConnection::Dispatch_Go(char* payload) 
{
    Recv_Go();
}

inline void WirelessConnection::Dispatch_Stop(char* payload) 
{
    Recv_Stop();
}

inline void WirelessConnection::Dispatch_SetPosition(char* payload) 
{
    struct Params { unsigned a, b; } p(*(Params*)payload);
    Recv_SetPosition(p.a, p.b);
}

inline void WirelessConnection::Dispatch_GetPosition(char* payload) 
{
    Recv_GetOrientation();    
}

inline void WirelessConnection::Dispatch_SetOrientation(char* payload)
{
    struct Params { float a; } p(*(Params*)payload);
    Recv_SetOrientation(p.a);
}

inline void WirelessConnection::Dispatch_GetOrientation(char* payload) 
{
    Recv_GetOrientation();
}

inline void WirelessConnection::Dispatch_SetDestination(char* payload)
{
    struct Params { unsigned a, b; } p(*(Params*)payload);
    Recv_SetDestination(p.a, p.b);
}

inline void WirelessConnection::Dispatch_GetDestination(char* payload)
{
    Recv_GetDestination();
}

inline void WirelessConnection::Dispatch_RequestPath(char* payload) 
{
    Recv_RequestPath();
}

inline void WirelessConnection::Dispatch_RelayPath(char* payload)
{
    struct Params { unsigned a, b, c; } p(*(Params*)payload);
    Recv_RelayPath(p.a, p.b, p.c);
}

inline void WirelessConnection::Dispatch_NotifyObstacle(char* payload) 
{
    struct Params { unsigned a, b; } p(*(Params*)payload);
    Recv_NotifyObstacle(p.a, p.b);
}

inline void WirelessConnection::SendRaw(eProtocolCommand cmd, const char* params, unsigned length, unsigned seq)
{
    if (!seq)
        seq = ++sequenceCounter;

    Datagram dg{ seq, cmd };
    memset(dg.payload, 0, sizeof(dg.payload));
    if (params && length)
    {
        if (length > 20)
            length = 20;
        memcpy(dg.payload, params, length);
    }
    wc.Send((char*)&dg, 28);
}

inline void WirelessConnection::HostPair() 
{
    char txBuffer[16] { 0 };
    SendRaw(eProto_HostPair, nullptr, 0);
}

inline void WirelessConnection::Ack(unsigned seq) 
{
    SendRaw(eProto_Ack, (char*)&seq, sizeof(unsigned), seq);
}

inline void WirelessConnection::Sync(unsigned seq)
{
    SendRaw(eProto_Sync, (char*)&seq, sizeof(unsigned), seq);
}

inline void WirelessConnection::BadCommand(unsigned seq) 
{
    SendRaw(eProto_BadCommand, (char*)&seq, sizeof(unsigned));    
}

inline void WirelessConnection::NotImplemented(eProtocolCommand command)
{
    SendRaw(eProto_NotImpl, (char*)&command, sizeof(eProtocolCommand));
}

inline void WirelessConnection::OK(unsigned id) 
{
    SendRaw(eProto_OK, (char*)&id, sizeof(unsigned));
}

inline void WirelessConnection::Drop(unsigned id) 
{
    SendRaw(eProto_Drop, (char*)&id, sizeof(unsigned));
}

inline void WirelessConnection::Echo(const char* data, unsigned length) 
{
    SendRaw(eProto_Echo, data, length);
}

inline void WirelessConnection::Ping(unsigned id) 
{
    SendRaw(eProto_Ping, (char*)id, sizeof(unsigned));
}

inline void WirelessConnection::Waiting() 
{
    SendRaw(eProto_Waiting, nullptr, 0);
}

inline void WirelessConnection::SetLeftWheelBias(float bias) 
{
    SendRaw(eProto_LeftBias, (char*)&bias, sizeof(float));    
}

inline void WirelessConnection::SetRightWheelBias(float bias)
{
    SendRaw(eProto_RightBias, (char*)&bias, sizeof(float));    
}

inline void WirelessConnection::TurnLeftDegrees(unsigned degrees)
{
    SendRaw(eProto_TurnLeftD, (char*)&degrees, sizeof(unsigned));
}

inline void WirelessConnection::TurnLeftTimed(unsigned ms)
{
    SendRaw(eProto_TurnLeftT, (char*)&ms, sizeof(unsigned));
}

inline void WirelessConnection::TurnRightDegrees(unsigned degrees)
{
    SendRaw(eProto_TurnRightD, (char*)&degrees, sizeof(unsigned));
}

inline void WirelessConnection::TurnRightTimed(unsigned ms)
{
    SendRaw(eProto_TurnRightT, (char*)&ms, sizeof(unsigned));
}

inline void WirelessConnection::SetSpeed(float speed)
{
    SendRaw(eProto_SetSpeed, (char*)&speed, sizeof(float));
}

inline void WirelessConnection::GetSpeed()
{
    SendRaw(eProto_GetSpeed, nullptr, 0);
}

inline void WirelessConnection::Go() 
{
    SendRaw(eProto_Go, nullptr, 0);
}

inline void WirelessConnection::Stop() 
{
    SendRaw(eProto_Stop, nullptr, 0);
}

inline void WirelessConnection::SetPosition(unsigned xPos, unsigned yPos)
{
    char buffer[sizeof(unsigned) * 2];
    memcpy(buffer, &xPos, sizeof(unsigned));
    memcpy(buffer + sizeof(unsigned), &yPos, sizeof(unsigned));
    SendRaw(eProto_SetPosition, buffer, sizeof(buffer));
}

inline void WirelessConnection::GetPosition() 
{
    SendRaw(eProto_GetPosition, nullptr, 0);
}

inline void WirelessConnection::SetOrientation(float radians)
{
    SendRaw(eProto_SetOrientation, (char*)&radians, sizeof(float));
}

inline void WirelessConnection::GetOrientation() 
{
    SendRaw(eProto_GetOrientation, nullptr, 0);
}

inline void WirelessConnection::SetDestination(unsigned xPos, unsigned yPos) 
{
    char buffer[sizeof(unsigned) * 2];
    memcpy(buffer, &xPos, sizeof(unsigned));
    memcpy(buffer + sizeof(unsigned), &yPos, sizeof(unsigned));
    SendRaw(eProto_SetDest, buffer, sizeof(buffer));
}

inline void WirelessConnection::GetDestination() 
{
    SendRaw(eProto_GetDest, nullptr, 0);
}

inline void WirelessConnection::RequestPath() 
{
    SendRaw(eProto_RequestPath, nullptr, 0);
}

inline void WirelessConnection::RelayPath(unsigned step, unsigned nodeX, unsigned nodeY)
{
    char buffer[sizeof(unsigned) * 3];
    memcpy(buffer + 0 * sizeof(unsigned), &step, sizeof(unsigned));
    memcpy(buffer + 1 * sizeof(unsigned), &nodeX, sizeof(unsigned));
    memcpy(buffer + 2 * sizeof(unsigned), &nodeY, sizeof(unsigned));
    SendRaw(eProto_RelayPath, buffer, sizeof(buffer));
}

inline void WirelessConnection::NotifyObstacle(unsigned xPos, unsigned yPos)
{
    char buffer[sizeof(unsigned) * 2];
    memcpy(buffer, &xPos, sizeof(unsigned));
    memcpy(buffer + sizeof(unsigned), &yPos, sizeof(unsigned));
    SendRaw(eProto_SetDest, buffer, sizeof(buffer));
}

#endif // WIRELESS_CONNECTION_H
