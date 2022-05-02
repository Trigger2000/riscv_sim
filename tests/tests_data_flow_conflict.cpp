#include "sim.h"
#include <bitset>
#include <gtest/gtest.h>
#include <iostream>

TEST(TestsDataFlowConflict, TestCROSS2)
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

TEST(TestsDataFlowConflict, TestCROSS3)
{
    static const std::vector<Ins> INS = { Ins::MakeIns_ADD(2, 1, 3),
                                          Ins::MakeIns_ADD(2, 3, 4),
                                          Ins::MakeIns_ADD(4, 3, 5) };

    Simulation s{};

    s.Init(INS);

    s.sd.SetRegfileData(1, 10);
    s.sd.SetRegfileData(2, 15);

    ASSERT_EQ(s.sd.GetRegfileData(1), 10);
    ASSERT_EQ(s.sd.GetRegfileData(2), 15);

    while (s.state_.reg_wb_fetch.PC < s.imem_sz * 4) {
        s.Iterate();
    }

    ASSERT_EQ(s.sd.GetRegfileData(1), 10);
    ASSERT_EQ(s.sd.GetRegfileData(2), 15);
    ASSERT_EQ(s.sd.GetRegfileData(3), 25);
    ASSERT_EQ(s.sd.GetRegfileData(4), 40);
    ASSERT_EQ(s.sd.GetRegfileData(5), 65);
}

TEST(TestsDataFlowConflict, TestADDLW)
{
    static const std::vector<Ins> INS = { Ins::MakeIns_LW(0x8, 3, 1),
                                          Ins::MakeIns_ADD(1, 2, 3) };

    Simulation s{};

    s.Init(INS);

    s.sm.DmemWrite(1, 13, 48, 32);
    s.sd.SetRegfileData(2, 10);
    s.sd.SetRegfileData(3, 40);

    ASSERT_EQ(s.sd.GetRegfileData(2), 10);
    ASSERT_EQ(s.sd.GetRegfileData(3), 40);

    while (s.state_.reg_wb_fetch.PC < s.imem_sz * 4) {
        s.Iterate();
    }

    ASSERT_EQ(s.sd.GetRegfileData(1), 13);
    ASSERT_EQ(s.sd.GetRegfileData(2), 10);
    ASSERT_EQ(s.sd.GetRegfileData(3), 23);
    ASSERT_EQ(s.sigs_.HU_STALL, 0);
}

TEST(TestsDataFlowConflict, TestSWLW)
{
    static const std::vector<Ins> INS = { Ins::MakeIns_SW(0x8, 4, 1),
                                          Ins::MakeIns_LW(0x8, 2, 3) };

    Simulation s{};

    s.Init(INS);

    s.sd.SetRegfileData(4, 0xFFEEDDCC);
    s.sd.SetRegfileData(1, 40);
    s.sd.SetRegfileData(2, 40);

    while (s.state_.reg_wb_fetch.PC < s.imem_sz * 4) {
        s.Iterate();
    }

    ASSERT_EQ(s.sd.GetRegfileData(3), 0xFFEEDDCC);
}