add_library_unity(guinsoodb_operator_projection OBJECT physical_projection.cpp
                  physical_tableinout_function.cpp physical_unnest.cpp)
set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:guinsoodb_operator_projection>
    PARENT_SCOPE)
