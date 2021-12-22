#ifndef __SIM_H_INCLUDED__
#define __SIM_H_INCLUDED__

#include "utils/ins.h"
#include <vector>

class simulation
{
  public:
    simulation ();

    bool Simulate (std::vector<Ins>) { return true; }

  private:
};

#endif // __SIM_H_INCLUDED__