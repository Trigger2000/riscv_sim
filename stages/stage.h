#ifndef __STAGE_H_INCLUDED__
#define __STAGE_H_INCLUDED__

#include <cstdint>
#include <array>

#include "../utils/ins.h"

const int bitness = 32;

// std::array<control_registers> current_state;
 
struct PC {
public:
    PC() = default;
    ~PC() = default;

    void reset() {
        current_pc_ = 0;
        next_pc_ = current_pc_ + 1;
    }

    void move_next() {
        current_pc_++;
        next_pc_ = current_pc_ + 1;
    }

    void move_offset(int32_t offset) {
        current_pc_ += offset;
        next_pc_ = current_pc_ + 1;
    }

    int32_t get_current() const {
        return current_pc_;
    }

private:
    int32_t current_pc_ = 0;
    int32_t next_pc_ = 1;
};

/* struct control_registers {
    // fetch out
    Ins ins_after_fetch;

    // decode out
    uint32_t D1 = 0;
    uint32_t D2 = 0;
    uint32_t sign_ext_imm = 0;
    uint32_t 

    // decode in
    uint32_t WB_A = 0;
    uint32_t WB_D = 0;
    uint32_t WB_WE = 0;
}; */

struct fetch_decode_register {
    Ins ins_after_fetch;
};

struct decode_execute_register {
    uint32_t D1 = 0;
    uint32_t D2 = 0;
    uint32_t sign_ext_imm = 0;
    uint32_t imm_30_7 = 0;
};

struct execute_memory_register {

};

struct memory_writeback_register {
    uint32_t WB_A = 0;
    uint32_t WB_D = 0;
    uint32_t WB_WE = 0;
};

struct writeback_fetch_register {

};

fetch_decode_register fetch_decode_reg;
decode_execute_register decode_execute_reg;
execute_memory_register execute_memory_reg;
memory_writeback_register memory_writeback_reg;
writeback_fetch_register writeback_fetch_reg;

#endif // __STAGE_H_INCLUDED__