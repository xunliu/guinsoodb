add_library_unity(guinsoodb_storage_checkpoint OBJECT table_data_writer.cpp
                  table_data_reader.cpp write_overflow_strings_to_disk.cpp)
set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:guinsoodb_storage_checkpoint>
    PARENT_SCOPE)
