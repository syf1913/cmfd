#pragma once

#include <string>

#include "SimulationTypes.hpp"

class SimulationConfig
{
public:
    SimulationConfig() = default;
    ~SimulationConfig() = default;
    SimulationConfig(const SimulationConfig&) = default;
    SimulationConfig(SimulationConfig&&) noexcept = default;
    SimulationConfig& operator=(const SimulationConfig&) = default;
    SimulationConfig& operator=(SimulationConfig&&) noexcept = default;

    [[nodiscard]] SimulationInput loadFromFile(const std::string& file_path) const;

private:
    [[nodiscard]] static std::string toLower(std::string value);
    [[nodiscard]] static ConvectionScheme parseConvectionScheme(const std::string& scheme_name);
    [[nodiscard]] static SolverType parseSolverType(const std::string& solver_name);
    static void validate(const SimulationInput& simulation_input);
};
