add_library_unity(
  guinsoodb_func_information_schema OBJECT information_schema_schemata.cpp
  information_schema_tables.cpp information_schema_columns.cpp)
set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:guinsoodb_func_information_schema>
    PARENT_SCOPE)
