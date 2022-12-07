#ifndef PCCOMMUNICATION_H
#define PCCOMMUNICATION_H
#include <algorithm>
#pragma once

#include "mbed.h"
#include <string>

class PCCommunication
{
public:
    PCCommunication(bool bSpuriousChar = false): usbLink(USBTX, USBRX)
    {
        if (bSpuriousChar)
            usbLink.read(inputBuffer, 1);
        Echo();
        EchoCommands();
        Reset();
    }

    PCCommunication(const PCCommunication&) = delete;
    PCCommunication(PCCommunication&&) = delete;

    void Reset()
    {
        memset(commandBuffer, 0, commandBufferSize);
        memset(inputBuffer, 0, inputBufferSize);
    }

    bool Echo(bool bEcho = true)
    {
        echo = bEcho;
        return echo;
    }

    bool EchoCommands(bool bEcho = true)
    {
        commandEcho = bEcho;
        return commandEcho;
    }

    bool Update()
    {
        bool hasNewline = false;
        if (usbLink.readable())
        {
            ThisThread::sleep_for(100ms);
            ssize_t amtRead = usbLink.read(&inputBuffer, inputBufferSize);
            if (amtRead > 0)
            {
                if (echo)
                {
                    usbLink.write(inputBuffer, amtRead);
                    //Write(inputBuffer);
                }

                for (ssize_t n{ 0 }; n < amtRead; ++n)
                {
                    commandBuffer[commandBufferOffset++] = inputBuffer[n];
                    if (inputBuffer[n] == '\n' || inputBuffer[n] == '\r')
                        hasNewline = true;
                }
            }
            if (hasNewline)
            {
                if (commandEcho)
                   printf("%s\n", commandBuffer);
                commandBufferOffset = 0;
            }
            memset(inputBuffer, 0, inputBufferSize);
        }
        return hasNewline;
    }

    void Write(const std::string& outputMsg)
    {
        //usbLink.write(outputMsg.c_str(), outputMsg.size());
        // Changed to printf for syncing
        printf("%s", outputMsg.c_str());
    }
    
    std::string ReadCommand()
    {
        std::string out{ commandBuffer };
        Reset();
        return out;
    }

private:
    static constexpr unsigned commandBufferSize{ 256 };
    static constexpr unsigned inputBufferSize{ 32 };

    BufferedSerial usbLink;
    unsigned commandBufferOffset{0};
    bool echo{ false }; // Reset to true during init
    bool commandEcho{ false }; // Reset to true during init
    char commandBuffer[commandBufferSize];
    char inputBuffer[inputBufferSize];
};

#endif // PCCOMMUNICATION_H
