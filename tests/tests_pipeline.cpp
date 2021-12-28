#include "sim.h"
#include <bitset>
#include <gtest/gtest.h>
#include <iostream>

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

TEST(TestsPipeline, TestADD)
{
    static const std::vector<Ins> INS = { Ins::MakeIns_ADD(2, 1, 3) };

    Simulation s{};

    s.Init(INS);

    s.sd.SetRegfileData(1, 10);
    s.sd.SetRegfileData(2, 15);

    while (s.state_.reg_wb_fetch.PC < s.imem_sz * 4) {
        s.Iterate();
    }

    ASSERT_EQ(s.sd.GetRegfileData(3), 25);
}

TEST(TestsPipeline, TestADD2)
{
    static const std::vector<Ins> INS = { Ins::MakeIns_ADD(2, 1, 3),
                                          Ins::MakeIns_ADD(5, 4, 6) };

    Simulation s{};

    s.Init(INS);

    s.sd.SetRegfileData(1, 10);
    s.sd.SetRegfileData(2, 15);
    s.sd.SetRegfileData(4, 25);
    s.sd.SetRegfileData(5, 45);

    while (s.state_.reg_wb_fetch.PC < s.imem_sz * 4) {
        s.Iterate();
    }

    ASSERT_EQ(s.sd.GetRegfileData(3), 25);
    ASSERT_EQ(s.sd.GetRegfileData(6), 70);
}

TEST(TestsPipeline, TestCROSS2)
{
    static const std::vector<Ins> INS = { Ins::MakeIns_ADD(2, 1, 3),
                                          Ins::MakeIns_ADD(3, 2, 4) };

    Simulation s{};

    s.Init(INS);

    s.sd.SetRegfileData(1, 10);
    s.sd.SetRegfileData(2, 15);

    while (s.state_.reg_wb_fetch.PC < s.imem_sz * 4) {
        s.Iterate();
    }

    ASSERT_EQ(s.sd.GetRegfileData(3), 25);
    ASSERT_EQ(s.sd.GetRegfileData(4), 40);
}

TEST(TestsPipeline, TestCROSS3)
{
    static const std::vector<Ins> INS = { Ins::MakeIns_ADD(2, 1, 3),
                                          Ins::MakeIns_ADD(2, 3, 4),
                                          Ins::MakeIns_ADD(1, 3, 5) };

    Simulation s{};

    s.Init(INS);

    s.sd.SetRegfileData(1, 10);
    s.sd.SetRegfileData(2, 15);

    while (s.state_.reg_wb_fetch.PC < s.imem_sz * 4) {
        s.Iterate();
    }

    ASSERT_EQ(s.sd.GetRegfileData(3), 25);
    ASSERT_EQ(s.sd.GetRegfileData(4), 40);
    ASSERT_EQ(s.sd.GetRegfileData(5), 35);
}

TEST(TestsPipeline, TestADDI)
{
    static const std::vector<Ins> INS = { Ins::MakeIns_ADDI(50, 1, 2) };

    Simulation s{};

    s.Init(INS);

    s.sd.SetRegfileData(1, 15);

    while (s.state_.reg_wb_fetch.PC < s.imem_sz * 4) {
        s.Iterate();
    }

    ASSERT_EQ(s.sd.GetRegfileData(2), 65);
}

TEST(TestsPipeline, TestSW)
{
    static const std::vector<Ins> INS = { Ins::MakeIns_SW(0x8, 0, 1) };

    Simulation s{};

    s.Init(INS);

    s.sd.SetRegfileData(0, 0xFFEEDDCC);
    s.sd.SetRegfileData(1, 40);

    while (s.state_.reg_wb_fetch.PC < s.imem_sz * 4) {
        s.Iterate();
    }

    ASSERT_EQ(s.sm.DmemRead(48, 32), 0xFFEEDDCC);
}

TEST(TestsPipeline, TestLW)
{
    static const std::vector<Ins> INS = { Ins::MakeIns_LW(0x8, 3, 1) };

    Simulation s{};

    s.Init(INS);

    s.sm.DmemWrite(1, 0xFFEEDDCC, 48, 32);
    s.sd.SetRegfileData(3, 40);

    while (s.state_.reg_wb_fetch.PC < s.imem_sz * 4) {
        s.Iterate();
    }

    ASSERT_EQ(s.sd.GetRegfileData(1), 0xFFEEDDCC);
}

TEST(TestsPipeline, TestADDLW)
{
    static const std::vector<Ins> INS = { Ins::MakeIns_LW(0x8, 3, 1),
                                          Ins::MakeIns_ADD(1, 2, 3) };

    Simulation s{};

    s.Init(INS);

    s.sm.DmemWrite(1, 13, 48, 32);
    s.sd.SetRegfileData(2, 10);
    s.sd.SetRegfileData(3, 40);

    while (s.state_.reg_wb_fetch.PC < s.imem_sz * 4) {
        s.Iterate();
    }

    ASSERT_EQ(s.sd.GetRegfileData(1), 13);
    ASSERT_EQ(s.sd.GetRegfileData(2), 10);
    ASSERT_EQ(s.sd.GetRegfileData(3), 23);
    ASSERT_EQ(s.sigs_.HU_STALL, 0);
}

TEST(TestsPipeline, TestADDLW2)
{
    static const std::vector<Ins> INS = { Ins::MakeIns_LW(0x8, 3, 1),
                                          Ins::MakeIns_LW(0x8, 3, 1),
                                          Ins::MakeIns_ADD(1, 2, 3) };

    Simulation s{};

    s.Init(INS);

    s.sm.DmemWrite(1, 13, 48, 32);
    s.sd.SetRegfileData(2, 10);
    s.sd.SetRegfileData(3, 40);

    while (s.state_.reg_wb_fetch.PC < s.imem_sz * 4) {
        s.Iterate();
    }

    ASSERT_EQ(s.sd.GetRegfileData(1), 13);
    ASSERT_EQ(s.sd.GetRegfileData(2), 10);
    ASSERT_EQ(s.sd.GetRegfileData(3), 23);
    ASSERT_EQ(s.sigs_.HU_STALL, 0);
}

TEST(TestsPipeline, TestLWaSW)
{
    static const std::vector<Ins> INS = { Ins::MakeIns_SW(0x8, 0, 1),
                                          Ins::MakeIns_LW(0x8, 2, 3) };

    Simulation s{};

    s.Init(INS);

    s.sd.SetRegfileData(0, 0xFFEEDDCC);
    s.sd.SetRegfileData(1, 40);
    s.sd.SetRegfileData(2, 40);

    while (s.state_.reg_wb_fetch.PC < s.imem_sz * 4) {
        s.Iterate();
    }

    ASSERT_EQ(s.sd.GetRegfileData(3), 0xFFEEDDCC);
}

TEST(TestsPipeline, Benchmark)
{
    static const std::vector<Ins> INS = {
        Ins::MakeIns_ADDI(1, 0, 1),
        Ins::MakeIns_ADDI(-2, 0, 2),
        Ins::MakeIns_ADD(1, 2, 0),
    };

    Simulation s{};

    s.run(INS);

    ASSERT_EQ(s.sd.GetRegfileData(1), 1);
    ASSERT_EQ(s.sd.GetRegfileData(2), -2);
    ASSERT_EQ(s.sd.GetRegfileData(0), -1);
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}