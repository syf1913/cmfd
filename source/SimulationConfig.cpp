#include "SimulationConfig.hpp"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <stdexcept>
#include <string>

#include <nlohmann/json.hpp>

namespace {

[[nodiscard]] std::string toLower(std::string value)
{
    std::transform(value.begin(), value.end(), value.begin(), [](unsigned char character) {
        return static_cast<char>(std::tolower(character));
    });
    return value;
}

[[nodiscard]] ConvectionScheme parseConvectionScheme(const std::string& scheme_name)
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

[[nodiscard]] SolverType parseSolverType(const std::string& solver_name)
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

}  // namespace

SimulationInput SimulationConfig::loadFromFile(const std::string& file_path)
{
    std::ifstream input_stream(file_path);
    if (!input_stream)
    {
        throw std::runtime_error("Failed to open input file: " + file_path);
    }

    const nlohmann::json input_json = nlohmann::json::parse(input_stream, nullptr, true, true);

    const auto& transport_json = input_json.at("transport");
    const auto& boundary_json  = input_json.at("boundary_conditions");
    const auto& grid_json      = input_json.at("grid");
    const auto& solver_json    = input_json.at("solver");

    SimulationInput simulation_input;
    simulation_input.output_file_name      = input_json.value("output_file_name", "cmfd.dat");
    simulation_input.convection_scheme     = parseConvectionScheme(input_json.at("convection_scheme").get<std::string>());
    simulation_input.density               = transport_json.at("density").get<double>();
    simulation_input.velocity              = transport_json.at("velocity").get<double>();
    simulation_input.diffusion_coefficient = transport_json.at("diffusion_coefficient").get<double>();
    simulation_input.left_boundary_value   = boundary_json.at("phi_left").get<double>();
    simulation_input.right_boundary_value  = boundary_json.at("phi_right").get<double>();
    simulation_input.x_min                 = grid_json.at("x_min").get<double>();
    simulation_input.x_max                 = grid_json.at("x_max").get<double>();
    simulation_input.expansion_factor      = grid_json.at("expansion_factor").get<double>();
    simulation_input.node_count            = grid_json.at("node_count").get<int>();
    simulation_input.omega                 = solver_json.value("omega", 1.0);
    simulation_input.solver_type           = parseSolverType(solver_json.at("type").get<std::string>());

    validate(simulation_input);
    return simulation_input;
}

void SimulationConfig::validate(const SimulationInput& simulation_input)
{
    if (simulation_input.node_count < 2)
    {
        throw std::runtime_error("node_count must be at least 2");
    }
    if (simulation_input.density <= 0.0)
    {
        throw std::runtime_error("density must be positive");
    }
    if (simulation_input.diffusion_coefficient == 0.0)
    {
        throw std::runtime_error("diffusion_coefficient must be non-zero");
    }
    if (simulation_input.expansion_factor <= 0.0)
    {
        throw std::runtime_error("expansion_factor must be positive");
    }
    if (simulation_input.x_max <= simulation_input.x_min)
    {
        throw std::runtime_error("x_max must be greater than x_min");
    }
}
