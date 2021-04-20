add_library_unity(guinsoodb_aggr_nested OBJECT list.cpp histogram.cpp)
set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:guinsoodb_aggr_nested>
    PARENT_SCOPE)
