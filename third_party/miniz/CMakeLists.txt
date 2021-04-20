cmake_policy(SET CMP0063 NEW)

add_library(miniz STATIC miniz.cpp)

target_include_directories(
  miniz
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
set_target_properties(miniz PROPERTIES EXPORT_NAME guinsoodb_miniz)

install(TARGETS miniz
        EXPORT "${GUINSOODB_EXPORT_SET}"
        LIBRARY DESTINATION "${INSTALL_LIB_DIR}"
        ARCHIVE DESTINATION "${INSTALL_LIB_DIR}")

disable_target_warnings(miniz)
