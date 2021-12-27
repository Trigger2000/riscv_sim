#include "stages/stage_memory.h"
#include "utils/registers.h"
#include "utils/simulation_state.h"
#include <bitset>
#include <gtest/gtest.h>
#include <iostream>

TEST(TestMemory, TestOperationR)
{
    SimulationSignals sigs{};

    RegisterExecuteMemory reg_in{};
    reg_in.WD = 15; // shouldn't affect
    reg_in.alu_res = 31;
    reg_in.WB_selector = 1;
    reg_in.MEM_WE = 0;
    reg_in.WB_WE = 1;
    reg_in.WB_A = 13;
    reg_in.mem_width = 0;

    StageMemory sm{};

    auto reg_out = sm.run(reg_in, &sigs);

    ASSERT_EQ(reg_out.WB_D, reg_in.alu_res);
    ASSERT_EQ(reg_out.WB_A, reg_in.WB_A);
    ASSERT_EQ(reg_out.WB_WE, reg_in.WB_WE);
    // ASSERT_EQ(sigs.HU_MEM_RD, 0);
    ASSERT_EQ(sigs.BP_MEM, reg_in.alu_res);
}

TEST(TestMemory, TestI_ARITHMETIC)
{
    SimulationSignals sigs{};

    RegisterExecuteMemory reg_in{};
    reg_in.WD = 13; // shouldn't affect
    reg_in.alu_res = 11;
    reg_in.WB_selector = 1;
    reg_in.MEM_WE = 0;
    reg_in.WB_WE = 1;
    reg_in.WB_A = 21; // shouldn't affect
    reg_in.mem_width = 0;

    StageMemory sm{};

    auto reg_out = sm.run(reg_in, &sigs);

    ASSERT_EQ(reg_out.WB_D, reg_in.alu_res);
    ASSERT_EQ(reg_out.WB_A, reg_in.WB_A);
    ASSERT_EQ(reg_out.WB_WE, reg_in.WB_WE);
    // ASSERT_EQ(sigs.HU_MEM_RD, 0);
    ASSERT_EQ(sigs.BP_MEM, reg_in.alu_res);
}

TEST(TestMemory, TestI_LOAD)
{
    SimulationSignals sigs{};

    RegisterExecuteMemory reg_in{};
    reg_in.WD = 31; // shouldn't affect
    reg_in.alu_res = 40;
    reg_in.WB_selector = 0;
    reg_in.MEM_WE = 0;
    reg_in.WB_WE = 1; // shouldn't affect
    reg_in.WB_A = 21; // shouldn't affect
    reg_in.mem_width = 8;

    StageMemory sm{};

    sm.DmemWrite(reg_in.WB_WE, 0xFFFFFFFF, reg_in.alu_res, reg_in.mem_width);

    auto reg_out = sm.run(reg_in, &sigs);

    ASSERT_EQ(reg_out.WB_D, 0xFF);
    ASSERT_EQ(reg_out.WB_A, reg_in.WB_A);
    ASSERT_EQ(reg_out.WB_WE, reg_in.WB_WE);
    // ASSERT_EQ(sigs.HU_MEM_RD, 0);
    ASSERT_EQ(sigs.BP_MEM, reg_in.alu_res);
}

TEST(TestMemory, TestI_JALR)
{
    // TODO: support
    // SimulationSignals sigs{};

    // RegisterExecuteMemory reg_in{};
    // reg_in.WD = 0xFFFFFFFF;
    // reg_in.alu_res = 40;
    // reg_in.WB_selector = 0;
    // reg_in.MEM_WE = 0;
    // reg_in.WB_WE = 1; // shouldn't affect
    // reg_in.WB_A = 21; // shouldn't affect
    // reg_in.mem_width = 8;

    // StageMemory sm{};

    // sm.DmemWrite(reg_in.WB_WE, reg_in.WD, reg_in.alu_res, reg_in.mem_width);

    // auto reg_out = sm.run(reg_in, &sigs);

    // ASSERT_EQ(reg_out.WB_D, 0xFF);
    // ASSERT_EQ(reg_out.WB_A, reg_in.WB_A);
    // ASSERT_EQ(reg_out.WB_WE, reg_in.WB_WE);
    // // ASSERT_EQ(sigs.HU_MEM_RD, 0);
    // ASSERT_EQ(sigs.BP_MEM, reg_in.alu_res);
}

TEST(TestMemory, TestS)
{
    SimulationSignals sigs{};

    RegisterExecuteMemory reg_in{};
    reg_in.WD = 0x0000FF8F;
    reg_in.alu_res = 40;
    reg_in.WB_selector = 1;
    reg_in.MEM_WE = 1;
    reg_in.WB_WE = 0;
    reg_in.WB_A = 21; // shouldn't affect
    reg_in.mem_width = 8;

    StageMemory sm{};

    auto reg_out = sm.run(reg_in, &sigs);

    ASSERT_EQ(reg_out.WB_D, 0x8F); // reads written value
    ASSERT_EQ(reg_out.WB_A, reg_in.WB_A);
    ASSERT_EQ(reg_out.WB_WE, reg_in.WB_WE);
    // ASSERT_EQ(sigs.HU_MEM_RD, 0);
    ASSERT_EQ(sigs.BP_MEM, reg_in.alu_res);
    ASSERT_EQ(sm.DmemRead(reg_in.alu_res, reg_in.mem_width), 0x8F);
}
