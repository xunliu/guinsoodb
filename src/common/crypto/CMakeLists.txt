add_library_unity(guinsoodb_common_crypto OBJECT md5.cpp)
set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:guinsoodb_common_crypto>
    PARENT_SCOPE)
