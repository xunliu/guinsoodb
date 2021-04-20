add_definitions(-DGUINSOODB_SOURCE_ID="\""${GIT_COMMIT_HASH}"\"")
add_definitions(-DGUINSOODB_VERSION="\""${GUINSOODB_VERSION}"\"")

add_library_unity(guinsoodb_func_table_version OBJECT pragma_version.cpp)

set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:guinsoodb_func_table_version>
    PARENT_SCOPE)
