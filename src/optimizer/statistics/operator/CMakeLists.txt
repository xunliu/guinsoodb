add_library_unity(
  guinsoodb_optimizer_statistics_op
  OBJECT
  propagate_aggregate.cpp
  propagate_cross_product.cpp
  propagate_filter.cpp
  propagate_get.cpp
  propagate_limit.cpp
  propagate_join.cpp
  propagate_order.cpp
  propagate_projection.cpp
  propagate_set_operation.cpp
  propagate_window.cpp)
set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:guinsoodb_optimizer_statistics_op>
    PARENT_SCOPE)
