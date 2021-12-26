#ifndef __SIM_H_INCLUDED__
#define __SIM_H_INCLUDED__

#include "elements/imem.h"
#include "utils/ins.h"
#include "utils/simulation_state.h"
#include <vector>

class Simulation
{
  public:
    Simulation()
    {}
    ~Simulation() = default;

    // bool Simulate(const std::vector<Ins>& ins)
    // {
    //     StageFetch(ins);
    //     StageExecute();

    //     while (1) {
    //         ...
    //         // run
    //     }
    // }

  private:
    SimulationState state_;
    SimulationSignals sigs_;
};

#endif // __SIM_H_INCLUDED__