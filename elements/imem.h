#ifndef __IMEM_H_INCLUDED__
#define __IMEM_H_INCLUDED__

#include <vector>

#include "../stages/stage.h"
#include "../utils/ins.h"

struct Imem
{
    Imem (std::vector<Ins> instructions) : instructions_ (instructions) {}
    ~Imem () = default;

    Ins Fetch (const PC &pc) { return instructions_[pc.get_current ()]; }

  private:
    std::vector<Ins> instructions_;
};

#endif // __IMEM_H_INCLUDED__