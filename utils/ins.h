#ifndef __INS_H_INCLUDED__
#define __INS_H_INCLUDED__

#include <bitset>
#include <cassert>
#include <cstddef>
#include <iostream>

class Ins
{
  public:
    enum class InsFormat
    {
        R,
        I,
        S,
        B,
        U,
        J,
        INVALID,
    };

    enum class InsMnemonic
    {
        // R
        ADD,
        SUB,
        SLL,
        SLT,
        SLTU,
        XOR,
        SRL,
        SRA,
        OR,
        AND,

        // I
        // ARITHMETIC
        ADDI,
        SLTI,
        SLTUI,
        XORI,
        ORI,
        ANDI,
        SLLI,
        SRLI,
        SRAI,
        // LOAD
        LB,
        LH,
        LW,
        LBU,
        LHU,
        // JALR
        JALR,

        // S
        SB,
        SH,
        SW,

        // B
        BEQ,
        BNE,
        BLT,
        BGE,
        BLTU,
        BGEU,

        // U
        LUI,
        AUIPC,

        // J
        JAL,

        INVALID,
    };

    Ins() : ins_raw(0), fmt(InsFormat::INVALID), mnm(InsMnemonic::INVALID)
    {}
    ~Ins() = default;

    Ins(uint32_t bits, InsFormat format, InsMnemonic mnemonic)
        : ins_raw(bits), fmt(format), mnm(mnemonic)
    {}

    inline InsFormat GetInsFormat() const
    {
        return fmt;
    }

    inline InsMnemonic GetInsMnemonic() const
    {
        return mnm;
    }

    inline uint32_t GetInsRaw() const
    {
        return ins_raw;
    }

    bool GetRs1(uint32_t* rs1) const
    {
        assert(rs1 != nullptr);

        switch (fmt) {
        case InsFormat::R:
        case InsFormat::I:
        case InsFormat::S:
        case InsFormat::B:
            *rs1 = (ins_raw & MASK_RS1) >> 15;
            return true;
        default:
            return false;
        }
    }

    bool GetRs2(uint32_t* rs2) const
    {
        assert(rs2 != nullptr);

        switch (fmt) {
        case InsFormat::R:
        case InsFormat::S:
        case InsFormat::B:
            *rs2 = (ins_raw & MASK_RS2) >> 20;
            return true;
        default:
            return false;
        }
    }

    bool GetRd(uint32_t* rd) const
    {
        assert(rd != nullptr);

        switch (fmt) {
        case InsFormat::R:
        case InsFormat::I:
        case InsFormat::U:
        case InsFormat::J:
            *rd = (ins_raw & MASK_RD) >> 7;
            return true;
        default:
            return false;
        }
    }

    uint32_t GetImmSign() const
    {
        return ins_raw & MASK_MSB;
    }

    bool GetImm(int32_t* imm) const
    {
        assert(imm != nullptr);

        switch (fmt) {
        case InsFormat::R:
            return false;
        case InsFormat::I: {
            uint32_t res = 0;
            uint32_t sign = 0;

            bool is_shift =
                (mnm == InsMnemonic::SLLI || mnm == InsMnemonic::SRLI ||
                 mnm == InsMnemonic::SRAI);

            if (is_shift) {
                res = (ins_raw & MASK_I_IMM_11_0 & (~MASK_MSB >> 1)) >> 20;
            } else {
                res = (ins_raw & MASK_I_IMM_11_0 & (~MASK_MSB)) >> 20;
                sign = ins_raw & MASK_MSB;
            }

            // convert to computer's encoding
            *imm = sign ? (~res + 1) : res;
        }
            return true;
        case InsFormat::S: {
            uint32_t imm_4_0 = (ins_raw & MASK_S_IMM_4_0) >> 7;
            uint32_t imm_11_5 =
                (ins_raw & MASK_S_IMM_11_5 & (~MASK_MSB)) >> 25;

            uint32_t res = (imm_11_5 << 5) | imm_4_0;
            uint32_t sign = ins_raw & MASK_MSB;

            // convert to computer's encoding
            *imm = sign ? (~res + 1) : res;
        }
            return true;
        case InsFormat::B: {
            uint32_t imm_11 = (ins_raw & MASK_B_IMM_11) >> 7;
            uint32_t imm_4_1 = (ins_raw & MASK_B_IMM_4_1) >> 8;
            uint32_t imm_10_5 = (ins_raw & MASK_B_IMM_10_5) >> 25;

            uint32_t res =
                0b0 | (imm_4_1 << 1) | (imm_10_5 << 5) | (imm_11 << 11);
            uint32_t sign = ins_raw & MASK_MSB;

            // convert to computer's encoding
            *imm = sign ? (~res + 1) : res;
        }
            return true;
        case InsFormat::U: {
            int32_t res = ins_raw & MASK_U_IMM_31_12 & (~MASK_MSB);
            uint32_t sign = ins_raw & MASK_MSB;

            // convert to computer's encoding
            *imm = sign ? (~res + 1) : res;
        }
            return true;
        case InsFormat::J: {
            uint32_t imm_19_12 = (ins_raw & MASK_J_IMM_19_12) >> 12;
            uint32_t imm_11 = (ins_raw & MASK_J_IMM_11) >> 20;
            uint32_t imm_10_1 = (ins_raw & MASK_J_IMM_10_1) >> 21;

            uint32_t res =
                0b0 | (imm_10_1 << 1) | (imm_11 << 11) | (imm_19_12 << 12);
            uint32_t sign = ins_raw & MASK_MSB;

            // convert to computer's encoding
            *imm = sign ? (~res + 1) : res;
        }
            return true;
        default:
            return false;
        }
    }

    enum Ins_R_OPCODE
    {
        R = 0b0110011,
    };

    static Ins MakeIns_R(uint32_t funct7, uint32_t rs2, uint32_t rs1,
                         uint32_t funct3, uint32_t rd, InsMnemonic mnemonic)
    {
        uint32_t ins = 0;
        ins = InsSetValueMask(ins, Ins_R_OPCODE::R, MASK_OPCODE, 0);
        ins = InsSetValueMask(ins, rd, MASK_RD, 7);
        ins = InsSetValueMask(ins, funct3, MASK_R_FUNCT3, 12);
        ins = InsSetValueMask(ins, rs1, MASK_RS1, 15);
        ins = InsSetValueMask(ins, rs2, MASK_RS2, 20);
        ins = InsSetValueMask(ins, funct7, MASK_R_FUNCT7, 25);

        return Ins(ins, InsFormat::R, mnemonic);
    }

    static Ins MakeIns_ADD(uint32_t rs2 = 0, uint32_t rs1 = 0, uint32_t rd = 0)
    {
        return MakeIns_R(0b0000000, rs2, rs1, 0b000, rd, InsMnemonic::ADD);
    }

    static Ins MakeIns_SUB(uint32_t rs2 = 0, uint32_t rs1 = 0, uint32_t rd = 0)
    {
        return MakeIns_R(0b0100000, rs2, rs1, 0b000, rd, InsMnemonic::SUB);
    }

    static Ins MakeIns_SLL(uint32_t rs2 = 0, uint32_t rs1 = 0, uint32_t rd = 0)
    {
        return MakeIns_R(0b0000000, rs2, rs1, 0b001, rd, InsMnemonic::SLL);
    }

    static Ins MakeIns_SLT(uint32_t rs2 = 0, uint32_t rs1 = 0, uint32_t rd = 0)
    {
        return MakeIns_R(0b0000000, rs2, rs1, 0b010, rd, InsMnemonic::SLT);
    }

    static Ins MakeIns_SLTU(uint32_t rs2 = 0, uint32_t rs1 = 0,
                            uint32_t rd = 0)
    {
        return MakeIns_R(0b0000000, rs2, rs1, 0b011, rd, InsMnemonic::SLTU);
    }

    static Ins MakeIns_XOR(uint32_t rs2 = 0, uint32_t rs1 = 0, uint32_t rd = 0)
    {
        return MakeIns_R(0b0000000, rs2, rs1, 0b100, rd, InsMnemonic::XOR);
    }

    static Ins MakeIns_SRL(uint32_t rs2 = 0, uint32_t rs1 = 0, uint32_t rd = 0)
    {
        return MakeIns_R(0b0000000, rs2, rs1, 0b101, rd, InsMnemonic::SRL);
    }

    static Ins MakeIns_SRA(uint32_t rs2 = 0, uint32_t rs1 = 0, uint32_t rd = 0)
    {
        return MakeIns_R(0b0100000, rs2, rs1, 0b101, rd, InsMnemonic::SRA);
    }

    static Ins MakeIns_OR(uint32_t rs2 = 0, uint32_t rs1 = 0, uint32_t rd = 0)
    {
        return MakeIns_R(0b0000000, rs2, rs1, 0b110, rd, InsMnemonic::OR);
    }

    static Ins MakeIns_AND(uint32_t rs2 = 0, uint32_t rs1 = 0, uint32_t rd = 0)
    {
        return MakeIns_R(0b0000000, rs2, rs1, 0b111, rd, InsMnemonic::AND);
    }

    enum Ins_I_OPCODE
    {
        ARITHMETIC = 0b0010011,
        LOAD = 0b0000011,
        JALR = 0b1100111,
    };

    static Ins MakeIns_I(int32_t imm, uint32_t rs1, uint32_t funct3,
                         uint32_t rd, Ins_I_OPCODE opcode,
                         InsMnemonic mnemonic)
    {
        int8_t sign = (imm < 0) ? 1 : 0;
        imm = abs(imm);
        imm |= (sign) ? 0x00000800 : 0x0;

        uint32_t ins = 0;
        ins = InsSetValueMask(ins, opcode, MASK_OPCODE, 0);
        ins = InsSetValueMask(ins, rd, MASK_RD, 7);
        ins = InsSetValueMask(ins, funct3, MASK_I_FUNCT3, 12);
        ins = InsSetValueMask(ins, rs1, MASK_RS1, 15);
        ins = InsSetValueMask(ins, imm, MASK_I_IMM_11_0, 20);

        return Ins(ins, InsFormat::I, mnemonic);
    }

    static Ins MakeIns_ADDI(int32_t imm = 0, uint32_t rs1 = 0, uint32_t rd = 0)
    {
        return MakeIns_I(imm, rs1, 0b000, rd, Ins_I_OPCODE::ARITHMETIC,
                         InsMnemonic::ADDI);
    }

    static Ins MakeIns_SLTI(int32_t imm = 0, uint32_t rs1 = 0, uint32_t rd = 0)
    {
        return MakeIns_I(imm, rs1, 0b010, rd, Ins_I_OPCODE::ARITHMETIC,
                         InsMnemonic::SLTI);
    }

    static Ins MakeIns_SLTUI(int32_t imm = 0, uint32_t rs1 = 0,
                             uint32_t rd = 0)
    {
        return MakeIns_I(imm, rs1, 0b011, rd, Ins_I_OPCODE::ARITHMETIC,
                         InsMnemonic::SLTUI);
    }

    static Ins MakeIns_XORI(int32_t imm = 0, uint32_t rs1 = 0, uint32_t rd = 0)
    {
        return MakeIns_I(imm, rs1, 0b100, rd, Ins_I_OPCODE::ARITHMETIC,
                         InsMnemonic::XORI);
    }

    static Ins MakeIns_ORI(int32_t imm = 0, uint32_t rs1 = 0, uint32_t rd = 0)
    {
        return MakeIns_I(imm, rs1, 0b110, rd, Ins_I_OPCODE::ARITHMETIC,
                         InsMnemonic::ORI);
    }

    static Ins MakeIns_ANDI(int32_t imm = 0, uint32_t rs1 = 0, uint32_t rd = 0)
    {
        return MakeIns_I(imm, rs1, 0b111, rd, Ins_I_OPCODE::ARITHMETIC,
                         InsMnemonic::ANDI);
    }

    static Ins MakeIns_SLLI(int32_t imm = 0, uint32_t rs1 = 0, uint32_t rd = 0)
    {
        imm &= 0b000000011111;

        return MakeIns_I(imm, rs1, 0b001, rd, Ins_I_OPCODE::ARITHMETIC,
                         InsMnemonic::SLLI);
    }

    static Ins MakeIns_SRLI(int32_t imm = 0, uint32_t rs1 = 0, uint32_t rd = 0)
    {
        imm &= 0b000000011111;

        return MakeIns_I(imm, rs1, 0b101, rd, Ins_I_OPCODE::ARITHMETIC,
                         InsMnemonic::SRLI);
    }

    static Ins MakeIns_SRAI(int32_t imm = 0, uint32_t rs1 = 0, uint32_t rd = 0)
    {
        imm &= 0b000000011111;
        imm |= 0b010000000000;
        return MakeIns_I(imm, rs1, 0b101, rd, Ins_I_OPCODE::ARITHMETIC,
                         InsMnemonic::SRAI);
    }

    static Ins MakeIns_LB(int32_t imm = 0, uint32_t rs1 = 0, uint32_t rd = 0)
    {
        return MakeIns_I(imm, rs1, 0b000, rd, Ins_I_OPCODE::LOAD,
                         InsMnemonic::LB);
    }

    static Ins MakeIns_LH(int32_t imm = 0, uint32_t rs1 = 0, uint32_t rd = 0)
    {
        return MakeIns_I(imm, rs1, 0b001, rd, Ins_I_OPCODE::LOAD,
                         InsMnemonic::LH);
    }

    static Ins MakeIns_LW(int32_t imm = 0, uint32_t rs1 = 0, uint32_t rd = 0)
    {
        return MakeIns_I(imm, rs1, 0b010, rd, Ins_I_OPCODE::LOAD,
                         InsMnemonic::LW);
    }

    static Ins MakeIns_LBU(int32_t imm = 0, uint32_t rs1 = 0, uint32_t rd = 0)
    {
        return MakeIns_I(imm, rs1, 0b100, rd, Ins_I_OPCODE::LOAD,
                         InsMnemonic::LBU);
    }

    static Ins MakeIns_LHU(int32_t imm = 0, uint32_t rs1 = 0, uint32_t rd = 0)
    {
        return MakeIns_I(imm, rs1, 0b101, rd, Ins_I_OPCODE::LOAD,
                         InsMnemonic::LHU);
    }

    static Ins MakeIns_JALR(int32_t imm = 0, uint32_t rs1 = 0, uint32_t rd = 0)
    {
        return MakeIns_I(imm, rs1, 0b000, rd, Ins_I_OPCODE::JALR,
                         InsMnemonic::JALR);
    }

    enum Ins_S_OPCODE
    {
        S = 0b0100011,
    };

    static Ins MakeIns_S(int32_t imm, uint32_t rs2, uint32_t rs1,
                         uint32_t funct3, InsMnemonic mnemonic)
    {
        int8_t sign = (imm < 0) ? 1 : 0;
        imm = abs(imm);

        uint32_t imm_4_0 = imm & 0x0000001F;
        uint32_t imm_11_5 = (imm & 0x00000FE0) >> 5;
        imm_11_5 |= (sign ? 0x00000040 : 0x0);
        uint32_t ins = 0;

        ins = InsSetValueMask(ins, Ins_S_OPCODE::S, MASK_OPCODE, 0);
        ins = InsSetValueMask(ins, imm_4_0, MASK_S_IMM_4_0, 7);
        ins = InsSetValueMask(ins, funct3, MASK_S_FUNCT3, 12);
        ins = InsSetValueMask(ins, rs1, MASK_RS1, 15);
        ins = InsSetValueMask(ins, rs2, MASK_RS2, 20);
        ins = InsSetValueMask(ins, imm_11_5, MASK_S_IMM_11_5, 25);

        return Ins(ins, InsFormat::S, mnemonic);
    }

    static Ins MakeIns_SB(int32_t imm = 0, uint32_t rs2 = 0, uint32_t rs1 = 0)
    {
        return MakeIns_S(imm, rs2, rs1, 0b000, InsMnemonic::SB);
    }

    static Ins MakeIns_SH(int32_t imm = 0, uint32_t rs2 = 0, uint32_t rs1 = 0)
    {
        return MakeIns_S(imm, rs2, rs1, 0b001, InsMnemonic::SH);
    }

    static Ins MakeIns_SW(int32_t imm = 0, uint32_t rs2 = 0, uint32_t rs1 = 0)
    {
        return MakeIns_S(imm, rs2, rs1, 0b010, InsMnemonic::SW);
    }

    enum Ins_B_OPCODE
    {
        B = 0b1100011,
    };

    static Ins MakeIns_B(int32_t imm, uint32_t rs2, uint32_t rs1,
                         uint32_t funct3, InsMnemonic mnemonic)
    {
        int8_t sign = (imm < 0) ? 1 : 0;
        imm = abs(imm);

        uint32_t imm_4_1 = (imm & 0x0000001E) >> 1;
        uint32_t imm_10_5 = (imm & 0x000007E0) >> 5;
        uint32_t imm_11 = (imm & 0x00000800) >> 11;
        uint32_t imm_12 = sign;
        uint32_t ins = 0;

        ins = InsSetValueMask(ins, Ins_B_OPCODE::B, MASK_OPCODE, 0);
        ins = InsSetValueMask(ins, imm_11, MASK_B_IMM_11, 7);
        ins = InsSetValueMask(ins, imm_4_1, MASK_B_IMM_4_1, 8);
        ins = InsSetValueMask(ins, funct3, MASK_B_FUNCT3, 12);
        ins = InsSetValueMask(ins, rs1, MASK_RS1, 15);
        ins = InsSetValueMask(ins, rs2, MASK_RS2, 20);
        ins = InsSetValueMask(ins, imm_10_5, MASK_B_IMM_10_5, 25);
        ins = InsSetValueMask(ins, imm_12, MASK_B_IMM_12, 31);

        return Ins(ins, InsFormat::B, mnemonic);
    }

    static Ins MakeIns_BEQ(int32_t imm = 0, uint32_t rs2 = 0, uint32_t rs1 = 0)
    {
        return MakeIns_B(imm, rs2, rs1, 0b000, InsMnemonic::BEQ);
    }

    static Ins MakeIns_BNE(int32_t imm = 0, uint32_t rs2 = 0, uint32_t rs1 = 0)
    {
        return MakeIns_B(imm, rs2, rs1, 0b001, InsMnemonic::BNE);
    }

    static Ins MakeIns_BLT(int32_t imm = 0, uint32_t rs2 = 0, uint32_t rs1 = 0)
    {
        return MakeIns_B(imm, rs2, rs1, 0b100, InsMnemonic::BLT);
    }

    static Ins MakeIns_BGE(int32_t imm = 0, uint32_t rs2 = 0, uint32_t rs1 = 0)
    {
        return MakeIns_B(imm, rs2, rs1, 0b101, InsMnemonic::BGE);
    }

    static Ins MakeIns_BLTU(int32_t imm = 0, uint32_t rs2 = 0,
                            uint32_t rs1 = 0)
    {
        return MakeIns_B(imm, rs2, rs1, 0b110, InsMnemonic::BLTU);
    }

    static Ins MakeIns_BGEU(int32_t imm = 0, uint32_t rs2 = 0,
                            uint32_t rs1 = 0)
    {
        return MakeIns_B(imm, rs2, rs1, 0b111, InsMnemonic::BGEU);
    }

    enum Ins_U_OPCODE
    {
        LUI = 0b0110111,
        AUIPC = 0b0010111,
    };

    static Ins MakeIns_U(int32_t imm, uint32_t rd, Ins_U_OPCODE opcode,
                         InsMnemonic mnemonic)
    {
        int8_t sign = (imm < 0) ? 1 : 0;
        imm = abs(imm);

        uint32_t imm_31_12 = imm & MASK_U_IMM_31_12;
        imm_31_12 |= (sign ? MASK_MSB : 0x0);
        uint32_t ins = 0;

        ins = InsSetValueMask(ins, opcode, MASK_OPCODE, 0);
        ins = InsSetValueMask(ins, rd, MASK_RD, 7);
        ins = InsSetValueMask(ins, imm_31_12, MASK_U_IMM_31_12, 0);

        return Ins(ins, InsFormat::U, mnemonic);
    }

    static Ins MakeIns_LUI(int32_t imm = 0, uint32_t rd = 0)
    {
        return MakeIns_U(imm, rd, Ins_U_OPCODE::LUI, InsMnemonic::LUI);
    }

    static Ins MakeIns_AUIPC(int32_t imm = 0, uint32_t rd = 0)
    {
        return MakeIns_U(imm, rd, Ins_U_OPCODE::AUIPC, InsMnemonic::AUIPC);
    }

    enum Ins_J_OPCODE
    {
        J = 0b1101111,
    };

    static Ins MakeIns_J(int32_t imm, uint32_t rd, InsMnemonic mnemonic)
    {
        int8_t sign = (imm < 0) ? 1 : 0;
        imm = abs(imm);

        uint32_t imm_10_1 = (imm & 0x000007FE) >> 1;
        uint32_t imm_11 = (imm & 0x00000800) >> 11;
        uint32_t imm_19_12 = (imm & 0x000FF000) >> 12;
        uint32_t imm_20 = sign;
        uint32_t ins = 0;

        ins = InsSetValueMask(ins, Ins_J_OPCODE::J, MASK_OPCODE, 0);
        ins = InsSetValueMask(ins, rd, MASK_RD, 7);
        ins = InsSetValueMask(ins, imm_19_12, MASK_J_IMM_19_12, 12);
        ins = InsSetValueMask(ins, imm_11, MASK_J_IMM_11, 20);
        ins = InsSetValueMask(ins, imm_10_1, MASK_J_IMM_10_1, 21);
        ins = InsSetValueMask(ins, imm_20, MASK_J_IMM_20, 31);

        return Ins(ins, InsFormat::J, mnemonic);
    }

    static Ins MakeIns_JAL(int32_t imm = 0, uint32_t rd = 0)
    {
        return MakeIns_J(imm, rd, InsMnemonic::JAL);
    }

  private:
    static const uint32_t INS_WIDTH = 32;
    static const uint32_t MASK_MSB = 0x80000000;

    // common
    static const uint32_t MASK_OPCODE = 0x0000007F;
    static const uint32_t MASK_RD = 0x00000F80;
    static const uint32_t MASK_RS1 = 0x000F8000;
    static const uint32_t MASK_RS2 = 0x01F00000;

    // R-type
    static const uint32_t MASK_R_FUNCT3 = 0x00007000;
    static const uint32_t MASK_R_FUNCT7 = 0xFE000000;

    // I-type
    static const uint32_t MASK_I_FUNCT3 = 0x00007000;
    static const uint32_t MASK_I_IMM_11_0 = 0xFFF00000;

    // S-type
    static const uint32_t MASK_S_IMM_4_0 = 0x00000F80;
    static const uint32_t MASK_S_FUNCT3 = 0x00007000;
    static const uint32_t MASK_S_IMM_11_5 = 0xFE000000;

    // B-type
    static const uint32_t MASK_B_IMM_11 = 0x00000080;
    static const uint32_t MASK_B_IMM_4_1 = 0x00000F00;
    static const uint32_t MASK_B_FUNCT3 = 0x00007000;
    static const uint32_t MASK_B_IMM_10_5 = 0x7E000000;
    static const uint32_t MASK_B_IMM_12 = 0x80000000;

    // U-type
    static const uint32_t MASK_U_IMM_31_12 = 0xFFFFF000;

    // J-type
    static const uint32_t MASK_J_IMM_19_12 = 0x000FF000;
    static const uint32_t MASK_J_IMM_11 = 0x00100000;
    static const uint32_t MASK_J_IMM_10_1 = 0x7FE00000;
    static const uint32_t MASK_J_IMM_20 = 0x80000000;

    static inline uint32_t InsSetValueMask(uint32_t ins, uint32_t val,
                                           uint32_t mask, size_t offset)
    {
        ins &= (~mask);
        val = (val << offset) & mask;
        return ins | val;
    }

    static int32_t ConvertNegativeEncoding(int32_t imm = 0)
    {
        return abs(imm) | ((imm < 0) ? MASK_MSB : 0x0);
    }

    uint32_t ins_raw;
    InsFormat fmt;
    InsMnemonic mnm;
};

#endif