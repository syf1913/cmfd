#pragma once

#include "LinearSystem.hpp"
#include "SimulationTypes.hpp"

class SolverStrategy
{
public:
    SolverStrategy() = default;
    virtual ~SolverStrategy() = default;
    SolverStrategy(const SolverStrategy&) = delete;
    SolverStrategy(SolverStrategy&&) noexcept = delete;
    SolverStrategy& operator=(const SolverStrategy&) = delete;
    SolverStrategy& operator=(SolverStrategy&&) noexcept = delete;

    [[nodiscard]] virtual SolverResult solve(LinearSystem& system,
                                             const SimulationInput& simulation_input) const = 0;
};
