add_extension_definitions()

add_definitions(-DGUINSOODB_ROOT_DIRECTORY="${PROJECT_SOURCE_DIR}"
                -DGUINSOODB_BUILD_DIRECTORY="${PROJECT_BINARY_DIR}")

add_library_unity(test_sqlite OBJECT test_sqllogictest.cpp)
set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:test_sqlite>
    PARENT_SCOPE)
