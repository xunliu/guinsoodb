add_library_unity(
  guinsoodb_planner_subquery OBJECT flatten_dependent_join.cpp
  has_correlated_expressions.cpp rewrite_correlated_expressions.cpp)
set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:guinsoodb_planner_subquery>
    PARENT_SCOPE)
