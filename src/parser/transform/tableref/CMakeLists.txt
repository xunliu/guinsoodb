add_library_unity(
  guinsoodb_transformer_tableref
  OBJECT
  transform_base_tableref.cpp
  transform_join.cpp
  transform_from.cpp
  transform_subquery.cpp
  transform_table_function.cpp
  transform_tableref.cpp)
set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:guinsoodb_transformer_tableref>
    PARENT_SCOPE)
