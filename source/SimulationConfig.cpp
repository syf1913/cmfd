#include "SimulationConfig.hpp"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <stdexcept>
#include <string>

#include <nlohmann/json.hpp>

SimulationInput SimulationConfig::loadFromFile(const std::string& file_path) const
{
    std::ifstream input_stream(file_path);
    if (!input_stream)
    {
        throw std::runtime_error("Failed to open input file: " + file_path);
    }

    const nlohmann::json input_json = nlohmann::json::parse(input_stream);
    const auto& transport_json = input_json.at("transport");
    const auto& boundary_json = input_json.at("boundary_conditions");
    const auto& grid_json = input_json.at("grid");
    const auto& solver_json = input_json.at("solver");

    const SimulationInput simulation_input{
        input_json.at("output_file_name").get<std::string>(),
        transport_json.at("density").get<double>(),
        transport_json.at("velocity").get<double>(),
        transport_json.at("diffusion_coefficient").get<double>(),
        boundary_json.at("phi_left").get<double>(),
        boundary_json.at("phi_right").get<double>(),
        parseConvectionScheme(input_json.at("convection_scheme").get<std::string>()),
        grid_json.at("x_min").get<double>(),
        grid_json.at("x_max").get<double>(),
        grid_json.at("expansion_factor").get<double>(),
        grid_json.at("node_count").get<int>(),
        parseSolverType(solver_json.at("type").get<std::string>()),
        solver_json.value("omega", 1.0),
    };

    validate(simulation_input);
    return simulation_input;
}

ConvectionScheme SimulationConfig::parseConvectionScheme(const std::string& scheme_name)
{
    const std::string normalized_name = toLower(scheme_name);
    if (normalized_name == "cds" || normalized_name == "central_difference")
    {
        return ConvectionScheme::centralDifference;
    }
    if (normalized_name == "uds" || normalized_name == "upwind_difference")
    {
        return ConvectionScheme::upwindDifference;
    }

    throw std::runtime_error("Unsupported convection scheme: " + scheme_name);
}

SolverType SimulationConfig::parseSolverType(const std::string& solver_name)
{
    const std::string normalized_name = toLower(solver_name);
    if (normalized_name == "jacobi")
    {
        return SolverType::jacobi;
    }
    if (normalized_name == "gauss_seidel" || normalized_name == "gauss-seidel" ||
        normalized_name == "gs")
    {
        return SolverType::gaussSeidel;
    }
    if (normalized_name == "sor" || normalized_name == "gsor")
    {
        return SolverType::successiveOverRelaxation;
    }
    if (normalized_name == "tdma")
    {
        return SolverType::tdma;
    }

    throw std::runtime_error("Unsupported solver type: " + solver_name);
}

std::string SimulationConfig::toLower(std::string value)
{
    std::transform(value.begin(), value.end(), value.begin(), [](unsigned char character) {
        return static_cast<char>(std::tolower(character));
    });
    return value;
}

void SimulationConfig::validate(const SimulationInput& simulation_input)
{
    if (simulation_input.node_count < 2)
    {
        throw std::runtime_error("node_count must be at least 2");
    }
    if (simulation_input.diffusion_coefficient == 0.0)
    {
        throw std::runtime_error("diffusion_coefficient must be non-zero");
    }
    if (simulation_input.expansion_factor <= 0.0)
    {
        throw std::runtime_error("expansion_factor must be positive");
    }
    if (simulation_input.x_max == simulation_input.x_min)
    {
        throw std::runtime_error("x_max must be different from x_min");
    }
}
