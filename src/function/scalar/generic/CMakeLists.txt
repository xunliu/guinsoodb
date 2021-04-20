add_library_unity(
  guinsoodb_func_generic
  OBJECT
  alias.cpp
  constant_or_null.cpp
  current_setting.cpp
  least.cpp
  stats.cpp
  typeof.cpp)

set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:guinsoodb_func_generic>
    PARENT_SCOPE)
