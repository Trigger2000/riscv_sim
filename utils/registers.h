#ifndef __REGISTERS_H_INCLUDED__
#define __REGISTERS_H_INCLUDED__

#include "utils/defines.h"
#include "utils/ins.h"

struct RegisterFetchDecode
{
    Ins ins = Ins::MakeIns_NOP();
    uint32_t PC = 0;
    uint32_t PC_R = false;
};

struct RegisterDecodeExecute
{
    struct ControlEx
    {
        Ins::InsMnemonic ALUOP = Ins::InsMnemonic::NOP;
        Ins::InsFormat ALU_FMT = Ins::InsFormat::NOP;
        uint32_t ALU_SRC2 = 0;
        uint32_t BRN_COND = 0;
        uint32_t CMP_OP = 0;
        uint32_t JUMP = 0;
        uint32_t JALR = 0;
    };

    ControlEx CONTROL_EX;
    int32_t D1 = 0;
    int32_t D2 = 0;
    uint32_t sign_bit = 0;
    uint32_t PC_EX;
    uint32_t V_EX = 0;

    // bus 30:7
    Ins ins = Ins::MakeIns_NOP();

    // TODO: remove these fields from tests and remove them from here
    int32_t imm = 0;
    uint32_t WB_A = 0;
};

struct RegisterExecuteMemory
{
    int32_t WD = 0;
    int32_t alu_res = 0;
    uint32_t WB_selector = 0;
    uint32_t MEM_WE = 0;
    uint32_t WB_WE = 0;
    uint32_t WB_A = 0;
    uint32_t mem_width = BYTE_SIZE;
    bool NOP = false;
};

struct RegisterMemoryWriteback
{
    int32_t WB_D = 0;
    uint32_t WB_A = 0;
    uint32_t WB_WE = 0;
};

struct RegisterWriteBackFetch
{
    uint32_t PC = 0;
};

#endif