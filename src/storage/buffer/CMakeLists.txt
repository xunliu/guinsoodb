add_library_unity(guinsoodb_storage_buffer OBJECT buffer_handle.cpp
                  buffer_list.cpp managed_buffer.cpp)
set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:guinsoodb_storage_buffer>
    PARENT_SCOPE)
