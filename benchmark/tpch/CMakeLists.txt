include_directories(../../extension/tpch/include)

set(BENCHMARK_SOURCE_FILES startup.cpp)

add_library(guinsoodb_benchmark_tpch OBJECT ${BENCHMARK_SOURCE_FILES})

set(BENCHMARK_OBJECT_FILES
    ${BENCHMARK_OBJECT_FILES} $<TARGET_OBJECTS:guinsoodb_benchmark_tpch>
    PARENT_SCOPE)
