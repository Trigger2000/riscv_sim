#ifndef __SIM_H_INCLUDED__
#define __SIM_H_INCLUDED__

#include "elements/hazard_unit.h"
#include "elements/imem.h"
#include "stages/stage_decode.h"
#include "stages/stage_execute.h"
#include "stages/stage_fetch.h"
#include "stages/stage_memory.h"
#include "stages/stage_writeback.h"
#include "utils/ins.h"
#include "utils/simulation_state.h"
#include <vector>

struct Simulation
{
    Simulation() = default;
    ~Simulation() = default;

    void Init(std::vector<Ins> ins)
    {
        for (int i = 0; i < 10; ++i) {
            ins.push_back(Ins::MakeIns_NOP());
        }

        sf = StageFetch{ ins };
        sd = StageDecode{};
        se = StageExecute{};
        sm = StageMemory{};
        sw = StageWriteBack{};

        imem_sz = ins.size();
    }

    void Iterate()
    {
        if (state_.reg_wb_fetch.PC >= imem_sz * 4) {
            return;
        }

        RunHazardUnit(&state_, &sigs_);
        sw.run(state_.reg_mem_wb, &sigs_);
        RunHazardUnit(&state_, &sigs_);
        state_.reg_mem_wb = sm.run(state_.reg_exec_mem, &sigs_);
        RunHazardUnit(&state_, &sigs_);
        state_.reg_exec_mem = se.run(state_.reg_decode_exec, &sigs_);
        RunHazardUnit(&state_, &sigs_);
        state_.reg_decode_exec = sd.run(state_.reg_fetch_decode, &sigs_);
        RunHazardUnit(&state_, &sigs_);
        if (!sigs_.HU_STALL) {
            state_.reg_fetch_decode = sf.run(&state_.reg_wb_fetch, &sigs_);
        } else {
            sf.run(&state_.reg_wb_fetch, &sigs_);
        }
    }

    bool run(std::vector<Ins> ins)
    {
        Init(ins);

        while (state_.reg_wb_fetch.PC < imem_sz * 4) {
            Iterate();
        }

        return true;
    }

    size_t imem_sz = 0;

    SimulationState state_;
    SimulationSignals sigs_;

    StageFetch sf{};
    StageDecode sd{};
    StageExecute se{};
    StageMemory sm{};
    StageWriteBack sw{};
};

#endif // __SIM_H_INCLUDED__