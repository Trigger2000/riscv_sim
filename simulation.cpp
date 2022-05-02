#include "sim.h"
#include <algorithm>

int main()
{
    static const std::vector<Ins::InsMnemonic> NOT_IMPLEMENTED = {
        Ins::InsMnemonic::LBU, Ins::InsMnemonic::LHU,   Ins::InsMnemonic::JALR,
        Ins::InsMnemonic::BEQ, Ins::InsMnemonic::BNE,   Ins::InsMnemonic::BLT,
        Ins::InsMnemonic::BGE, Ins::InsMnemonic::BLTU,  Ins::InsMnemonic::BGEU,
        Ins::InsMnemonic::LUI, Ins::InsMnemonic::AUIPC, Ins::InsMnemonic::JAL,
    };

    std::vector<Ins> IMEM = { Ins::MakeIns_ADD(1, 2, 3) };

    for (size_t i = 0; i < IMEM.size(); ++i) {
        if (std::find(NOT_IMPLEMENTED.begin(), NOT_IMPLEMENTED.end(),
                      IMEM[i].GetInsMnemonic()) != NOT_IMPLEMENTED.end()) {
            std::cerr << "[ERROR] instruction number " << i
                      << " in provided IMEM is not supported\n";
            return 1;
        }
    }

    Simulation sim;
    sim.run(IMEM);

    return 0;
}