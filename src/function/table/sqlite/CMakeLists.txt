add_library_unity(
  guinsoodb_func_sqlite
  OBJECT
  pragma_collations.cpp
  pragma_database_list.cpp
  pragma_database_size.cpp
  pragma_functions.cpp
  pragma_table_info.cpp
  sqlite_master.cpp)
set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:guinsoodb_func_sqlite>
    PARENT_SCOPE)
