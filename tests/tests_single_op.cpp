#include "sim.h"
#include <bitset>
#include <gtest/gtest.h>
#include <iostream>

TEST(TestsSingleOp, TestADD)
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

TEST(TestsSingleOp, TestADDI)
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
    static const std::vector<Ins> INS = { Ins::MakeIns_SW(0x8, 4, 1) };

    Simulation s{};

    s.Init(INS);

    s.sd.SetRegfileData(4, 0xFFEEDDCC);
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

// TEST(TestsSingleOp, TestBEQ)
// {
//     static const std::vector<Ins> INS = { Ins::MakeIns_BEQ(0x8, 2, 3) };

//     Simulation s{};

//     s.Init(INS);

//     s.sd.SetRegfileData(2, 10);
//     s.sd.SetRegfileData(3, 15);

//     ASSERT_EQ(s.sd.GetRegfileData(2), 10);
//     ASSERT_EQ(s.sd.GetRegfileData(3), 15);

//     while (s.state_.reg_wb_fetch.PC < s.imem_sz * 4) {
//         s.Iterate();

//     }

// }

// TEST(TestsSingleOp, TestBNE)
// {
//     static const std::vector<Ins> INS = { Ins::MakeIns_BEQ() };

//     Simulation s{};

//     s.Init(INS);

//     s.sd.SetRegfileData(1, 10);
//     s.sd.SetRegfileData(2, 15);

//     ASSERT_EQ(s.sd.GetRegfileData(1), 10);
//     ASSERT_EQ(s.sd.GetRegfileData(2), 15);

//     while (s.state_.reg_wb_fetch.PC < s.imem_sz * 4) {
//         s.Iterate();
//     }

//     ASSERT_EQ(s.sd.GetRegfileData(1), 10);
//     ASSERT_EQ(s.sd.GetRegfileData(2), 15);
//     ASSERT_EQ(s.sd.GetRegfileData(3), 25);
//     ASSERT_EQ(s.sd.GetRegfileData(4), 40);
//     ASSERT_EQ(s.sd.GetRegfileData(5), 65);
// }

// TEST(TestsSingleOp, TestBLT)
// {
//     static const std::vector<Ins> INS = { Ins::MakeIns_BEQ() };

//     Simulation s{};

//     s.Init(INS);

//     s.sd.SetRegfileData(1, 10);
//     s.sd.SetRegfileData(2, 15);

//     ASSERT_EQ(s.sd.GetRegfileData(1), 10);
//     ASSERT_EQ(s.sd.GetRegfileData(2), 15);

//     while (s.state_.reg_wb_fetch.PC < s.imem_sz * 4) {
//         s.Iterate();
//     }

//     ASSERT_EQ(s.sd.GetRegfileData(1), 10);
//     ASSERT_EQ(s.sd.GetRegfileData(2), 15);
//     ASSERT_EQ(s.sd.GetRegfileData(3), 25);
//     ASSERT_EQ(s.sd.GetRegfileData(4), 40);
//     ASSERT_EQ(s.sd.GetRegfileData(5), 65);
// }

// TEST(TestsSingleOp, TestBGE)
// {
//     static const std::vector<Ins> INS = { Ins::MakeIns_BEQ() };

//     Simulation s{};

//     s.Init(INS);

//     s.sd.SetRegfileData(1, 10);
//     s.sd.SetRegfileData(2, 15);

//     ASSERT_EQ(s.sd.GetRegfileData(1), 10);
//     ASSERT_EQ(s.sd.GetRegfileData(2), 15);

//     while (s.state_.reg_wb_fetch.PC < s.imem_sz * 4) {
//         s.Iterate();
//     }

//     ASSERT_EQ(s.sd.GetRegfileData(1), 10);
//     ASSERT_EQ(s.sd.GetRegfileData(2), 15);
//     ASSERT_EQ(s.sd.GetRegfileData(3), 25);
//     ASSERT_EQ(s.sd.GetRegfileData(4), 40);
//     ASSERT_EQ(s.sd.GetRegfileData(5), 65);
// }

// TEST(TestsSingleOp, TestBLTU)
// {
//     static const std::vector<Ins> INS = { Ins::MakeIns_BEQ() };

//     Simulation s{};

//     s.Init(INS);

//     s.sd.SetRegfileData(1, 10);
//     s.sd.SetRegfileData(2, 15);

//     ASSERT_EQ(s.sd.GetRegfileData(1), 10);
//     ASSERT_EQ(s.sd.GetRegfileData(2), 15);

//     while (s.state_.reg_wb_fetch.PC < s.imem_sz * 4) {
//         s.Iterate();
//     }

//     ASSERT_EQ(s.sd.GetRegfileData(1), 10);
//     ASSERT_EQ(s.sd.GetRegfileData(2), 15);
//     ASSERT_EQ(s.sd.GetRegfileData(3), 25);
//     ASSERT_EQ(s.sd.GetRegfileData(4), 40);
//     ASSERT_EQ(s.sd.GetRegfileData(5), 65);
// }

// TEST(TestsSingleOp, TestBGEU)
// {
//     static const std::vector<Ins> INS = { Ins::MakeIns_BEQ() };

//     Simulation s{};

//     s.Init(INS);

//     s.sd.SetRegfileData(1, 10);
//     s.sd.SetRegfileData(2, 15);

//     ASSERT_EQ(s.sd.GetRegfileData(1), 10);
//     ASSERT_EQ(s.sd.GetRegfileData(2), 15);

//     while (s.state_.reg_wb_fetch.PC < s.imem_sz * 4) {
//         s.Iterate();
//     }

//     ASSERT_EQ(s.sd.GetRegfileData(1), 10);
//     ASSERT_EQ(s.sd.GetRegfileData(2), 15);
//     ASSERT_EQ(s.sd.GetRegfileData(3), 25);
//     ASSERT_EQ(s.sd.GetRegfileData(4), 40);
//     ASSERT_EQ(s.sd.GetRegfileData(5), 65);
// }