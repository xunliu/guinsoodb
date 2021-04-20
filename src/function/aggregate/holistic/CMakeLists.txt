add_library_unity(guinsoodb_aggr_holistic OBJECT quantile.cpp
                  approximate_quantile.cpp reservoir_quantile.cpp)
set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:guinsoodb_aggr_holistic>
    PARENT_SCOPE)
