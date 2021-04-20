//===----------------------------------------------------------------------===//
//                         GuinsooDB
//
// guinsoodb/common/checksum.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "guinsoodb/common/constants.hpp"

namespace guinsoodb {

//! Compute a checksum over a buffer of size size
uint64_t Checksum(uint8_t *buffer, size_t size);

} // namespace guinsoodb
