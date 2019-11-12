find_path(Benchmark_INCLUDE_DIR
        NAMES benchmark/benchmark.h
        NO_SYSTEM_ENVIRONMENT_PATH
        DOC "The directory where benchmark includes reside"
        )

find_library(Benchmark_LIBRARY
        NAMES benchmark
        NO_SYSTEM_ENVIRONMENT_PATH
        DOC "The benchmark library"
        )

set(Benchmark_INCLUDE_DIRS ${Benchmark_INCLUDE_DIR})
set(Benchmark_LIBRARIES    ${Benchmark_LIBRARY})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Benchmark
        FOUND_VAR Benchmark_FOUND
        REQUIRED_VARS Benchmark_INCLUDE_DIR Benchmark_LIBRARY
        )

mark_as_advanced(Benchmark_FOUND)