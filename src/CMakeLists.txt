add_definitions(-DGUINSOODB)

if(NOT MSVC)
  set(CMAKE_CXX_FLAGS_DEBUG
      "${CMAKE_CXX_FLAGS_DEBUG} -Wextra -Wno-unused-parameter -Wno-redundant-move"
  )
endif()

# clang-tidy config from presto-cpp set(CMAKE_CXX_CLANG_TIDY clang-tidy
# --checks=*,-abseil-*,-android-*,-cert-err58-cpp,-clang-analyzer-
# osx-*,-cppcoreguidelines-avoid-c-arrays,-cppcoreguidelines-avoid-magic-
# numbers,-cppcoreguidelines-pro-bounds-array-to-pointer-
# decay,-cppcoreguidelines-pro-bounds-pointer-arithmetic,-cppcoreguidelines-pro-
# type-reinterpret-cast,-cppcoreguidelines-pro-type-
# vararg,-fuchsia-*,-google-*,-hicpp-avoid-c-arrays,-hicpp-deprecated-
# headers,-hicpp-no-array-decay,-hicpp-use-equals-default,-hicpp-
# vararg,-llvmlibc-*,-llvm-header-guard,-llvm-include-order,-mpi-*,-misc-non-
# private-member-variables-in-classes,-misc-no-recursion,-misc-unused-
# parameters,-modernize-avoid-c-arrays,-modernize-deprecated-headers,-modernize-
# use-nodiscard,-modernize-use-trailing-return-
# type,-objc-*,-openmp-*,-readability-avoid-const-params-in-decls,-readability-
# convert-member-functions-to-static,-readability-magic-numbers,-zircon-*)

if(AMALGAMATION_BUILD)

  if(WIN32)
    add_definitions(/bigobj)
  endif()

  add_library(guinsoodb SHARED "${PROJECT_SOURCE_DIR}/src/amalgamation/guinsoodb.cpp")
  add_library(guinsoodb_static STATIC
              "${PROJECT_SOURCE_DIR}/src/amalgamation/guinsoodb.cpp")

  link_threads(guinsoodb)
  link_threads(guinsoodb_static)

  install(FILES "${PROJECT_SOURCE_DIR}/src/amalgamation/guinsoodb.hpp"
                "${PROJECT_SOURCE_DIR}/src/include/guinsoodb.h"
          DESTINATION "${INSTALL_INCLUDE_DIR}")
  install(FILES "${PROJECT_SOURCE_DIR}/src/include/guinsoodb/common/winapi.hpp"
          DESTINATION "${INSTALL_INCLUDE_DIR}/guinsoodb/common")

else()

  add_definitions(-DGUINSOODB_MAIN_LIBRARY)

  add_subdirectory(optimizer)
  add_subdirectory(planner)
  add_subdirectory(parser)
  add_subdirectory(function)
  add_subdirectory(catalog)
  add_subdirectory(common)
  add_subdirectory(execution)
  add_subdirectory(main)
  add_subdirectory(parallel)
  add_subdirectory(storage)
  add_subdirectory(transaction)

  set(GUINSOODB_LINK_LIBS
      ${CMAKE_DL_LIBS}
      fmt
      pg_query
      guinsoodb_re2
      miniz
      utf8proc
      hyperloglog)

  add_library(guinsoodb SHARED ${ALL_OBJECT_FILES})
  target_link_libraries(guinsoodb ${GUINSOODB_LINK_LIBS})
  link_threads(guinsoodb)

  add_library(guinsoodb_static STATIC ${ALL_OBJECT_FILES})
  target_link_libraries(guinsoodb_static ${GUINSOODB_LINK_LIBS})
  link_threads(guinsoodb_static)

  target_include_directories(
    guinsoodb PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

  target_include_directories(
    guinsoodb_static PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                         $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

  install(
    DIRECTORY "${PROJECT_SOURCE_DIR}/src/include/guinsoodb"
    DESTINATION "${INSTALL_INCLUDE_DIR}"
    FILES_MATCHING
    PATTERN "*.hpp")
  install(FILES "${PROJECT_SOURCE_DIR}/src/include/guinsoodb.hpp"
                "${PROJECT_SOURCE_DIR}/src/include/guinsoodb.h"
          DESTINATION "${INSTALL_INCLUDE_DIR}")

endif()

if(BUILD_PYTHON OR BUILD_R)
  if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(ALL_COMPILE_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG}")
  elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(ALL_COMPILE_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE}")
  elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    set(ALL_COMPILE_FLAGS
        "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
  else()
    set(ALL_COMPILE_FLAGS "${CMAKE_CXX_FLAGS}")
  endif()

  get_target_property(guinsoodb_libs guinsoodb LINK_LIBRARIES)

  if(BUILD_PYTHON)
    add_custom_target(
      guinsoodb_python ALL
      COMMAND python3 setup.py install --binary-dir=${CMAKE_BINARY_DIR}
              --compile-flags=${ALL_COMPILE_FLAGS} --libs="${guinsoodb_libs}"
      DEPENDS guinsoodb
      WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/tools/pythonpkg
      COMMENT Build
      Python package)
    add_extension_dependencies(guinsoodb_python)
  endif()
  if(BUILD_R)
    add_custom_target(
      guinsoodb_r ALL
      COMMAND
        GUINSOODB_R_BINDIR=${CMAKE_BINARY_DIR}
        GUINSOODB_R_CFLAGS=\"${ALL_COMPILE_FLAGS}\"
        GUINSOODB_R_LIBS=\"${guinsoodb_libs}\" R CMD INSTALL .
      DEPENDS guinsoodb
      WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/tools/rpkg
      COMMENT Build
      R package)
    add_extension_dependencies(guinsoodb_r)
  endif()
endif()

install(
  TARGETS guinsoodb guinsoodb_static
  EXPORT "${GUINSOODB_EXPORT_SET}"
  LIBRARY DESTINATION "${INSTALL_LIB_DIR}"
  ARCHIVE DESTINATION "${INSTALL_LIB_DIR}")
