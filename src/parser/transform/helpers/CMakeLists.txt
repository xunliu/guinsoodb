add_library_unity(
  guinsoodb_transformer_helpers
  OBJECT
  transform_alias.cpp
  transform_cte.cpp
  transform_groupby.cpp
  transform_orderby.cpp
  transform_sample.cpp
  transform_typename.cpp
  nodetype_to_string.cpp)
set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:guinsoodb_transformer_helpers>
    PARENT_SCOPE)
