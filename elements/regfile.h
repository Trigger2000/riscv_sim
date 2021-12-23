#ifndef __REGFILE_H_INCLUDED__
#define __REGFILE_H_INCLUDED__

#include "utils/defines.h"
#include "utils/ins.h"
#include "utils/simulation_state.h"

struct Regfile
{
    Regfile() = default;
    /* Regfile(uint32_t A1, uint32_t A2, uint32_t A3, uint32_t D3, bool WE3)
        : A1_(A1), A2_(A2), WE3_(WE3)
    {
        if (WE3_) {
            A3_ = A3;
            assert(A3_ < BITNESS);
            regs_[A3_] = D3;
        }
    } */

    ~Regfile() = default;

    void SetA1(uint32_t rs1)
    {
        A1_ = rs1;
    }

    void SetA2(uint32_t)
    {
        A2_ = rs2;
    }

    void SetRegister(uint32_t adr, uint32_t data)
    {
        assert(adr < BITNESS);
        regs_[adr] = data;
    }

    int32_t Get_D1()
    {
        assert(A1_ < BITNESS);
        return regs_[A1_];
    }

    int32_t Get_D2()
    {
        assert(A2_ < BITNESS);
        return regs_[A2_];
    }

  private:
    uint32_t regs_[BITNESS] = { 0 };

    uint32_t A1_ = 0;  // rs1
    uint32_t A2_ = 0;  // rs2
    uint32_t A3_ = 0;  // WB_A, aka rd
    bool WE3_ = false; // WB_WE
};

#endif