#pragma once

#include <filesystem>
#include <memory>
#include <string>

#include <spdlog/logger.h>

#include "SimulationTypes.hpp"

class Reporter
{
public:
    Reporter() = default;
    ~Reporter() = default;
    Reporter(const Reporter&) = default;
    Reporter(Reporter&&) noexcept = default;
    Reporter& operator=(const Reporter&) = default;
    Reporter& operator=(Reporter&&) noexcept = default;

    void writeOutput(const std::filesystem::path& executable_path,
                     const SimulationInput& simulation_input,
                     const SolverResult& solver_result,
                     const PostProcessResult& post_process_result) const;

    void printConsoleSummary(const std::filesystem::path& executable_path,
                             const std::string& input_file_path,
                             const SimulationInput& simulation_input,
                             const SolverResult& solver_result,
                             const PostProcessResult& post_process_result) const;

private:
    static constexpr int column_width = 20;
    static constexpr int precision = 11;

    [[nodiscard]] static std::string executableStem(const std::filesystem::path& executable_path);
    [[nodiscard]] static std::string convectionSchemeName(ConvectionScheme convection_scheme);
    [[nodiscard]] static std::string solverName(SolverType solver_type);
    [[nodiscard]] static std::shared_ptr<spdlog::logger> createFileLogger(
        const std::string& log_file_name);
    [[nodiscard]] static std::shared_ptr<spdlog::logger> createDataLogger(
        const std::string& data_file_name);
    [[nodiscard]] static std::string formatScientific(double value);
    [[nodiscard]] static std::string formatHeaderRow();
    [[nodiscard]] static std::string formatDataRow(double x_value,
                                                  double exact_phi_value,
                                                  double phi_value,
                                                  double error_value);
    static void setConsolePattern();
    static void logInfo(const std::shared_ptr<spdlog::logger>& logger, const std::string& message);
    static void writeDataFile(const std::shared_ptr<spdlog::logger>& data_logger,
                              const PostProcessResult& post_process_result);
};
