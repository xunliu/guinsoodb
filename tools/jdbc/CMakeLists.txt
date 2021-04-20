find_package(Java 1.8)
find_package(JNI)

cmake_minimum_required(VERSION 3.11.0)

if(NOT JNI_FOUND OR NOT Java_FOUND)
  message(FATAL_ERROR No compatible Java/JNI found)
endif()

include(UseJava)
project(GuinsooDBJDummy NONE)

file(GLOB JAVA_SRC_FILES src/main/java/org/guinsoodb/*.java)
file(GLOB JAVA_TEST_FILES src/test/java/org/guinsoodb/test/*.java)

set(CMAKE_JAVA_COMPILE_FLAGS -source 1.7 -target 1.7 -encoding utf-8)

add_jar(guinsoodb_jdbc ${JAVA_SRC_FILES} ${JAVA_TEST_FILES}
        META-INF/services/java.sql.Driver GENERATE_NATIVE_HEADERS guinsoodb-native)

set(OS_NAME "unknown")
if(APPLE)
  set(OS_NAME "osx")
endif()
if(WIN32)
  set(OS_NAME "windows")
endif()
if(UNIX AND NOT APPLE)
  set(OS_NAME "linux") # sorry BSD
endif()

set(JVM_BITNESS "amd64")
# TODO handle 32 bit architectures

include_directories(../../extension/parquet/include)

add_library(guinsoodb_java SHARED src/jni/guinsoodb_java.cpp)
target_compile_options(guinsoodb_java PRIVATE -fexceptions)
target_link_libraries(guinsoodb_java guinsoodb-native guinsoodb_static parquet_extension)
if(OS_NAME STREQUAL "linux")
  target_link_libraries(guinsoodb_java -static-libgcc -static-libstdc++
  )# static link to libstdc++ to target more linux distro
endif()
string(JOIN "_" LIB_SUFFIX ".so" ${OS_NAME} ${JVM_BITNESS})
set_target_properties(guinsoodb_java PROPERTIES SUFFIX ${LIB_SUFFIX})
set_target_properties(guinsoodb_java PROPERTIES PREFIX "lib")

add_custom_command(
  OUTPUT dummy_jdbc_target
  DEPENDS guinsoodb_java guinsoodb_jdbc
  COMMAND ${Java_JAR_EXECUTABLE} uf guinsoodb_jdbc.jar -C
          $<TARGET_FILE_DIR:guinsoodb_java> $<TARGET_FILE_NAME:guinsoodb_java>)

add_custom_target(jdbc ALL DEPENDS dummy_jdbc_target)
