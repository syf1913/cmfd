#include "SolverFactory.hpp"

#include <cmath>
#include <memory>
#include <stdexcept>

#include <xtensor/containers/xtensor.hpp>

namespace
{
class SolverBase : public SolverStrategy
{
protected:
    static constexpr int max_iterations = 1000;
    static constexpr double residual_tolerance = 1.0e-4;

    [[nodiscard]] static xt::xtensor<double, 1> makeVector(std::size_t size,
                                                           double value = 0.0)
    {
        auto values = xt::xtensor<double, 1>::from_shape({size});
        values.fill(value);
        return values;
    }

    [[nodiscard]] static double computeResidual(const LinearSystem& system)
    {
        double residual = 0.0;
        const int last_index = system.getLastIndex();

        for (int node_index = 1; node_index < last_index; ++node_index)
        {
            residual += std::abs(
                -system.coefficientAt(node_index, LinearSystem::ae_column) *
                    system.phi()(static_cast<std::size_t>(node_index + 1)) -
                system.coefficientAt(node_index, LinearSystem::aw_column) *
                    system.phi()(static_cast<std::size_t>(node_index - 1)) +
                system.coefficientAt(node_index, LinearSystem::q_column) -
                system.coefficientAt(node_index, LinearSystem::ap_column) *
                    system.phi()(static_cast<std::size_t>(node_index)));
        }

        return residual;
    }
};

class JacobiSolver final : public SolverBase
{
public:
    JacobiSolver() = default;
    ~JacobiSolver() override = default;
    JacobiSolver(const JacobiSolver&) = default;
    JacobiSolver(JacobiSolver&&) noexcept = default;
    JacobiSolver& operator=(const JacobiSolver&) = default;
    JacobiSolver& operator=(JacobiSolver&&) noexcept = default;

    [[nodiscard]] SolverResult solve(LinearSystem& system,
                                     const SimulationInput&) const override
    {
        SolverResult result{0, 0.0, {}};
        const int last_index = system.getLastIndex();
        xt::xtensor<double, 1> phi_old = system.phi();

        for (int iteration = 1; iteration <= max_iterations; ++iteration)
        {
            phi_old = system.phi();
            for (int node_index = 1; node_index < last_index; ++node_index)
            {
                system.phi()(static_cast<std::size_t>(node_index)) =
                    (-system.coefficientAt(node_index, LinearSystem::ae_column) *
                         phi_old(static_cast<std::size_t>(node_index + 1)) -
                     system.coefficientAt(node_index, LinearSystem::aw_column) *
                         phi_old(static_cast<std::size_t>(node_index - 1)) +
                     system.coefficientAt(node_index, LinearSystem::q_column)) /
                    system.coefficientAt(node_index, LinearSystem::ap_column);
            }

            result.residual = computeResidual(system);
            result.iterations = iteration;
            result.residual_history.push_back(result.residual);
            if (result.residual < residual_tolerance)
            {
                break;
            }
        }

        return result;
    }
};

class GaussSeidelSolver final : public SolverBase
{
public:
    GaussSeidelSolver() = default;
    ~GaussSeidelSolver() override = default;
    GaussSeidelSolver(const GaussSeidelSolver&) = default;
    GaussSeidelSolver(GaussSeidelSolver&&) noexcept = default;
    GaussSeidelSolver& operator=(const GaussSeidelSolver&) = default;
    GaussSeidelSolver& operator=(GaussSeidelSolver&&) noexcept = default;

    [[nodiscard]] SolverResult solve(LinearSystem& system,
                                     const SimulationInput&) const override
    {
        SolverResult result{0, 0.0, {}};
        const int last_index = system.getLastIndex();

        for (int iteration = 1; iteration <= max_iterations; ++iteration)
        {
            for (int node_index = 1; node_index < last_index; ++node_index)
            {
                system.phi()(static_cast<std::size_t>(node_index)) =
                    (-system.coefficientAt(node_index, LinearSystem::ae_column) *
                         system.phi()(static_cast<std::size_t>(node_index + 1)) -
                     system.coefficientAt(node_index, LinearSystem::aw_column) *
                         system.phi()(static_cast<std::size_t>(node_index - 1)) +
                     system.coefficientAt(node_index, LinearSystem::q_column)) /
                    system.coefficientAt(node_index, LinearSystem::ap_column);
            }

            result.residual = computeResidual(system);
            result.iterations = iteration;
            result.residual_history.push_back(result.residual);
            if (result.residual < residual_tolerance)
            {
                break;
            }
        }

        return result;
    }
};

class SorSolver final : public SolverBase
{
public:
    SorSolver() = default;
    ~SorSolver() override = default;
    SorSolver(const SorSolver&) = default;
    SorSolver(SorSolver&&) noexcept = default;
    SorSolver& operator=(const SorSolver&) = default;
    SorSolver& operator=(SorSolver&&) noexcept = default;

    [[nodiscard]] SolverResult solve(LinearSystem& system,
                                     const SimulationInput& simulation_input) const override
    {
        SolverResult result{0, 0.0, {}};
        const int last_index = system.getLastIndex();

        for (int iteration = 1; iteration <= max_iterations; ++iteration)
        {
            for (int node_index = 1; node_index < last_index; ++node_index)
            {
                const double phi_candidate =
                    (-system.coefficientAt(node_index, LinearSystem::ae_column) *
                         system.phi()(static_cast<std::size_t>(node_index + 1)) -
                     system.coefficientAt(node_index, LinearSystem::aw_column) *
                         system.phi()(static_cast<std::size_t>(node_index - 1)) +
                     system.coefficientAt(node_index, LinearSystem::q_column)) /
                    system.coefficientAt(node_index, LinearSystem::ap_column);

                system.phi()(static_cast<std::size_t>(node_index)) +=
                    simulation_input.omega *
                    (phi_candidate - system.phi()(static_cast<std::size_t>(node_index)));
            }

            result.residual = computeResidual(system);
            result.iterations = iteration;
            result.residual_history.push_back(result.residual);
            if (result.residual < residual_tolerance)
            {
                break;
            }
        }

        return result;
    }
};

class TdmaSolver final : public SolverBase
{
public:
    TdmaSolver() = default;
    ~TdmaSolver() override = default;
    TdmaSolver(const TdmaSolver&) = default;
    TdmaSolver(TdmaSolver&&) noexcept = default;
    TdmaSolver& operator=(const TdmaSolver&) = default;
    TdmaSolver& operator=(TdmaSolver&&) noexcept = default;

    [[nodiscard]] SolverResult solve(LinearSystem& system,
                                     const SimulationInput&) const override
    {
        SolverResult result{1, 0.0, {}};
        const int last_index = system.getLastIndex();
        xt::xtensor<double, 1> bpr = makeVector(static_cast<std::size_t>(system.getNodeCount()));
        xt::xtensor<double, 1> v = makeVector(static_cast<std::size_t>(system.getNodeCount()));

        for (int node_index = 1; node_index < last_index; ++node_index)
        {
            bpr(static_cast<std::size_t>(node_index)) =
                1.0 / (system.coefficientAt(node_index, LinearSystem::ap_column) -
                       system.coefficientAt(node_index, LinearSystem::aw_column) *
                           system.coefficientAt(node_index - 1, LinearSystem::ae_column) *
                           bpr(static_cast<std::size_t>(node_index - 1)));

            v(static_cast<std::size_t>(node_index)) =
                system.coefficientAt(node_index, LinearSystem::q_column) -
                system.coefficientAt(node_index, LinearSystem::aw_column) *
                    v(static_cast<std::size_t>(node_index - 1)) *
                    bpr(static_cast<std::size_t>(node_index - 1));
        }

        for (int node_index = last_index - 1; node_index >= 1; --node_index)
        {
            system.phi()(static_cast<std::size_t>(node_index)) =
                (v(static_cast<std::size_t>(node_index)) -
                 system.coefficientAt(node_index, LinearSystem::ae_column) *
                     system.phi()(static_cast<std::size_t>(node_index + 1))) *
                bpr(static_cast<std::size_t>(node_index));
        }

        result.residual = computeResidual(system);
        result.residual_history.push_back(result.residual);
        return result;
    }
};
}  // namespace

std::unique_ptr<SolverStrategy> SolverFactory::create(SolverType solver_type) const
{
    switch (solver_type)
    {
    case SolverType::jacobi:
        return std::make_unique<JacobiSolver>();
    case SolverType::gaussSeidel:
        return std::make_unique<GaussSeidelSolver>();
    case SolverType::successiveOverRelaxation:
        return std::make_unique<SorSolver>();
    case SolverType::tdma:
        return std::make_unique<TdmaSolver>();
    }

    throw std::runtime_error("Unsupported solver type");
}
