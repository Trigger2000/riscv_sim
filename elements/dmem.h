#ifndef __DMEM_H_INCLUDED__
#define __DMEM_H_INCLUDED__

#include "utils/defines.h"
#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <cstring>

struct Dmem
{
    Dmem() = default;
    ~Dmem() = default;

    void write_data(uint32_t WE, int32_t WD, uint32_t A, uint32_t width)
    {
        if (WE == 0) {
            return;
        }

        assert(A < DMEM_SIZE);

        switch (width) {
        case BYTE_SIZE: {
            int8_t data = static_cast<int8_t>(WD);
            std::memcpy(&(memory[A]), &data, 1);
        } break;
        case 2 * BYTE_SIZE: {
            int16_t data = static_cast<int16_t>(WD);
            std::memcpy(&(memory[A]), &data, 2);
        } break;
        case 4 * BYTE_SIZE: {
            int32_t data = static_cast<int32_t>(WD);
            std::memcpy(&(memory[A]), &data, 4);
        } break;
        default:
            exit(1);
        }
    }

    int32_t read_data(uint32_t A, uint32_t width)
    {
        assert(A < DMEM_SIZE);
        int32_t result = 0;

        switch (width) {
        case BYTE_SIZE: {
            std::memcpy(&result, &(memory[A]), 1);
        } break;
        case 2 * BYTE_SIZE: {
            std::memcpy(&result, &(memory[A]), 2);
        } break;
        case 4 * BYTE_SIZE: {
            std::memcpy(&result, &(memory[A]), 4);
        } break;
        default:
            exit(1);
        }

        return result;
    }

  private:
    int8_t memory[DMEM_SIZE];
};

#endif