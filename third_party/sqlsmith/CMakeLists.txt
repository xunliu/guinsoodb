include_directories(include)

include_directories(../../extension/tpch/include)

add_executable(
  sqlsmith
  guinsoodb.cc
  dump.cc
  expr.cc
  grammar.cc
  impedance.cc
  log.cc
  prod.cc
  random.cc
  relmodel.cc
  schema.cc
  sqlsmith.cc)
target_link_libraries(sqlsmith guinsoodb tpch_extension)
link_threads(sqlsmith)

set_target_properties(
  sqlsmith
  PROPERTIES
    COMPILE_FLAGS
    "-Wno-potentially-evaluated-expression -Wno-delete-non-virtual-dtor")
