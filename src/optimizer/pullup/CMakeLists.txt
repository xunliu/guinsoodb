add_library_unity(
  guinsoodb_optimizer_pullup
  OBJECT
  pullup_filter.cpp
  pullup_projection.cpp
  pullup_from_left.cpp
  pullup_both_side.cpp
  pullup_set_operation.cpp)
set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:guinsoodb_optimizer_pullup>
    PARENT_SCOPE)
