#ifndef __STAGE_FETCH_H_INCLUDED__
#define __STAGE_FETCH_H_INCLUDED__

#include "elements/imem.h"
#include "utils/ins.h"
#include "utils/pc.h"
#include "utils/simulation_state.h"

struct StageFetch
{
    StageFetch(const Imem& instruction_memory) : ins_mem_(instruction_memory)
    {}
    ~StageFetch() = default;

    void run(SimulationState* state)
    {
        pc_ = state->reg_wb_fetch.pc;

        Ins instr = ins_mem_.Fetch(pc_);
        state->reg_fetch_decode.pc = pc_;
        state->reg_fetch_decode.ins_after_fetch = instr;

        // change later
        state->reg_wb_fetch.pc = pc_;
        state->reg_wb_fetch.pc.move_next();
    }

  private:
    ProgramCounter pc_;
    Imem ins_mem_;
};

#endif
