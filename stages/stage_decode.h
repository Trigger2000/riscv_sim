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

        // std::cout << "[DECODE] WB_WE " << signals->WB_WE << "\n";
        // std::cout << "[DECODE] WB_D " << signals->WB_D << "\n";
        // std::cout << "[DECODE] WB_A " << signals->WB_A << "\n";

        if (signals->WB_WE != 0) {
            regfile_.SetD(signals->WB_A, signals->WB_D);
        }

        RegisterDecodeExecute output_reg{};

        if (input_reg.ins.IsNOP() || signals->HU_STALL) {
            if (signals->HU_STALL) {
                signals->BP_MEM = 0;
                signals->BP_WB = 0;
                signals->HU_RS1 = 0;
                signals->HU_RS2 = 0;
            }

            return output_reg;
        }

        if (signals->PC_R == 1 || input_reg.PC_R == 1) {
            output_reg.V_EX = 1;
        }

        Ins instr_after_fetch = input_reg.ins;
        output_reg.ins = instr_after_fetch;

        uint32_t rs1 = 0;
        uint32_t rs2 = 0;
        instr_after_fetch.GetRs1(&rs1);
        instr_after_fetch.GetRs2(&rs2);

        signals->A1_D = rs1;
        signals->A2_D = rs2;
        signals->PC_DE = input_reg.PC_DE;

        // std::cout << "[DECODE] A1 " << rs1 << "\n";
        // std::cout << "[DECODE] A2 " << rs2 << "\n";

        output_reg.D1 = regfile_.GetD(rs1);
        output_reg.D2 = regfile_.GetD(rs2);

        // may not be needed
        output_reg.sign_bit =
            SignExtendSignBit(instr_after_fetch.GetImmSign());

        output_reg.CONTROL_EX = RunControlUnit(instr_after_fetch);

        output_reg.ins = instr_after_fetch;

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
        output.ALU_FMT = instr.GetInsFormat();

        return output;
    }
};

#endif