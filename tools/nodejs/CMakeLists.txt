include_directories(src)
include_directories(/usr/local/lib/node_modules/node-addon-api/)
include_directories(/usr/local/Cellar/node/14.12.0/include/node/)

set(CMAKE_SHARED_LINKER_FLAGS "-bundle" "-undefined dynamic_lookup")
add_library(node_guinsoodb src/guinsoodb_node.cpp src/database.cpp src/connection.cpp
                        src/statement.cpp src/utils.cpp)
target_link_libraries(node_guinsoodb guinsoodb_static)
link_threads(node_guinsoodb)
