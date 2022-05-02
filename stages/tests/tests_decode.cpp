#include "stages/stage_decode.h"
#include "utils/registers.h"
#include "utils/simulation_state.h"
#include <bitset>
#include <gtest/gtest.h>
#include <iostream>

TEST(TestsDecodeNoSignal, TestOutputR)
{
    StageDecode d{};

    SimulationSignals sig{};

    RegisterFetchDecode reg_in{};

    for (int i = 0; i < BITNESS; ++i) {
        d.SetRegfileData(i, i);
    }

    for (int i = 0; i < BITNESS - 1; ++i) {
        reg_in.ins = Ins::MakeIns_ADD(i, i + 1, 13);

        auto reg_out = d.run(reg_in, &sig);

        ASSERT_EQ(reg_out.D1, i + 1);
        ASSERT_EQ(reg_out.D2, i);
        ASSERT_EQ(reg_out.V_EX, 0);
        // TODO change to instr comparison
        // ASSERT_EQ(reg_out.WB_A, 13);
        // ASSERT_EQ(reg_out.imm, 0);
        ASSERT_EQ(reg_out.sign_bit, 0);
        ASSERT_EQ(reg_out.CONTROL_EX.ALU_SRC2, 0);
        ASSERT_EQ(reg_out.CONTROL_EX.ALUOP, Ins::InsMnemonic::ADD);
        ASSERT_EQ(reg_out.CONTROL_EX.ALU_FMT, Ins::InsFormat::R);
    }
}

TEST(TestsDecodeNoSignal, TestOutputI)
{
    StageDecode d{};

    SimulationSignals sig{};

    RegisterFetchDecode reg_in{};

    for (int i = 0; i < BITNESS; ++i) {
        d.SetRegfileData(i, i);
    }

    for (int i = 0; i < BITNESS - 1; ++i) {
        reg_in.ins = Ins::MakeIns_ADDI(i * ((i % 2) ? 1 : -1), i + 1, 13);

        auto reg_out = d.run(reg_in, &sig);

        ASSERT_EQ(reg_out.D1, i + 1);
        ASSERT_EQ(reg_out.D2, 0);
        ASSERT_EQ(reg_out.V_EX, 0);
        // TODO change to instr comparison
        // ASSERT_EQ(reg_out.WB_A, 13);
        // ASSERT_EQ(reg_out.imm, i * ((i % 2) ? 1 : -1));
        ASSERT_EQ(reg_out.sign_bit,
                  (i * ((i % 2) ? 1 : -1) < 0) ? 0xFFFFFFFF : 0);
        ASSERT_EQ(reg_out.CONTROL_EX.ALU_SRC2, 1);
        ASSERT_EQ(reg_out.CONTROL_EX.ALUOP, Ins::InsMnemonic::ADDI);
        ASSERT_EQ(reg_out.CONTROL_EX.ALU_FMT, Ins::InsFormat::I);
    }
}

TEST(TestsDecodeNoSignal, TestOutputS)
{
    StageDecode d{};

    SimulationSignals sig{};

    RegisterFetchDecode reg_in{};

    for (int i = 0; i < BITNESS; ++i) {
        d.SetRegfileData(i, i);
    }

    for (int i = 0; i < BITNESS - 1; ++i) {
        reg_in.ins = Ins::MakeIns_SB(i * ((i % 2) ? 1 : -1), i + 1, i);

        auto reg_out = d.run(reg_in, &sig);

        ASSERT_EQ(reg_out.D1, i);
        ASSERT_EQ(reg_out.D2, i + 1);
        ASSERT_EQ(reg_out.V_EX, 0);
        // TODO change to instr comparison
        // ASSERT_EQ(reg_out.WB_A, 0);
        // ASSERT_EQ(reg_out.imm, i * ((i % 2) ? 1 : -1));
        ASSERT_EQ(reg_out.sign_bit,
                  (i * ((i % 2) ? 1 : -1) < 0) ? 0xFFFFFFFF : 0);
        ASSERT_EQ(reg_out.CONTROL_EX.ALU_SRC2, 1);
        ASSERT_EQ(reg_out.CONTROL_EX.ALUOP, Ins::InsMnemonic::SB);
        ASSERT_EQ(reg_out.CONTROL_EX.ALU_FMT, Ins::InsFormat::S);
    }
}

TEST(TestsDecodeNoSignal, TestOutputB)
{
    StageDecode d{};

    SimulationSignals sig{};

    RegisterFetchDecode reg_in{};

    for (int i = 0; i < BITNESS; ++i) {
        d.SetRegfileData(i, i);
    }

    for (int i = 0; i < BITNESS - 1; ++i) {
        reg_in.ins = Ins::MakeIns_BEQ(2 * i * ((i % 2) ? 1 : -1), i + 1, i);

        auto reg_out = d.run(reg_in, &sig);

        ASSERT_EQ(reg_out.D1, i);
        ASSERT_EQ(reg_out.D2, i + 1);
        ASSERT_EQ(reg_out.V_EX, 0);
        // TODO change to instr comparison
        // ASSERT_EQ(reg_out.WB_A, 0);
        // ASSERT_EQ(reg_out.imm, i * ((i % 2) ? 2 : -2));
        ASSERT_EQ(reg_out.sign_bit,
                  (i * ((i % 2) ? 1 : -1) < 0) ? 0xFFFFFFFF : 0);
        ASSERT_EQ(reg_out.CONTROL_EX.ALU_SRC2, 1);
        ASSERT_EQ(reg_out.CONTROL_EX.ALUOP, Ins::InsMnemonic::BEQ);
        ASSERT_EQ(reg_out.CONTROL_EX.ALU_FMT, Ins::InsFormat::B);
    }
}

TEST(TestsDecodeNoSignal, TestOutputU)
{
    StageDecode d{};

    SimulationSignals sig{};

    RegisterFetchDecode reg_in{};

    for (int i = 0; i < BITNESS; ++i) {
        d.SetRegfileData(i, i);
    }

    for (int i = 0; i < BITNESS; ++i) {
        reg_in.ins = Ins::MakeIns_LUI(0x1000, i);
        auto reg_out = d.run(reg_in, &sig);

        ASSERT_EQ(reg_out.D1, 0);
        ASSERT_EQ(reg_out.D2, 0);
        ASSERT_EQ(reg_out.V_EX, 0);
        // TODO change to instr comparison
        // ASSERT_EQ(reg_out.WB_A, i);
        // ASSERT_EQ(reg_out.imm, 0x1000);
        ASSERT_EQ(reg_out.sign_bit, 0);
        ASSERT_EQ(reg_out.CONTROL_EX.ALU_SRC2, 1);
        ASSERT_EQ(reg_out.CONTROL_EX.ALUOP, Ins::InsMnemonic::LUI);
        ASSERT_EQ(reg_out.CONTROL_EX.ALU_FMT, Ins::InsFormat::U);
    }
}

TEST(TestsDecodeNoSignal, TestOutputJ)
{
    StageDecode d{};

    SimulationSignals sig{};

    RegisterFetchDecode reg_in{};

    for (int i = 0; i < BITNESS; ++i) {
        d.SetRegfileData(i, i);
    }

    for (int i = 0; i < BITNESS; ++i) {
        reg_in.ins = Ins::MakeIns_JAL(i * ((i % 2) ? 2 : -2), i);

        auto reg_out = d.run(reg_in, &sig);

        ASSERT_EQ(reg_out.D1, 0);
        ASSERT_EQ(reg_out.D2, 0);
        ASSERT_EQ(reg_out.V_EX, 0);
        // TODO change to instr comparison
        // ASSERT_EQ(reg_out.WB_A, i);
        // ASSERT_EQ(reg_out.imm, i * ((i % 2) ? 2 : -2));
        ASSERT_EQ(reg_out.sign_bit,
                  (i * ((i % 2) ? 1 : -1) < 0) ? 0xFFFFFFFF : 0);
        ASSERT_EQ(reg_out.CONTROL_EX.ALU_SRC2, 1);
        ASSERT_EQ(reg_out.CONTROL_EX.ALUOP, Ins::InsMnemonic::JAL);
        ASSERT_EQ(reg_out.CONTROL_EX.ALU_FMT, Ins::InsFormat::J);
    }
}

TEST(TestsDecode, TestWB1)
{
    StageDecode d{};

    SimulationSignals sig{};

    sig.WB_A = 1;
    sig.WB_D = 123;
    sig.WB_WE = 0;

    RegisterFetchDecode reg_in{};

    reg_in.ins = Ins::MakeIns_JAL(32, 2);

    ASSERT_EQ(d.GetRegfileData(1), 0);

    auto reg_out = d.run(reg_in, &sig);

    ASSERT_EQ(reg_out.D1, 0);
    ASSERT_EQ(reg_out.D2, 0);
    ASSERT_EQ(reg_out.V_EX, 0);
    // TODO change to instr comparison
    // ASSERT_EQ(reg_out.WB_A, 2);
    // ASSERT_EQ(reg_out.imm, 32);
    ASSERT_EQ(d.GetRegfileData(1), 0);
    ASSERT_EQ(reg_out.sign_bit, 0);
    ASSERT_EQ(reg_out.CONTROL_EX.ALU_SRC2, 1);
    ASSERT_EQ(reg_out.CONTROL_EX.ALUOP, Ins::InsMnemonic::JAL);
    ASSERT_EQ(reg_out.CONTROL_EX.ALU_FMT, Ins::InsFormat::J);
}

TEST(TestsDecode, TestWB2)
{
    StageDecode d{};

    SimulationSignals sig{};

    sig.WB_A = 1;
    sig.WB_D = 123;
    sig.WB_WE = 1;

    RegisterFetchDecode reg_in{};

    reg_in.ins = Ins::MakeIns_JAL(32, 2);

    ASSERT_EQ(d.GetRegfileData(1), 0);

    auto reg_out = d.run(reg_in, &sig);

    ASSERT_EQ(reg_out.D1, 0);
    ASSERT_EQ(reg_out.D2, 0);
    ASSERT_EQ(reg_out.V_EX, 0);
    // TODO change to instr comparison
    // ASSERT_EQ(reg_out.WB_A, 2);
    // ASSERT_EQ(reg_out.imm, 32);
    ASSERT_EQ(d.GetRegfileData(1), 123);
    ASSERT_EQ(reg_out.sign_bit, 0);
    ASSERT_EQ(reg_out.CONTROL_EX.ALU_SRC2, 1);
    ASSERT_EQ(reg_out.CONTROL_EX.ALUOP, Ins::InsMnemonic::JAL);
    ASSERT_EQ(reg_out.CONTROL_EX.ALU_FMT, Ins::InsFormat::J);
}