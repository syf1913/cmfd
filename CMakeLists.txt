cmake_minimum_required(VERSION 3.14)

project(cmfd LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(JSON_VERSION "v3.11.2")
set(SPDLOG_VERSION "1.14.1")
set(XTL_VERSION "0.8.0")
set(XTENSOR_VERSION "0.26.0")

set(THIRD_PART_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third-part")
set(JSON_DIR "${THIRD_PART_DIR}/json-${JSON_VERSION}")
set(SPDLOG_DIR "${THIRD_PART_DIR}/spdlog-${SPDLOG_VERSION}")
set(XTL_DIR "${THIRD_PART_DIR}/xtl-${XTL_VERSION}")
set(XTENSOR_DIR "${THIRD_PART_DIR}/xtensor-${XTENSOR_VERSION}")

if(NOT EXISTS "${JSON_DIR}/include/nlohmann/json.hpp")
    message(FATAL_ERROR "Missing nlohmann/json header. Expected: ${JSON_DIR}/include/nlohmann/json.hpp")
endif()

if(NOT EXISTS "${SPDLOG_DIR}/include/spdlog/spdlog.h")
    message(FATAL_ERROR "Missing spdlog header. Expected: ${SPDLOG_DIR}/include/spdlog/spdlog.h")
endif()

if(NOT EXISTS "${XTL_DIR}/include/xtl/xsequence.hpp")
    message(FATAL_ERROR "Missing xtl header. Expected: ${XTL_DIR}/include/xtl/xsequence.hpp")
endif()

if(NOT EXISTS "${XTENSOR_DIR}/include/xtensor/containers/xtensor.hpp")
    message(FATAL_ERROR "Missing xtensor header. Expected: ${XTENSOR_DIR}/include/xtensor/containers/xtensor.hpp")
endif()

add_executable(cmfd
    source/LinearSystem.cpp
    source/PostProcessor.cpp
    source/ProblemAssembler.cpp
    source/Reporter.cpp
    source/main.cpp
    source/SimulationConfig.cpp
    source/SolverFactory.cpp
)

target_include_directories(cmfd
    PRIVATE
        "${JSON_DIR}/include"
        "${SPDLOG_DIR}/include"
        "${XTL_DIR}/include"
        "${XTENSOR_DIR}/include"
)

find_package(Threads REQUIRED)
target_link_libraries(cmfd PRIVATE Threads::Threads)
