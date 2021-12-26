#ifndef __MUX_H_INCLUDED__
#define __MUX_H_INCLUDED__

#include <cassert>
#include <cstdint>

template <typename T>
T Mux2(T i0, T i1, uint32_t sel)
{
    switch (sel) {
    case 0b0:
        return i0;
    case 0b1:
        return i1;
    default:
        assert(false);
    }
}

template <typename T>
T Mux3(T i0, T i1, T i2, uint32_t sel)
{
    switch (sel) {
    case 0b00:
        return i0;
    case 0b01:
        return i1;
    case 0b10:
        return i2;
    default:
        assert(false);
    }
}

#endif