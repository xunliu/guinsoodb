include_directories(../../third_party/sqlite/include)
add_library(guinsoodb_benchmark_micro OBJECT append.cpp bulkupdate.cpp cast.cpp
                                          data_skipping.cpp in.cpp storage.cpp)
set(BENCHMARK_OBJECT_FILES
    ${BENCHMARK_OBJECT_FILES} $<TARGET_OBJECTS:guinsoodb_benchmark_micro>
    PARENT_SCOPE)
