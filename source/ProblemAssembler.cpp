#include "ProblemAssembler.hpp"

#include <algorithm>
#include <cmath>

LinearSystem ProblemAssembler::build(const SimulationInput& simulation_input) const
{
    LinearSystem system(simulation_input.node_count);
    const int last_index = system.getLastIndex();

    double dx = 0.0;
    if (simulation_input.expansion_factor == 1.0)
    {
        dx = (simulation_input.x_max - simulation_input.x_min) /
             static_cast<double>(simulation_input.node_count - 1);
    }
    else
    {
        dx = (simulation_input.x_max - simulation_input.x_min) *
             (1.0 - simulation_input.expansion_factor) /
             (1.0 - std::pow(simulation_input.expansion_factor, simulation_input.node_count - 1));
    }

    system.xCoordinates()(0) = simulation_input.x_min;
    for (int node_index = 1; node_index < simulation_input.node_count; ++node_index)
    {
        system.xCoordinates()(static_cast<std::size_t>(node_index)) =
            system.xCoordinates()(static_cast<std::size_t>(node_index - 1)) + dx;
        dx *= simulation_input.expansion_factor;
    }

    system.phi()(0) = simulation_input.left_boundary_value;
    system.phi()(static_cast<std::size_t>(last_index)) = simulation_input.right_boundary_value;

    const double density_velocity = simulation_input.density * simulation_input.velocity;
    for (int node_index = 1; node_index < last_index; ++node_index)
    {
        const double x_w = system.xCoordinates()(static_cast<std::size_t>(node_index - 1));
        const double x_p = system.xCoordinates()(static_cast<std::size_t>(node_index));
        const double x_e = system.xCoordinates()(static_cast<std::size_t>(node_index + 1));

        double ae_convection = 0.0;
        double aw_convection = 0.0;
        if (simulation_input.convection_scheme == ConvectionScheme::centralDifference)
        {
            ae_convection = density_velocity / (x_e - x_w);
            aw_convection = -ae_convection;
        }
        else
        {
            ae_convection = std::min(density_velocity, 0.0) / (x_e - x_p);
            aw_convection = -std::max(density_velocity, 0.0) / (x_p - x_w);
        }

        const double dx_ratio = 2.0 / (x_e - x_w);
        const double ae_diffusion =
            -simulation_input.diffusion_coefficient * dx_ratio / (x_e - x_p);
        const double aw_diffusion =
            -simulation_input.diffusion_coefficient * dx_ratio / (x_p - x_w);

        system.coefficientAt(node_index, LinearSystem::ae_column) = ae_convection + ae_diffusion;
        system.coefficientAt(node_index, LinearSystem::aw_column) = aw_convection + aw_diffusion;
        system.coefficientAt(node_index, LinearSystem::ap_column) =
            -system.coefficientAt(node_index, LinearSystem::aw_column) -
            system.coefficientAt(node_index, LinearSystem::ae_column);
    }

    if (simulation_input.node_count > 2)
    {
        system.coefficientAt(1, LinearSystem::q_column) -=
            system.coefficientAt(1, LinearSystem::aw_column) * system.phi()(0);
        system.coefficientAt(1, LinearSystem::aw_column) = 0.0;

        system.coefficientAt(last_index - 1, LinearSystem::q_column) -=
            system.coefficientAt(last_index - 1, LinearSystem::ae_column) *
            system.phi()(static_cast<std::size_t>(last_index));
        system.coefficientAt(last_index - 1, LinearSystem::ae_column) = 0.0;
    }

    return system;
}
