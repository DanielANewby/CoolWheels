#ifndef BASIC_NAVIGATION_H
#define BASIC_NAVIGATION_H
#pragma once

#include "mbed.h"

#include "CW_NavigationBase.h"
#include "CW_MotionController.h"
#include "..\Shared\WirelessConnection.h"
#include "..\Shared\CWSignal.h"

class BasicNavigation : public Navigation
{
public:
    BasicNavigation(WirelessConnection& _wc, MotionController& _mc):
    wc(_wc),
    mc(_mc),
    stopSignal{ _mc.stopSignal, this, &BasicNavigation::OnStopped }
    {}

    virtual std::vector<std::pair<unsigned, unsigned>> GetPath() const
    {
        std::vector<std::pair<unsigned, unsigned>> out;
        unsigned pathX{ x }, pathY { y };
        while (pathY != destY)
        {
            (pathY < destY) ? ++pathY : --pathY;
            out.emplace_back(std::make_pair(pathX, pathY));            
        }
        while (pathX != destX)
        {
            (pathX < destX) ? ++pathX : --pathX;
            out.emplace_back(std::make_pair(pathX, pathY));
        }
        return out;
    }

    virtual void AddObstacle(unsigned, unsigned) { /* Do nothing in Basic nav */ }

    virtual void Step()
    {
        printf("Nav: Stepping\n");
        auto nodes = GetPath();
        if (!nodes.size()) {
            printf("Finished\n");
            return;
        }
        auto next = nodes[0];
        nextX = next.first;
        nextY = next.second;
        mc.Forward();
    }

    virtual void Go()
    {
        printf("Nav: Go\n");
        bRunning = true;
        Step();
    }

    virtual void Update()
    {
    }

    void OnStopped()
    {
        printf("Nav: Stop notify\n");
        bool turning = false;
        if (nextX < 8 && nextY < 8)
        {
            // We're reaching the correct vertical position
            if (nextY == destY && y != destY)
            {
                printf("Turning\n");
                turning = true;
                if (destX < x)
                    mc.TurnLeft();
                else
                    mc.TurnRight();
            }
            // Update position and remote display
            SetPosition(nextX, nextY);
            wc.SetPosition(nextX, nextY);
            nextX = 9;
            nextY = 9;
        }
        if (bRunning && !turning)
            Step();
    }

private:
    WirelessConnection& wc;
    MotionController& mc;
    SignalReceiver stopSignal;

    bool bRunning{ false };
    unsigned nextX{ 9 }, nextY { 9 };
};

#endif // BASIC_NAVIGATION_H
