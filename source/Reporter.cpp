#include "Reporter.hpp"

#include <filesystem>
#include <memory>
#include <string>

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>

constexpr const char* kConsolePattern = "[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %s:%# %v";
constexpr const char* kLogFilePattern = "[%Y-%m-%d %H:%M:%S.%e] [%l] %s:%# %v";
constexpr const char* kDataFilePattern = "%v";

void Reporter::writeOutput(const std::filesystem::path& executable_path,
                           const SimulationInput& simulation_input,
                           const SolverResult& solver_result,
                           const PostProcessResult& post_process_result) const
{
    const std::string executable_name = executableStem(executable_path);
    const std::string log_file_name = executable_name + ".log";
    const std::string data_file_name = executable_name + ".dat";

    auto log_logger = createFileLogger(log_file_name);
    auto data_logger = createDataLogger(data_file_name);

    if (simulation_input.solver_type != SolverType::tdma)
    {
        for (std::size_t iteration_index = 0;
             iteration_index < solver_result.residual_history.size();
             ++iteration_index)
        {
            logInfo(log_logger,
                    fmt::format("{:6d} ITER., RES = {}",
                                static_cast<int>(iteration_index + 1),
                                formatScientific(solver_result.residual_history[iteration_index])));
        }
    }

    logInfo(log_logger, "");
    logInfo(log_logger,
            fmt::format("PECLET NUMBER: PE = {}",
                        formatScientific(post_process_result.peclet_number)));
    logInfo(log_logger,
            fmt::format("ERROR NORM = {}",
                        formatScientific(post_process_result.error_norm)));
    logInfo(log_logger, convectionSchemeName(simulation_input.convection_scheme));
    logInfo(log_logger, solverName(simulation_input.solver_type));
    logInfo(log_logger, "");
    logInfo(log_logger, formatHeaderRow());
    logInfo(log_logger, "");

    writeDataFile(data_logger, post_process_result);

    for (std::size_t row_index = 0; row_index < post_process_result.result_table.shape()[0];
         ++row_index)
    {
        const std::string data_row =
            formatDataRow(post_process_result.result_table(row_index, 0),
                          post_process_result.result_table(row_index, 1),
                          post_process_result.result_table(row_index, 2),
                          post_process_result.result_table(row_index, 3));
        logInfo(log_logger, data_row);
    }

    log_logger->flush();
    data_logger->flush();
    spdlog::drop(log_logger->name());
    spdlog::drop(data_logger->name());
}

void Reporter::printConsoleSummary(const std::filesystem::path& executable_path,
                                   const std::string& input_file_path,
                                   const SimulationInput& simulation_input,
                                   const SolverResult& solver_result,
                                   const PostProcessResult& post_process_result) const
{
    const std::string executable_name = executableStem(executable_path);
    setConsolePattern();
    SPDLOG_INFO("Loaded input from: {}", input_file_path);
    SPDLOG_INFO("Log written to: {}.log", executable_name);
    SPDLOG_INFO("Data written to: {}.dat", executable_name);
    SPDLOG_INFO("Nodes: {}", simulation_input.node_count);
    SPDLOG_INFO("Iterations: {}", solver_result.iterations);
    SPDLOG_INFO("Final residual: {}", formatScientific(solver_result.residual));
    SPDLOG_INFO("Peclet number: {}", formatScientific(post_process_result.peclet_number));
    SPDLOG_INFO("Error norm: {}", formatScientific(post_process_result.error_norm));
}

std::string Reporter::executableStem(const std::filesystem::path& executable_path)
{
    return executable_path.stem().string();
}

std::string Reporter::convectionSchemeName(ConvectionScheme convection_scheme)
{
    return convection_scheme == ConvectionScheme::centralDifference ? "CDS USED FOR CONVECTION"
                                                                   : "UDS USED FOR CONVECTION";
}

std::string Reporter::solverName(SolverType solver_type)
{
    switch (solver_type)
    {
    case SolverType::jacobi:
        return "JACOBI SOLVER";
    case SolverType::gaussSeidel:
        return "GAUSS-SEIDEL SOLVER";
    case SolverType::successiveOverRelaxation:
        return "GSOR SOLVER";
    case SolverType::tdma:
        return "TDMA SOLVER";
    }

    return "UNKNOWN SOLVER";
}

std::shared_ptr<spdlog::logger> Reporter::createFileLogger(const std::string& log_file_name)
{
    auto file_logger = spdlog::basic_logger_mt("cmfd_log_logger", log_file_name, true);
    file_logger->set_pattern(kLogFilePattern);
    return file_logger;
}

std::shared_ptr<spdlog::logger> Reporter::createDataLogger(const std::string& data_file_name)
{
    auto data_logger = spdlog::basic_logger_mt("cmfd_data_logger", data_file_name, true);
    data_logger->set_pattern(kDataFilePattern);
    return data_logger;
}

std::string Reporter::formatScientific(double value)
{
    return fmt::format("{:>{}.{}E}", value, column_width, precision);
}

std::string Reporter::formatHeaderRow()
{
    return fmt::format("{:>{}} {:>{}} {:>{}} {:>{}}",
                       "X",
                       column_width,
                       "FI_EXACT",
                       column_width,
                       "FI",
                       column_width,
                       "ERROR",
                       column_width);
}

std::string Reporter::formatDataRow(double x_value,
                                    double exact_phi_value,
                                    double phi_value,
                                    double error_value)
{
    return fmt::format("{} {} {} {}",
                       formatScientific(x_value),
                       formatScientific(exact_phi_value),
                       formatScientific(phi_value),
                       formatScientific(error_value));
}

void Reporter::setConsolePattern()
{
    spdlog::set_pattern(kConsolePattern);
}

void Reporter::logInfo(const std::shared_ptr<spdlog::logger>& logger, const std::string& message)
{
    logger->log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION},
                spdlog::level::info,
                message);
}

void Reporter::writeDataFile(const std::shared_ptr<spdlog::logger>& data_logger,
                             const PostProcessResult& post_process_result)
{
    data_logger->info("{}", formatHeaderRow());
    for (std::size_t row_index = 0; row_index < post_process_result.result_table.shape()[0];
         ++row_index)
    {
        data_logger->info("{}",
                          formatDataRow(post_process_result.result_table(row_index, 0),
                                        post_process_result.result_table(row_index, 1),
                                        post_process_result.result_table(row_index, 2),
                                        post_process_result.result_table(row_index, 3)));
    }
}
