#ifndef __STAGE_MEMORY_H_INCLUDED__
#define __STAGE_MEMORY_H_INCLUDED__

#include "elements/dmem.h"
#include "elements/mux.h"
#include "utils/ins.h"
#include "utils/registers.h"
#include "utils/simulation_state.h"

struct StageMemory
{
    StageMemory() = default;
    ~StageMemory() = default;

    RegisterMemoryWriteback run(const RegisterExecuteMemory& input_reg,
                                SimulationSignals* signals)
    {
        assert(signals != nullptr);

        // std::cout << "[MEM] PB_MEM " << signals->BP_MEM << "\n";
        // std::cout << "[MEM] HU_MEM_RD " << signals->HU_MEM_RD << "\n";
        // std::cout << "[MEM] A " << input_reg.alu_res << "\n";
        // std::cout << "[MEM] WB_A " << input_reg.WB_A << "\n";
        // std::cout << "[MEM] WB_WE " << input_reg.WB_WE << "\n";
        // std::cout << "[MEM] WB_WD " << input_reg.WD << "\n";

        signals->BP_MEM = input_reg.alu_res;
        signals->HU_MEM_RD = input_reg.WB_A;

        RegisterMemoryWriteback output_reg;

        if (input_reg.NOP) {
            return output_reg;
        }

        output_reg.WB_A = input_reg.WB_A;
        output_reg.WB_WE = input_reg.WB_WE;

        memory.write_data(input_reg.MEM_WE, input_reg.WD, input_reg.alu_res,
                          input_reg.mem_width);

        int32_t RD = memory.read_data(input_reg.alu_res, input_reg.mem_width);

        output_reg.WB_D = Mux2(RD, input_reg.alu_res, input_reg.WB_selector);

        return output_reg;
    }

    void DmemWrite(uint32_t WE, int32_t WD, uint32_t A, uint32_t width)
    {
        memory.write_data(WE, WD, A, width);

        return;
    }

    int32_t DmemRead(uint32_t A, uint32_t width)
    {
        return memory.read_data(A, width);
    }

  private:
    Dmem memory;
};

#endif