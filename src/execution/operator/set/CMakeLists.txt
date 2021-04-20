add_library_unity(guinsoodb_operator_set OBJECT physical_union.cpp
                  physical_recursive_cte.cpp)
set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:guinsoodb_operator_set>
    PARENT_SCOPE)
