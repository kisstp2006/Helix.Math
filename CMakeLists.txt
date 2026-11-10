cmake_minimum_required(VERSION 3.28)
project(Helix.Math LANGUAGES CXX)

if(NOT COMMAND helix_add_library)
    include("${CMAKE_CURRENT_SOURCE_DIR}/External/Helix.CMake/Helix.cmake")
endif()

helix_require(Core)
helix_add_library(helix_math ALIAS Math PUBLIC_DEPS Helix::Core)

helix_add_test_suite(helix_math_tests DEPS Helix::Math)
helix_add_benchmark_suite(helix_math_bench DEPS Helix::Math)
