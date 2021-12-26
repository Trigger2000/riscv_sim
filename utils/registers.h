#ifndef __REGISTERS_H_INCLUDED__
#define __REGISTERS_H_INCLUDED__

#include "utils/ins.h"

struct RegisterFetchDecode
{
    Ins ins = Ins();
    uint32_t PC = 0;
    uint32_t PC_R = false;
};

struct RegisterDecodeExecute
{
    struct ControlEx
    {
        Ins::InsMnemonic ALUOP = Ins::InsMnemonic::INVALID;
        uint32_t ALU_SRC2 = 0;
    };

    ControlEx CONTROL_EX;
    int32_t D1 = 0;
    int32_t D2 = 0;
    uint32_t sign_bit = 0;
    uint32_t PC_EX;
    uint32_t V_EX = 0;

    // bus 30:7
    int32_t imm = 0;
    uint32_t WB_A = 0;
};

struct RegisterExecuteMemory
{
    int32_t WD = 0;
    int32_t alu_res = 0;
    uint32_t CONTROL_EX = 0; // rename?
    uint32_t MEM_WE = 0;
    uint32_t WB_WE = 0;
    uint32_t WB_A = 0;
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