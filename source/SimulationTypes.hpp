#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include <xtensor/containers/xtensor.hpp>

enum class ConvectionScheme : std::uint8_t
{
    centralDifference,
    upwindDifference,
};

enum class SolverType : std::uint8_t
{
    jacobi,
    gaussSeidel,
    successiveOverRelaxation,
    tdma,
};

struct SimulationInput
{
    std::string output_file_name;

    int node_count{};

    double density{};
    double velocity{};
    double diffusion_coefficient{};
    double left_boundary_value{};
    double right_boundary_value{};
    double x_min{};
    double x_max{};
    double expansion_factor{};
    double omega{};

    ConvectionScheme convection_scheme{};
    SolverType solver_type{};

    SimulationInput() = default;
    ~SimulationInput() = default;
    SimulationInput(const SimulationInput&) = default;
    SimulationInput(SimulationInput&&) noexcept = default;
    SimulationInput& operator=(const SimulationInput&) = default;
    SimulationInput& operator=(SimulationInput&&) noexcept = default;
};

struct SolverResult
{
    int iterations{};
    double residual{};
    std::vector<double> residual_history;

    SolverResult() = default;
    ~SolverResult() = default;
    SolverResult(const SolverResult&) = default;
    SolverResult(SolverResult&&) noexcept = default;
    SolverResult& operator=(const SolverResult&) = default;
    SolverResult& operator=(SolverResult&&) noexcept = default;
};

struct PostProcessResult
{
    double peclet_number{};
    double error_norm{};

    xt::xtensor<double, 1> exact_phi;
    xt::xtensor<double, 2> result_table;

    PostProcessResult() = default;
    ~PostProcessResult() = default;
    PostProcessResult(const PostProcessResult&) = default;
    PostProcessResult(PostProcessResult&&) noexcept = default;
    PostProcessResult& operator=(const PostProcessResult&) = default;
    PostProcessResult& operator=(PostProcessResult&&) noexcept = default;
};
