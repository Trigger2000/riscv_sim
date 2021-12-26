#ifndef __STAGE_H_INCLUDED__
#define __STAGE_H_INCLUDED__

#include <array>
#include <cstdint>

#include "utils/ins.h"
#include "utils/registers.h"

// bundle for registers
struct SimulationState
{
    RegisterFetchDecode reg_fetch_decode{};
    RegisterDecodeExecute reg_decode_exec{};
    RegisterExecuteMemory reg_exec_mem{};
    RegisterMemoryWriteback reg_mem_wb{};
    RegisterWriteBackFetch reg_wb_fetch{};
};

// any data that traverses without registers
struct SimulationSignals
{
    // from fetch

    // from decode

    // from execute
    int32_t PC_DISP = 0;
    uint32_t PC_EX = 0;

    // from memory
    int32_t BP_MEM = 0;

    // from writeback
    int32_t WB_D = 0;
    int32_t BP_WB = 0;
    uint32_t WB_WE = 0;
    uint32_t WB_A = 0;
    uint32_t HU_MEM_RD = 0;

    // from HU
    // FIXME: probably the same thing
    uint32_t HU_PC_REDIRECT = 0;
    uint32_t PC_R = 0;
    uint32_t HU_RS1 = 0;
    uint32_t HU_RS2 = 0;
};

#endif // __STAGE_H_INCLUDED__