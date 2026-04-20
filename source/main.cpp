#include <iostream>

#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

#include <xtensor/containers/xarray.hpp>
#include <xtensor/core/xmath.hpp>
#include <xtensor/io/xio.hpp>
#include <xtensor/views/xview.hpp>

int main()
{
    const std::string commented_json = R"json(
    {
        // line comment
        "name": "Alice",
        /* block comment */
        "age": 22
    }
    )json";
    nlohmann::json parsed = nlohmann::json::parse(commented_json, nullptr, true, true);

    xt::xtensor<int, 2> matrix = {{0, 0, 0}, {0, 0, 0}};
    int value = 1;
    for (std::size_t row = 0; row < matrix.shape()[0]; ++row)
    {
        for (std::size_t col = 0; col < matrix.shape()[1]; ++col)
        {
            matrix(row, col) = value++;
        }
    }

    nlohmann::json user = {
        {"name", "Alice"},
        {"age", 22},
        {"skills", {"C++", "CMake", "JSON", "spdlog", "xtensor"}}};

    user["active"] = true;
    user["matrix"] = {
        {matrix(0, 0), matrix(0, 1), matrix(0, 2)},
        {matrix(1, 0), matrix(1, 1), matrix(1, 2)}};

    std::cout << "Pretty JSON:\n"
              << user.dump(4) << "\n\n";
    spdlog::info("Serialized user json length: {}", user.dump().size());

    std::string name = parsed.at("name").get<std::string>();
    int age = parsed.at("age").get<int>();

    std::cout << "name=" << name << ", age=" << age << '\n';
    std::cout << "matrix(1,2)=" << matrix(1, 2) << '\n';
    return 0;
}
