include_directories(../../third_party/dsdgen/include)
include_directories(../../third_party/sqlite/include)
add_library(guinsoodb_benchmark_tpcds OBJECT sf1.cpp)
set(BENCHMARK_OBJECT_FILES
    ${BENCHMARK_OBJECT_FILES} $<TARGET_OBJECTS:guinsoodb_benchmark_tpcds>
    PARENT_SCOPE)
