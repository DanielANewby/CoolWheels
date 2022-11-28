#ifndef CW_WIRELESS_CONTROLLER_H
#define CW_WIRELESS_CONTROLLER_H
#pragma once

#include "nRF24L01P.h"
#include <memory>

class WirelessController
{
public:
    WirelessController(unsigned long id, unsigned long remote, unsigned long long address, int addressWidth = DEFAULT_NRF24L01P_ADDRESS_WIDTH):
    wirelessUnit(
        PinMap[kPinMap_MOSI], 
        PinMap[kPinMap_MISO],
        PinMap[kPinMap_SCK],
        PinMap[kPinMap_CSN],
        PinMap[kPinMap_CE],
        PinMap[kPinMap_IRQ]
    ),
    localIdentifier{ id },
    remoteIdentifier{ remote }
    {
        wirelessUnit.powerUp();
        wirelessUnit.setRfOutputPower(-6);
        wirelessUnit.setTxAddress(address, addressWidth);
        wirelessUnit.setRxAddress(address, addressWidth);
        wirelessUnit.setTransferSize(TransmitBufferSize);
        wirelessUnit.setReceiveMode();
        wirelessUnit.enable();
#ifndef NDEBUG
        printf("nRF24L01+ Frequency    : %d MHz\r\n",    wirelessUnit.getRfFrequency());
        printf("nRF24L01+ Output power : %d dBm\r\n",    wirelessUnit.getRfOutputPower());
        printf("nRF24L01+ Data Rate    : %d kbps\r\n",   wirelessUnit.getAirDataRate());
        printf("nRF24L01+ TX Address   : 0x%010llX\r\n", wirelessUnit.getTxAddress());
        printf("nRF24L01+ RX Address   : 0x%010llX\r\n", wirelessUnit.getRxAddress());
#endif
    }

    ~WirelessController()
    {
        wirelessUnit.powerDown();
    }

    unsigned Send(const char* buffer, const unsigned length)
    {
        // Create and prepare a local sendq
        const char* moniker{ reinterpret_cast<char*>(&localIdentifier) };
        char sendBuffer[TransmitBufferSize] { moniker[0], moniker[1], moniker[2], moniker[3] };
        constexpr unsigned txSize { sizeof(sendBuffer) - 4 };

        // Transmit txSize count of data in fixed-window fragments
        unsigned n {0};
        for (unsigned remain{ length }; n < length; n += txSize)
        {
            // Clear buffer for security -- note that we're assuming memset isn't optimized out here
            // and it really shouldn't be
            memset(sendBuffer + 4, 0, txSize);

            // Load buffer and transmit
            memcpy(sendBuffer + 4, buffer + n, min(remain, txSize));
            wirelessUnit.write(NRF24L01P_PIPE_P0, sendBuffer, TransmitBufferSize);
            
            // Assume write succeeded
            remain -= txSize;
            wait_us(50);
        }
        
        return n;
    }

    unsigned Read(char* buffer, unsigned length)
    {
        unsigned readAmount{ 0 };
        char recvBuffer[TransmitBufferSize] { 0 };
        constexpr unsigned txSize { sizeof(recvBuffer) - 4 };

        while (readAmount < length)
        {
            int count{ 0 };
            int toRead = length - readAmount + sizeof(unsigned);
            if (toRead > TransmitBufferSize)
                toRead = TransmitBufferSize;
            memset(recvBuffer, 0, sizeof(recvBuffer));
            if ((count = wirelessUnit.read(NRF24L01P_PIPE_P0, recvBuffer, toRead)) <= sizeof(unsigned))
                break; // Error or no data
            count -= sizeof(unsigned);
            if ((*(unsigned*)(recvBuffer)) != remoteIdentifier) // Don't do this at home
                continue; // Discard packet
            memcpy(buffer + readAmount, recvBuffer + sizeof(unsigned), count);
            readAmount += count;
        }
        memset(recvBuffer, 0, sizeof(recvBuffer)); // x-value memset might get optimized out
        return readAmount;
    }
    
    bool Readable()
    {
        return wirelessUnit.readable(NRF24L01P_PIPE_P0);
    }

private:
    enum ePinMap { kPinMap_MOSI, kPinMap_MISO, kPinMap_SCK, kPinMap_CSN, kPinMap_CE, kPinMap_IRQ, kPinMap_Count };
    static constexpr PinName PinMap[kPinMap_Count] { D11, D12, D13, D14, D10, D15 };
    static constexpr unsigned TransmitBufferSize{ 32 };
    static const unsigned HostIdentifier = 0x48343230; // "H420"
    static const unsigned BotIdentifier = 0x43573639; // "CW69"
    static constexpr unsigned long long HostAddress = { 0x0000000000 };
    static constexpr unsigned long long BotAddress  = { 0x0000000000 };

    nRF24L01P wirelessUnit;
    unsigned localIdentifier;
    unsigned remoteIdentifier;
};

#endif // CW_WIRELESS_CONTROLLER_H
