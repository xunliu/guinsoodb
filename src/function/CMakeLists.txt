add_subdirectory(aggregate)
add_subdirectory(pragma)
add_subdirectory(scalar)
add_subdirectory(table)

add_library_unity(
  guinsoodb_function
  OBJECT
  cast_rules.cpp
  function.cpp
  pragma_function.cpp
  macro_function.cpp
  udf_function.cpp)
set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:guinsoodb_function>
    PARENT_SCOPE)
