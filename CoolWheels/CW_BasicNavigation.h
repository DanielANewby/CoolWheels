#ifndef BASIC_NAVIGATION_H
#define BASIC_NAVIGATION_H
#pragma once

#include "CW_NavigationBase.h"

class BasicNavigation : public Navigation
{
public:
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
    }

    virtual void Go()
    {
    }

    virtual void Update()
    {
        
    }

private:
};

#endif // BASIC_NAVIGATION_H
