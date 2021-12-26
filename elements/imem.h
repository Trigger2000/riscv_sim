#ifndef __IMEM_H_INCLUDED__
#define __IMEM_H_INCLUDED__

#include <vector>

#include "utils/defines.h"
#include "utils/ins.h"
#include "utils/simulation_state.h"

struct Imem
{
    Imem() = default;

    Imem(std::vector<Ins> instructions) : instructions_(instructions)
    {}
    ~Imem() = default;

    Ins Fetch(const uint32_t pc)
    {
        assert(pc % INSTRUCTION_LENGTH == 0);
        uint32_t pos = static_cast<uint32_t>(pc / INSTRUCTION_LENGTH);
        assert(pos < instructions_.size());

        return instructions_[pos];
    }

  private:
    std::vector<Ins> instructions_;
};

#endif // __IMEM_H_INCLUDED__