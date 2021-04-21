//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/common/vector_size.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/common/types.hpp"

namespace guinsoodb {

//! The vector size used in the execution engine
#ifndef STANDARD_VECTOR_SIZE
#define STANDARD_VECTOR_SIZE 1024
#endif

#if ((STANDARD_VECTOR_SIZE & (STANDARD_VECTOR_SIZE - 1)) != 0)
#error Vector size should be a power of two
#endif

//! Zero selection vector: completely filled with the value 0 [READ ONLY]
extern const sel_t ZERO_VECTOR[STANDARD_VECTOR_SIZE];

} // namespace guinsoodb
