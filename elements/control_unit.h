#ifndef __CU_H_INCLUDED__
#define __CU_H_INCLUDED__

#include "utils/ins.h"

struct Control_unit
{
    Control_unit(Ins instr) : instr_(instr)
    {}
    ~Control_unit() = default;

    // extend this method while developing
    void get_signals()
    {}

  private:
    Ins instr_;
};

#endif