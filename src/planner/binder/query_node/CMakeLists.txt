add_library_unity(
  guinsoodb_bind_query_node
  OBJECT
  bind_select_node.cpp
  bind_setop_node.cpp
  bind_recursive_cte_node.cpp
  plan_query_node.cpp
  plan_recursive_cte_node.cpp
  plan_select_node.cpp
  plan_setop.cpp
  plan_subquery.cpp)
set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:guinsoodb_bind_query_node>
    PARENT_SCOPE)
