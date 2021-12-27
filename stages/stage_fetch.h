#ifndef __STAGE_FETCH_H_INCLUDED__
#define __STAGE_FETCH_H_INCLUDED__

#include "elements/imem.h"
#include "elements/mux.h"
#include "utils/defines.h"
#include "utils/ins.h"
#include "utils/registers.h"
#include "utils/simulation_state.h"
#include <cassert>

struct StageFetch
{
    StageFetch(const Imem& instruction_memory) : ins_mem_(instruction_memory)
    {}
    StageFetch() = default;
    ~StageFetch() = default;

    RegisterFetchDecode run(RegisterWriteBackFetch* input_reg,
                            SimulationSignals* signals)
    {
        assert(input_reg != nullptr);
        assert(signals != nullptr);

        uint32_t pc = input_reg->PC;

        RegisterFetchDecode output_reg{};
        output_reg.ins = ins_mem_.Fetch(pc);

        output_reg.PC_R = signals->PC_R;
        output_reg.PC = pc;

        uint32_t start_pc = Mux2<uint32_t>(pc, signals->PC_EX, signals->PC_R);
        int32_t pc_disp = Mux2<int32_t>(INSTRUCTION_LENGTH, signals->PC_DISP,
                                        static_cast<int32_t>(signals->PC_R));

        input_reg->PC = AluFetch(start_pc, pc_disp);

        return output_reg;
    }

  private:
    Imem ins_mem_;

    uint32_t AluFetch(uint32_t input_pc, int32_t pc_disp)
    {
        return (pc_disp < 0) ? input_pc - static_cast<uint32_t>((-1) * pc_disp)
                             : input_pc + static_cast<uint32_t>(pc_disp);
    }
};

#endif
