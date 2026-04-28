#include "LinearSystem.hpp"

LinearSystem::LinearSystem(int node_count)
    : node_count_(node_count),
      x_coordinates_(makeVector(static_cast<std::size_t>(node_count))),
      phi_(makeVector(static_cast<std::size_t>(node_count))),
      coefficients_(makeMatrix(static_cast<std::size_t>(node_count), 4))
{
}

int LinearSystem::getNodeCount() const
{
    return node_count_;
}

int LinearSystem::getLastIndex() const
{
    return node_count_ - 1;
}

xt::xtensor<double, 1>& LinearSystem::xCoordinates()
{
    return x_coordinates_;
}

const xt::xtensor<double, 1>& LinearSystem::xCoordinates() const
{
    return x_coordinates_;
}

xt::xtensor<double, 1>& LinearSystem::phi()
{
    return phi_;
}

const xt::xtensor<double, 1>& LinearSystem::phi() const
{
    return phi_;
}

xt::xtensor<double, 2>& LinearSystem::coefficients()
{
    return coefficients_;
}

const xt::xtensor<double, 2>& LinearSystem::coefficients() const
{
    return coefficients_;
}

double LinearSystem::coefficientAt(int node_index, std::size_t column_index) const
{
    return coefficients_(static_cast<std::size_t>(node_index), column_index);
}

double& LinearSystem::coefficientAt(int node_index, std::size_t column_index)
{
    return coefficients_(static_cast<std::size_t>(node_index), column_index);
}

xt::xtensor<double, 1> LinearSystem::makeVector(std::size_t size, double value)
{
    auto values = xt::xtensor<double, 1>::from_shape({size});
    values.fill(value);
    return values;
}

xt::xtensor<double, 2> LinearSystem::makeMatrix(std::size_t rows,
                                                std::size_t cols,
                                                double value)
{
    auto values = xt::xtensor<double, 2>::from_shape({rows, cols});
    values.fill(value);
    return values;
}
