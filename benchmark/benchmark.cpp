#include <benchmark/benchmark.h>
#include <iostream>
#include "../src/glpk/orig.h"
#include "../src/glpk/param.h"
static std::string S = "";
static std::string T = "";
static int PARAM = 7;

void readin() {
  std::string line;
  int count = 0;
  while (std::cin >> line) {
    if (!std::cin.eof()) {
      if (count == 0) {

        S = line;
      } else if (count == 1) {
        T = line;
      } else if (count == 2) {

        PARAM = std::stoi(line);
      }

      count++;
    } else if (count > 2) {
      std::cout << "running benchmarks on trees:" << std::endl;
      std::cout << S << std::endl;
      std::cout << T << std::endl;
      std::cout << "with parameter = " + std::stoi(line) << std::endl;
      break;
    } else {
      break;
    }
  }
}

static void form(benchmark::State &state) {

  readin();
  for (auto _ : state) {
    state.PauseTiming();
    auto s = S;
    auto t = T;
    auto sprnorm = orig(s, t, false);
    state.ResumeTiming();
    sprnorm.formulate();
    state.PauseTiming();
    sprnorm.delete_problem();
    state.ResumeTiming();
  }
}

static void solve(benchmark::State &state) {
  for (auto _ : state) {
    state.PauseTiming();
    auto s = S;
    auto t = T;
    auto sprnorm = orig(s, t, false);
    sprnorm.formulate();
    state.ResumeTiming();
    sprnorm.solve();
    state.PauseTiming();
    sprnorm.delete_problem();
    state.ResumeTiming();
  }
}


static void paramsolve(benchmark::State &state) {
  for (auto _ : state) {
    state.PauseTiming();
    auto s = S;
    auto t = T;
    auto sprparam = param(s, t, PARAM, false);
    sprparam.formulate();
    state.ResumeTiming();
    sprparam.solve(PARAM);
    state.PauseTiming();
    sprparam.delete_problem();
    state.ResumeTiming();
  }
}

//for runbenches.sh script n = 3
BENCHMARK(form);
BENCHMARK(solve);
BENCHMARK(paramsolve);
BENCHMARK_MAIN();