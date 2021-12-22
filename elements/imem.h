#ifndef __IMEM_H_INCLUDED__
#define __IMEM_H_INCLUDED__

#include <vector>

#include "../stages/stage.h"
#include "../utils/ins.h"


struct imem {

    imem(std::vector<Ins> instructions): instructions_(instructions) {}
    ~imem() = default;

    Ins fetch(const PC& pc) {
        return instructions_[pc.get_current()];
    }

private:
    std::vector<Ins> instructions_;
};

#endif // __IMEM_H_INCLUDED__