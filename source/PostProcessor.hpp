#pragma once

#include "LinearSystem.hpp"
#include "SimulationTypes.hpp"

class PostProcessor
{
public:
    PostProcessor() = default;
    ~PostProcessor() = default;
    PostProcessor(const PostProcessor&) = default;
    PostProcessor(PostProcessor&&) noexcept = default;
    PostProcessor& operator=(const PostProcessor&) = default;
    PostProcessor& operator=(PostProcessor&&) noexcept = default;

    [[nodiscard]] PostProcessResult build(const SimulationInput& simulation_input,
                                          const LinearSystem& system) const;

private:
    [[nodiscard]] static xt::xtensor<double, 1> makeVector(std::size_t size,
                                                           double value = 0.0);
    [[nodiscard]] static xt::xtensor<double, 2> makeMatrix(std::size_t rows,
                                                           std::size_t cols,
                                                           double value = 0.0);
};
