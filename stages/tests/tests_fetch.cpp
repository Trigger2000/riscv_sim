#include "stages/stage_fetch.h"
#include "utils/registers.h"
#include "utils/simulation_state.h"
#include <bitset>
#include <gtest/gtest.h>
#include <iostream>

TEST(FetchTestNoSignals, FetchInvalidIns)
{
    Imem imem({ Ins() });
    StageFetch f{ imem };
    RegisterWriteBackFetch reg_in;
    SimulationSignals sigs;

    ASSERT_EQ(reg_in.PC, 0);

    EXPECT_DEATH({ f.run(&reg_in, &sigs); }, "");
}

TEST(FetchTestNoSignals, FetchInvalidPC)
{
    Imem imem({ Ins() });
    StageFetch f{ imem };
    RegisterWriteBackFetch reg_in;
    SimulationSignals sigs;

    ASSERT_EQ(reg_in.PC, 0);

    reg_in.PC = 100;

    EXPECT_DEATH({ f.run(&reg_in, &sigs); }, "");
}

TEST(FetchTestNoSignals, FetchSingleIns)
{
    static const Ins ADD = Ins::MakeIns_ADD(2, 1, 0);
    Imem imem({ ADD });
    StageFetch f{ imem };
    RegisterWriteBackFetch reg_in;
    SimulationSignals sigs;

    ASSERT_EQ(reg_in.PC, 0);

    RegisterFetchDecode reg_out = f.run(&reg_in, &sigs);

    ASSERT_EQ(reg_out.ins.GetInsRaw(), ADD.GetInsRaw());
    ASSERT_EQ(reg_out.PC, 0);
    ASSERT_EQ(reg_out.PC_R, 0);
    ASSERT_EQ(reg_in.PC, 1 * INSTRUCTION_LENGTH);
}

TEST(FetchTestNoSignals, FetchManyIns)
{
    static const std::vector<Ins> INS = { Ins::MakeIns_ADD(2, 1, 0),
                                          Ins::MakeIns_ADDI(31, 100, 0),
                                          Ins::MakeIns_BEQ(100, 5, 3),
                                          Ins::MakeIns_JALR(100, 3, 4) };

    Imem imem(INS);

    StageFetch f{ imem };
    RegisterWriteBackFetch reg_in;
    SimulationSignals sigs;

    ASSERT_EQ(reg_in.PC, 0);

    for (size_t i = 0; i < INS.size(); ++i) {
        RegisterFetchDecode reg_out = f.run(&reg_in, &sigs);
        ASSERT_EQ(reg_out.ins.GetInsRaw(), INS[i].GetInsRaw());
        ASSERT_EQ(reg_out.PC, i * INSTRUCTION_LENGTH);
        ASSERT_EQ(reg_out.PC_R, 0);
        ASSERT_EQ(reg_in.PC, (i + 1) * INSTRUCTION_LENGTH);
    }
}

TEST(FetchTestSetSignals, PC_R)
{
    static const Ins ADD = Ins::MakeIns_ADD(2, 1, 0);
    Imem imem({ ADD });
    StageFetch f{ imem };
    RegisterWriteBackFetch reg_in;
    SimulationSignals sigs;
    sigs.PC_R = 1;

    ASSERT_EQ(reg_in.PC, 0);

    RegisterFetchDecode reg_out = f.run(&reg_in, &sigs);

    ASSERT_EQ(reg_out.ins.GetInsRaw(), ADD.GetInsRaw());
    ASSERT_EQ(reg_out.PC, 0);
    ASSERT_EQ(reg_out.PC_R, 1);
    ASSERT_EQ(reg_in.PC, 0);
}

TEST(FetchTestSetSignals, PC_R_PC_DISP)
{
    static const Ins ADD = Ins::MakeIns_ADD(2, 1, 0);
    Imem imem({ ADD });
    StageFetch f{ imem };
    RegisterWriteBackFetch reg_in;
    SimulationSignals sigs;
    sigs.PC_R = 1;
    sigs.PC_DISP = 4 * INSTRUCTION_LENGTH;

    ASSERT_EQ(reg_in.PC, 0);

    RegisterFetchDecode reg_out = f.run(&reg_in, &sigs);

    ASSERT_EQ(reg_out.ins.GetInsRaw(), ADD.GetInsRaw());
    ASSERT_EQ(reg_out.PC, 0);
    ASSERT_EQ(reg_out.PC_R, 1);
    ASSERT_EQ(reg_in.PC, 4 * INSTRUCTION_LENGTH);
}

TEST(FetchTestSetSignals, PC_R_PC_DISP_Neg)
{
    static const std::vector<Ins> INS = {
        Ins::MakeIns_ADD(2, 1, 0),   Ins::MakeIns_ADDI(31, 100, 0),
        Ins::MakeIns_BEQ(100, 5, 3), Ins::MakeIns_JALR(100, 3, 4),
        Ins::MakeIns_ADD(2, 1, 0),   Ins::MakeIns_ADDI(31, 100, 0),
        Ins::MakeIns_BEQ(100, 5, 3), Ins::MakeIns_JALR(100, 3, 4),
        Ins::MakeIns_ADD(2, 1, 0),   Ins::MakeIns_ADDI(31, 100, 0),
        Ins::MakeIns_BEQ(100, 5, 3), Ins::MakeIns_JALR(100, 3, 4),
        Ins::MakeIns_ADD(2, 1, 0),   Ins::MakeIns_ADDI(31, 100, 0),
        Ins::MakeIns_BEQ(100, 5, 3), Ins::MakeIns_JALR(100, 3, 4),
    };
    Imem imem(INS);
    StageFetch f{ imem };
    RegisterWriteBackFetch reg_in;
    SimulationSignals sigs;
    sigs.PC_R = 1;
    sigs.PC_DISP = -4 * INSTRUCTION_LENGTH;

    ASSERT_EQ(reg_in.PC, 0);

    reg_in.PC = 10 * INSTRUCTION_LENGTH;
    sigs.PC_EX = reg_in.PC;

    RegisterFetchDecode reg_out = f.run(&reg_in, &sigs);
    //  Ins Imem::Fetch(uint32_t): Assertion `pos < instructions_.size()'
    //  failed

    ASSERT_EQ(reg_out.ins.GetInsRaw(), INS[10].GetInsRaw());
    ASSERT_EQ(reg_out.PC, 10 * INSTRUCTION_LENGTH);
    ASSERT_EQ(reg_out.PC_R, 1);
    ASSERT_EQ(reg_in.PC, 6 * INSTRUCTION_LENGTH);
}

TEST(FetchTestSetSignals, PC_R_PC_DISP_PC_EX)
{
    static const Ins ADD = Ins::MakeIns_ADD(2, 1, 0);
    Imem imem({ ADD });
    StageFetch f{ imem };
    RegisterWriteBackFetch reg_in;
    SimulationSignals sigs;
    sigs.PC_R = 1;
    sigs.PC_EX = 3 * INSTRUCTION_LENGTH;
    sigs.PC_DISP = 4 * INSTRUCTION_LENGTH;

    ASSERT_EQ(reg_in.PC, 0);

    RegisterFetchDecode reg_out = f.run(&reg_in, &sigs);

    ASSERT_EQ(reg_out.ins.GetInsRaw(), ADD.GetInsRaw());
    ASSERT_EQ(reg_out.PC, 0);
    ASSERT_EQ(reg_out.PC_R, 1);
    ASSERT_EQ(reg_in.PC, 7 * INSTRUCTION_LENGTH);
}

TEST(FetchTestSetSignals, PC_R_PC_DISP_PC_EX_Neg)
{
    static const Ins ADD = Ins::MakeIns_ADD(2, 1, 0);
    Imem imem({ ADD });
    StageFetch f{ imem };
    RegisterWriteBackFetch reg_in;
    SimulationSignals sigs;
    sigs.PC_R = 1;
    sigs.PC_EX = 3 * INSTRUCTION_LENGTH;
    sigs.PC_DISP = -1 * INSTRUCTION_LENGTH;

    ASSERT_EQ(reg_in.PC, 0);

    RegisterFetchDecode reg_out = f.run(&reg_in, &sigs);

    ASSERT_EQ(reg_out.ins.GetInsRaw(), ADD.GetInsRaw());
    ASSERT_EQ(reg_out.PC, 0);
    ASSERT_EQ(reg_out.PC_R, 1);
    ASSERT_EQ(reg_in.PC, 2 * INSTRUCTION_LENGTH);
}
