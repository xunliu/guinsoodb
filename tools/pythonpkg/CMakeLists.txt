include_directories(src)

find_package(PythonLibs)
find_package(pybind11)
if(NOT pybind11_FOUND)
  return()
endif()
if(NOT PythonLibs_FOUND)
  return()
endif()
include_directories(../../)
include_directories(src/include)

# this is used for clang-tidy checks
add_library(
  python_guinsoodb
  guinsoodb_python.cpp
  src/array_wrapper.cpp
  src/pandas_scan.cpp
  src/pyconnection.cpp
  src/pyrelation.cpp
  src/pyresult.cpp
  src/map.cpp
  src/vector_conversion.cpp)
target_include_directories(python_guinsoodb PRIVATE ${PYTHON_INCLUDE_DIRS})
target_link_libraries(python_guinsoodb guinsoodb_static ${PYTHON_LIBRARIES})
