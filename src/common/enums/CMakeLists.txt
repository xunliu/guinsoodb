add_library_unity(
  guinsoodb_common_enums
  OBJECT
  catalog_type.cpp
  expression_type.cpp
  join_type.cpp
  logical_operator_type.cpp
  physical_operator_type.cpp
  statement_type.cpp
  relation_type.cpp)
set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:guinsoodb_common_enums>
    PARENT_SCOPE)
