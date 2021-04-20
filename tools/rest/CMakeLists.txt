include_directories(.)

include_directories(../../third_party/httplib)

add_executable(guinsoodb_rest_server server.cpp)

if(${BUILD_SUN})
  set(LINK_EXTRA -lsocket)
endif()

add_extension_definitions()

target_link_libraries(guinsoodb_rest_server guinsoodb_static ${LINK_EXTRA})
link_threads(guinsoodb_rest_server)

link_extension_libraries(guinsoodb_rest_server)
