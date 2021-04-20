add_library_unity(guinsoodb_merge_join OBJECT merge_join.cpp
                  merge_join_complex.cpp merge_join_simple.cpp)
set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:guinsoodb_merge_join>
    PARENT_SCOPE)
