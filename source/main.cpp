#include <exception>
#include <string>

#include <spdlog/spdlog.h>

#include "PostProcessor.hpp"
#include "ProblemAssembler.hpp"
#include "Reporter.hpp"
#include "SimulationConfig.hpp"
#include "SolverFactory.hpp"

int main(int argc, char* argv[])
{
    try
    {
        const std::string input_file_path = (argc > 1) ? argv[1] : "input.json";

        const SimulationInput simulation_input = SimulationConfig::loadFromFile(input_file_path);

        const ProblemAssembler problem_assembler;
        LinearSystem system = problem_assembler.build(simulation_input);

        const SolverFactory solver_factory;
        const auto solver = solver_factory.create(simulation_input.solver_type);
        const SolverResult solver_result = solver->solve(system, simulation_input);

        const PostProcessor post_processor;
        const PostProcessResult post_process_result = post_processor.build(simulation_input, system);

        const Reporter reporter;
        reporter.writeOutput(argv[0], simulation_input, solver_result, post_process_result);
        reporter.printConsoleSummary(argv[0],
                                     input_file_path,
                                     simulation_input,
                                     solver_result,
                                     post_process_result);
    }
    catch (const std::exception& exception)
    {
        spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %s:%# %v");
        SPDLOG_ERROR("Error: {}", exception.what());
        return 1;
    }

    return 0;
}
