add_subdirectory(algebraic)
add_subdirectory(distributive)
add_subdirectory(holistic)
add_subdirectory(nested)
add_subdirectory(regression)

add_library_unity(
  guinsoodb_func_aggr
  OBJECT
  algebraic_functions.cpp
  distributive_functions.cpp
  holistic_functions.cpp
  nested_functions.cpp
  regression_functions.cpp)
set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:guinsoodb_func_aggr>
    PARENT_SCOPE)
