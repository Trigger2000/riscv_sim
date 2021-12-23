#include "sim.h"

int main()
{
    std::vector<Ins> inss = { Ins::MakeIns_ADD(1, 2, 3) };

    Simulation sim;
    sim.Simulate(inss);

    return 0;
}