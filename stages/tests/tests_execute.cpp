#include "stages/stage_execute.h"
#include "utils/registers.h"
#include "utils/simulation_state.h"
#include <bitset>
#include <gtest/gtest.h>
#include <iostream>

TEST(TestsExecute, TestADD)
{
    // struct RegisterExecuteMemory
    // {
    //     int32_t WD = 0;
    //     int32_t alu_res = 0;
    //     uint32_t CONTROL_EX = 0; // rename?
    //     uint32_t MEM_WE = 0;
    //     uint32_t WB_WE = 0;
    //     uint32_t WB_A = 0;
    // };

    // struct RegisterDecodeExecute
    // {
    //     struct ControlEx
    //     {
    //         Ins::InsMnemonic ALUOP = Ins::InsMnemonic::INVALID;
    //         uint32_t ALU_SRC2 = 0;
    //     };

    //     ControlEx CONTROL_EX;
    //     int32_t D1 = 0;
    //     int32_t D2 = 0;
    //     uint32_t sign_bit = 0;
    //     uint32_t PC_EX;
    //     uint32_t V_EX = 0;

    //     // bus 30:7
    //     int32_t imm = 0;
    //     uint32_t WB_A = 0;
    // };

    // RegisterDecodeExecute reg_in;
    // reg_in.CONTROL_EX.ALUOP = Ins::InsMnemonic::ADD;
    // reg_in.CONTROL_EX.ALU_SRC2
}
