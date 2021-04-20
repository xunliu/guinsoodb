add_library_unity(guinsoodb_catalog_default_entries OBJECT default_functions.cpp
                  default_schemas.cpp default_views.cpp)
set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:guinsoodb_catalog_default_entries>
    PARENT_SCOPE)
