add_library_unity(guinsoodb_operator_order OBJECT physical_order.cpp
                  physical_top_n.cpp)
set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:guinsoodb_operator_order>
    PARENT_SCOPE)
