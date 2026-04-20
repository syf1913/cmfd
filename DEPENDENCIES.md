# Dependency Lock (No Package Manager)

This project vendors all third-party dependencies under `third-part/`.
Builds are fully local and do not download anything after vendoring.

## Required layout

- `third-part/json-v3.11.2/include/nlohmann/json.hpp`
- `third-part/spdlog-1.14.1/include/spdlog/...`
- `third-part/xtl-0.8.0/include/xtl/...`
- `third-part/xtensor-0.26.0/include/xtensor/xtensor.hpp`

## Pinned versions

- nlohmann/json: `v3.11.2`
- spdlog: `v1.14.1`
- xtl: `0.8.0`
- xtensor: `0.26.0`

## Minimal-size vendoring strategy

This project uses header-only integration in CMake to reduce repository size:

- nlohmann/json: only `single_include/nlohmann/json.hpp`
- spdlog: only `include/` tree
- xtl: only `include/` tree
- xtensor: only `include/` tree

Use the helper script:

```bash
sh scripts/fetch_third_party_minimal.sh
# or
bash scripts/fetch_third_party_minimal.sh
```

Then commit `third-part/` into the repository.

## Build

```bash
cmake -S . -B build
cmake --build build -j
./build/cmfd
```

If CMake reports missing dependencies, verify the folder names and paths above.
