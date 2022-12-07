#ifndef CW_NAVIGATION_BASE_H
#define CW_NAVIGATION_BASE_H
#pragma once

#include <vector>
#include <utility>

class Navigation
{
public:
    Navigation():
    x(0), y(0),
    destX(0), destY(0),
    orientation(0.0)
    {}

    virtual ~Navigation() {}

    virtual void SetPosition(unsigned xPos, unsigned yPos)
    {
        x = xPos;
        y = yPos;
    }

    virtual void GetPosition(unsigned& xOut, unsigned& yOut) const
    {
        xOut = x;
        yOut = y;
    }
    
    virtual void SetOrientation(float _orientation)
    {
        orientation = _orientation;
    }

    virtual float GetOrientation() const
    {
        return orientation;
    }

    virtual void SetDestination(unsigned xPos, unsigned yPos)
    {
        destX = xPos;
        destY = yPos;
    }

    virtual void GetDestination(unsigned& xOut, unsigned& yOut) const
    {
        xOut = destX;
        yOut = destY;
    }

    virtual std::vector<std::pair<unsigned, unsigned>> GetPath() const = 0;
    virtual void AddObstacle(unsigned obsX, unsigned obsY) = 0;

    virtual void Step() = 0;
    virtual void Go() = 0;

    virtual void Update() = 0;

protected:
    unsigned x, y;
    unsigned destX, destY;
    float orientation;
};

#endif // CW_NAVIGATION_BASE_H
