#ifndef __IMEM_H_INCLUDED__
#define __IMEM_H_INCLUDED__

#include <vector>

#include "utils/ins.h"
#include "utils/simulation_state.h"

struct Imem
{
    Imem() = default;

    Imem(std::vector<Ins> instructions) : instructions_(instructions)
    {}
    ~Imem() = default;

    // void set(std::vector<Ins>& instructions)
    // {
    //     instructions_ = instructions;
    // }

    Ins Fetch(const ProgramCounter& pc)
    {
        return instructions_[pc.get_current()];
    }

  private:
    std::vector<Ins> instructions_;
};

#endif // __IMEM_H_INCLUDED__