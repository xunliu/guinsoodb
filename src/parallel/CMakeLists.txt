if(${DISABLE_THREADS})
  add_definitions(-DGUINSOODB_NO_THREADS)
endif()

add_library_unity(guinsoodb_parallel OBJECT executor.cpp pipeline.cpp
                  task_scheduler.cpp thread_context.cpp)
set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:guinsoodb_parallel>
    PARENT_SCOPE)
