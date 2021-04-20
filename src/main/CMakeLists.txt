add_subdirectory(relation)

set(GUINSOODB_MAIN_FILES
    appender.cpp
    client_context.cpp
    connection.cpp
    database.cpp
    materialized_query_result.cpp
    prepared_statement.cpp
    prepared_statement_data.cpp
    relation.cpp
    query_profiler.cpp
    query_result.cpp
    stream_query_result.cpp)

if(NOT CLANG_TIDY)
  set(GUINSOODB_MAIN_FILES ${GUINSOODB_MAIN_FILES} guinsoodb-c.cpp)
endif()

add_library_unity(guinsoodb_main OBJECT ${GUINSOODB_MAIN_FILES})
set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:guinsoodb_main>
    PARENT_SCOPE)
