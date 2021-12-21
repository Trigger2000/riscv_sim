#include "sim.h"

int main() {
    std::vector<Ins> inss = {Ins::make_ADD(1, 2, 3)};

    simulation sim;
    sim.Simulate(inss);

    return 0;
}