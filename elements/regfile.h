#ifndef __REGFILE_H_INCLUDED__
#define __REGFILE_H_INCLUDED__

#include <array>

#include "../stages/stage.h"
#include "../utils/ins.h"

struct regfile {

    regfile(uint32_t A1, uint32_t A2, uint32_t A3, uint32_t D3, bool WE3):
    A1_(A1), A2_(A2), WE3_(WE3)
    {
        if (WE3_) {
            A3_ = A3;
            assert(A3_ < bitness);
            registers_[A3_] = D3;
        }
    }

    ~regfile() = default;

    int32_t get_D1() {
        assert(A1_ < bitness);
        return registers_[A1_];
    }

    int32_t get_D2() {
        assert(A2_ < bitness);
        return registers_[A2_];
    }

    // write back data
    /* void set_D3(int32_t data) {
        assert(A3_ < bitness);
        registers_[A3_] = data;
    }

    // WB_WE3
    void set_WE(bool write_enable) {
        WE3_ = write_enable;
    }

    void set_A3(int32_t data) {
        A3_ = data;
    } */

private:
    std::array<int32_t, bitness> registers_{0};

    uint32_t A1_ = 0; // rs1
    uint32_t A2_ = 0; // rs2
    uint32_t A3_ = 0; // WB_A, aka rd
    bool WE3_ = false; // WB_WE
};

#endif