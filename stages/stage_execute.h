#ifndef __STAGE_EXECUTE_H_INCLUDED__
#define __STAGE_EXECUTE_H_INCLUDED__

#include "elements/mux.h"
#include "utils/ins.h"
#include "utils/registers.h"
#include "utils/simulation_state.h"
#include <cstddef>

struct StageExecute
{
    StageExecute() = default;
    ~StageExecute() = default;

    RegisterExecuteMemory run(const RegisterDecodeExecute& input_reg,
                              SimulationSignals* signals)
    {
        int32_t imm = input_reg.imm;
        signals->PC_DISP = static_cast<uint32_t>(imm);
        signals->PC_EX = input_reg.PC_EX;

        RegisterExecuteMemory output_reg;
        output_reg.WB_A = input_reg.WB_A;
        output_reg.WD = input_reg.D1;

        int32_t RS1V = Mux3<int32_t>(input_reg.D1, signals->BP_MEM,
                                     signals->BP_WB, signals->HU_RS1);

        int32_t RS2V = Mux3<int32_t>(input_reg.D2, signals->BP_MEM,
                                     signals->BP_WB, signals->HU_RS2);

        int32_t alu_src2 =
            Mux2<int32_t>(RS2V, imm, input_reg.CONTROL_EX.ALU_SRC2);

        output_reg.alu_res =
            run_alu(RS1V, alu_src2, input_reg.CONTROL_EX.ALUOP);

        run_we_gen(input_reg.CONTROL_EX, input_reg.V_EX, &output_reg.WB_WE,
                   &output_reg.MEM_WE);

        return output_reg;
    }

  private:
    int32_t run_alu(int32_t op1, int32_t op2, Ins::InsMnemonic aluop)
    {
        switch (aluop) {
        case Ins::InsMnemonic::ADD:
        case Ins::InsMnemonic::ADDI:
            return op1 + op2;
        case Ins::InsMnemonic::SUB:
            return op1 - op2;
        case Ins::InsMnemonic::XOR:
        case Ins::InsMnemonic::XORI:
            return op1 ^ op2;
        case Ins::InsMnemonic::OR:
        case Ins::InsMnemonic::ORI:
            return op1 | op2;
        case Ins::InsMnemonic::AND:
        case Ins::InsMnemonic::ANDI:
            return op1 & op2;
        default:
            return 0;
        }
    }

    void run_we_gen(RegisterDecodeExecute::ControlEx control_ex, uint32_t v_ex,
                    uint32_t* wb_we, uint32_t* mem_we)
    {}
};

#endif