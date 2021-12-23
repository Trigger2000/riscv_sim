#include "elements/control_unit.h"
#include "elements/regfile.h"
#include "utils/ins.h"
#include "utils/simulation_state.h"

#ifndef __STAGE_DECODE_H_INCLUDED__
#define __STAGE_DECODE_H_INCLUDED__

bool PC_R = false;

struct StageDecode
{
    StageDecode() = default;
    ~StageDecode() = default;

    void run(SimulationState* state)
    {
        // control_registers output;

        Ins instr_after_fetch = FETCH_DECODE_REG.ins_after_fetch;
        register_file.instr_after_fetch.GetRs1(), instr_after_fetch.GetRs2(),
            // MEM_WB_REG.WB_A, MEM_WB_REG.WB_D, MEM_WB_REG.WB_WE);

            state->DECODE_EXEC_REG.D1 = register_file.Get_D1();
        state->DECODE_EXEC_REG.D2 = register_file.Get_D2();

        uint32_t sign_bit_not_extended = instr_after_fetch.GetImmSign();
        DECODE_EXEC_REG.sign_bit = sign_extend_sign_bit(sign_bit_not_extended);

        Control_unit control_unit(instr_after_fetch);

        // extend this method while developing
        control_unit.get_signals();

        DECODE_EXEC_REG.pc = FETCH_DECODE_REG.pc;

        if (PC_R != false || FETCH_DECODE_REG.PC_R != false) {
            DECODE_EXEC_REG.v_de = true;
        }

        DECODE_EXEC_REG.wb_a = instr_after_fetch.GetRd();
        DECODE_EXEC_REG.imm = instr_after_fetch.GetImm();
    }

    uint32_t sign_extend_sign_bit(uint32_t input)
    {
        uint32_t result = 0x00000000;
        if (input & 1 == 1) {
            result = 0xFFFFFFFF;
        }
        return result;
    }

  private:
    Regfile register_file_{};
};

#endif