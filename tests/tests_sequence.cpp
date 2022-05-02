#include "sim.h"
#include <bitset>
#include <gtest/gtest.h>
#include <iostream>

TEST(TestsSequence, TestADD2)
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

TEST(TestsSequence, Benchmark)
{
    static const std::vector<Ins> INS = {
        Ins::MakeIns_ADDI(1, 4, 1),
        Ins::MakeIns_ADDI(-2, 4, 2),
        Ins::MakeIns_ADD(1, 2, 4),
    };

    Simulation s{};

    s.run(INS);

    ASSERT_EQ(s.sd.GetRegfileData(1), 1);
    ASSERT_EQ(s.sd.GetRegfileData(2), -2);
    ASSERT_EQ(s.sd.GetRegfileData(4), -1);
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}