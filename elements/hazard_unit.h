#ifndef __HAZARD_UNIT_H_INCLUDED__
#define __HAZARD_UNIT_H_INCLUDED_

#include "utils/simulation_state.h"

void RunHazardUnit(SimulationState* state, SimulationSignals* signals)
{
    signals->HU_RS1 = 0;
    signals->HU_RS2 = 0;
    if (state->reg_mem_wb.WB_WE == 1 || state->reg_exec_mem.WB_WE == 1) {
        if (signals->HU_MEM_RD == signals->A1_EX ||
            signals->HU_MEM_RD == signals->A2_EX) {
            signals->HU_RS1 = 1;
            signals->HU_RS2 = 1;
        } else if (signals->HU_WB_RD == signals->A1_EX ||
                   signals->HU_WB_RD == signals->A2_EX) {
            signals->HU_RS1 = 2;
            signals->HU_RS2 = 2;
        }
    }
}
#endif