#include "elements/regfile.h"
#include "utils/ins.h"
#include "utils/registers.h"
#include <cassert>

#ifndef __STAGE_DECODE_H_INCLUDED__
#define __STAGE_DECODE_H_INCLUDED__

struct StageDecode
{
    StageDecode() = default;
    ~StageDecode() = default;

    void SetRegfileData(uint32_t adr, int32_t data)
    {
        regfile_.SetD(adr, data);
    }

    int32_t GetRegfileData(uint32_t adr)
    {
        return regfile_.GetD(adr);
    }

    RegisterDecodeExecute run(const RegisterFetchDecode& input_reg,
                              SimulationSignals* signals)
    {
        assert(signals != nullptr);

        RegisterDecodeExecute output_reg;
        Ins instr_after_fetch = input_reg.ins;

        uint32_t rs1 = 0;
        uint32_t rs2 = 0;
        instr_after_fetch.GetRs1(&rs1);
        instr_after_fetch.GetRs2(&rs2);

        output_reg.D1 = regfile_.GetD(rs1);
        output_reg.D2 = regfile_.GetD(rs2);

        if (signals->WB_WE != 0) {
            regfile_.SetD(signals->WB_A, signals->WB_D);
        }

        // maybe not needed
        output_reg.sign_bit =
            SignExtendSignBit(instr_after_fetch.GetImmSign());

        output_reg.CONTROL_EX = RunControlUnit(instr_after_fetch);
        output_reg.PC_EX = input_reg.PC;

        if (signals->PC_R == 1 || input_reg.PC_R == 1) {
            output_reg.V_EX = 1;
        }

        instr_after_fetch.GetRd(&output_reg.WB_A);
        instr_after_fetch.GetImm(&output_reg.imm);

        return output_reg;
    }

  private:
    Regfile regfile_;

    uint32_t SignExtendSignBit(uint32_t input)
    {
        return input ? 0xFFFFFFFF : 0x00000000;
    }

    RegisterDecodeExecute::ControlEx RunControlUnit(Ins instr)
    {
        RegisterDecodeExecute::ControlEx output;

        // set signals here
        output.ALUOP = instr.GetInsMnemonic();
        output.ALU_SRC2 = (instr.GetInsFormat() != Ins::InsFormat::R) ? 1 : 0;

        return output;
    }
};

#endif