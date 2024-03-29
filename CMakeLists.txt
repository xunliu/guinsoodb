cmake_minimum_required(VERSION 2.8.12)
cmake_policy(SET CMP0026 NEW)
cmake_policy(SET CMP0051 NEW)

project(GuinsooDB)

find_package(Threads REQUIRED)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_VERBOSE_MAKEFILE OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_MACOSX_RPATH 1)

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
endif()

# Determine install paths
set(INSTALL_LIB_DIR
    lib
    CACHE PATH "Installation directory for libraries")
set(INSTALL_BIN_DIR
    bin
    CACHE PATH "Installation directory for executables")
set(INSTALL_INCLUDE_DIR
    include
    CACHE PATH "Installation directory for header files")
if(WIN32 AND NOT CYGWIN)
  set(DEF_INSTALL_CMAKE_DIR cmake)
else()
  set(DEF_INSTALL_CMAKE_DIR lib/cmake/GuinsooDB)
endif()
set(INSTALL_CMAKE_DIR
    ${DEF_INSTALL_CMAKE_DIR}
    CACHE PATH "Installation directory for CMake files")
set(GUINSOODB_EXPORT_SET "GuinsooDBExports")

# Make relative install paths absolute
foreach(p LIB BIN INCLUDE CMAKE)
  set(var INSTALL_${p}_DIR)
  if(NOT IS_ABSOLUTE "${${var}}")
    set(${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
  endif()
endforeach()

set(M32_FLAG "")
if(FORCE_32_BIT)
  set(M32_FLAG " -m32 ")
endif()


option(DISABLE_UNITY "Disable unity builds." FALSE)

option(FORCE_COLORED_OUTPUT
       "Always produce ANSI-colored output (GNU/Clang only)." FALSE)
if(${FORCE_COLORED_OUTPUT})
  if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    add_compile_options(-fdiagnostics-color=always)
  elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    add_compile_options(-fcolor-diagnostics)
  endif()
endif()

option(ENABLE_SANITIZER "Enable address sanitizer." TRUE)
option(ENABLE_THREAD_SANITIZER "Enable thread sanitizer." FALSE)
option(ENABLE_UBSAN "Enable undefined behavior sanitizer." TRUE)
option(
  FORCE_SANITIZER
  "Forces building with sanitizers even if the Python and R modules are enabled."
  FALSE)
if((BUILD_PYTHON OR BUILD_R)
   AND (ENABLE_SANITIZER OR ENABLE_UBSAN)
   AND ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug"))
  if(FORCE_SANITIZER)
    message(
      WARNING
        "FORCE_SANITIZER is set and the Python/R builds are enabled. Sanitizers will be linked as a shared library (-shared-libasan). You may need to do LD_PRELOAD tricks to load packages built in this way."
    )
    set(CXX_EXTRA_DEBUG "${CXX_EXTRA_DEBUG} -shared-libasan")
  else()
    message(
      WARNING
        "Sanitizers are not enabled but will not be built because the Python/R builds are enabled. Use FORCE_SANITIZER to force building of the sanitizers even when building these packages."
    )
    set(ENABLE_SANITIZER FALSE)
    set(ENABLE_UBSAN FALSE)
  endif()
endif()
if(${ENABLE_THREAD_SANITIZER})
  if(${ENABLE_SANITIZER})
    message(
      WARNING
        "Both thread and address sanitizers are enabled. This is not supported by clang. The address sanitizer will be disabled, and we will run with only the thread sanitizer."
    )
  endif()
  if(FORCE_ASSERT)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=thread")
  else()
    set(CXX_EXTRA_DEBUG "${CXX_EXTRA_DEBUG} -fsanitize=thread")
  endif()
elseif(${ENABLE_SANITIZER})
  if(FORCE_ASSERT)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
  else()
    set(CXX_EXTRA_DEBUG "${CXX_EXTRA_DEBUG} -fsanitize=address")
  endif()
endif()

if(${ENABLE_UBSAN})
  if(FORCE_ASSERT)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined")
  else()
    set(CXX_EXTRA_DEBUG "${CXX_EXTRA_DEBUG} -fsanitize=undefined")
  endif()
endif()

option(EXPLICIT_EXCEPTIONS "Explicitly enable C++ exceptions." FALSE)
if(${EXPLICIT_EXCEPTIONS})
  set(CXX_EXTRA "${CXX_EXTRA} -fexceptions")
endif()

set(SUN FALSE)
if(${CMAKE_SYSTEM_NAME} STREQUAL "SunOS")
  set(CXX_EXTRA "${CXX_EXTRA} -mimpure-text")
  add_definitions(-DSUN=1)
  set(SUN TRUE)
endif()

execute_process(
  COMMAND git log -1 --format=%h
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_COMMIT_HASH
  OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(
  COMMAND git describe --tags --abbrev=0
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_LAST_TAG
  OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(
  COMMAND git describe --tags --long
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_ITERATION
  OUTPUT_STRIP_TRAILING_WHITESPACE)

string(REGEX REPLACE "v([0-9]+).[0-9]+.[0-9]+" "\\1" GUINSOODB_MAJOR_VERSION "${GIT_LAST_TAG}")
string(REGEX REPLACE "v[0-9]+.([0-9]+).[0-9]+" "\\1" GUINSOODB_MINOR_VERSION "${GIT_LAST_TAG}")
string(REGEX REPLACE "v[0-9]+.[0-9]+.([0-9]+)" "\\1" GUINSOODB_PATCH_VERSION "${GIT_LAST_TAG}")
string(REGEX REPLACE ".*-([0-9]+)-.*" "\\1" GUINSOODB_DEV_ITERATION "${GIT_ITERATION}")

if(${GUINSOODB_DEV_ITERATION} EQUAL 0)
  # on a tag; directly use the version
  set(GUINSOODB_VERSION "${GIT_LAST_TAG}")
else()
  # not on a tag, increment the patch version by one and add a -devX suffix
  math(EXPR GUINSOODB_PATCH_VERSION "${GUINSOODB_PATCH_VERSION}+1")

  set(GUINSOODB_VERSION "v${GUINSOODB_MAJOR_VERSION}.${GUINSOODB_MINOR_VERSION}.${GUINSOODB_PATCH_VERSION}-dev${GUINSOODB_DEV_ITERATION}")
endif()

option(AMALGAMATION_BUILD
       "Build from the amalgamation files, rather than from the normal sources."
       FALSE)

option(BUILD_ICU_EXTENSION "Build the ICU extension." FALSE)
option(BUILD_PARQUET_EXTENSION "Build the Parquet extension." FALSE)
option(BUILD_TPCH_EXTENSION "Build the TPC-H extension." FALSE)
option(BUILD_FTS_EXTENSION "Build the FTS extension." FALSE)
option(BUILD_HTTPFS_EXTENSION "Build the HTTP File System extension." FALSE)
option(BUILD_BENCHMARKS "Enable building of the benchmark suite." FALSE)
option(BUILD_SQLSMITH "Enable building of SQLSmith." FALSE)
option(BUILD_TPCE "Enable building of the TPC-E tool." FALSE)
option(JDBC_DRIVER "Build the GuinsooDB JDBC driver" TRUE)
option(BUILD_PYTHON "Build the GuinsooDB Python extension" FALSE)
option(BUILD_REST "Build the GuinsooDB REST server" FALSE)
option(BUILD_SHELL "Build the GuinsooDB Shell and SQLite API Wrappers" TRUE)
option(DISABLE_THREADS "Disable support for multi-threading" FALSE)
option(CLANG_TIDY "Enable build for clang-tidy, this disables all source files excluding the core database. This does not produce a working build." FALSE)
option(BUILD_UNITTESTS "Build the C++ Unit Tests." TRUE)
option(
  ASSERT_EXCEPTION
  "Throw an exception on an assert failing, instead of triggering a sigabort"
  TRUE)
option(FORCE_ASSERT "Enable checking of assertions, even in release mode" FALSE)

option(TREAT_WARNINGS_AS_ERRORS "Treat warnings as errors" FALSE)
option(EXPORT_DLL_SYMBOLS "Export dll symbols on Windows, else import" TRUE)
option(BUILD_RDTSC "Enable the rdtsc instruction." FALSE)

if(${BUILD_RDTSC})
  add_compile_definitions(RDTSC)
endif()

if(BUILD_PYTHON
   OR BUILD_R
   OR JDBC_DRIVER)
  set(BUILD_ICU_EXTENSION TRUE)
  set(BUILD_PARQUET_EXTENSION TRUE)
endif()

if(BUILD_SQLSMITH)
  set(BUILD_TPCH_EXTENSION TRUE)
endif()

if(TREAT_WARNINGS_AS_ERRORS)
  message("Treating warnings as errors.")
endif()

if(!ASSERT_EXCEPTION)
  set(CMAKE_CXX_FLAGS_DEBUG
      "${CMAKE_CXX_FLAGS_DEBUG} -DGUINSOODB_USE_STANDARD_ASSERT")
endif()

if(FORCE_ASSERT)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGUINSOODB_FORCE_ASSERT")
endif()

if(NOT MSVC)
  set(CMAKE_CXX_FLAGS_DEBUG
      "${CMAKE_CXX_FLAGS_DEBUG} -g -O0 -DDEBUG -Wall ${M32_FLAG} ${CXX_EXTRA}")
  set(CMAKE_CXX_FLAGS_RELEASE
      "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG ${M32_FLAG} ${CXX_EXTRA}")
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELEASE} -g")

  set(CXX_EXTRA_DEBUG
      "${CXX_EXTRA_DEBUG} -Wunused-variable -Wunused-const-variable -Werror=vla -Wnarrowing  -Wnarrowing -pedantic"
  )
  if(TREAT_WARNINGS_AS_ERRORS)
    set(CXX_EXTRA_DEBUG "${CXX_EXTRA_DEBUG} -Werror")
  endif()

  if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION
                                                   VERSION_GREATER 8.0)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${CXX_EXTRA_DEBUG}")
  elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang"
         AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 9.0)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${CXX_EXTRA_DEBUG}")
  else()
    message(WARNING "Please use a recent compiler for debug builds")
  endif()
else()
  set(CMAKE_CXX_WINDOWS_FLAGS
      "/wd4244 /wd4267 /wd4200 /wd26451 /wd26495 /D_CRT_SECURE_NO_WARNINGS")
  if(TREAT_WARNINGS_AS_ERRORS)
    set(CMAKE_CXX_WINDOWS_FLAGS "${CMAKE_CXX_WINDOWS_FLAGS} /WX")
  endif()
  # remove warning from CXX flags
  string(REGEX REPLACE "/W[0-4]" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  # add to-be-ignored warnings
  set(CMAKE_CXX_FLAGS
      "${CMAKE_CXX_FLAGS} /wd4244 /wd4267 /wd4200 /wd26451 /wd26495 /D_CRT_SECURE_NO_WARNINGS"
  )
endif()

# todo use CHECK_CXX_COMPILER_FLAG(-fsanitize=address SUPPORTS_SANITIZER) etc.

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(DEFAULT_BUILD_TYPE "Release")
  message(STATUS "Setting build type to '${DEFAULT_BUILD_TYPE}'.")
  set(CMAKE_BUILD_TYPE
      "${DEFAULT_BUILD_TYPE}"
      CACHE STRING "Choose the type of build." FORCE)
endif()

include_directories(src/include)
include_directories(third_party/fmt/include)
include_directories(third_party/hyperloglog)
include_directories(third_party/re2)
include_directories(third_party/miniz)
include_directories(third_party/utf8proc/include)
include_directories(third_party/miniparquet)
include_directories(third_party/concurrentqueue)
include_directories(third_party/pcg)
include_directories(third_party/tdigest)

# todo only regenerate ub file if one of the input files changed hack alert
function(enable_unity_build UB_SUFFIX SOURCE_VARIABLE_NAME)
  set(files ${${SOURCE_VARIABLE_NAME}})

  # Generate a unique filename for the unity build translation unit
  set(unit_build_file ${CMAKE_CURRENT_BINARY_DIR}/ub_${UB_SUFFIX}.cpp)
  set(temp_unit_build_file ${CMAKE_CURRENT_BINARY_DIR}/ub_${UB_SUFFIX}.cpp.tmp)
  # Exclude all translation units from compilation
  set_source_files_properties(${files} PROPERTIES HEADER_FILE_ONLY true)

  set(rebuild FALSE)
  # check if any of the source files have changed
  foreach(source_file ${files})
    if(${CMAKE_CURRENT_SOURCE_DIR}/${source_file} IS_NEWER_THAN
       ${unit_build_file})
      set(rebuild TRUE)
    endif()
  endforeach(source_file)
  # write a temporary file
  file(WRITE ${temp_unit_build_file} "// Unity Build generated by CMake\n")
  foreach(source_file ${files})
    file(
      APPEND ${temp_unit_build_file}
      "#include <${CMAKE_CURRENT_SOURCE_DIR}/${source_file}>\n"
    )
  endforeach(source_file)

  execute_process(
    COMMAND ${CMAKE_COMMAND} -E compare_files ${unit_build_file}
            ${temp_unit_build_file}
    RESULT_VARIABLE compare_result
    OUTPUT_VARIABLE bla
    ERROR_VARIABLE bla)
  if(compare_result EQUAL 0)
    # files are identical: do nothing
  elseif(compare_result EQUAL 1)
    # files are different: rebuild
    set(rebuild TRUE)
  else()
    # error while compiling: rebuild
    set(rebuild TRUE)
  endif()

  if(${rebuild})
    file(WRITE ${unit_build_file} "// Unity Build generated by CMake\n")
    foreach(source_file ${files})
      file(
        APPEND ${unit_build_file}
        "#include <${CMAKE_CURRENT_SOURCE_DIR}/${source_file}>\n"
      )
    endforeach(source_file)
  endif()

  # Complement list of translation units with the name of ub
  set(${SOURCE_VARIABLE_NAME}
      ${${SOURCE_VARIABLE_NAME}} ${unit_build_file}
      PARENT_SCOPE)
endfunction(enable_unity_build)

function(add_library_unity NAME MODE)
  set(SRCS ${ARGN})
  if(NOT DISABLE_UNITY)
    enable_unity_build(${NAME} SRCS)
  endif()
  add_library(${NAME} OBJECT ${SRCS})
endfunction()

function(disable_target_warnings NAME)
  if(MSVC)
    target_compile_options(${NAME} PRIVATE "/W0")
  elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang"
         OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    target_compile_options(${NAME} PRIVATE "-w")
  endif()
endfunction()

function(add_extension_definitions)
  include_directories(${PROJECT_SOURCE_DIR}/extension)

  if(${BUILD_ICU_EXTENSION})
    include_directories(${PROJECT_SOURCE_DIR}/extension/icu/include)
    add_definitions(-DBUILD_ICU_EXTENSION=${BUILD_ICU_EXTENSION})
  endif()

  if(${BUILD_PARQUET_EXTENSION})
    include_directories(${PROJECT_SOURCE_DIR}/extension/parquet/include)
    add_definitions(-DBUILD_PARQUET_EXTENSION=${BUILD_PARQUET_EXTENSION})
  endif()

  if(${BUILD_TPCH_EXTENSION})
    include_directories(${PROJECT_SOURCE_DIR}/extension/tpch/include)
    add_definitions(-DBUILD_TPCH_EXTENSION=${BUILD_TPCH_EXTENSION})
  endif()

  if(${BUILD_FTS_EXTENSION})
    include_directories(${CMAKE_SOURCE_DIR}/extension/fts/include)
    add_definitions(-DBUILD_FTS_EXTENSION=${BUILD_FTS_EXTENSION})
  endif()

  if(${BUILD_HTTPFS_EXTENSION})
    if (APPLE)
      set(OPENSSL_INCLUDE_DIR /usr/local/opt/openssl/include)
      else()
      find_package(OpenSSL REQUIRED)
    endif()
    include_directories(${CMAKE_SOURCE_DIR}/extension/httpfs/include ${OPENSSL_INCLUDE_DIR})
    add_definitions(-DBUILD_HTTPFS_EXTENSION=${BUILD_HTTPFS_EXTENSION})
  endif()
endfunction()

function(add_extension_dependencies LIBRARY)
  if(${BUILD_PARQUET_EXTENSION})
    add_dependencies(${LIBRARY} parquet_extension)
  endif()

  if(${BUILD_ICU_EXTENSION})
    add_dependencies(${LIBRARY} icu_extension)
  endif()

  if(${BUILD_TPCH_EXTENSION})
    add_dependencies(${LIBRARY} tpch_extension)
  endif()

  if(${BUILD_FTS_EXTENSION})
    add_dependencies(${LIBRARY} fts_extension)
  endif()

  if(${BUILD_HTTPFS_EXTENSION})
    add_dependencies(${LIBRARY} httpfs_extension)
  endif()
endfunction()

function(link_extension_libraries LIBRARY)
  if(${BUILD_PARQUET_EXTENSION})
    target_link_libraries(${LIBRARY} parquet_extension)
  endif()

  if(${BUILD_ICU_EXTENSION})
    target_link_libraries(${LIBRARY} icu_extension)
  endif()

  if(${BUILD_TPCH_EXTENSION})
    target_link_libraries(${LIBRARY} tpch_extension)
  endif()

  if(${BUILD_FTS_EXTENSION})
    target_link_libraries(${LIBRARY} fts_extension)
  endif()

  if(${BUILD_HTTPFS_EXTENSION})
    if (APPLE)
      set(OPENSSL_LIBRARIES /usr/local/opt/openssl/lib/libcrypto.dylib /usr/local/opt/openssl/lib/libssl.dylib )
      else()
      find_package(OpenSSL REQUIRED)
    endif()
    target_link_libraries(${LIBRARY} httpfs_extension ${OPENSSL_LIBRARIES})
  endif()
endfunction()

function(link_threads LIBRARY)
  target_link_libraries(${LIBRARY} Threads::Threads)
endfunction()

if(${EXPORT_DLL_SYMBOLS})
  # For Windows DLL export symbols
  add_definitions(-DGUINSOODB_BUILD_LIBRARY)
endif()

add_subdirectory(src)

add_subdirectory(tools)

add_subdirectory(extension)

if(NOT CLANG_TIDY)
  if(${BUILD_UNITTESTS})
    add_subdirectory(test)
    if(NOT WIN32
      AND NOT SUN
      AND ${BUILD_BENCHMARKS})
      add_subdirectory(benchmark)
    endif()
  endif()
  add_subdirectory(third_party)
endif()

# Write the export set for build and install tree
install(EXPORT "${GUINSOODB_EXPORT_SET}" DESTINATION "${INSTALL_CMAKE_DIR}")
export(EXPORT "${GUINSOODB_EXPORT_SET}"
       FILE "${PROJECT_BINARY_DIR}/${GUINSOODB_EXPORT_SET}.cmake")

# Only write the cmake package configuration if the templates exist
set(CMAKE_CONFIG_TEMPLATE "${CMAKE_SOURCE_DIR}/GuinsooDBConfig.cmake.in")
set(CMAKE_CONFIG_VERSION_TEMPLATE
    "${CMAKE_SOURCE_DIR}/GuinsooDBConfigVersion.cmake.in")
if(EXISTS ${CMAKE_CONFIG_TEMPLATE} AND EXISTS ${CMAKE_CONFIG_VERSION_TEMPLATE})

  # Configure cmake package config for the build tree
  set(CONF_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/src/include")
  configure_file(${CMAKE_CONFIG_TEMPLATE}
                 "${PROJECT_BINARY_DIR}/GuinsooDBConfig.cmake" @ONLY)

  # Configure cmake package config for the install tree
  file(RELATIVE_PATH REL_INCLUDE_DIR "${INSTALL_CMAKE_DIR}"
       "${INSTALL_INCLUDE_DIR}")
  set(CONF_INCLUDE_DIRS "\${GuinsooDB_CMAKE_DIR}/${REL_INCLUDE_DIR}")
  configure_file(
    ${CMAKE_CONFIG_TEMPLATE}
    "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/GuinsooDBConfig.cmake" @ONLY)

  # Configure cmake package version for build and install tree
  configure_file(${CMAKE_CONFIG_VERSION_TEMPLATE}
                 "${PROJECT_BINARY_DIR}/GuinsooDBConfigVersion.cmake" @ONLY)

  # Install the cmake package
  install(
    FILES "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/GuinsooDBConfig.cmake"
          "${PROJECT_BINARY_DIR}/GuinsooDBConfigVersion.cmake"
    DESTINATION "${INSTALL_CMAKE_DIR}")
endif()

