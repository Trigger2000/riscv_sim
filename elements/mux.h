#ifndef __MUX_H_INCLUDED__
#define __MUX_H_INCLUDED__

#include <cassert>
#include <cstdint>

static uint32_t Mux2(uint32_t i0, uint32_t i1, uint32_t sel)
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

static uint32_t Mux3(uint32_t i0, uint32_t i1, uint32_t i2, uint32_t sel)
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