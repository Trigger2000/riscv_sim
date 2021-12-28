#ifndef __HAZARD_UNIT_H_INCLUDED__
#define __HAZARD_UNIT_H_INCLUDED_

#include "utils/simulation_state.h"

void RunHazardUnit(SimulationState* state, SimulationSignals* signals)
{
    signals->HU_RS1 = 0;
    signals->HU_RS2 = 0;

    if (state->reg_exec_mem.WB_WE == 1) {
        if (signals->HU_MEM_RD == signals->A1_D) {
            signals->HU_RS1 = 1;
        }

        if (signals->HU_MEM_RD == signals->A2_D) {
            signals->HU_RS2 = 1;
        }
    }

    if (state->reg_mem_wb.WB_WE == 1) {
        if (signals->HU_WB_RD == signals->A1_D) {
            signals->HU_RS1 = 2;
        }

        if (signals->HU_WB_RD == signals->A2_D) {
            signals->HU_RS2 = 2;
        }
    }

    auto mnm = state->reg_decode_exec.ins.GetInsMnemonic();
    bool is_load =
        (mnm == Ins::InsMnemonic::LB) || (mnm == Ins::InsMnemonic::LH) ||
        (mnm == Ins::InsMnemonic::LW) || (mnm == Ins::InsMnemonic::LBU) ||
        (mnm == Ins::InsMnemonic::LHU);

    if (is_load) {
        uint32_t load_rd = 0;
        uint32_t RS1 = 0;
        uint32_t RS2 = 0;

        bool load_has_rd = state->reg_decode_exec.ins.GetRd(&load_rd);
        if (!load_has_rd) {
            exit(1);
        }

        bool ins_has_rs1 = state->reg_fetch_decode.ins.GetRs1(&RS1);
        bool ins_has_rs2 = state->reg_fetch_decode.ins.GetRs2(&RS2);

        bool has_confllict_rs1 = false;
        bool has_confllict_rs2 = false;

        if (ins_has_rs1) {
            has_confllict_rs1 = load_rd == RS1;
        }

        if (ins_has_rs2) {
            has_confllict_rs2 = load_rd == RS2;
        }

        mnm = state->reg_fetch_decode.ins.GetInsMnemonic();
        is_load =
            (mnm == Ins::InsMnemonic::LB) || (mnm == Ins::InsMnemonic::LH) ||
            (mnm == Ins::InsMnemonic::LW) || (mnm == Ins::InsMnemonic::LBU) ||
            (mnm == Ins::InsMnemonic::LHU);

        if (has_confllict_rs1 || has_confllict_rs2 || is_load) {
            signals->HU_STALL = 1;
        }
    }

    if (signals->HU_STALL && signals->BP_WB) {
        signals->HU_STALL = 0;
    }
}
#endif