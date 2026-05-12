# CMFD — 1D Convection-Diffusion Finite Volume Solver

Steady 1D convection-diffusion finite-volume solver with Dirichlet boundary conditions.

## Build

```bash
bash scripts/fetch_third_party_minimal.sh   # first time only
cmake -S . -B build
cmake --build build -j
```

Requires CMake ≥ 3.14, a C++17 compiler, and pthreads. Dependencies (nlohmann/json, spdlog, xtensor, xtl) are header-only and vendored under `third-part/`.

## Run

```bash
./build/cmfd [input.json]   # defaults to input.json
```

## Output

- `<name>.log` — iteration history and result table
- `<name>.dat` — result table: `[x, phi_exact, phi_numeric, error]`

## License

MIT
