#pragma once

#include <memory>

#include "SimulationTypes.hpp"
#include "SolverStrategy.hpp"

class SolverFactory
{
public:
    SolverFactory() = default;
    ~SolverFactory() = default;
    SolverFactory(const SolverFactory&) = default;
    SolverFactory(SolverFactory&&) noexcept = default;
    SolverFactory& operator=(const SolverFactory&) = default;
    SolverFactory& operator=(SolverFactory&&) noexcept = default;

    [[nodiscard]] std::unique_ptr<SolverStrategy> create(SolverType solver_type) const;
};
