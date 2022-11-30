#ifndef SEVEN_SEGMENT_H
#define SEVEN_SEGMENT_H
#pragma once

#include "mbed.h"

enum SegmentBit {
    Bottom      = 0x01,
    BottomLeft  = 0x02,
    BottomRight = 0x04,
    Middle      = 0x08,
    TopLeft     = 0x10,
    TopRight    = 0x20,
    Top         = 0x40,
    Dot         = 0x80
};

// kDisplayNumbers, 0 - 9
static const char SegmentDisplayNumbers[] {
    Top | TopLeft | TopRight | BottomLeft | BottomRight | Bottom, // 0
    TopRight | BottomRight, // 1
    Top | TopRight | Middle | BottomLeft | Bottom, // 2
    Top | TopRight | Middle | BottomRight | Bottom, // 3
    TopLeft | TopRight | Middle | BottomRight, // 4
    Top | TopLeft | Middle | BottomRight | Bottom, // 5
    Top | TopLeft | Middle | BottomLeft | BottomRight | Bottom, // 6
    Top | TopRight | BottomRight, // 7
    Top | TopLeft | TopRight | Middle | BottomLeft | BottomRight | Bottom, // 8
    Top | TopLeft | TopRight | Middle | BottomRight, // 9
};

// A-Z
static const char SegmenDisplayAlpha[] {
    Top | TopLeft | TopRight | Middle | BottomLeft | BottomRight,
    Top | TopLeft | TopRight | Middle | BottomLeft | BottomRight | Bottom | Dot,
    Top | TopLeft | BottomLeft | Bottom,
    Top | TopLeft | TopRight | BottomLeft | BottomRight | Bottom,
    Top | TopLeft | Middle | BottomLeft | Bottom,
    Top | TopLeft | Middle | BottomLeft,
    Top | TopLeft | Middle | BottomLeft | BottomRight | Bottom,
    TopLeft | Middle | BottomLeft | TopRight | BottomRight,
    TopRight | BottomRight,
    TopRight | BottomRight | Bottom,
    0, // K
    TopLeft | BottomLeft | Bottom,
    0, // M
    0, // N
    Top | TopLeft | TopRight | BottomLeft | BottomRight | Bottom,
    Top | TopLeft | TopRight | Middle | BottomLeft,
    Top | TopLeft | TopRight | BottomLeft | BottomRight | Bottom | Dot,
    Top | TopLeft | TopRight | Middle | BottomLeft | BottomRight | Dot,
    Top | TopLeft | Middle | BottomRight | Bottom,
    Top | TopLeft | BottomLeft,
    TopLeft | TopRight | BottomLeft | BottomRight | Bottom,
    0, // V
    0, // W,
    0, // X
    TopLeft | TopRight | Middle | BottomLeft,
    Top | TopRight | Middle | BottomLeft | Bottom,
};

class SevenSegmentController : public BusOut
{
public:
    SevenSegmentController(PinName bottom, PinName bottomLeft, PinName bottomRight, PinName middle, PinName topLeft, PinName topRight, PinName top, PinName dot):
    BusOut{ bottom, bottomLeft, bottomRight, middle, topLeft, topRight, top, dot }
    {}

    ~SevenSegmentController() {}

    void Put(char a)
    {
        int out = SegmentsFromChar(a);
        write(out);
    }

private:
    int SegmentsFromChar(char a)
    {
        if (a >= 0 && a <= 9)
            return SegmentDisplayNumbers[a];
        if (a >= 'A' && a <= 'Z')
            return SegmenDisplayAlpha[a - 'A'];
        if (a >= 'a' && a <= 'z')
            return SegmenDisplayAlpha[a - 'a'];
        return 0;
    }
};

#endif // SEVEN_SEGMENT_H
