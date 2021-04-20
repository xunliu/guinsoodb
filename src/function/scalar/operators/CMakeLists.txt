add_library_unity(
  guinsoodb_func_ops
  OBJECT
  add.cpp
  arithmetic.cpp
  bitwise.cpp
  multiply.cpp
  subtract.cpp)
set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:guinsoodb_func_ops>
    PARENT_SCOPE)
