#include "stages/stage_execute.h"
#include "utils/registers.h"
#include "utils/simulation_state.h"
#include <bitset>
#include <gtest/gtest.h>
#include <iostream>

// R

TEST(TestsExecute, TestADD)
{
    SimulationSignals sigs{};

    sigs.HU_RS1 = 0;
    sigs.HU_RS2 = 0;
    sigs.BP_MEM = 33; // shouldn't affect
    sigs.BP_WB = 31;  // shouldn't affect

    RegisterDecodeExecute reg_in;
    reg_in.CONTROL_EX.ALUOP = Ins::InsMnemonic::ADD;
    reg_in.CONTROL_EX.ALU_FMT = Ins::InsFormat::R;
    reg_in.CONTROL_EX.ALU_SRC2 = 0;
    reg_in.D1 = 10;
    reg_in.D2 = 13;
    reg_in.sign_bit = 0xFFFFFFFF; // shouldn't affect
    reg_in.V_EX = 0;
    reg_in.ins = Ins::MakeIns_ADD(0, 0, 11); // only 11 should affect
    int32_t imm = 0;
    reg_in.ins.GetImm(&imm);
    uint32_t rd = 0;
    reg_in.ins.GetRd(&rd);

    StageExecute se{};

    auto reg_out = se.run(reg_in, &sigs);

    ASSERT_EQ(reg_out.WD, reg_in.D2);
    ASSERT_EQ(reg_out.alu_res, reg_in.D1 + reg_in.D2);
    ASSERT_EQ(reg_out.WB_selector, 1);
    ASSERT_EQ(reg_out.MEM_WE, 0);
    ASSERT_EQ(reg_out.WB_WE, 1);
    ASSERT_EQ(reg_out.WB_A, rd);
    ASSERT_EQ(reg_out.mem_width, 8);
}

TEST(TestsExecute, TestADD_Neg)
{
    SimulationSignals sigs{};

    sigs.HU_RS1 = 0;
    sigs.HU_RS2 = 0;
    sigs.BP_MEM = 33; // shouldn't affect
    sigs.BP_WB = 31;  // shouldn't affect

    RegisterDecodeExecute reg_in;
    reg_in.CONTROL_EX.ALUOP = Ins::InsMnemonic::ADD;
    reg_in.CONTROL_EX.ALU_FMT = Ins::InsFormat::R;
    reg_in.CONTROL_EX.ALU_SRC2 = 0;
    reg_in.D1 = 10;
    reg_in.D2 = -13;
    reg_in.sign_bit = 0xFFFFFFFF; // shouldn't affect
    reg_in.V_EX = 0;
    reg_in.ins = Ins::MakeIns_ADD(0, 0, 11); // only 11 should affect
    int32_t imm = 0;
    reg_in.ins.GetImm(&imm);
    uint32_t rd = 0;
    reg_in.ins.GetRd(&rd);

    StageExecute se{};

    auto reg_out = se.run(reg_in, &sigs);

    ASSERT_EQ(reg_out.WD, reg_in.D2);
    ASSERT_EQ(reg_out.alu_res, reg_in.D1 + reg_in.D2);
    ASSERT_EQ(reg_out.WB_selector, 1);
    ASSERT_EQ(reg_out.MEM_WE, 0);
    ASSERT_EQ(reg_out.WB_WE, 1);
    ASSERT_EQ(reg_out.WB_A, rd);
    ASSERT_EQ(reg_out.mem_width, 8);
}

// I

// ARITHMETIC

TEST(TestsExecute, TestADDI)
{
    SimulationSignals sigs{};

    sigs.HU_RS1 = 0;
    sigs.HU_RS2 = 0;
    sigs.BP_MEM = 33; // shouldn't affect
    sigs.BP_WB = 31;  // shouldn't affect

    RegisterDecodeExecute reg_in;
    reg_in.CONTROL_EX.ALUOP = Ins::InsMnemonic::ADDI;
    reg_in.CONTROL_EX.ALU_FMT = Ins::InsFormat::I;
    reg_in.CONTROL_EX.ALU_SRC2 = 1;
    reg_in.D1 = 10;
    reg_in.D2 = 13;               // shouldn't affect
    reg_in.sign_bit = 0xFFFFFFFF; // shouldn't affect
    reg_in.V_EX = 0;
    reg_in.ins =
        Ins::MakeIns_ADDI(100, 0, 11); // only 100 and 11 should affect
    int32_t imm = 0;
    reg_in.ins.GetImm(&imm);
    uint32_t rd = 0;
    reg_in.ins.GetRd(&rd);

    StageExecute se{};

    auto reg_out = se.run(reg_in, &sigs);

    ASSERT_EQ(reg_out.WD, reg_in.D2);
    ASSERT_EQ(reg_out.alu_res, reg_in.D1 + imm);
    ASSERT_EQ(reg_out.WB_selector, 1);
    ASSERT_EQ(reg_out.MEM_WE, 0);
    ASSERT_EQ(reg_out.WB_WE, 1);
    ASSERT_EQ(reg_out.WB_A, rd);
    ASSERT_EQ(reg_out.mem_width, 8);
}

TEST(TestsExecute, TestADDI_Neg)
{
    SimulationSignals sigs{};

    sigs.HU_RS1 = 0;
    sigs.HU_RS2 = 0;
    sigs.BP_MEM = 33; // shouldn't affect
    sigs.BP_WB = 31;  // shouldn't affect

    RegisterDecodeExecute reg_in;
    reg_in.CONTROL_EX.ALUOP = Ins::InsMnemonic::ADDI;
    reg_in.CONTROL_EX.ALU_FMT = Ins::InsFormat::I;
    reg_in.CONTROL_EX.ALU_SRC2 = 1;
    reg_in.D1 = 10;
    reg_in.D2 = 13;               // shouldn't affect
    reg_in.sign_bit = 0xFFFFFFFF; // shouldn't affect
    reg_in.V_EX = 0;
    reg_in.ins =
        Ins::MakeIns_ADDI(-100, 0, 11); // only -100 and 11 should affect
    int32_t imm = 0;
    reg_in.ins.GetImm(&imm);
    uint32_t rd = 0;
    reg_in.ins.GetRd(&rd);

    StageExecute se{};

    auto reg_out = se.run(reg_in, &sigs);

    ASSERT_EQ(reg_out.WD, reg_in.D2);
    ASSERT_EQ(reg_out.alu_res, reg_in.D1 + imm);
    ASSERT_EQ(reg_out.WB_selector, 1);
    ASSERT_EQ(reg_out.MEM_WE, 0);
    ASSERT_EQ(reg_out.WB_WE, 1);
    ASSERT_EQ(reg_out.WB_A, rd);
    ASSERT_EQ(reg_out.mem_width, 8);
}

// LOAD

TEST(TestsExecute, TestLB)
{
    SimulationSignals sigs{};

    sigs.HU_RS1 = 0;
    sigs.HU_RS2 = 0;
    sigs.BP_MEM = 33; // shouldn't affect
    sigs.BP_WB = 31;  // shouldn't affect

    RegisterDecodeExecute reg_in;
    reg_in.CONTROL_EX.ALUOP = Ins::InsMnemonic::LB;
    reg_in.CONTROL_EX.ALU_FMT = Ins::InsFormat::I;
    reg_in.CONTROL_EX.ALU_SRC2 = 1;
    reg_in.D1 = 10;
    reg_in.D2 = 13;               // shouldn't affect
    reg_in.sign_bit = 0xFFFFFFFF; // shouldn't affect
    reg_in.V_EX = 0;
    reg_in.ins = Ins::MakeIns_LB(20, 0, 11); // only 20 and 11 should affect
    int32_t imm = 0;
    reg_in.ins.GetImm(&imm);
    uint32_t rd = 0;
    reg_in.ins.GetRd(&rd);

    StageExecute se{};

    auto reg_out = se.run(reg_in, &sigs);

    ASSERT_EQ(reg_out.WD, reg_in.D2);
    ASSERT_EQ(reg_out.alu_res, reg_in.D1 + imm);
    ASSERT_EQ(reg_out.WB_selector, 0);
    ASSERT_EQ(reg_out.MEM_WE, 0);
    ASSERT_EQ(reg_out.WB_WE, 1);
    ASSERT_EQ(reg_out.WB_A, rd);
    ASSERT_EQ(reg_out.mem_width, 8);
}

TEST(TestsExecute, TestLB_Neg)
{
    SimulationSignals sigs{};

    sigs.HU_RS1 = 0;
    sigs.HU_RS2 = 0;
    sigs.BP_MEM = 33; // shouldn't affect
    sigs.BP_WB = 31;  // shouldn't affect

    RegisterDecodeExecute reg_in;
    reg_in.CONTROL_EX.ALUOP = Ins::InsMnemonic::LB;
    reg_in.CONTROL_EX.ALU_FMT = Ins::InsFormat::I;
    reg_in.CONTROL_EX.ALU_SRC2 = 1;
    reg_in.D1 = 10;
    reg_in.D2 = 13;               // shouldn't affect
    reg_in.sign_bit = 0xFFFFFFFF; // shouldn't affect
    reg_in.V_EX = 0;
    reg_in.ins = Ins::MakeIns_LB(-20, 0, 11); // only -20 and 11 should affect
    int32_t imm = 0;
    reg_in.ins.GetImm(&imm);
    uint32_t rd = 0;
    reg_in.ins.GetRd(&rd);

    StageExecute se{};

    auto reg_out = se.run(reg_in, &sigs);

    ASSERT_EQ(reg_out.WD, reg_in.D2);
    ASSERT_EQ(reg_out.alu_res, reg_in.D1 + imm);
    ASSERT_EQ(reg_out.WB_selector, 0);
    ASSERT_EQ(reg_out.MEM_WE, 0);
    ASSERT_EQ(reg_out.WB_WE, 1);
    ASSERT_EQ(reg_out.WB_A, rd);
    ASSERT_EQ(reg_out.mem_width, 8);
}

// JALR

// S

TEST(TestsExecute, TestSB)
{
    SimulationSignals sigs{};

    sigs.HU_RS1 = 0;
    sigs.HU_RS2 = 0;
    sigs.BP_MEM = 100; // shouldn't affect
    sigs.BP_WB = 111;  // shouldn't affect

    RegisterDecodeExecute reg_in;
    reg_in.CONTROL_EX.ALUOP = Ins::InsMnemonic::SB;
    reg_in.CONTROL_EX.ALU_FMT = Ins::InsFormat::S;
    reg_in.CONTROL_EX.ALU_SRC2 = 1;
    reg_in.D1 = 40; // adr
    reg_in.D2 = 10; // data
    reg_in.sign_bit = 0;
    reg_in.V_EX = 0;
    reg_in.ins = Ins::MakeIns_SB(-20, 0, 0); // only -20 should affect
    int32_t imm = 0;
    reg_in.ins.GetImm(&imm);
    uint32_t rd = 0;
    reg_in.ins.GetRd(&rd);

    StageExecute se{};

    auto reg_out = se.run(reg_in, &sigs);

    ASSERT_EQ(reg_out.WD, reg_in.D2);
    ASSERT_EQ(reg_out.alu_res, reg_in.D1 + imm);
    ASSERT_EQ(reg_out.WB_selector, 1);
    ASSERT_EQ(reg_out.MEM_WE, 1);
    ASSERT_EQ(reg_out.WB_WE, 0);
    ASSERT_EQ(reg_out.WB_A, rd);
    ASSERT_EQ(reg_out.mem_width, 8);
}

TEST(TestsExecute, TestSB_Neg)
{
    SimulationSignals sigs{};

    sigs.HU_RS1 = 0;
    sigs.HU_RS2 = 0;
    sigs.BP_MEM = 100; // shouldn't affect
    sigs.BP_WB = 111;  // shouldn't affect

    RegisterDecodeExecute reg_in;
    reg_in.CONTROL_EX.ALUOP = Ins::InsMnemonic::SB;
    reg_in.CONTROL_EX.ALU_FMT = Ins::InsFormat::S;
    reg_in.CONTROL_EX.ALU_SRC2 = 1;
    reg_in.D1 = 40; // adr
    reg_in.D2 = 10; // data
    reg_in.sign_bit = 0;
    reg_in.V_EX = 0;
    reg_in.ins = Ins::MakeIns_SB(20, 0, 0); // only 20 should affect
    int32_t imm = 0;
    reg_in.ins.GetImm(&imm);
    uint32_t rd = 0;
    reg_in.ins.GetRd(&rd);

    StageExecute se{};

    auto reg_out = se.run(reg_in, &sigs);

    ASSERT_EQ(reg_out.WD, reg_in.D2);
    ASSERT_EQ(reg_out.alu_res, reg_in.D1 + imm);
    ASSERT_EQ(reg_out.WB_selector, 1);
    ASSERT_EQ(reg_out.MEM_WE, 1);
    ASSERT_EQ(reg_out.WB_WE, 0);
    ASSERT_EQ(reg_out.WB_A, rd);
    ASSERT_EQ(reg_out.mem_width, 8);
}

// TEST(TestsExecute, TestBEQ)
// {
//     SimulationSignals sigs{};

//     sigs.HU_RS1 = 0;
//     sigs.HU_RS2 = 0;
//     sigs.BP_MEM = 0;
//     sigs.BP_WB = 0;

//     RegisterDecodeExecute reg_in;
//     reg_in.CONTROL_EX.ALUOP = Ins::InsMnemonic::BEQ;
//     reg_in.CONTROL_EX.ALU_FMT = Ins::InsFormat::B;
//     reg_in.CONTROL_EX.ALU_SRC2 = 0;
//     reg_in.D1 = 0;
//     reg_in.D2 = 0;
//     reg_in.sign_bit = 0;
//     reg_in.V_EX = 0;
//     reg_in.imm = 0;
//     reg_in.WB_A = 0;
//     reg_in.ins = Ins::MakeIns_ADD();

//     StageExecute se{};

//     auto reg_out = se.run(reg_in, &sigs);

//     ASSERT_EQ(reg_out.WD, 0);
//     ASSERT_EQ(reg_out.alu_res, 0);
//     ASSERT_EQ(reg_out.WB_selector, 0);
//     ASSERT_EQ(reg_out.MEM_WE, 0);
//     ASSERT_EQ(reg_out.WB_WE, 0);
//     ASSERT_EQ(reg_out.WB_A, 0);
// }

// TEST(TestsExecute, TestLUI)
// {
//     SimulationSignals sigs{};

//     sigs.HU_RS1 = 0;
//     sigs.HU_RS2 = 0;
//     sigs.BP_MEM = 0;
//     sigs.BP_WB = 0;

//     RegisterDecodeExecute reg_in;
//     reg_in.CONTROL_EX.ALUOP = Ins::InsMnemonic::LUI;
//     reg_in.CONTROL_EX.ALU_FMT = Ins::InsFormat::U;
//     reg_in.CONTROL_EX.ALU_SRC2 = 0;
//     reg_in.D1 = 0;
//     reg_in.D2 = 0;
//     reg_in.sign_bit = 0;
//     reg_in.V_EX = 0;
//     reg_in.imm = 0;
//     reg_in.WB_A = 0;
//     reg_in.ins = Ins::MakeIns_ADD();

//     StageExecute se{};

//     auto reg_out = se.run(reg_in, &sigs);

//     ASSERT_EQ(reg_out.WD, 0);
//     ASSERT_EQ(reg_out.alu_res, 0);
//     ASSERT_EQ(reg_out.WB_selector, 0);
//     ASSERT_EQ(reg_out.MEM_WE, 0);
//     ASSERT_EQ(reg_out.WB_WE, 0);
//     ASSERT_EQ(reg_out.WB_A, 0);
// }

// TEST(TestsExecute, TestJAL)
// {
//     SimulationSignals sigs{};

//     sigs.HU_RS1 = 0;
//     sigs.HU_RS2 = 0;
//     sigs.BP_MEM = 0;
//     sigs.BP_WB = 0;

//     RegisterDecodeExecute reg_in;
//     reg_in.CONTROL_EX.ALUOP = Ins::InsMnemonic::JAL;
//     reg_in.CONTROL_EX.ALU_FMT = Ins::InsFormat::J;
//     reg_in.CONTROL_EX.ALU_SRC2 = 0;
//     reg_in.D1 = 0;
//     reg_in.D2 = 0;
//     reg_in.sign_bit = 0;
//     reg_in.V_EX = 0;
//     reg_in.imm = 0;
//     reg_in.WB_A = 0;
//     reg_in.ins = Ins::MakeIns_ADD();

//     StageExecute se{};

//     auto reg_out = se.run(reg_in, &sigs);

//     ASSERT_EQ(reg_out.WD, 0);
//     ASSERT_EQ(reg_out.alu_res, 0);
//     ASSERT_EQ(reg_out.WB_selector, 0);
//     ASSERT_EQ(reg_out.MEM_WE, 0);
//     ASSERT_EQ(reg_out.WB_WE, 0);
//     ASSERT_EQ(reg_out.WB_A, 0);
// }