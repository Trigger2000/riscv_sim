#include "sim.h"
#include <bitset>
#include <gtest/gtest.h>
#include <iostream>

TEST(TestsPipeline, TestADD)
{
    static const std::vector<Ins> INS = { Ins::MakeIns_ADD(2, 1, 0) };

    Simulation s{};

    s.run(INS);

    ASSERT_EQ(s.sd.GetRegfileData(0), 0);

    // SIGNALS
    // ASSERT_EQ(s.sigs_.PC_DISP, 0);
    // ASSERT_EQ(s.sigs_.PC_EX, 0);
    // ASSERT_EQ(s.sigs_.A1_EX, 0);
    // ASSERT_EQ(s.sigs_.A2_EX, 0);
    // ASSERT_EQ(s.sigs_.BP_MEM, 0);
    // ASSERT_EQ(s.sigs_.WB_D, 0);
    // ASSERT_EQ(s.sigs_.BP_WB 0);
    // ASSERT_EQ(s.sigs_.WB_WE, 0);
    // ASSERT_EQ(s.sigs_.WB_A, 0);
    // ASSERT_EQ(s.sigs_.HU_MEM_RD, 0);
    // ASSERT_EQ(s.sigs_.HU_WB_RD, 0);
    // ASSERT_EQ(s.sigs_.HU_PC_REDIRECT, 0);
    // ASSERT_EQ(s.sigs_.PC_R, 0);
    // ASSERT_EQ(s.sigs_.HU_RS1, 0);
    // ASSERT_EQ(s.sigs_.HU_RS2, 0);

    // // FETCH-DECODE
    // ASSERT_EQ(s.state_.reg_fetch_decode.PC, 0);
    // ASSERT_EQ(s.state_.reg_fetch_decode.PC_R, 0);
    // ASSERT_EQ(s.state_.reg_fetch_decode.ins.GetInsMnemonic(), 0);

    // // DECODE-EXECUTE
    // ASSERT_EQ(s.state_.reg_decode_exec.CONTROL_EX, 0);
    // ASSERT_EQ(s.state_.reg_decode_exec.D1, 0);
    // ASSERT_EQ(s.state_.reg_decode_exec.D2, 0);
    // ASSERT_EQ(s.state_.reg_decode_exec.imm, 0);
    // ASSERT_EQ(s.state_.reg_decode_exec.ins, 0);
    // ASSERT_EQ(s.state_.reg_decode_exec.PC_EX, 0);
    // ASSERT_EQ(s.state_.reg_decode_exec.sign_bit, 0);
    // ASSERT_EQ(s.state_.reg_decode_exec.V_EX, 0);
    // ASSERT_EQ(s.state_.reg_decode_exec.WB_A, 0);

    // // EXECUTE-MEM
    // ASSERT_EQ(s.state_.reg_exec_mem.WD, 0);
    // ASSERT_EQ(s.state_.reg_exec_mem.alu_res, 0);
    // ASSERT_EQ(s.state_.reg_exec_mem.WB_selector, 0);
    // ASSERT_EQ(s.state_.reg_exec_mem.MEM_WE, 0);
    // ASSERT_EQ(s.state_.reg_exec_mem.WB_WE, 0);
    // ASSERT_EQ(s.state_.reg_exec_mem.WB_A, 0);
    // ASSERT_EQ(s.state_.reg_exec_mem.mem_width, 0);
    // ASSERT_EQ(s.state_.reg_exec_mem.NOP, 0);

    // // MEM-WB
    // ASSERT_EQ(s.state_.reg_mem_wb.WB_A, 0);
    // ASSERT_EQ(s.state_.reg_mem_wb.WB_D, 0);
    // ASSERT_EQ(s.state_.reg_mem_wb.WB_WE, 0);

    // // WB-FETCH
    // ASSERT_EQ(s.state_.reg_wb_fetch.PC, 0);
}

TEST(TestsPipeline, TestADD_set_regfile)
{
    static const std::vector<Ins> INS = { Ins::MakeIns_ADD(2, 1, 3) };

    Simulation s{};

    s.Init(INS);

    s.sd.SetRegfileData(1, 10);
    s.sd.SetRegfileData(2, 15);

    s.Iterate();
    s.Iterate();
    s.Iterate();
    s.Iterate();
    s.Iterate();
    s.Iterate();
    s.Iterate();

    ASSERT_EQ(s.sd.GetRegfileData(3), 25);
}

// TEST(TestsPipeline, TestSW)
// {
//     static const std::vector<Ins> INS = { Ins::MakeIns_SW(0x8, 0, 1) };

//     Simulation s{};

//     s.Init(INS);

//     s.sd.SetRegfileData(0, 0xFFEEDDCC);
//     s.sd.SetRegfileData(1, 40);

//     s.Iterate();
//     s.Iterate();
//     s.Iterate();
//     s.Iterate();
//     s.Iterate();
//     s.Iterate();
//     s.Iterate();

//     ASSERT_EQ(s.sm.DmemRead(40, 32), 0xFFEEDDCC);
// }

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}