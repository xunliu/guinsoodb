add_library_unity(guinsoodb_func_nested OBJECT list_extract.cpp list_value.cpp
                  struct_extract.cpp struct_pack.cpp)
set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:guinsoodb_func_nested>
    PARENT_SCOPE)
