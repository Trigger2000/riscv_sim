#include <bitset>
#include <cstddef>
#include <iostream>

class Ins {
public:
    static const int32_t INS_WIDTH = 32;
    
    // common
    static const int32_t MASK_OPCODE = 0x0000007F;

    enum InsFormat {
        R,
        I,
        S,
        B,
        U,
        J,
    };

    // R-type
    static const int32_t MASK_R_RD = 0x00000F80;
    static const int32_t MASK_R_FUNCT3 = 0x00007000;
    static const int32_t MASK_R_RS1 = 0x000F8000;
    static const int32_t MASK_R_RS2 = 0x01F00000;
    static const int32_t MASK_R_FUNCT7 = 0xFE000000;

    enum Ins_R_OPCODE {
        R = 0b0110011,
    };

    static int32_t MakeIns_R(int32_t funct7, int32_t rs2, int32_t rs1, int32_t funct3, int32_t rd) {
        int32_t ins InsSetValueMask(ins, Ins_R_OPCODE::R, MASK_OPCODE, 0);
        ins = InsSetValueMask(ins, rd, MASK_R_RD, 7);
        ins = InsSetValueMask(ins, funct3, MASK_R_FUNCT3, 12);
        ins = InsSetValueMask(ins, rs1, MASK_R_RS1, 15);
        ins = InsSetValueMask(ins, rs2, MASK_R_RS2, 20);
        return InsSetValueMask(ins, funct7, MASK_R_FUNCT7, 25);
    }

    static int32_t MakeIns_ADD(int32_t rs2, int32_t rs1, int32_t rd) {
        return MakeIns_R(0b0000000, rs2, rs1, 0b000, rd);
    }

    static int32_t MakeIns_SUB(int32_t rs2, int32_t rs1, int32_t rd) {
        return MakeIns_R(0b0100000, rs2, rs1, 0b000, rd);
    }

    static int32_t MakeIns_SLL(int32_t rs2, int32_t rs1, int32_t rd) {
        return MakeIns_R(0b0000000, rs2, rs1, 0b001, rd);
    }
    
    static int32_t MakeIns_SLT(int32_t rs2, int32_t rs1, int32_t rd) {
        return MakeIns_R(0b0000000, rs2, rs1, 0b010, rd);
    }
    
    static int32_t MakeIns_SLTU(int32_t rs2, int32_t rs1, int32_t rd) {
        return MakeIns_R(0b0000000, rs2, rs1, 0b011, rd);
    }
    
    static int32_t MakeIns_XOR(int32_t rs2, int32_t rs1, int32_t rd) {
        return MakeIns_R(0b0000000, rs2, rs1, 0b100, rd);
    }
    
    static int32_t MakeIns_SRL(int32_t rs2, int32_t rs1, int32_t rd) {
        return MakeIns_R(0b0000000, rs2, rs1, 0b101, rd);
    }

    static int32_t MakeIns_SRA(int32_t rs2, int32_t rs1, int32_t rd) {
        return MakeIns_R(0b0100000, rs2, rs1, 0b101, rd);
    }
    
    static int32_t MakeIns_OR(int32_t rs2, int32_t rs1, int32_t rd) {
        return MakeIns_R(0b0000000, rs2, rs1, 0b110, rd);
    }
    
    static int32_t MakeIns_AND(int32_t rs2, int32_t rs1, int32_t rd) {
        return MakeIns_R(0b0000000, rs2, rs1, 0b111, rd);
    }
    
    // I-type
    static const int32_t MASK_I_RD = 0x00000F80;
    static const int32_t MASK_I_FUNCT3 = 0x00007000;
    static const int32_t MASK_I_RS1 = 0x000F8000;
    static const int32_t MASK_I_IMM_11_0 = 0xFFF00000;

    enum Ins_I_OPCODE {
        ARITHMETIC = 0b0110011,
        LOAD = 0b0000011,
        JALR = 0b1100111,
    };

    static int32_t MakeIns_I(int32_t imm, int32_t rs1, int32_t funct3, int32_t rd, Ins_I_OPCODE opcode) {
        int32_t ins InsSetValueMask(ins, opcode, MASK_OPCODE, 0);
        ins = InsSetValueMask(ins, rd, MASK_I_RD, 7);
        ins = InsSetValueMask(ins, funct3, MASK_I_FUNCT3, 12);
        ins = InsSetValueMask(ins, rs1, MASK_I_RS1, 15);
        return InsSetValueMask(ins, imm, MASK_I_IMM_11_0, 20);
    }

    static int32_t MakeIns_ADDI(int32_t imm, int32_t rs1, int32_t rd) {
        return MakeIns_I(imm, rs1, 0b000, rd, Ins_I_OPCODE::ARITHMETIC);
    }

    static int32_t MakeIns_SLTI(int32_t imm, int32_t rs1, int32_t rd) {
        return MakeIns_I(imm, rs1, 0b010, rd, Ins_I_OPCODE::ARITHMETIC);
    }
    
    static int32_t MakeIns_SLTIU(int32_t imm, int32_t rs1, int32_t rd) {
        return MakeIns_I(imm, rs1, 0b011, rd, Ins_I_OPCODE::ARITHMETIC);
    }
    
    static int32_t MakeIns_XORI(int32_t imm, int32_t rs1, int32_t rd) {
        return MakeIns_I(imm, rs1, 0b100, rd, Ins_I_OPCODE::ARITHMETIC);
    }
    
    static int32_t MakeIns_ORI(int32_t imm, int32_t rs1, int32_t rd) {
        return MakeIns_I(imm, rs1, 0b110, rd, Ins_I_OPCODE::ARITHMETIC);
    }

    static int32_t MakeIns_ANDI(int32_t imm, int32_t rs1, int32_t rd) {
        return MakeIns_I(imm, rs1, 0b111, rd, Ins_I_OPCODE::ARITHMETIC);
    }

    static int32_t MakeIns_SLLI(int32_t imm, int32_t rs1, int32_t rd) {
        imm &= 0b000000011111;

        return MakeIns_I(imm, rs1, 0b001, rd, Ins_I_OPCODE::ARITHMETIC);
    }
    
    static int32_t MakeIns_SRLI(int32_t imm, int32_t rs1, int32_t rd) {
        imm &= 0b000000011111;

        return MakeIns_I(imm, rs1, 0b101, rd, Ins_I_OPCODE::ARITHMETIC);
    }
    
    static int32_t MakeIns_SRAI(int32_t imm, int32_t rs1, int32_t rd) {
        imm &= 0b000000011111;
        imm |= 0b010000000000;
        return MakeIns_I(imm, rs1, 0b101, rd, Ins_I_OPCODE::ARITHMETIC);
    }

    static int32_t MakeIns_LB(int32_t imm, int32_t rs1, int32_t rd) {
        return MakeIns_I(imm, rs1, 0b000, rd, Ins_I_OPCODE::LOAD);
    }

    static int32_t MakeIns_LH(int32_t imm, int32_t rs1, int32_t rd) {
        return MakeIns_I(imm, rs1, 0b001, rd, Ins_I_OPCODE::LOAD);
    }

    static int32_t MakeIns_LW(int32_t imm, int32_t rs1, int32_t rd) {
        return MakeIns_I(imm, rs1, 0b010, rd, Ins_I_OPCODE::LOAD);
    }

    static int32_t MakeIns_LBU(int32_t imm, int32_t rs1, int32_t rd) {
        return MakeIns_I(imm, rs1, 0b100, rd, Ins_I_OPCODE::LOAD);
    }

    static int32_t MakeIns_LHU(int32_t imm, int32_t rs1, int32_t rd) {
        return MakeIns_I(imm, rs1, 0b101, rd, Ins_I_OPCODE::LOAD);
    }

    static int32_t MakeIns_JALR(int32_t imm, int32_t rs1, int32_t rd) {
        return MakeIns_I(imm, rs1, 0b000, rd, Ins_I_OPCODE::JALR);
    }

    // S-type
    static const int32_t MASK_S_IMM_4_0 = 0x00000F80;
    static const int32_t MASK_S_FUNCT3 = 0x00007000;
    static const int32_t MASK_S_RS1 = 0x000F8000;
    static const int32_t MASK_S_RS2 = 0x01F00000;
    static const int32_t MASK_S_IMM_11_5 = 0xFE000000;

    enum Ins_S_OPCODE {
        S = 0b0100011,
    };

    static int32_t MakeIns_S(int32_t imm, int32_t rs2, int32_t rs1, int32_t funct3) {
        int32_t imm_4_0 = imm & 0x0000001F;
        int32_t imm_11_5 = (imm & 0x00000FE0) >> 5;
        int32_t ins InsSetValueMask(ins, Ins_S_OPCODE::S, MASK_OPCODE, 0);
        ins = InsSetValueMask(ins, imm_4_0, MASK_S_IMM_4_0, 7);
        ins = InsSetValueMask(ins, funct3, MASK_S_FUNCT3, 12);
        ins = InsSetValueMask(ins, rs1, MASK_S_RS1, 15);
        ins = InsSetValueMask(ins, rs2, MASK_S_RS2, 20);
        return InsSetValueMask(ins, imm_11_5, MASK_S_IMM_11_5, 25);
    }

    static int32_t MakeIns_SB(int32_t imm_11_0, int32_t rs2, int32_t rs1, int32_t imm_4_0) {
        return MakeIns_S(imm, rs2, rs1, 0b000);
        
    }
    
    static int32_t MakeIns_SH(int32_t imm_11_0, int32_t rs2, int32_t rs1, int32_t imm_4_0) {
        return MakeIns_S(imm, rs2, rs1, 0b001);
    }

    static int32_t MakeIns_SW(int32_t imm_11_0, int32_t rs2, int32_t rs1, int32_t imm_4_0) {
        return MakeIns_S(imm, rs2, rs1, 0b010);
    }

    // B-type
    static const int32_t MASK_B_IMM_11 = 0x00000080;
    static const int32_t MASK_B_IMM_4_1 = 0x00000F00;
    static const int32_t MASK_B_FUNCT3 = 0x00007000;
    static const int32_t MASK_B_RS1 = 0x000F8000;
    static const int32_t MASK_B_RS2 = 0x01F00000;
    static const int32_t MASK_B_IMM_10_5 = 0x7E000000;
    static const int32_t MASK_B_IMM_12 = 0x80000000;

    enum Ins_B_OPCODE {
        B = 0b1100011,
    };

    
    static int32_t MakeIns_B(int32_t imm, int32_t rs2, int32_t rs1, int32_t funct3) {
        int32_t imm_4_1 = imm & 0x0000001E >> 1;
        int32_t imm_10_5 = (imm & 0x000007E0) >> 5;
        int32_t imm_11 = (imm & 0x00000800) >> 11;
        int32_t imm_12 = (imm & 0x00001000) >> 12;
        int32_t ins InsSetValueMask(ins, Ins_B_OPCODE::B, MASK_OPCODE, 0);
        ins = InsSetValueMask(ins, imm_11, MASK_B_IMM_11, 7);
        ins = InsSetValueMask(ins, imm_4_1, MASK_B_IMM_4_1, 8);
        ins = InsSetValueMask(ins, funct3, MASK_B_FUNCT3, 12);
        ins = InsSetValueMask(ins, rs1, MASK_B_RS1, 15);
        ins = InsSetValueMask(ins, rs2, MASK_B_RS2, 20);
        ins = InsSetValueMask(ins, imm_10_5, MASK_B_IMM_10_5, 25);
        return InsSetValueMask(ins, imm_12, MASK_B_IMM_12, 31);
    }

    static int32_t MakeIns_BEQ(int32_t imm, int32_t rs2, int32_t rs1) {
        return MakeIns_B(imm, rs2, rs1, 0b000);
    }

    static int32_t MakeIns_BEQ(int32_t imm, int32_t rs2, int32_t rs1) {
        return MakeIns_B(imm, rs2, rs1, 0b001);
    }

    static int32_t MakeIns_BEQ(int32_t imm, int32_t rs2, int32_t rs1) {
        return MakeIns_B(imm, rs2, rs1, 0b100);
    }

    static int32_t MakeIns_BEQ(int32_t imm, int32_t rs2, int32_t rs1) {
        return MakeIns_B(imm, rs2, rs1, 0b101);
    }

    static int32_t MakeIns_BEQ(int32_t imm, int32_t rs2, int32_t rs1) {
        return MakeIns_B(imm, rs2, rs1, 0b110);
    }

    static int32_t MakeIns_BEQ(int32_t imm, int32_t rs2, int32_t rs1) {
        return MakeIns_B(imm, rs2, rs1, 0b111);
    }

    // U-type
    static const int32_t MASK_U_RD = 0x00000F80;
    static const int32_t MASK_U_IMM_31_12 = 0xFFFFF000;

    enum Ins_U_OPCODE {
        LUI = 0b0110111,
        AUIPC = 0b0010111,
    };

    static int32_t MakeIns_U(int32_t imm, int32_t rd, Ins_U_OPCODE opcode) {
        int32_t imm_31_12 = imm & 0xFFFFF000;
        int32_t ins InsSetValueMask(ins, opcode, MASK_OPCODE, 0);
        ins = InsSetValueMask(ins, rd, MASK_U_RD, 7);
        return InsSetValueMask(ins, imm_31_12, MASK_U_IMM_31_12, 12);
    }

    static int32_t MakeIns_LUI(int32_t imm, int32_t rd) {
        return MakeIns_U(imm, rd, Ins_U_OPCODE::LUI);
    }

    static int32_t MakeIns_AUIPC(int32_t imm, int32_t rd) {
        return MakeIns_U(imm, rd, Ins_U_OPCODE::AUIPC);
    }

    // J-type
    static const int32_t MASK_J_RD = 0x00000F80;
    static const int32_t MASK_J_IMM_19_12 = 0x000FF000;
    static const int32_t MASK_J_IMM_11 = 0x00100000;
    static const int32_t MASK_J_IMM_10_1 = 0x7FE00000;
    static const int32_t MASK_J_IMM_20 = 0x80000000;

    enum Ins_J_OPCODE {
        J = 0b1101111,
    };

    static int32_t MakeIns_J(int32_t imm, int32_t rd) {
        int32_t imm_10_1 = imm & 0x000007FE >> 1;
        int32_t imm_11 = (imm & 0x00000800) >> 11;
        int32_t imm_19_12 = (imm & 0x000FF000) >> 12;
        int32_t imm_20 = (imm & 0x00100000) >> 20;

        int32_t ins InsSetValueMask(ins, Ins_J_OPCODE::J, MASK_OPCODE, 0);
        ins = InsSetValueMask(ins, rd, MASK_U_RD, 7);
        ins = InsSetValueMask(ins, imm_19_12, MASK_J_IMM_19_12, 12);
        ins = InsSetValueMask(ins, imm_11, MASK_J_IMM_11, 20);
        ins = InsSetValueMask(ins, imm_10_1, MASK_J_IMM_10_1, 21);
        return InsSetValueMask(ins, imm_20, MASK_J_IMM_20, 31);
    }

    static int32_t MakeIns_JAL(int32_t imm, int32_t rd) {
        return MakeIns_J(imm, rd);
    }

    Ins() = delete;
    ~Ins() = default;

    Ins(int32_t bits) : ins_raw(bits) {
        OpcodeToFormat(ins_raw)
    }

    // int32_t Get1reg();
    // int32_t Get2reg();
    // InsOpcode GetOpcode();
    // int32_t GetImm();
private:
    static inline int32_t InsSetValueMask(int32_t ins, int32_t val, int32_t mask, size_t offset) {
        ins &= (~mask);
        val = (val << offset) & mask;
        return ins | val;
    }

    static bool OpcodeToFormat(int32_t ins) {
        int32_t opcode = ins & MASK_OPCODE;

        switch (opcode) {
            case Ins_R_OPCODE::R:
                fmt = InsFormat::R;
                break;
            case Ins_I_OPCODE::ARITHMETIC:
                fmt = InsFormat::I;
                break;
            case Ins_I_OPCODE::LOAD:
                fmt = InsFormat::I;
                break;
            case Ins_S_OPCODE::S:
                fmt = InsFormat::S;
                break;
            case Ins_B_OPCODE::B:
                fmt = InsFormat::B;
                break;
            case Ins_U_OPCODE::LUI:
                fmt = InsFormat::U;
                break;
            case Ins_U_OPCODE::AUIPC:
                fmt = InsFormat::U;
                break;
            case Ins_J_OPCODE::J: 
                fmt = InsFormat::J;
                break;
            case Ins_I_OPCODE::JALR:
                fmt = InsFormat::I;
                break;
            default:
                std::cerr << "[ERROR] <Ins>: invalid operation opcode!\n";
                return false;
        }

        return true;
    }

    int32_t ins_raw;
    InsFormat fmt;
    /// Opcode op_;
};
