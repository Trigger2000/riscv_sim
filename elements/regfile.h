#ifndef __REGFILE_H_INCLUDED__
#define __REGFILE_H_INCLUDED__

#include "utils/defines.h"
#include "utils/ins.h"
#include "utils/simulation_state.h"

struct Regfile
{
    Regfile() = default;
    ~Regfile() = default;

    void SetD(uint32_t adr, int32_t data)
    {
        assert(adr < BITNESS);
        regs_[adr] = data;
    }

    int32_t GetD(uint32_t A)
    {
        assert(A < BITNESS);
        return regs_[A];
    }

  private:
    int32_t regs_[BITNESS] = { 0 };
};

#endif