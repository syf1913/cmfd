#pragma once

#include <cstddef>

#include <xtensor/containers/xtensor.hpp>

class LinearSystem
{
public:
    static constexpr std::size_t ae_column = 0;
    static constexpr std::size_t aw_column = 1;
    static constexpr std::size_t ap_column = 2;
    static constexpr std::size_t q_column = 3;

    LinearSystem() = delete;
    explicit LinearSystem(int node_count);
    ~LinearSystem() = default;
    LinearSystem(const LinearSystem&) = default;
    LinearSystem(LinearSystem&&) noexcept = default;
    LinearSystem& operator=(const LinearSystem&) = default;
    LinearSystem& operator=(LinearSystem&&) noexcept = default;

    [[nodiscard]] int getNodeCount() const;
    [[nodiscard]] int getLastIndex() const;

    [[nodiscard]] xt::xtensor<double, 1>& xCoordinates();
    [[nodiscard]] const xt::xtensor<double, 1>& xCoordinates() const;

    [[nodiscard]] xt::xtensor<double, 1>& phi();
    [[nodiscard]] const xt::xtensor<double, 1>& phi() const;

    [[nodiscard]] xt::xtensor<double, 2>& coefficients();
    [[nodiscard]] const xt::xtensor<double, 2>& coefficients() const;

    [[nodiscard]] double coefficientAt(int node_index, std::size_t column_index) const;
    double& coefficientAt(int node_index, std::size_t column_index);

private:
    [[nodiscard]] static xt::xtensor<double, 1> makeVector(std::size_t size, double value = 0.0);
    [[nodiscard]] static xt::xtensor<double, 2> makeMatrix(std::size_t rows,
                                                           std::size_t cols,
                                                           double value = 0.0);

    int node_count_;
    xt::xtensor<double, 1> x_coordinates_;
    xt::xtensor<double, 1> phi_;
    xt::xtensor<double, 2> coefficients_;
};
