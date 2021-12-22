#include "ins.h" 
#include <gtest/gtest.h>
#include <iostream>
#include <bitset>

// opcode: 0110011
// rd:     00000
// funct3: 000
// rs1:    00000
// rs2:    00000
// funct7: 0000000

TEST(InsCreationTest_R, ADD) {
    Ins ins = Ins::MakeIns_ADD(0, 0, 0);
    ASSERT_EQ(ins.GetInsRaw(), 0b00000000000000000000000000110011);

    ins = Ins::MakeIns_ADD(13, 12, 11);  
    ASSERT_EQ(ins.GetInsRaw(), 0b00000000110101100000010110110011);
}

TEST(InsCreationTest_R, SUB) {
    Ins ins = Ins::MakeIns_SUB(0, 0, 0);
    ASSERT_EQ(ins.GetInsRaw(), 0b01000000000000000000000000110011);

    ins = Ins::MakeIns_SUB(13, 12, 11);  
    ASSERT_EQ(ins.GetInsRaw(), 0b01000000110101100000010110110011);
}

TEST(InsCreationTest_R, SLL) {
    Ins ins = Ins::MakeIns_SLL(0, 0, 0);
    ASSERT_EQ(ins.GetInsRaw(), 0b00000000000000000001000000110011);

    ins = Ins::MakeIns_SLL(13, 12, 11);  
    ASSERT_EQ(ins.GetInsRaw(), 0b00000000110101100001010110110011);
}

TEST(InsCreationTest_S, SB) {
    Ins ins = Ins::MakeIns_SB(0, 0, 0);
    ASSERT_EQ(ins.GetInsRaw(), 0b00000000000000000000000000100011);
    int32_t imm = 0;
    uint32_t rs2 = 0;
    uint32_t rs1 = 0;
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, 0);
    ASSERT_EQ(ins.GetRs2(&rs2), true);
    ASSERT_EQ(rs2, 0);
    ASSERT_EQ(ins.GetRs1(&rs1), true);
    ASSERT_EQ(rs1, 0);

    ins = Ins::MakeIns_SB(55, 12, 11);  
    ASSERT_EQ(ins.GetInsRaw(), 0b00000010110001011000101110100011);
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, 55);
    ASSERT_EQ(ins.GetRs2(&rs2), true);
    ASSERT_EQ(rs2, 12);
    ASSERT_EQ(ins.GetRs1(&rs1), true);
    ASSERT_EQ(rs1, 11);

    ins = Ins::MakeIns_SB(-55, 12, 11);
    ASSERT_EQ(ins.GetInsRaw(), 0b10000010110001011000101110100011);
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, -55);
    ASSERT_EQ(ins.GetRs2(&rs2), true);
    ASSERT_EQ(rs2, 12);
    ASSERT_EQ(ins.GetRs1(&rs1), true);
    ASSERT_EQ(rs1, 11);
}

TEST(InsCreationTest_S, SH) {
    Ins ins = Ins::MakeIns_SH(0, 0, 0);
    ASSERT_EQ(ins.GetInsRaw(), 0b00000000000000000001000000100011);
    int32_t imm = 0;
    uint32_t rs2 = 0;
    uint32_t rs1 = 0;
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, 0);
    ASSERT_EQ(ins.GetRs2(&rs2), true);
    ASSERT_EQ(rs2, 0);
    ASSERT_EQ(ins.GetRs1(&rs1), true);
    ASSERT_EQ(rs1, 0);

    ins = Ins::MakeIns_SH(119, 12, 11);  
    ASSERT_EQ(ins.GetInsRaw(), 0b00000110110001011001101110100011);
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, 119);
    ASSERT_EQ(ins.GetRs2(&rs2), true);
    ASSERT_EQ(rs2, 12);
    ASSERT_EQ(ins.GetRs1(&rs1), true);
    ASSERT_EQ(rs1, 11);

    ins = Ins::MakeIns_SH(-119, 12, 11);  
    ASSERT_EQ(ins.GetInsRaw(), 0b10000110110001011001101110100011);
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, -119);
    ASSERT_EQ(ins.GetRs2(&rs2), true);
    ASSERT_EQ(rs2, 12);
    ASSERT_EQ(ins.GetRs1(&rs1), true);
    ASSERT_EQ(rs1, 11);
}

TEST(InsCreationTest_S, SW) {
    Ins ins = Ins::MakeIns_SW(0, 0, 0);
    ASSERT_EQ(ins.GetInsRaw(), 0b00000000000000000010000000100011);
    int32_t imm = 0;
    uint32_t rs2 = 0;
    uint32_t rs1 = 0;
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, 0);
    ASSERT_EQ(ins.GetRs2(&rs2), true);
    ASSERT_EQ(rs2, 0);
    ASSERT_EQ(ins.GetRs1(&rs1), true);
    ASSERT_EQ(rs1, 0);

    ins = Ins::MakeIns_SW(119, 12, 11);  
    ASSERT_EQ(ins.GetInsRaw(), 0b00000110110001011010101110100011);
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, 119);
    ASSERT_EQ(ins.GetRs2(&rs2), true);
    ASSERT_EQ(rs2, 12);
    ASSERT_EQ(ins.GetRs1(&rs1), true);
    ASSERT_EQ(rs1, 11);

    ins = Ins::MakeIns_SW(-119, 12, 11);  
    ASSERT_EQ(ins.GetInsRaw(), 0b10000110110001011010101110100011);
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, -119);
    ASSERT_EQ(ins.GetRs2(&rs2), true);
    ASSERT_EQ(rs2, 12);
    ASSERT_EQ(ins.GetRs1(&rs1), true);
    ASSERT_EQ(rs1, 11);
}


TEST(InsCreationTest_J, JAL) {
    Ins ins = Ins::MakeIns_JAL(0, 0);
    ASSERT_EQ(ins.GetInsRaw(), 0b00000000000000000000000001101111);
    int32_t imm = 0;
    uint32_t rd = 0;
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, 0);
    ASSERT_EQ(ins.GetRd(&rd), true);
    ASSERT_EQ(rd, 0);

    ins = Ins::MakeIns_JAL(22, 5);
    ASSERT_EQ(ins.GetInsRaw(), 0b00000001011000000000001011101111);
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, 22);
    ASSERT_EQ(ins.GetRd(&rd), true);
    ASSERT_EQ(rd, 5);

    ins = Ins::MakeIns_JAL(-22, 5);
    ASSERT_EQ(ins.GetInsRaw(), 0b10000001011000000000001011101111);
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, -22);
    ASSERT_EQ(ins.GetRd(&rd), true);
    ASSERT_EQ(rd, 5);
}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}