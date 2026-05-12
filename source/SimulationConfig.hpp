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

    [[nodiscard]] static SimulationInput loadFromFile(const std::string& file_path);

private:
    static void validate(const SimulationInput& simulation_input);
};
