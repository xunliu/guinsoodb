add_library_unity(guinsoodb_operator_filter OBJECT physical_filter.cpp)
set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:guinsoodb_operator_filter>
    PARENT_SCOPE)
