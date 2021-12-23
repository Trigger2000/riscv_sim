#ifndef __REGISTERS_H_INCLUDED__
#define __REGISTERS_H_INCLUDED__

#include "utils/ins.h"
#include "utils/pc.h"

struct RegisterFetchDecode
{
    Ins ins_after_fetch;
    ProgramCounter pc;
    bool PC_R = false;
};

struct RegisterDecodeExecute
{
    struct CONTROL_EX
    {
        // FIXME:
        uint32_t ALUOP;
    };

    uint32_t D1 = 0;
    uint32_t D2 = 0;
    uint32_t sign_bit = 0;
    ProgramCounter pc;
    bool v_de = 0;

    // bus 30:7
    uint32_t imm = 0;
    uint32_t wb_a = 0; // for wb_a
};

struct RegisterExecuteMemory
{};

struct RegisterMemoryWriteback
{
    uint32_t WB_A = 0;
    uint32_t WB_D = 0;
    uint32_t WB_WE = 0;
};

struct RegisterWriteBackFetch
{
    ProgramCounter pc;
};

#endif