add_library_unity(guinsoodb_optimizer_join_order OBJECT query_graph.cpp
                  relation.cpp)
set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:guinsoodb_optimizer_join_order>
    PARENT_SCOPE)
