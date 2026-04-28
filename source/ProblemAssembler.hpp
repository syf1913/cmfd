#pragma once

#include "LinearSystem.hpp"
#include "SimulationTypes.hpp"

class ProblemAssembler
{
public:
    ProblemAssembler() = default;
    ~ProblemAssembler() = default;
    ProblemAssembler(const ProblemAssembler&) = default;
    ProblemAssembler(ProblemAssembler&&) noexcept = default;
    ProblemAssembler& operator=(const ProblemAssembler&) = default;
    ProblemAssembler& operator=(ProblemAssembler&&) noexcept = default;

    [[nodiscard]] LinearSystem build(const SimulationInput& simulation_input) const;
};
