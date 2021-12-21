#include <cstdint>

//int32_t PC = 0;

struct PC {

    PC() = default;
    ~PC() = default;

    void reset() {
        current_pc_ = 0;
        next_pc_ = current_pc_ + 1;
    }

    void move() {
        current_pc_++;
        next_pc_ = current_pc_ + 1;
    }

    void move_value(int32_t offset) {
        current_pc_ += offset;
        next_pc_ = current_pc_ + 1;
    }


private:
    int32_t current_pc_ = 0;
    int32_t next_pc_ = 1;
};

struct control_register {
    

};

