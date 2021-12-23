#ifndef __STAGE_H_INCLUDED__
#define __STAGE_H_INCLUDED__

#include <array>
#include <cstdint>

#include "utils/ins.h"
#include "utils/registers.h"

struct SimulationState
{
    RegisterFetchDecode reg_fetch_decode{};
    RegisterDecodeExecute reg_decode_exec{};
    RegisterExecuteMemory reg_exec_mem{};
    RegisterMemoryWriteback reg_mem_wb{};
    RegisterWriteBackFetch reg_wb_fetch{};

    // uint32_t PC_R
    // uint32_t PC_EX
    // uint32_t PC_DISP
};

#endif // __STAGE_H_INCLUDED__