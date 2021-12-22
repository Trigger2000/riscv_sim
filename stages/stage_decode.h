#include "stage.h"
#include "../utils/ins.h"
#include "../elements/regfile.h"
// #include ""

#ifndef __STAGE_DECODE_H_INCLUDED__
#define __STAGE_DECODE_H_INCLUDED__

struct decode {

    decode() = default;
    ~decode() = default;

    control_registers run() {
        control_registers output;

        Ins instr_after_fetch = fetch_decode_reg.ins_after_fetch;
        regfile register_file(instr_after_fetch.GetRs1(), instr_after_fetch.GetRs2(),
                              memory_writeback_reg.WB_A, memory_writeback_reg.WB_D,
                              memory_writeback_reg.WB_WE);
        
        output.D1 = register_file.get_D1();
        output.D2 = register_file.get_D2();
    }

private:
};


#endif