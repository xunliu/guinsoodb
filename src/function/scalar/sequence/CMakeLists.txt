add_library_unity(guinsoodb_func_seq OBJECT nextval.cpp)
set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:guinsoodb_func_seq>
    PARENT_SCOPE)
