#include "PostProcessor.hpp"

#include <cmath>
#include <limits>

PostProcessResult PostProcessor::build(const SimulationInput& simulation_input,
                                       const LinearSystem& system) const
{
    const std::size_t node_count = static_cast<std::size_t>(simulation_input.node_count);
    PostProcessResult result{
        0.0,
        0.0,
        makeVector(node_count),
        makeMatrix(node_count, 4),
    };

    const int last_index = system.getLastIndex();
    const double density_velocity = simulation_input.density * simulation_input.velocity;
    const double domain_length = simulation_input.x_max - simulation_input.x_min;
    result.peclet_number = density_velocity * domain_length / simulation_input.diffusion_coefficient;

    result.exact_phi(0) = simulation_input.left_boundary_value;
    result.exact_phi(static_cast<std::size_t>(last_index)) = simulation_input.right_boundary_value;

    for (int node_index = 1; node_index < last_index; ++node_index)
    {
        const double x_position = system.xCoordinates()(static_cast<std::size_t>(node_index));
        double exact_value = 0.0;

        if (std::abs(result.peclet_number) < std::numeric_limits<double>::epsilon())
        {
            exact_value = simulation_input.left_boundary_value +
                          (x_position - simulation_input.x_min) / domain_length *
                              (simulation_input.right_boundary_value -
                               simulation_input.left_boundary_value);
        }
        else
        {
            const double shifted_position = (x_position - simulation_input.x_min) / domain_length;
            const double numerator = std::exp(result.peclet_number * shifted_position) - 1.0;
            const double denominator = std::exp(result.peclet_number) - 1.0;
            exact_value = simulation_input.left_boundary_value +
                          numerator / denominator *
                              (simulation_input.right_boundary_value -
                               simulation_input.left_boundary_value);
        }

        result.exact_phi(static_cast<std::size_t>(node_index)) = exact_value;
        result.error_norm +=
            std::abs(exact_value - system.phi()(static_cast<std::size_t>(node_index)));
    }

    result.error_norm /= static_cast<double>(simulation_input.node_count);

    for (int node_index = 0; node_index < simulation_input.node_count; ++node_index)
    {
        const std::size_t row_index = static_cast<std::size_t>(node_index);
        result.result_table(row_index, 0) = system.xCoordinates()(row_index);
        result.result_table(row_index, 1) = result.exact_phi(row_index);
        result.result_table(row_index, 2) = system.phi()(row_index);
        result.result_table(row_index, 3) = result.exact_phi(row_index) - system.phi()(row_index);
    }

    return result;
}

xt::xtensor<double, 1> PostProcessor::makeVector(std::size_t size, double value)
{
    auto values = xt::xtensor<double, 1>::from_shape({size});
    values.fill(value);
    return values;
}

xt::xtensor<double, 2> PostProcessor::makeMatrix(std::size_t rows,
                                                 std::size_t cols,
                                                 double value)
{
    auto values = xt::xtensor<double, 2>::from_shape({rows, cols});
    values.fill(value);
    return values;
}
