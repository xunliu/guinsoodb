add_library_unity(
  guinsoodb_operator_aggregate OBJECT physical_hash_aggregate.cpp
  physical_perfecthash_aggregate.cpp physical_simple_aggregate.cpp
  physical_window.cpp)
set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:guinsoodb_operator_aggregate>
    PARENT_SCOPE)
