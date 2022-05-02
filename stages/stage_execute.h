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
        assert(signals != nullptr);

        int32_t imm = 0;
        input_reg.ins.GetImm(&imm);
        // input_reg.ins.GetRs1(&(signals->A1_EX));
        // input_reg.ins.GetRs2(&(signals->A2_EX));

        signals->PC_DISP = static_cast<uint32_t>(imm);

        int32_t RS1V = Mux3<int32_t>(input_reg.D1, signals->BP_MEM,
                                     signals->BP_WB, signals->HU_RS1);

        int32_t RS2V = Mux3<int32_t>(input_reg.D2, signals->BP_MEM,
                                     signals->BP_WB, signals->HU_RS2);

        // std::cout << "[EXECUTE] D1 " << input_reg.D1 << "\n";
        // std::cout << "[EXECUTE] D2 " << input_reg.D2 << "\n";
        // std::cout << "[EXECUTE] A1_EX " << signals->A1_D << "\n";
        // std::cout << "[EXECUTE] A2_EX " << signals->A2_D << "\n";
        // std::cout << "[EXECUTE] PC_DISP " << signals->PC_DISP << "\n";
        // std::cout << "[EXECUTE] A1_EX " << signals->A1_D << "\n";
        // std::cout << "[EXECUTE] RS1V " << RS1V << "\n";
        // std::cout << "[EXECUTE] RS2V " << RS2V << "\n";
        // std::cout << "[EXECUTE] IMM " << imm << "\n";
        // std::cout << "[EXECUTE] HU_RS1 " << signals->HU_RS1 << "\n";
        // std::cout << "[EXECUTE] HU_RS2 " << signals->HU_RS2 << "\n";

        RegisterExecuteMemory output_reg{};

        if (input_reg.ins.IsNOP()) {
            output_reg.NOP = true;

            return output_reg;
        }

        input_reg.ins.GetRd(&output_reg.WB_A);

        output_reg.WD = RS2V;
        int32_t alu_src2 =
            Mux2<int32_t>(RS2V, imm, input_reg.CONTROL_EX.ALU_SRC2);

        output_reg.alu_res =
            RunAlu(RS1V, alu_src2, input_reg.CONTROL_EX.ALUOP);

        // std::cout << "[EXECUTE] ALU_RES " << output_reg.alu_res << "\n";

        RunWEGen(input_reg.CONTROL_EX, input_reg.V_EX, &output_reg.WB_WE,
                 &output_reg.MEM_WE);

        output_reg.WB_selector = 1;
        if (input_reg.CONTROL_EX.ALUOP == Ins::InsMnemonic::LB ||
            input_reg.CONTROL_EX.ALUOP == Ins::InsMnemonic::LH ||
            input_reg.CONTROL_EX.ALUOP == Ins::InsMnemonic::LW) {
            output_reg.WB_selector = 0;
        }

        if (input_reg.CONTROL_EX.ALUOP == Ins::InsMnemonic::LB ||
            input_reg.CONTROL_EX.ALUOP == Ins::InsMnemonic::SB) {
            output_reg.mem_width = 8;
        }

        if (input_reg.CONTROL_EX.ALUOP == Ins::InsMnemonic::LH ||
            input_reg.CONTROL_EX.ALUOP == Ins::InsMnemonic::SH) {
            output_reg.mem_width = 16;
        }

        if (input_reg.CONTROL_EX.ALUOP == Ins::InsMnemonic::LW ||
            input_reg.CONTROL_EX.ALUOP == Ins::InsMnemonic::SW) {
            output_reg.mem_width = 32;
        }

        return output_reg;
    }

  private:
    int32_t RunAlu(int32_t op1, int32_t op2, Ins::InsMnemonic aluop)
    {
        switch (aluop) {
        case Ins::InsMnemonic::ADD:
        case Ins::InsMnemonic::ADDI:
        case Ins::InsMnemonic::LB:
        case Ins::InsMnemonic::LH:
        case Ins::InsMnemonic::LW:
        case Ins::InsMnemonic::SB:
        case Ins::InsMnemonic::SH:
        case Ins::InsMnemonic::SW:
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
        case Ins::InsMnemonic::SLL:
        case Ins::InsMnemonic::SLLI:
            assert(op1 >= 0); // otherwise UB in C++
            return op1 << op2;
        case Ins::InsMnemonic::SRL:
        case Ins::InsMnemonic::SRA:
        case Ins::InsMnemonic::SRLI:
        case Ins::InsMnemonic::SRAI:
            return op1 >> op2;
        case Ins::InsMnemonic::SLT:
        case Ins::InsMnemonic::SLTI:
        case Ins::InsMnemonic::SLTU: // SLTU and SLTUI compares unsigned values
        case Ins::InsMnemonic::SLTUI:
            return op1 < op2 ? 1 : 0;
        default:
            return 0;
        }
    }

    void RunWEGen(RegisterDecodeExecute::ControlEx control_ex,
                  uint32_t v_ex [[maybe_unused]], uint32_t* wb_we,
                  uint32_t* mem_we)
    {
        *mem_we = 0;
        *wb_we = 0;

        if (control_ex.ALU_FMT == Ins::InsFormat::S) {
            *mem_we = 1;
        }

        if (control_ex.ALU_FMT == Ins::InsFormat::I ||
            control_ex.ALU_FMT == Ins::InsFormat::R) {
            *wb_we = 1;
        }
    }
};

#endif