#ifndef __STAGE_WRITEBACK_H_INCLUDED__
#define __STAGE_WRITEBACK_H_INCLUDED__

#include "utils/registers.h"
#include "utils/simulation_state.h"

struct StageWriteBack
{
    StageWriteBack() = default;
    ~StageWriteBack() = default;

    void run(const RegisterMemoryWriteback& input_reg,
             SimulationSignals* signals)
    {
        signals->WB_WE = input_reg.WB_WE;
        signals->WB_D = input_reg.WB_D;
        signals->BP_WB = input_reg.WB_D;
        signals->WB_A = input_reg.WB_A;
        signals->HU_WB_RD = input_reg.WB_A;
    }
};

#endif